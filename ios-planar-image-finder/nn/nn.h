#ifndef KDTREE_H
#define KDTREE_H

#include <algorithm>
#include <map>
#include <cassert>
#include <cstring>
#include <arm_neon.h>

#include "nn/heap.h"
#include "nn/allocator.h"
#include "accel.h"

using namespace std;

namespace nn {



	/**
	 * Randomized kd-tree index
	 *
	 * Contains the k-d trees and other information for indexing a set of points
	 * for nearest-neighbor matching.
	 */
	class KDTreeIndex {
	private:
		KDTreeIndex(const KDTreeIndex&);
		KDTreeIndex& operator=(const KDTreeIndex&);
	private:
		enum {
			/**
			 * To improve efficiency, only SAMPLE_MEAN random values are used to
			 * compute the mean and variance at each level when building a tree.
			 * A value of 100 seems to perform as well as using all values.
			 */
			SAMPLE_MEAN = 100,
			/**
			 * Top random dimensions to consider
			 *
			 * When creating random trees, the dimension on which to subdivide is
			 * selected at random from among the top RAND_DIM dimensions with the
			 * highest variance.  A value of 5 works well.
			 */
			RAND_DIM = 5
		};

		// indices to vectors in the dataset.
		std::vector<int> vind;

		const float* dataset;

		size_t size_;
		size_t veclen_;

		std::vector<float> mean;
		std::vector<float> var;

		// All nodes that have vec[divfeat] < divval are placed in the
		// child1 subtree, else child2., A leaf node is indicated if both children are null.
		struct TreeNode {
			// Index of the vector feature used for subdivision.
			// If this is a leaf node (both children are NULL) then
			// this holds vector index for this leaf.
			int divfeat;
			// The value used for subdivision
			float divval;
			TreeNode* child1;
			TreeNode* child2;
		};
		TreeNode* trees[4];

		// This record represents a branch point when finding neighbors in
		// the tree.  It contains a record of the minimum distance to the query
		// point, as well as the node at which the search resumes.
		struct Branch {
			TreeNode* node;		// Tree node at which search resumes
			float mindistsq;	// Minimum distance to query for all nodes below.
			bool operator<(const Branch& rhs) {
				return mindistsq < rhs.mindistsq;
			}
			static Branch make_branch(TreeNode* aNode, float dist) {
				Branch branch;
				branch.node = aNode;
				branch.mindistsq = dist;
				return branch;
			}
		};

		PooledAllocator pool;
		Heap<Branch> heap;
		vector<bool> checked;


		class KNNResultSet {
			const float* target;
			int veclen;
			int* indices;
			float* dists;
			int count;

		public:
			KNNResultSet(const float* target_, int veclen_, int* _indices, float* _dists)
				: target(target_)
				, veclen(veclen_)
				, count(0)
				, indices(_indices)
				, dists(_dists)
			{ }

			size_t size() const { return count; }
			bool full() const { return count == 2; }


			bool addPoint(const float* point, int index) {
				// point already present
				for (int i = 0; i < count; ++i)
					if (indices[i] == index) return false;

				// distance from reference point
				float dist;
				accel::vSquaredDistance(dist, target, point, veclen);

				if (count < 2) {
					// Fill the resultset
					indices[count] = index;
					dists[count] = dist;
					++count;
				}
				else if (dist < dists[count - 1] || (dist == dists[count - 1] && index < indices[count - 1])) {
					indices[count - 1] = index;
					dists[count - 1] = dist;
				}
				else return false;

				int i = count - 1;

				while (i >= 1 && (dists[i] < dists[i - 1] || (dists[i] == dists[i - 1] && indices[i] < indices[i - 1]))) {
					swap(indices[i], indices[i - 1]);
					swap(dists[i], dists[i - 1]);
					i--;
				}

				return true;
			}

			float worstDist() const {
				return (count < 2) ? numeric_limits<float>::max() : dists[count - 1];
			}
		};


