#include <iostream>
#include <list>
#include <time.h>

#include "Board.h"

Board::Board(int size, int max_pieces)
: mCount(0)
, mSize(size)
, mMaxPieces(max_pieces)
, mTempDepth(0)
, mBoard(0)
, mMoves()
, mParams()
, kmdb(0)
{
	mParams.width = size;
	mParams.height = size;
	mParams.max_pieces = max_pieces;
	mBoard=new int[mSize*mSize];
	for (unsigned int i=0; i<mSize*mSize; ++i)
		mBoard[i]=EMPTY;
}
Board::Board(const Board& o)
: mCount(o.mCount)
, mSize(o.mSize)
, mMaxPieces(o.mMaxPieces)
, mTempDepth(o.mTempDepth)
, mBoard(0)
, mMoves()
, mParams(o.mParams)
, kmdb(o.kmdb)
{
	mBoard=new int[mSize*mSize];
	for (unsigned int i=0; i<mSize*mSize; ++i)
		mBoard[i]=o.mBoard[i];
}
Board& Board::operator=(const Board& o) {
	mCount=o.mCount;
	mMaxPieces=o.mMaxPieces;
	mParams=o.mParams;
	kmdb=o.kmdb;
	if (mSize!=o.mSize) {
		mSize=o.mSize;
		delete mBoard;
		mBoard=new int[mSize*mSize];
	}
	for (unsigned int i=0; i<mSize*mSize; ++i)
		mBoard[i]=o.mBoard[i];
	return *this;
}
Board::~Board() {
	delete mBoard;
}

int Board::Score2() const {
	bool won=false;

	// Find horizontal lines
	if (mParams.rules_horizontal_lines) {
		for (int y=0; y<mParams.height && !won; ++y) {
			int x=0;
			int count = 1;
			int piece = Get(x,y);
			while(++x<mParams.width && !won) {
				int p=Get(x,y);
				if (p==piece) ++count;
				else {
					piece=p;
					count=1;
				}
				won|=(count==mParams.window_size && p!=EMPTY);
			}
		}
	}
	
	// Find vertical lines
	if (mParams.rules_vertical_lines) {
		for (int x=0; x<mParams.width && !won; ++x) {
			int y=0;
			int piece = Get(x,y);
			int count = 1;
			while(++y<mParams.height && !won) {
				int p=Get(x,y);
				if (p==piece) ++count;
				else {
					piece=p;
					count=1;
				}
				won|=(count==mParams.window_size && p!=EMPTY);
			}
		}
	}
	
	// Find oblique lines
	if (mParams.rules_oblique_lines) {
		int n = mParams.width+mParams.height-1;
		
		for (int z=0; z<n && !won; ++z) {
			int x = z;
			int y = 0;
			if (x>=mParams.width) {
				y=1+(x%mParams.width);
				x=0;
			}
			int piece = Get(x,y);
			int count = 1;
			while(++x<mParams.width && ++y<mParams.height && !won) {
				int p=Get(x,y);
				if (p==piece) ++count;
				else {
					piece=p;
					count=1;
				}
				won|=(count==mParams.window_size && p!=EMPTY);
			}
//		}
		
//		for (int z=0; z<n && !won; ++z) {
			//int
			x = z;
			//int
			y = 0;
			if (x>=mParams.width) {
				y=1+(x%mParams.width);
				x=0;
			}
			y=mParams.height-1-y;
			//int
			piece = Get(x,y);
			//int
			count = 1;
			while(++x<mParams.width && --y>=0 && !won) {
				int p=Get(x,y);
				if (p==piece) ++count;
				else {
					piece=p;
					count=1;
				}
				won|=(count==mParams.window_size && p!=EMPTY);
			}
		}
	}
	
	int score = (won?(2+RemainingCells()):((RemainingCells()==0)?1:0));
	return score;
}
 
