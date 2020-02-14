#include "constants.hpp"
#include "gamestate.hpp"
#include <cstdlib>
#include <inttypes.h>

namespace checkers
{

/**
 * Initializes the board to the starting position
 */
GameState::GameState()
{
	// Initialize the board
	for(int i=0;i<cPlayerPieces;i++)
	{
		mCell[i]=CELL_RED;
		mCell[cSquares-1-i]=CELL_WHITE;
	}
	for(int i=cPlayerPieces;i<cSquares-cPlayerPieces;i++)
	{
		mCell[i]=CELL_EMPTY;
	}
	// Initialize move related variables
	mLastMove = Move(Move::MOVE_BOG);
	mMovesUntilDraw = cMovesUntilDraw;
	mNextPlayer = CELL_RED;
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
	int moves_left;
	ss >> board >> last_move >> next_player >> moves_left;

	assert(board.size() == (unsigned)cSquares);
	assert(next_player.size() == 1);
	assert(moves_left >= 0 && moves_left < 256);

	// Parse the board
	for (int i = 0; i < cSquares; ++i)
	{
		if (board[i] == MESSAGE_SYMBOLS[CELL_EMPTY])
			mCell[i] = CELL_EMPTY;
		else if (board[i] == MESSAGE_SYMBOLS[CELL_RED])
			mCell[i] = CELL_RED;
		else if (board[i] == MESSAGE_SYMBOLS[CELL_WHITE])
			mCell[i] = CELL_WHITE;
		else if (board[i] == MESSAGE_SYMBOLS[CELL_RED | CELL_KING])
			mCell[i] = CELL_RED | CELL_KING;
		else if (board[i] == MESSAGE_SYMBOLS[CELL_WHITE | CELL_KING])
			mCell[i] = CELL_WHITE | CELL_KING;
		else
			assert("Invalid cell" && false);
	}

	// Parse last move
	mLastMove = Move(last_move);

	// Parse next player
	if (next_player[0] == MESSAGE_SYMBOLS[CELL_EMPTY])
		mNextPlayer = CELL_EMPTY;
	else if (next_player[0] == MESSAGE_SYMBOLS[CELL_RED])
		mNextPlayer = CELL_RED;
	else if (next_player[0] == MESSAGE_SYMBOLS[CELL_WHITE])
		mNextPlayer = CELL_WHITE;
	else if (next_player[0] == MESSAGE_SYMBOLS[CELL_RED | CELL_KING])
		mNextPlayer = CELL_RED | CELL_KING;
	else if (next_player[0] == MESSAGE_SYMBOLS[CELL_WHITE | CELL_KING])
		mNextPlayer = CELL_WHITE | CELL_KING;
	else
	{
		std::cerr << "Invalid next player" << std::endl;
		assert(false);
		exit(-1);
	}

	// Set number of moves left until draw
	mMovesUntilDraw = moves_left;
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
    mMovesUntilDraw = pRH.mMovesUntilDraw;
    mNextPlayer     = pRH.mNextPlayer;
    mLastMove       = pRH.mLastMove;

    // Perform move
    doMove(pMove);
}

/**
 * Constructs a state that is the result of rotating the board 180 degrees and swapping colors
 *
 */
GameState GameState::reversed() const
{
	GameState result = *this;
	for (int i = 0; i < 32; ++i)
	{
		if (mCell[31-i] == CELL_EMPTY)
			result.mCell[i] = CELL_EMPTY;
		else
			result.mCell[i] = mCell[31-i] ^ (CELL_RED | CELL_WHITE);
	}
    result.mNextPlayer ^= (CELL_RED | CELL_WHITE);
    result.mLastMove = mLastMove.reversed();
    return result;
}

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
bool GameState::tryJump(std::vector<Move> &pMoves, int pR, int pC,
             bool pKing, uint8_t *pBuffer, int pDepth) const
{
    // Remove ourself temporarily
	uint8_t lOldSelf = at(pR, pC);
	mutableAt(pR, pC) = CELL_EMPTY;

	pBuffer[pDepth]=rowColToCell(pR,pC);

    bool lFound=false;
    uint8_t lOther = mNextPlayer ^ (CELL_WHITE|CELL_RED);

    // Try capturing downwards
    if(mNextPlayer==CELL_RED||pKing)
    {
        // Try capturing left
        if((at(pR+1,pC-1)&lOther) && at(pR+2,pC-2)==CELL_EMPTY)
        {
            lFound=true;
            uint8_t lOldValue=at(pR+1,pC-1);
            mutableAt(pR+1,pC-1)=CELL_EMPTY;
            tryJump(pMoves,pR+2,pC-2,pKing,pBuffer,pDepth+1);
            mutableAt(pR+1,pC-1)=lOldValue;
        }
        // Try capturing right
        if((at(pR+1,pC+1)&lOther) && at(pR+2,pC+2)==CELL_EMPTY)
        {
            lFound=true;
            uint8_t lOldValue=at(pR+1,pC+1);
            mutableAt(pR+1,pC+1)=CELL_EMPTY;
            tryJump(pMoves,pR+2,pC+2,pKing,pBuffer,pDepth+1);
            mutableAt(pR+1,pC+1)=lOldValue;
        }
    }
    // Try capturing upwards
    if(mNextPlayer==CELL_WHITE||pKing)
    {
        // Try capturing left
        if((at(pR-1,pC-1)&lOther) && at(pR-2,pC-2)==CELL_EMPTY)
        {
            lFound=true;
            uint8_t lOldValue=at(pR-1,pC-1);
            mutableAt(pR-1,pC-1)=CELL_EMPTY;
            tryJump(pMoves,pR-2,pC-2,pKing,pBuffer,pDepth+1);
            mutableAt(pR-1,pC-1)=lOldValue;
        }
        // Try capturing right
        if((at(pR-1,pC+1)&lOther) && at(pR-2,pC+2)==CELL_EMPTY)
        {
            lFound=true;
            uint8_t lOldValue=at(pR-1,pC+1);
            mutableAt(pR-1,pC+1)=CELL_EMPTY;
            tryJump(pMoves,pR-2,pC+2,pKing,pBuffer,pDepth+1);
            mutableAt(pR-1,pC+1)=lOldValue;
        }
    }

    // Restore ourself
	mutableAt(pR, pC) = lOldSelf;

    if(!lFound&&pDepth>0)
        pMoves.push_back(Move(pBuffer,pDepth+1));

    return lFound;
}

/**
 * Tries to make a move from a certain position
 *
 * \param pMoves vector where the valid moves will be inserted
 * \param pCell the cell where the move is tried from
 * \param pOther the \ref ECell code corresponding to the player
 * who is not making the move
 * \param pKing true if the piece is a king
 */
void GameState::tryMove(std::vector<Move> &pMoves, int pCell, bool pKing) const
{
    int lR=cellToRow(pCell);
    int lC=cellToCol(pCell);
    // Try moving downwards
    if(mNextPlayer==CELL_RED||pKing)
    {
        // Try moving right
        if(at(lR+1,lC-1)==CELL_EMPTY)
            pMoves.push_back(Move(pCell,rowColToCell(lR+1,lC-1)));
        //try moving left
        if(at(lR+1,lC+1)==CELL_EMPTY)
            pMoves.push_back(Move(pCell,rowColToCell(lR+1,lC+1)));
    }
    // Try moving upwards
    if(mNextPlayer==CELL_WHITE||pKing)
    {
        // Try moving right
        if(at(lR-1,lC-1)==CELL_EMPTY)
            pMoves.push_back(Move(pCell,rowColToCell(lR-1,lC-1)));
        // Try moving left
        if(at(lR-1,lC+1)==CELL_EMPTY)
            pMoves.push_back(Move(pCell,rowColToCell(lR-1,lC+1)));
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

    if (mMovesUntilDraw <= 0)
    {
    	pStates.push_back(GameState(*this, Move(Move::MOVE_DRAW)));
    	return;
    }

    // Normal moves are forbidden if any jump is found
    bool lFound=false;
    int lPieces[cPlayerPieces];
    uint8_t lMoveBuffer[cPlayerPieces];
	std::vector<Move> lMoves;
    int lNumPieces=0;
    for (int i = 0; i < cSquares; ++i)
    {
        // Is this a piece which belongs to the player making the move?
        if (at(i) & mNextPlayer)
        {
            bool lIsKing = at(i)&CELL_KING;

            if (tryJump(lMoves, cellToRow(i), cellToCol(i), lIsKing, lMoveBuffer))
                lFound=true;

            lPieces[lNumPieces++]=i;
        }
    }

    // Try normal moves if no jump was found
    if (!lFound)
    {
        for (int k = 0; k < lNumPieces; ++k)
        {
            int lCell = lPieces[k];
            bool lIsKing = at(lCell) & CELL_KING;
            tryMove(lMoves, lCell, lIsKing);
        }
    }

    // Convert moves to GameStates
    for (unsigned i = 0; i < lMoves.size(); ++i)
    	pStates.push_back(GameState(*this, lMoves[i]));

    // Admit loss if no moves can be found
    if (pStates.size() == 0)
    	pStates.push_back(GameState(*this, Move(mNextPlayer == CELL_WHITE ? Move::MOVE_RW : Move::Move::MOVE_WW)));
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
    if (pMove.isJump())
    {
    	// Row and column of source cell
        int sr = cellToRow(pMove[0]);
        int sc = cellToCol(pMove[0]);

    	// Perform all jumps
        for(unsigned i=1;i<pMove.length();++i)
        {
        	// Destination cell
            int dr = cellToRow(pMove[i]);
            int dc = cellToCol(pMove[i]);

            // Move the jumping piece
            at(pMove[i]) = at(pMove[i-1]);
            at(pMove[i-1]) = CELL_EMPTY;

            // Promote to king if we should
            if ((dr==7 && (at(pMove[i])&CELL_RED)) || (dr==0 && (at(pMove[i])&CELL_WHITE)))
                at(pMove[i])|=CELL_KING;

            // Remove the piece being jumped over
            at(rowColToCell((sr+dr)>>1,(sc+dc)>>1)) = CELL_EMPTY;

            // Prepare for next jump
            sr = dr;
            sc = dc;
        }

        // Reset number of moves left until draw
        mMovesUntilDraw = cMovesUntilDraw;
    }
    else if(pMove.isNormal())
    {
    	// Move the piece
        at(pMove[1]) = at(pMove[0]);
        at(pMove[0]) = CELL_EMPTY;

        // Promote to king if we should
        int lDR=cellToRow(pMove[1]);
        if ((lDR==7 && (at(pMove[1])&CELL_RED)) || (lDR==0 && (at(pMove[1])&CELL_WHITE)))
            at(pMove[1]) |= CELL_KING;

        // Decrease number of moves left until draw
        --mMovesUntilDraw;
    }

    // Remember last move
    mLastMove = pMove;

    // Swap player
    mNextPlayer = mNextPlayer ^ (CELL_RED | CELL_WHITE);

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
	static const std::string board_top    = (cell_text == SIMPLE_TEXT) ? "     -----------------\n" : "    ╭─────────────────╮\n";
	static const std::string board_bottom = (cell_text == SIMPLE_TEXT) ? "     -----------------\n" : "    ╰─────────────────╯\n";
	static const std::string board_left   = (cell_text == SIMPLE_TEXT) ? "| " : "│ ";
	static const std::string board_right  = (cell_text == SIMPLE_TEXT) ? "|" : "│";


	bool is_winner = (isEOG() && ((pPlayer == CELL_RED && isRedWin()) || (pPlayer == CELL_WHITE && isWhiteWin())));
	bool is_my_turn = (mNextPlayer == pPlayer);
	int red_pieces = 0;
	int white_pieces = 0;
	for (int i = 0; i < cSquares; ++i)
	{
		if (at(i) & CELL_RED)
			++red_pieces;
		else if (at(i) & CELL_WHITE)
			++white_pieces;
	}

	// Use a stringstream to compose the string
	std::stringstream ss;

	// Draw the board with numbers around it indicating cell index and put text to the right of the board
	ss << board_top;
	ss << "  0 " << board_left; for(int c=0;c<8;++c) ss << cell_text[at(0,c)]; ss << board_right << " 3\n";
	ss << "  4 " << board_left; for(int c=0;c<8;++c) ss << cell_text[at(1,c)]; ss << board_right << " 7\n";
	ss << "  8 " << board_left; for(int c=0;c<8;++c) ss << cell_text[at(2,c)]; ss << board_right << " 11     Last move: " + mLastMove.toString() + (is_winner ? " (WOHO! I WON!)\n" : "\n");
	ss << " 12 " << board_left; for(int c=0;c<8;++c) ss << cell_text[at(3,c)]; ss << board_right << " 15     Next player: " + cell_text[mNextPlayer] + (is_my_turn ? " (My turn)\n" : " (Opponents turn)\n");
	ss << " 16 " << board_left; for(int c=0;c<8;++c) ss << cell_text[at(4,c)]; ss << board_right << " 19     Moves until draw: " << (int)mMovesUntilDraw << "\n";
	ss << " 20 " << board_left; for(int c=0;c<8;++c) ss << cell_text[at(5,c)]; ss << board_right << " 23     Red pieces:   " << red_pieces << "\n";
	ss << " 24 " << board_left; for(int c=0;c<8;++c) ss << cell_text[at(6,c)]; ss << board_right << " 27     White pieces: " << white_pieces << "\n";
	ss << " 28 " << board_left; for(int c=0;c<8;++c) ss << cell_text[at(7,c)]; ss << board_right << " 31\n";
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
    assert(mNextPlayer == CELL_WHITE || mNextPlayer == CELL_RED);
    ss << " " << mLastMove.toMessage() << " " << MESSAGE_SYMBOLS[mNextPlayer] << " " << (int)mMovesUntilDraw;

	return ss.str();
}

/*namespace checkers*/ }