	public:
		KDTreeIndex(const float* inputData, size_t rows, size_t cols)
			: dataset(inputData)
			, size_(rows)
			, veclen_(cols)
			, vind(rows)
			, mean(cols)
			, var(cols)
			, pool()
			, heap(rows)
			, checked(rows, false) {
			// Create a permutable array of indices to the input vectors.
			for (size_t i = 0; i < size_; i++) vind[i] = i;
		}

		void buildIndex() {
			// Construct the randomized trees.
			for (int i = 0; i < 4; i++) {
				// Randomize the order of vectors to allow for unbiased sampling.
				for (int j = size_; j > 0; --j)
					swap(vind[j - 1], vind[accel::rand(j)]);

				trees[i] = divideTree(0, size_ - 1);
			}
		}

		void findNeighbors(const float* target_, int* _indices, float* _dists, int maxCheck) {
			KNNResultSet result(target_, veclen_, _indices, _dists);
			int i;
			Branch branch;
			int checkCount = 0;
			heap.clear();
			std::fill(checked.begin(), checked.end(), false);

			// Search once through each tree down to root
			for (i = 0; i < 4; ++i)
				searchLevel(result, target_, trees[i], 0.f, checkCount, maxCheck);

			// Keep searching other branches from heap until finished
			while (heap.popMin(branch) && (checkCount < maxCheck || !result.full()))
				searchLevel(result, target_, branch.node, branch.mindistsq, checkCount, maxCheck);

			assert(result.full());
		}


	private:


		/**
		 * Create a tree node that subdivides the list of vecs from vind[first]
		 * to vind[last].  The routine is called recursively on each sublist.
		 * Place a pointer to this new tree node in the location pTree.
		 *
		 * Params: pTree = the new node to create
		 * 			first = index of the first vector
		 * 			last = index of the last vector
		 */
		TreeNode* divideTree(int first, int last) {
			TreeNode* node = pool.allocate<TreeNode>(); // allocate memory

			/* If only one exemplar remains, then make this a leaf node. */
			if (first == last) {
				node->child1 = node->child2 = NULL;    /* Mark as leaf node. */
				node->divfeat = vind[first];    /* Store index of this vec. */
			}
			else {
				chooseDivision(node, first, last);
				subdivide(node, first, last);
			}

			return node;
		}


		/**
		 * Choose which feature to use in order to subdivide this set of vectors.
		 * Make a random choice among those with the highest variance, and use
		 * its variance as the threshold value.
		 */
		void chooseDivision(TreeNode* node, int first, int last) {
			// Compute mean values.  Only the first SAMPLE_MEAN values need to be
			// sampled to get a good estimate.
			const int end(std::min(first + SAMPLE_MEAN, last));
			const float d(1.f / float(end - first + 1));
			accel::vClear(&mean[0], veclen_);
			accel::vClear(&var[0], veclen_);
			const float32x4_t dddd = {d, d, d, d};
			float32x4_t* vmean((float32x4_t*) &mean[0]);
			float32x4_t* vvar((float32x4_t*) &var[0]);

			for (int j = first; j <= end; ++j) {
				const float32x4_t* v((const float32x4_t*) &dataset[veclen_ * vind[j]]);

				for (size_t k = 0; k < (veclen_ >> 2); ++k) {
					vmean[k] = vaddq_f32(vmean[k], v[k]);
				}
			}

			for (size_t k(0); k < (veclen_ >> 2); ++k) vmean[k] *= dddd;

			/* Compute variances (no need to divide by count). */
			for (int j = first; j <= end; ++j) {
				const float32x4_t* v((const float32x4_t*) &dataset[veclen_ * vind[j]]);

				for (size_t k(0); k < (veclen_ >> 2); ++k) {
					register float32x4_t c(vsubq_f32(v[k], vmean[k]));
					vvar[k] = vmlaq_f32(vvar[k], c, c);
				}
			}

			/* Select one of the highest variance indices at random. */
			node->divfeat = selectDivision();
			node->divval = mean[node->divfeat];
		}


