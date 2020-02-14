#ifndef _CHECKERS_GAMESTATE_HPP_
#define _CHECKERS_GAMESTATE_HPP_

#include "constants.hpp"
#include "move.hpp"
#include <stdint.h>
#include <cassert>
#include <cstring>
#include <iostream>
#include <string>

namespace checkers
{

/**
 * Represents a game state with a 8x8 board
 *
 * Cells are numbered as follows:
 *
 *    col 0  1  2  3  4  5  6  7
 * row  -------------------------
 *  0  |     0     1     2     3 |  0
 *  1  |  4     5     6     7    |  1
 *  2  |     8     9    10    11 |  2
 *  3  | 12    13    14    15    |  3
 *  4  |    16    17    18    19 |  4
 *  5  | 20    21    22    23    |  5
 *  6  |    24    25    26    27 |  6
 *  7  | 28    29    30    31    |  7
 *      -------------------------
 *        0  1  2  3  4  5  6  7
 *
 * The staring board looks like this:
 *
 *    col 0  1  2  3  4  5  6  7
 * row  -------------------------
 *  0  |    rr    rr    rr    rr |  0
 *  1  | rr    rr    rr    rr    |  1
 *  2  |    rr    rr    rr    rr |  2
 *  3  | ..    ..    ..    ..    |  3
 *  4  |    ..    ..    ..    .. |  4
 *  5  | ww    ww    ww    ww    |  5
 *  6  |    ww    ww    ww    ww |  6
 *  7  | ww    ww    ww    ww    |  7
 *      -------------------------
 *        0  1  2  3  4  5  6  7
 *
 * The red player starts from the top of the board (row 0,1,2)
 * The white player starts from the bottom of the board (row 5,6,7),
 * Red moves first.
 */
class GameState
{
public:
	static const int cSquares = 32;		// 32 valid squares
	static const int cPlayerPieces = 12;	// 12 pieces per player
	static const int cMovesUntilDraw = 50;	///< 25 moves per player

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
	 * Constructs a state that is the result of rotating the board 180 degrees and swapping colors
	 *
	 */
	GameState reversed() const;

	/**
	 * Returns the content of a cell in the board.
	 *
	 * This function returns a byte representing the contents of the cell,
	 * using the enumeration values in ECell
	 *
	 * For example, to check if cell 23 contains a white piece, you would check if
	 *
	 * (lBoard.At(23)&CELL_WHITE)
	 *
	 * to check if it is a red piece,
	 *
	 *   (lBoard.At(23)&CELL_RED)
	 *
	 * and to check if it is a king, you would check if
	 *
	 *   (lBoard.At(23)&CELL_KING)
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
	 * Rows are numbered (0 to 7) from the upper row in the board,
	 * as seen by the player this program is playing.
	 *
	 * Columns are numbered starting from the left (also 0 to 7).
	 *
	 * Cells corresponding to white squares in the board, which will
	 * never contain a piece, always return CELL_INVALID
	 *
	 * If the cell falls outside of the board, return CELL_INVALID
	 *
	 * You can use it in the same way as the version that requires a cell index
	 */
	uint8_t at(int pR, int pC) const
	{
		if (pR < 0 || pR > 7 || pC < 0 || pC > 7)
			return CELL_INVALID;
		if ((pR & 1) == (pC & 1))
			return CELL_INVALID;
		return mCell[pR * 4 + (pC >> 1)];
	}

private:
	///private version of above function (allows modifying cells)
	uint8_t &mutableAt(int pR, int pC) const
	{
		//this is a bit ugly, but is useful for the implementation of
		//findPossibleMoves. It won't affect in single-threaded programs
		//and you're not allowed to use threads anyway
		return const_cast<uint8_t&>(mCell[pR * 4 + (pC >> 1)]);
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
		int lC = ((pCell) & 3) << 1;
		if (!((pCell) & 4))
			lC++;
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
		return (pRow * 4 + (pCol >> 1));
	}

private:
	/**
	 * Tries to make a jump from a certain position of the board
	 *
	 * \param pMoves a vector where the valid moves will be inserted
	 * \param pOther the \ref ECell code corresponding to the player who is not making the move
	 * \param pR the row of the cell we are moving from
	 * \param pC the col
	 * \param pKing true if the moving piece is a king
	 * \param pBuffer a buffer where the list of jump positions is
	 * inserted (for multiple jumps)
	 * \param pDepth the number of multiple jumps before this attempt
	 */
	bool tryJump(std::vector<Move> &pMoves, int pR, int pC, bool pKing,
			uint8_t *pBuffer, int pDepth = 0) const;

	/**
	 * Tries to make a move from a certain position
	 *
	 * \param pMoves vector where the valid moves will be inserted
	 * \param pCell the cell where the move is tried from
	 * \param pOther the \ref ECell code corresponding to the player
	 * who is not making the move
	 * \param pKing true if the piece is a king
	 */
	void tryMove(std::vector<Move> &pMoves, int pCell, bool pKing) const;

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

	/*
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

	const uint8_t getMovesUntilDraw() const
	{
		return mMovesUntilDraw;
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
	/// returns true if the game ended in red win
	bool isRedWin() const
	{
		return mLastMove.isRedWin();
	}
	/// returns true if the game ended in white win
	bool isWhiteWin() const
	{
		return mLastMove.isWhiteWin();
	}
	/// returns true if the game ended in draw
	bool isDraw() const
	{
		return mLastMove.isDraw();
	}

private:
	uint8_t mCell[cSquares];
	uint8_t mMovesUntilDraw;
	uint8_t mNextPlayer;
	Move mLastMove;
};

/*namespace checkers*/}

#endif
