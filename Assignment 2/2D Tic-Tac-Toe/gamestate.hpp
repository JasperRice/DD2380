#ifndef _TICTACTOE_GAMESTATE_HPP_
#define _TICTACTOE_GAMESTATE_HPP_

#include "constants.hpp"
#include "move.hpp"
#include <stdint.h>
#include <cassert>
#include <cstring>
#include <iostream>
#include <string>

namespace TICTACTOE
{

	/**
	* Represents a game state with a 4x4 board
	*
	* Cells are numbered as follows:
	*
	*    col 0  1  2  3
	* row  ---------------
	*  0  |  0  1  2  3  |  0
	*  1  |  4  5  6  7  |  1
	*  2  |  8  9  10 11 |  2
	*  3  | 12  13 14 15 |  3
	*      ---------------
	*        0  1  2  3

	*
	* The staring board looks like this:
	*
	*    col 0  1  2  3
	* row  ---------------
	*  0  |  .  .  .  .  |  0
	*  1  |  .  .  .  .  |  1
	*  2  |  .  .  .  .  |  2
	*  3  |  .  .  .  .  |  3
	*      ---------------
	*        0  1  2  3
	*
	* O moves first.
	*/
	class GameState
	{
	public:
		static const int cSquares = 16;		// 16 valid squares

		/**
		* Initializes the board to the starting position
		*/
		GameState();

		/**
		* Constructs a board from a message string
		*
		* \param pMessage the compact string representation of the state
		*/
		GameState(const std::string &pMessage);

		/**
		* Constructs a board which is the result of applying move \p pMove to board \p pRH
		*
		* \param pRH the starting board position
		* \param pMove the movement to perform
		* \sa DoMove()
		*/
		GameState(const GameState &pRH, const Move &pMove);


		/**
		* Returns the content of a cell in the board.
		*
		* This function returns a byte representing the contents of the cell,
		* using the enumeration values in ECell
		*
		* For example, to check if cell 10 contains a O piece, you would check if
		*
		* (lBoard.At(10)&CELL_O)
		*
		* to check if it is a X piece,
		*
		*   (lBoard.At(10)&CELL_X)
		*
		*/
		uint8_t& at(int pPos)
		{
			assert(pPos >= 0);
			assert(pPos < cSquares);
			return mCell[pPos];
		}

		/**
		* const version of the above function
		*/
		const uint8_t& at(int pPos) const
		{
			assert(pPos >= 0);
			assert(pPos < cSquares);
			return mCell[pPos];
		}

		/**
		* returns the content of a cell in the board.
		*
		* Rows are numbered (0 to 3) from the upper row in the board,
		* as seen by the player this program is playing.
		*
		* Columns are numbered starting from the left (also 0 to 3).
		*
		* If the cell falls outside of the board, return CELL_INVALID
		*
		* You can use it in the same way as the version that requires a cell index
		*/
		uint8_t at(int pR, int pC) const
		{
			if (pR < 0 || pR > 3 || pC < 0 || pC > 3)
			return CELL_INVALID;
			return mCell[pR * 4 + pC ];
		}

	private:
		///private version of above function (allows modifying cells)
		uint8_t &mutableAt(int pR, int pC) const
		{
			//this is a bit ugly, but is useful for the implementation of
			//findPossibleMoves. It won't affect in single-threaded programs
			//and you're not allowed to use threads anyway
			return const_cast<uint8_t&>(mCell[pR * 4 + pC]);
		}

	public:

		///returns the row corresponding to a cell index
		static int cellToRow(int pCell)
		{
			return ((pCell) >> 2);
		}

		///returns the col corresponding to a cell index
		static int cellToCol(int pCell)
		{
			int lC = ((pCell) & 3);
			return lC;
		}

		///returns the cell corresponding to a row and col

		///It doesn't check if it corresponds to a black square in the board,
		///or if it falls within the board.
		///
		///If it doesn't, the result is undefined, and the program is likely
		///to crash
		static int rowColToCell(int pRow, int pCol)
		{
			return (pRow * 4 + pCol);
		}

	private:

		/**
		* Tries to make a move in a certain position
		*
		* \param pMoves vector where the valid moves will be inserted
		* \param pCell the cell where the move is tried from
		* \param pOther the \ref ECell code corresponding to the player
		* who is not making the move
		*/
		void tryMove(std::vector<Move> &pMoves, int pCell) const;


	private:

		/**
		* Checks if a move end up being a special move(Winning=1, Draw =2) *
		* \param pCell the cell where the move is tried
		* \param pPlayer says who is making the move
		*/
		int Special_Move(int pCell, Cell pPlayer) const
		{
			int lR=cellToRow(pCell);
			int lC=cellToCol(pCell);
			//check if winning move:
			int checkWin[4][4] = {
				{at(0,0), at(0,1), at(0,2), at(0,3)} ,
				{at(1,0), at(1,1), at(1,2), at(1,3)} ,
				{at(2,0), at(2,1), at(2,2), at(2,3)} ,
				{at(3,0), at(3,1), at(3,2), at(3,3)}
			};
			//make the move temporaly:
			checkWin[lR][lC]=pPlayer;

			//Check horizontal
			int Pcount=0;
			for(int i = 0;i < 4; i++) {
				for(int j = 0; j < 4;j++) {
					if(checkWin[i][j]==pPlayer) {
						Pcount=Pcount+1;
					}
				}
				if(Pcount==4)
				return 1;
				Pcount=0;
			}
			//Check verticly
			Pcount=0;
			for(int i = 0;i < 4; i++) {
				for(int j = 0; j < 4;j++) {
					if(checkWin[j][i]==pPlayer) {
						Pcount=Pcount+1;
					}
				}
				if(Pcount==4)
				return 1;
				Pcount=0;
			}
			//Check diagonals
			Pcount=0;
			int j=0;
			for(int i = 0;i < 4; i++) {
				if(checkWin[j][i]==pPlayer) {
					Pcount=Pcount+1;
				}
				if(Pcount==4)
				return 1;
				j++;
			}
			Pcount=0;
			j=3;
			for(int i = 0;i < 4; i++) {
				if(checkWin[j][i]==pPlayer) {
					Pcount=Pcount+1;
				}
				if(Pcount==4)
				return 1;
				j--;
			}
			//Check Draw
			//count pieces
			int cpieces=0;
			for (int k = 0; k < cSquares; ++k)
			{
				// Try move
				if(at(k)!=CELL_EMPTY)
				{
					cpieces+=1;
				}
			}

			if(cpieces==cSquares-1 )
			return 2;

			return 0;
		}

	public:
		/**
		* Returns a list of all valid moves for \p pWho
		*
		* \param pMoves a vector where the list of moves will be appended
		* \param pWho the \ref ECell code (CELL_OWN or CELL_OTHER) of the
		* player making the move
		*/
		void findPossibleMoves(std::vector<GameState> &pMoves) const;

		/**
		* Transforms the board by performing a move
		*
		* It doesn't check that the move is valid, so you should only use
		* it with moves returned by findPossibleMoves
		* \param pMove the move to perform
		*/
		void doMove(const Move &pMove);

		/**
		* Compares two game states. Return true if game states are identical, otherwise false.
		*
		* \param gameState game state to compare to
		*/
		bool isEqual(GameState gameState)
		{
			bool equal = true;
			for (int i = 0; i < cSquares; ++i)
			if (mCell[i] != gameState.at(i))
			equal = false;
			if (mNextPlayer != gameState.getNextPlayer())
			equal = false;
			if (mLastMove.toMessage().compare(gameState.getMove().toMessage()) != 0)
			equal = false;
			return equal;
		}

		/**
		* Convert the board to a human readable string ready to be printed to std::cerr
		*
		* Useful for debug purposes. Don't call it in the final version.
		*/
		std::string toString(int pPlayer) const;

		/**
		* Convert the board to a machine readable string ready to be printed to std::cout
		*
		* This is used for passing board states between bots
		*/
		std::string toMessage() const;

		/**
		* Get the last move made (the move that lead to this state)
		*/
		const Move& getMove() const
		{
			return mLastMove;
		}

		const uint8_t getNextPlayer() const
		{
			return mNextPlayer;
		}

		/// returns true if the movement marks beginning of game
		bool isBOG() const
		{
			return mLastMove.isBOG();
		}
		/// returns true if the movement marks end of game
		bool isEOG() const
		{
			return mLastMove.isEOG();
		}
		/// returns true if the game ended in X win
		bool isXWin() const
		{
			return mLastMove.isXWin();
		}
		/// returns true if the game ended in white win
		bool isOWin() const
		{
			return mLastMove.isOWin();
		}
		/// returns true if the game ended in draw
		bool isDraw() const
		{
			std::cerr << "gamestate send draw" << std::endl;
			return mLastMove.isDraw();
		}

	private:
		uint8_t mCell[cSquares];
		uint8_t mNextPlayer;
		Move mLastMove;
	};

/*namespace TICTACTOE*/}

#endif