int Board::Score() const {
	Move last = LastMove();
	if (last==NO_MOVE) return 0;
	int piece = mBoard[last];
	int won=2+RemainingCells();

	if (mCount>=mMaxPieces*3) {
		for (unsigned int y=1; y<mSize-1; ++y) for (unsigned int x=1; x<mSize-1; ++x) {
			int nw = Get(x-1,y-1);
			int n = Get(x,y-1);
			int ne = Get(x+1,y-1);
			int w = Get(x-1,y);
			int c = Get(x,y);
			int e = Get(x+1,y);
			int sw = Get(x-1,y+1);
			int s = Get(x,y+1);
			int se = Get(x+1,y+1);
			if (((y==1) && ((nw!=EMPTY && nw==n && nw==ne) || (w!=EMPTY && w==c && w==e))) ||
			  (sw!=EMPTY && sw==s && se==s) ||
			  ((x==1) && ((nw!=EMPTY && nw==w && nw==sw) || (n!=EMPTY && n==c && n==s))) ||
			  (ne!=EMPTY && ne==e && ne==se) ||
			  (c!=EMPTY && c==nw && c==se) ||
			  (c!=EMPTY && c==ne && c==sw)) return won;
		}
	}
	return (RemainingCells()==0)?1:0;
//	return (won?(2+RemainingCells()):((RemainingCells()==0)?1:0));
}

void Board::ComputeMove(Move& move, int max_depth) {
	Move negascout, negascoutkm,alphabeta;
	bool disagreement = false;
	clock_t then;
	
	std::cout << "* NegaScout\n";
	mParams.engine_use_killer_moves=false;
	then = clock();
	ComputeMoveWithNegascout(negascout, max_depth);
	std::cout << "Timed: " << (clock()-then) << " clocks.\n";
	
	std::cout << "* NegaScoutWithKillerMoves\n";
	mParams.engine_use_killer_moves=true;
	then = clock();
	ComputeMoveWithNegascout(negascoutkm, max_depth);
	std::cout << "Timed: " << (clock()-then) << " clocks.\n";
	disagreement|=(negascoutkm!=negascout);

	std::cout << "* AlphaBeta\n";
	mParams.engine_use_killer_moves=false;
	then = clock();
	ComputeMoveWithAlphaBeta(alphabeta, max_depth);
	std::cout << "Timed: " << (clock()-then) << " clocks.\n";
	disagreement|=(alphabeta!=negascout);

	
	move=negascout;
	if (disagreement) {
		std::cout << "NegaScout wants to move to [" << ToX(negascout) << ", " << ToY(negascout) << "]\n";
		if (negascoutkm!=move)
			std::cout << "...but NegaScoutWithKillerMoves disagrees and wants to move to [" << ToX(negascoutkm) << ", " << ToY(negascoutkm) << "]\n";
		if (alphabeta!=move)
			std::cout << "...but AlphaBeta disagrees and wants to move to [" << ToX(alphabeta) << ", " << ToY(alphabeta) << "]\n";
	}
	else
		std::cout << "All agree to move to [" << ToX(move) << ", " << ToY(move) << "]\n";
		
}

void Board::ComputeMoveWithNegascout(Move& move, int max_depth) {
	int first_depth = max_depth;
	int last_depth = max_depth;
	int a=-INT_MAX;
	if (kmdb) kmdb->Clear();
	//std::cout << "best: ";
	for (int depth = first_depth; depth <= last_depth; ++depth) {
		mTempDepth=depth;
		Move m=NO_MOVE;
		while (NextMove(m)) {
			Push(m);
			int score = -NegaScout(-INT_MAX, -a);
			if (score>a) {
				a=score;
				move=m;
				//std::cout << score << ", ";
			}
			Pop();
		}
	}
	//std::cout << "\n";
}

void Board::ComputeMoveWithAlphaBeta(Move& move, int max_depth) {
	mTempDepth=max_depth;
	int a=-INT_MAX;
	Move m=NO_MOVE;
	//std::cout << "best: ";
	while (NextMove(m)) {
		Push(m);
		int score = -AlphaBeta(-INT_MAX, -a);
		if (score>a) {
			a=score;
			move=m;
			//std::cout << score << ", ";
		}
		Pop();
	}
	//std::cout << "\n";
}

void Board::ComputeMoveWithNegamax(Move& move, int max_depth) {
	int a=-INT_MAX;
	mTempDepth=max_depth;
	Move m=NO_MOVE;
	//std::cout << "best: ";
	while (NextMove(m)) {
		Push(m);
		int score = -NegaMax();
		if (score>a) {
			a=score;
			move=m;
			//std::cout << score << ", ";
		}
		Pop();
	}
	//std::cout << "\n";
}


