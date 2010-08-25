#ifndef BOARD_INCLUDED
#define BOARD_INCLUDED
#include <limits.h>
#include <list>
#include <map>
#include <stack>
#include "KillerMoves.h"


class Board {
public:
	struct Params {
		int  window_size;
		int width;
		int height;
		int max_pieces;
		bool rules_horizontal_lines;
		bool rules_vertical_lines;
		bool rules_oblique_lines;
		bool engine_use_killer_moves;
		Params()
			: window_size(3)
			, width(4)
			, height(4)
			, max_pieces(3)
			, rules_horizontal_lines(true)
			, rules_vertical_lines(true)
			, rules_oblique_lines(true)
			, engine_use_killer_moves(true)
		{ }
	};
public:
	Board(int size, int max_pieces);
	Board(const Board& o);
	Board& operator=(const Board& o);
	virtual ~Board();
	inline int Get(int index) const { return mBoard[index]; }
	inline void Set(int index, int value) {
		int& v = mBoard[index];
		if (value==EMPTY && v!=EMPTY) --mCount;
		else if (v==EMPTY) ++mCount;
		v=value;
	}
	inline void Set(int x, int y, int value) {
		Set(x+y*mSize, value);
	}
	inline int Get(int x, int y) const {
		return Get(x+y*mSize);
	}
	inline int Size() const { return mSize; }
	inline int MaxPieces() const { return mMaxPieces; }
	inline int Count() const { return mCount; }
	inline int RemainingCells() const { return mSize*mSize-mCount; }
	//
	inline Move FromXY(int x, int y) const { return x+y*mSize; }
	inline int ToX(Move move) const { return move%mSize; }
	inline int ToY(Move move) const { return move/mSize; }
	//
	int Score() const;
	int Score2() const;
	void ComputeMove(Move& move, int max_depth=INT_MAX);
	void ComputeMoveWithAlphaBeta(Move& move, int max_depth);
	void ComputeMoveWithNegascout(Move& move, int max_depth);
	void ComputeMoveWithNegamax(Move& move, int max_depth);
	
	void SetKMDB(KillerMoves* db) { kmdb = db; }
	inline void Push(Move move) {
		Set(move, mCount%mMaxPieces);
		mMoves.push(move);
	}
	inline Move LastMove() const {
		if (!mMoves.empty())
			return mMoves.top();
		return NO_MOVE;
	}
	inline void Pop() {
		if (mMoves.empty())
			return;
		Set(mMoves.top(), EMPTY);
		mMoves.pop();
	}
private:
	bool NextMove(Move& move) const;
	int AlphaBeta(int a, int b);
	int NegaScout(int a, int b);
	int NegaMax();
private:
	static const int EMPTY = -1;
	unsigned int mSize;
	unsigned int mMaxPieces;
	unsigned int mCount;
	unsigned int mTempDepth;
	std::stack<Move> mMoves;
	int* mBoard;
	KillerMoves* kmdb;
	Params mParams;
	
};

#endif // BOARD_INCLUDED
