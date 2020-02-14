#include "constants.hpp"
#include "gamestate.hpp"
#include <cstdlib>
#include <inttypes.h>

namespace TICTACTOE3D
{

/**
 * Initializes the board to the starting position
 */
GameState::GameState()
{
	// Initialize the board (empty)
	for(int i=0;i<64;i++)
	{
		mCell[i]=CELL_EMPTY;
	}	
	// Initialize move related variables
	mLastMove = Move(Move::MOVE_BOG);
	// Player X starts
	mNextPlayer = CELL_X;
}

/**
 * Constructs a board from a message string
 *
 * \param pMessage the compact string representation of the state
 */
GameState::GameState(const std::string &pMessage)
{	
	// Split the message with a stringstream
	std::stringstream ss(pMessage);
	std::string board, last_move, next_player;
	ss >> board >> last_move >> next_player;

	assert(board.size() == (unsigned)cSquares);
	assert(next_player.size() == 1);
	
	// Parse the board
	for (int i = 0; i < cSquares; ++i)
	{
		if (board[i] == MESSAGE_SYMBOLS[CELL_EMPTY])
			mCell[i] = CELL_EMPTY;
		else if (board[i] == MESSAGE_SYMBOLS[CELL_X])
			mCell[i] = CELL_X;
		else if (board[i] == MESSAGE_SYMBOLS[CELL_O])
			mCell[i] = CELL_O;
		else
			assert("Invalid cell" && false);
	}

	// Parse last move
	mLastMove = Move(last_move);

	// Parse next player
	if (next_player[0] == MESSAGE_SYMBOLS[CELL_EMPTY])
		mNextPlayer = CELL_EMPTY;
	else if (next_player[0] == MESSAGE_SYMBOLS[CELL_X])
		mNextPlayer = CELL_X;
	else if (next_player[0] == MESSAGE_SYMBOLS[CELL_O])
		mNextPlayer = CELL_O;
	else
	{
		std::cerr << "Invalid next player" << std::endl;
		assert(false);
		exit(-1);
	}
}

/**
 * Constructs a board which is the result of applying move \p pMove to board \p pRH
 *
 * \param pRH the starting board position
 * \param pMove the movement to perform
 * \sa DoMove()
 */
GameState::GameState(const GameState &pRH,const Move &pMove)
{

	// Copy board
    memcpy(mCell, pRH.mCell, sizeof(mCell));

    // Copy move status
    mNextPlayer     = pRH.mNextPlayer;
    mLastMove       = pRH.mLastMove;

    // Perform move
    doMove(pMove);

}


/**
 * Tries to make a move on a certain position *
 * \param pMoves vector where the valid moves will be inserted
 * \param pCell the cell where the move is tried
 * \param pOther the \ref ECell code corresponding to the player
 * who is not making the move

 */
void GameState::tryMove(std::vector<Move> &pMoves, int pCell) const
{
    int lR=cellToRow(pCell);
    int lC=cellToCol(pCell);
	int lL=cellToLay(pCell);
	int SpecialMove=0;
	
    // Try move X
    if(mNextPlayer==CELL_X)
    {
        // Try move
        if(at(lR,lC,lL)==CELL_EMPTY)
		{
			//Check if special move
			SpecialMove = GameState::Special_Move(pCell,CELL_X);
			
			if(SpecialMove>0)
			{
				pMoves.push_back(Move(pCell,CELL_X,SpecialMove));
			}
			else
			{
				pMoves.push_back(Move(pCell,CELL_X)); 
			}
		}                   
    }
    // Try move O
    if(mNextPlayer==CELL_O)
    {
        // Try move
        if(at(lR,lC,lL)==CELL_EMPTY)
		{
			//Check if special move
			SpecialMove = GameState::Special_Move(pCell,CELL_O);
			
			if(SpecialMove>0)
			{
				pMoves.push_back(Move(pCell,CELL_O,SpecialMove));
			}
			else
			{
				pMoves.push_back(Move(pCell,CELL_O)); 
			}
		}                  
    }
}


/**
 * Returns a list of all valid moves for \p pWho
 *
 * \param pMoves a vector where the list of moves will be appended
 * \param pWho the \ref ECell code (CELL_OWN or CELL_OTHER) of the
 * player making the move
 */
void GameState::findPossibleMoves(std::vector<GameState> &pStates) const
{
    pStates.clear();

    if (mLastMove.isEOG())
    	return;

	std::vector<Move> lMoves;
	
    for (int k = 0; k < cSquares; ++k)
    {
        int lCell = k;       
        tryMove(lMoves, lCell);		
    }   
	
    // Convert moves to GameStates
    for (unsigned i = 0; i < lMoves.size(); ++i)
    	pStates.push_back(GameState(*this, lMoves[i]));	
    
}

/**
 * Transforms the board by performing a move
 *
 * It doesn't check that the move is valid, so you should only use
 * it with moves returned by findPossibleMoves
 * \param pMove the move to perform
 */
void GameState::doMove(const Move &pMove)
{
   
   // set the piece
		at(pMove[0]) = pMove[1];
    
    // Remember last move
    mLastMove = pMove;

    // Swap player
    mNextPlayer = mNextPlayer ^ (CELL_X | CELL_O);

}

/**
 * Convert the board to a human readable string ready to be printed to std::cerr
 *
 * Useful for debug purposes. Don't call it in the final version.
 */
std::string GameState::toString(int pPlayer) const
{
	// Select preferred printing style by setting cell_text to SIMPLE_TEXT, UNICODE_TEXT or COLOR_TEXT
	static const std::string *cell_text = COLOR_TEXT;
	static const std::string board_top    = (cell_text == SIMPLE_TEXT) ? "     -----------------\n" : "    ╭─────────╮        ╭─────────╮        ╭─────────╮        ╭─────────╮\n";
	static const std::string board_bottom = (cell_text == SIMPLE_TEXT) ? "     -----------------\n" : "    ╰─────────╯        ╰─────────╯        ╰─────────╯        ╰─────────╯\n";
	static const std::string board_left   = (cell_text == SIMPLE_TEXT) ? "| " : "│ ";
	static const std::string board_right  = (cell_text == SIMPLE_TEXT) ? "|" : "│";

	bool is_winner = (isEOG() && ((pPlayer == CELL_X && isXWin()) || (pPlayer == CELL_O && isOWin())));
	bool is_my_turn = (mNextPlayer == pPlayer);
	int X_pieces = 0;
	int O_pieces = 0;
	
	for (int i = 0; i < cSquares; ++i)
	{
		if (at(i) & CELL_X)
			++X_pieces;
		else if (at(i) & CELL_O)
			++O_pieces;
	}

	// Use a stringstream to compose the string
	std::stringstream ss;

	// Draw the board with numbers around it indicating cell index and put text to the right of the board
	ss << "      layer 0            layer 1            layer 2            layer 3 \n";
	ss << board_top;
	ss << "  0 " << board_left; for(int c=0;c<4;++c) ss << cell_text[at(0,c,0)]; ss << board_right << " 3  " << " 16 " << board_left; for(int c=0;c<4;++c) ss << cell_text[at(0,c,1)]; ss << board_right << " 19 " << " 32 " << board_left; for(int c=0;c<4;++c) ss << cell_text[at(0,c,2)]; ss << board_right << " 35 " << " 48 " << board_left; for(int c=0;c<4;++c) ss << cell_text[at(0,c,3)]; ss << board_right << " 51  Last move: " + mLastMove.toString() + (is_winner ? " (WOHO! I WON!)\n" : "\n");
	ss << "  4 " << board_left; for(int c=0;c<4;++c) ss << cell_text[at(1,c,0)]; ss << board_right << " 7  " << " 20 " << board_left; for(int c=0;c<4;++c) ss << cell_text[at(1,c,1)]; ss << board_right << " 23 " << " 36 " << board_left; for(int c=0;c<4;++c) ss << cell_text[at(1,c,2)]; ss << board_right << " 39 " << " 52 " << board_left; for(int c=0;c<4;++c) ss << cell_text[at(1,c,3)]; ss << board_right << " 55  Next player: " + cell_text[mNextPlayer] + (is_my_turn ? " (My turn)\n" : " (Opponents turn)\n");
	ss << "  8 " << board_left; for(int c=0;c<4;++c) ss << cell_text[at(2,c,0)]; ss << board_right << " 11 " << " 24 " << board_left; for(int c=0;c<4;++c) ss << cell_text[at(2,c,1)]; ss << board_right << " 27 " << " 40 " << board_left; for(int c=0;c<4;++c) ss << cell_text[at(2,c,2)]; ss << board_right << " 43 " << " 56 " << board_left; for(int c=0;c<4;++c) ss << cell_text[at(2,c,3)]; ss << board_right << " 59  X pieces:   " << X_pieces << "\n";    
	ss << " 12 " << board_left; for(int c=0;c<4;++c) ss << cell_text[at(3,c,0)]; ss << board_right << " 15 " << " 28 " << board_left; for(int c=0;c<4;++c) ss << cell_text[at(3,c,1)]; ss << board_right << " 31 " << " 44 " << board_left; for(int c=0;c<4;++c) ss << cell_text[at(3,c,2)]; ss << board_right << " 47 " << " 60 " << board_left; for(int c=0;c<4;++c) ss << cell_text[at(3,c,3)]; ss << board_right << " 63  O pieces: " << O_pieces << "\n"; 
	ss << board_bottom;

	return ss.str();
}

/**
 * Convert the board to a machine readable string ready to be printed to std::cout
 *
 * This is used for passing board states between bots
 */
std::string GameState::toMessage() const
{
	// Use a stringstream to compose the message
	std::stringstream ss;

	// The board goes first
    for(int i=0;i<cSquares;i++)
		ss << MESSAGE_SYMBOLS[mCell[i]];

    // Then the information about moves
    assert(mNextPlayer == CELL_O || mNextPlayer == CELL_X);
    ss << " " << mLastMove.toMessage() << " " << MESSAGE_SYMBOLS[mNextPlayer] ;

	return ss.str();
}

/*namespace TICTACTOE3D*/ }

