#include <iostream>
#include "KillerMoves.h"

void KillerMoveList::Consider(Move move, int score) {
	if (move==NO_MOVE) return;
	if (score>WorstScore()) {
		// If list is full, remove worst move
		if (mCount==mMax) --mCount;
		
		// If move is already present:
		for (int i=0; i<mCount; ++i) {
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
			else return;
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
//		std::cout << "+" << move << ",";
	}
}

std::ostream& operator<< (std::ostream &out, const KillerMoveList &v) {
	for (int i=0; i<v.mCount; ++i)
		out << "<move ordinal='" << i << "' score='" << v.mScores[i] << "' position='" << v.mMoves[i] << "' />\n";
	return out;
}

std::ostream& operator<<(std::ostream &out, const KillerMoves &v) {
	std::map<int, KillerMoveList>::const_iterator it = v.mKillerMoveLists.begin();
	out << "<killermoves>\n";
	while (it!=v.mKillerMoveLists.end()) {
		int depth = (*it).first;
		const KillerMoveList& moves = (*it).second;
		++it;
		out << "    <movelist depth=\"" << depth << "\">\n" << moves << "</movelist>\n";
	}
	out << "</killermoves>\n";
	return out;
}



//////////////////////////////////

void KillerMoves::Test() {
	KillerMoves km;
	km.Consider(0, 0,15);
	km.Consider(0, 1,1);
	km.Consider(0, 2,20);
	km.Consider(0, 3,15);
	km.Consider(0, 4,15);
	km.Consider(0, 5,15);
	km.Consider(0, 2,15);
	km.Consider(0, 3,15);
	km.Consider(0, 6,15);
	km.Consider(0, 0,17);
	km.Consider(0, 0,1);
	km.Consider(0, 8,150);
	std::cout << km;
}