bool Board::NextMove(Move& move) const {
	
	bool last_was_a_killer=false;
	int killer_moves = 0;
	const KillerMoveList* kml = 0;
	if (kmdb) kml=kmdb->Get(mCount);
	if (kml) killer_moves = kml->Count();
	if (mParams.engine_use_killer_moves && kml) {
		for (int kmi=0; kmi<killer_moves; ++kmi) {
			Move km= kml->Get(kmi);
			// On start, get the first available killer move
			if (move==NO_MOVE && Get(km)==EMPTY) {
				move=km;
				return true;
			}
			// If we start from a previous killer mode, get the next one available
			if (move==km) {
				last_was_a_killer=true;
				for (int kmj=kmi+1; kmj<killer_moves; ++kmj) {
					Move next=kml->Get(kmj);
					if (Get(next)==EMPTY) {
						move=next;
						return true;
					}
				}
			}
		}
		if (last_was_a_killer) move=NO_MOVE;
	}

	if (move==NO_MOVE) move=0;
	else ++move;
	
	while(move<mSize*mSize) {
		if (Get(move)==EMPTY) {
			bool skip=false;
			if (mParams.engine_use_killer_moves && kml) {
				for (int zzz=0; zzz<killer_moves; ++zzz) {
					Move z = kml->Get(zzz);
					if (move==z) {
						skip=true;
						break;
					}
				}
			}
			if (!skip) {
				return true;
			}
		}
		++move;
	}
	move=NO_MOVE;
	return false;
}

int Board::AlphaBeta(int /*a*/, int b) {
	int value = -Score();
	--mTempDepth;
	if (value==0 && mTempDepth!=0) {
		Move move=NO_MOVE;
		// Improved AlphaBeta, Fail-Soft version (F3)
		// Finds a better value when it's out of the search window.
		int m=-INT_MAX;
		// Original AlphaBeta, Fail-Hard version (F2)
		// int m=a;
		while (NextMove(move)) {
			Push(move);
			int v = -AlphaBeta(-b, -m);
			Pop();
			m=(v>m)?v:m;
			if (b<=m) {
				if(mParams.engine_use_killer_moves && kmdb)
					kmdb->Consider(mCount, move, m);
				break;
			}
		}
		value=m;
	}
	++mTempDepth;
	return value;
}

int Board::NegaScout(int a, int b) {
	int value = -Score();
	--mTempDepth;
	if (value==0 && mTempDepth!=0) {
#define USE_FAIL_SOFT
#ifdef USE_FAIL_SOFT
		int m=-INT_MAX;
		int n=b;
		Move move=NO_MOVE;
		while(NextMove(move)) {
			Push(move);
			int t=-NegaScout(-n,-(a>m?a:m));
			if (t>m) {
				if (n==b || mTempDepth<=2)
					m=t;
				else
					m=-NegaScout(-b, -t);
			}
			Pop();
			if (b<=m) {
				if(mParams.engine_use_killer_moves && kmdb)
					kmdb->Consider(mCount, move, m);
				break;
			}
			n=(a>m?a:m)+1;
		}
		value = m;
#else
		int aa = a;
		int bb = b;
		bool first=true;
		Move move=NO_MOVE;
		while(NextMove(move)) {
			Push(move);
			int t = -NegaScout (-bb, -aa);
			if ((t>aa) && (t<b) && (!first))
				aa = -NegaScout(-b, -t);
			Pop(move);
			first=false;
			
			aa = (aa>t)?aa:t;
			if (b<=aa) {
				if(mParams.engine_use_killer_moves && kmdb)
					kmdb->Consider(mCount, move, aa);
				break;
			}
			bb = aa + 1;
		}
		value=aa;
#endif
	}
	++mTempDepth;
	return value;
}

int Board::NegaMax() {
	int value = -Score();
	--mTempDepth;
	if (value==0 && mTempDepth!=0) {
		int bestvalue=-INT_MAX;
		Move move=NO_MOVE;
		while(NextMove(move)) {
			Push(move);
			int v = -NegaMax();
			Pop();
			bestvalue = (v>bestvalue?v:bestvalue);
		}
		value = bestvalue;
	}
	++mTempDepth;
	return value;
}

