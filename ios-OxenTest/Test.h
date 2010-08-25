#ifndef KILLER_MOVES_INCLUDED
#define KILLER_MOVES_INCLUDED

#include <limits.h>
#include <iostream>
#include <map>

typedef unsigned int Move;
static const int INFINITY = INT_MAX;

class KillerMoveList {
public:
	KillerMoveList(): mMoves(0), mScores(0), mCount(0), mMax(0) { }
	KillerMoveList(int max_moves) : mMoves(0), mScores(0), mCount(0), mMax(max_moves) {
		mMoves = new Move[mMax];
		mScores = new int[mMax];
	}
	KillerMoveList(const KillerMoveList& o): mMoves(0), mScores(0), mCount(o.mCount), mMax(o.mMax) {
		mMoves = new Move[mMax];
		mScores = new int[mMax];
		for (int i=0; i<mCount; ++i) {
			mMoves[i]=o.mMoves[i];
			mScores[i]=o.mScores[i];
		}
	}
	~KillerMoveList() {
		delete mMoves;
		delete mScores;
	}
	KillerMoveList& operator=(const KillerMoveList& o) {
		if (mMax!=o.mMax) {
			delete mMoves;
			delete mScores;
			mMoves = new Move[o.mMax];
			mScores = new int[o.mMax];
		}
		mCount=o.mCount;
		mMax=o.mMax;
		for (int i=0; i<mCount; ++i) {
			mMoves[i]=o.mMoves[i];
			mScores[i]=o.mScores[i];
		}
	}
	inline int WorstScore() const {
		if (mCount==0)
			return -INFINITY;
		else
			return mScores[mCount-1];
	}
	void Consider(Move move, int score) {
		if (score>WorstScore()) {
			// If list is full, remove worst move
			if (mCount==mMax) --mCount;
			
			// If move is already present:
			bool shift=false;
			for (int i=0; i<mCount; ++i) {
				if (!shift) {
					if (mMoves[i]!=move) continue;
					if (mScores[i]<score) {
						// Found our move, with a smaller score!
						// 1: Check if we need to change order
						int pre = INFINITY;
						if (i>0) pre=mScores[i-1];
						int post = -INFINITY;
						if (i<mCount) post=mScores[i+1];
						if (pre>=score && post<=score) {
							mScores[i]=score;
							return;
						}
						// 2: Remove old score and exit from loop
						for (int j=i; j<mCount-1; ++j) {
							mMoves[j]=mMoves[j+1];
							mScores[j]=mScores[j+1];
						}
						--mCount;
						break;
					}
				}
			}
			
			// Search best index
			int index=0;
			while(index<mCount && mScores[index]>=score) ++index;
			
			if (index<mCount) {
				// Prepare insertion
				for (int i=mCount; i>index; --i) {
					mMoves[i]=mMoves[i-1];
					mScores[i]=mScores[i-1];
				}
			}
			mMoves[index]=move;
			mScores[index]=score;
			++mCount;
		}
	}
	
	int GetScore(Move move) const {
		for (int i=0; i<mCount; ++i)
			if (mMoves[i]==move) return mScores[i];
		return -INFINITY;
	}
	
	void dump() {
		for (int i=0; i<mCount; ++i)
			std::cout << i << ": Move " << mMoves[i] << ", score=" << mScores[i] << "\n";
	}
	
private:
	Move* mMoves;
	int*  mScores;
	int   mCount;
	int   mMax;
};

class KillerMoves {
public:
	KillerMoves() { }
	KillerMoves(const KillerMoves& o): mKillerMoveLists(o.mKillerMoveLists) { }
	inline int WorstScore(int depth) const {
		std::map<int, KillerMoveList>::const_iterator it = mKillerMoveLists.find(depth);
		if (it==mKillerMoveLists.end())
			return -INFINITY;
		return (*it).second.WorstScore();
	}
	inline void Consider(int depth, Move move, int score) {
		mKillerMoveLists[depth].Consider(move,score);
	}
	inline int GetScore(int depth, Move move) const {
		std::map<int, KillerMoveList>::const_iterator it = mKillerMoveLists.find(depth);
		if (it==mKillerMoveLists.end())
			return -INFINITY;
		return (*it).second.GetScore(move);
	}

private:
	std::map<int, KillerMoveList> mKillerMoveLists;
};

#endif // KILLER_MOVES_INCLUDED
