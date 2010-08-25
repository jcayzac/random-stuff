#ifndef KILLER_MOVES_INCLUDED
#define KILLER_MOVES_INCLUDED

#include <limits.h>
#include <iostream>
#include <map>
#include <queue>

typedef unsigned int Move;
static const unsigned int NO_MOVE=0xffffffff;

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
		return *this;
	}
	inline int WorstScore() const {
		if (mCount==0)
			return -INFINITY;
		else
			return mScores[mCount-1];
	}
	void Consider(Move move, int score);
	inline int GetScore(Move move) const {
		for (int i=0; i<mCount; ++i)
			if (mMoves[i]==move) return mScores[i];
		return -INFINITY;
	}
	inline int Count() const { return mCount; }
	inline Move Get(int index) const {
		if (index>=0 && index<mCount)
			return mMoves[index];
		return NO_MOVE;
	}
	friend std::ostream& operator<<(std::ostream &out, const KillerMoveList &v);
	
private:
	Move* mMoves;
	int*  mScores;
	int   mCount;
	int   mMax;
};

class KillerMoves {
public:
	static const int KILLER_SCORE_BASE = 100000;
public:
	KillerMoves(): mKillerMoveLists() { }
	KillerMoves(const KillerMoves& o): mKillerMoveLists(o.mKillerMoveLists) { }
	inline int WorstScore(int depth) const {
		std::map<int, KillerMoveList>::const_iterator it = mKillerMoveLists.find(depth);
		if (it==mKillerMoveLists.end())
			return -INFINITY;
		return (*it).second.WorstScore();
	}
	inline void Consider(int depth, Move move, int score) {
		std::map<int, KillerMoveList>::const_iterator it = mKillerMoveLists.find(depth);
		if (it==mKillerMoveLists.end())
			mKillerMoveLists[depth]=KillerMoveList(20);
		mKillerMoveLists[depth].Consider(move,score);
	}
	inline int GetScore(int depth, Move move) const {
		std::map<int, KillerMoveList>::const_iterator it = mKillerMoveLists.find(depth);
		if (it==mKillerMoveLists.end())
			return -INFINITY;
		return KILLER_SCORE_BASE+(*it).second.GetScore(move);
	}
	inline Move Get(int depth, int index) const {
		std::map<int, KillerMoveList>::const_iterator it = mKillerMoveLists.find(depth);
		if (it==mKillerMoveLists.end())
			return NO_MOVE;
		return (*it).second.Get(index);
	}
	inline const KillerMoveList* Get(int depth) const {
		std::map<int, KillerMoveList>::const_iterator it = mKillerMoveLists.find(depth);
		if (it==mKillerMoveLists.end())
			return 0;
		const KillerMoveList& ref = (*it).second;
		return &ref;
	}
	inline Move Count(int depth) const {
		std::map<int, KillerMoveList>::const_iterator it = mKillerMoveLists.find(depth);
		if (it==mKillerMoveLists.end())
			return 0;
		return (*it).second.Count();
	}
	inline void Clear() {
		mKillerMoveLists.clear();
	}
	friend std::ostream& operator<<(std::ostream &out, const KillerMoves &v);
	
	static void Test();
private:
	std::map<int, KillerMoveList> mKillerMoveLists;
};

std::ostream& operator<<(std::ostream &out, const KillerMoveList &v);
std::ostream& operator<<(std::ostream &out, const KillerMoves &v);

#endif // KILLER_MOVES_INCLUDED