		/**
		 * Select the top RAND_DIM largest values from var and return the index of
		 * one of these selected at random.
		 */
		int selectDivision() {
			int num = 0;
			int topind[RAND_DIM];

			/* Create a list of the indices of the top RAND_DIM values. */
			for (size_t i = 0; i < veclen_; ++i) {
				if (num < RAND_DIM  ||  var[i] > var[topind[num - 1]]) {
					/* Put this element at end of topind. */
					if (num < RAND_DIM) topind[num++] = i;  /* Add to list. */
					else topind[num - 1] = i;               /* Replace last element. */

					/* Bubble end value down to right location by repeated swapping. */
					int j = num - 1;

					while (j > 0  &&  var[topind[j]] > var[topind[j - 1]]) {
						swap(topind[j], topind[j - 1]);
						--j;
					}
				}
			}

			/* Select a random integer in range [0,num-1], and return that index. */
			return topind[accel::rand(num)];
		}


		/**
		 *  Subdivide the list of exemplars using the feature and division
		 *  value given in this node.  Call divideTree recursively on each list.
		*/
		void subdivide(TreeNode* node, int first, int last) {
			/* Move vector indices for left subtree to front of list. */
			int i = first;
			int j = last;

			while (i <= j) {
				int ind = vind[i];
				float val = dataset[veclen_ * ind + node->divfeat];

				if (val < node->divval) ++i;
				else {
					/* Move to end of list by swapping vind i and j. */
					swap(vind[i], vind[j]);
					--j;
				}
			}

			/* If either list is empty, it means we have hit the unlikely case
				in which all remaining features are identical. Split in the middle
			    to maintain a balanced tree.
			*/
			if ((i == first) || (i == last + 1))
				i = (first + last + 1) >> 1;

			node->child1 = divideTree(first, i - 1);
			node->child2 = divideTree(i, last);
		}



		/**
		 *  Search starting from a given node of the tree.  Based on any mismatches at
		 *  higher levels, all exemplars below this level must have a distance of
		 *  at least "mindistsq".
		*/
		void searchLevel(KNNResultSet& result, const float* vec, TreeNode* node, float mindistsq, int& checkCount, int maxCheck) {
			if (result.worstDist() < mindistsq) return;

			/* If this is a leaf node, then do check and return. */
			if (!node->child1 &&  !node->child2) {
				/* Do not check same node more than once when searching multiple trees.
					Once a vector is checked, we set its location in vind to the
					current checkID.
				*/
				if (!((checked[node->divfeat] || checkCount >= maxCheck) && result.full())) {
					++checkCount;
					checked[node->divfeat] = true;
					result.addPoint(&dataset[veclen_ * node->divfeat], node->divfeat);
				}

				return;
			}

			/* Which child branch should be taken first? */
			const float val(vec[node->divfeat]);
			const float diff(val - node->divval);
			TreeNode* bestChild = (diff < 0) ? node->child1 : node->child2;
			TreeNode* otherChild = (diff < 0) ? node->child2 : node->child1;
			/* Create a branch record for the branch not taken.  Add distance
				of this feature boundary (we don't attempt to correct for any
				use of this feature in a parent node, which is unlikely to
				happen and would have only a small effect).  Don't bother
				adding more branches to heap after halfway point, as cost of
				adding exceeds their value.
			*/
			const float c(val - node->divval);
			float new_distsq = mindistsq + c * c;

			if (new_distsq < result.worstDist() ||  !result.full())
				heap.insert(Branch::make_branch(otherChild, new_distsq));

			/* Call recursively to search next level down. */
			searchLevel(result, vec, bestChild, mindistsq, checkCount, maxCheck);
		}
	};   // class KDTree

}

#endif //KDTREE_H
