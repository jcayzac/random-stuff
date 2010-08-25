#include <iostream>
#include <inttypes.h>
#include <limits.h>

#include "DecisionMaker.h"
#include "Board.h"
#include "KillerMoves.h"
//#define DUMP


enum TCellState {
	STATE_EMPTY=-1,
	STATE_X=0,
	STATE_O,
	STATE_OX,
	STATE_FARMER
};

#define BOARD_SIZE 6
#define MAX_PIECES 3


static inline const char* pieceName(int p) {
		switch (p) {
			case STATE_EMPTY:	return " ";
			case STATE_O:		return "O";
			case STATE_X:		return "X";
			case STATE_OX:		return "*";
			default:			return "-";
		}
	}
static void Print(Board& board, Move m) {
	int size = board.Size();
	int count = board.Count();
	for (int y=0; y<size; ++y) {
		for (int z=0; z<size; ++z) std::cout << "+---"; std::cout << "+\n|";
		for (int x=0; x<size; ++x) {
			Move p = x + y*size;
			if (p==m)
				std::cout << "[" << pieceName(board.Get(p)) << "]|";
			else
				std::cout << " " << pieceName(board.Get(p)) << " |";
		}
		std::cout << "    ";
		if (y==0 && count!=0 && m!=NO_MOVE)
			std::cout << "Player " << 1+((count-1)%2) <<	
			": ["<< board.ToX(m) << ", " << board.ToY(m) << "]";
		std::cout << "\n";
	}
	for (int z=0; z<size; ++z) std::cout << "+---"; std::cout << "+\n\n";
}

void setInitialGame(Board& board) {
	board.Set(2,0,STATE_X);
	board.Set(1,2,STATE_O);
//	board.Set(2,1,STATE_OX);
//	board.Set(3,2,STATE_X);
//	board.Set(2,2,STATE_O);
//	board.Set(0,1,STATE_OX);
}


int main() {
	//KillerMoves::Test();
	//return 0;
	
	Board board(BOARD_SIZE, MAX_PIECES);
	setInitialGame(board);
	std::cout << "INITIAL GAME:\n";
	Move move=NO_MOVE;
	Print(board, move);
	
	KillerMoves p1db, p2db;
	for(int turn=board.Count();turn<BOARD_SIZE*BOARD_SIZE;++turn) {
		std::cout << "* Turn " << 1+turn << ":\n";
		
		board.SetKMDB((turn%2==0)?&p1db:&p2db);
		board.ComputeMove(move,7);
		board.Push(move);
		
		Print(board, move);

		int value = board.Score();
		if (value) {
			std::cout << "Exit with value = " << value << "\n";
			break;
		}
	}
	return 0;
}
