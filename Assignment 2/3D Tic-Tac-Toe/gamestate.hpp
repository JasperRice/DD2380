#ifndef _TICTACTOE3D_GAMESTATE_HPP_
#define _TICTACTOE3D_GAMESTATE_HPP_

#include "constants.hpp"
#include "move.hpp"
#include <stdint.h>
#include <cassert>
#include <cstring>
#include <iostream>
#include <string>

namespace TICTACTOE3D
{

/**
 * Represents a game state with a 4x4 board
 *
 * Cells are numbered as follows:
 *			lay 0						lay 1						lay 2						lay 3			
 *    col 0  1  2  3  		    col 0  1  2  3  		    col 0  1  2  3  		    col 0  1  2  3  		
 * row  ---------------		 row  ---------------		 row  ---------------		 row  ---------------		
 *  0  |  0  1  2  3  |  0	  0  | 16  17 18 19 |  0	  0  | 32  33 34 35 |  0	  0  | 48  49 50 51 |  0	
 *  1  |  4  5  6  7  |  1	  1  | 20  21 22 23 |  1	  1  | 36  37 38 39 |  1	  1  | 52  53 54 55 |  1	
 *  2  |  8  9  10 11 |  2	  2  | 24  25 26 27 |  2	  2  | 40  41 42 43 |  2	  2  | 56  57 58 59 |  2	
 *  3  | 12  13 14 15 |  3	  3  | 28  29 30 31 |  3	  3  | 44  45 46 47 |  3	  3  | 60  61 62 63 |  3	
 *      ---------------		      ---------------		      ---------------		      ---------------		
 *        0  1  2  3		        0  1  2  3		            0  1  2  3		            0  1  2  3		    
 
 *
 * The staring board looks like this:
 *			lay 0           			lay 1						lay 2						lay 3
 *    col 0  1  2  3            col 0  1  2  3              col 0  1  2  3              col 0  1  2  3  
 * row  ---------------      row  ---------------        row  ---------------        row  ---------------
 *  0  |  .  .  .  .  |  0    0  |  .  .  .  .  |  0      0  |  .  .  .  .  |  0      0  |  .  .  .  .  |  0
 *  1  |  .  .  .  .  |  1	  1  |  .  .  .  .  |  1	  1  |  .  .  .  .  |  1	  1  |  .  .  .  .  |  1	
 *  2  |  .  .  .  .  |  2    2  |  .  .  .  .  |  2      2  |  .  .  .  .  |  2      2  |  .  .  .  .  |  2
 *  3  |  .  .  .  .  |  3    3  |  .  .  .  .  |  3      3  |  .  .  .  .  |  3      3  |  .  .  .  .  |  3
 *      ---------------           ---------------             ---------------             ---------------
 *        0  1  2  3                0  1  2  3                  0  1  2  3                  0  1  2  3
 * 
 * O moves first.
 */
class GameState
{
public:
	static const int cSquares = 64;		// 16 valid squares
	
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
	 * Layers are numbered (0 to 3) from down to up
	 *
	 * If the cell falls outside of the board, return CELL_INVALID
	 *
	 * You can use it in the same way as the version that requires a cell index
	 */
	uint8_t at(int pR, int pC, int pL) const
	{
		if (pR < 0 || pR > 3 || pC < 0 || pC > 3|| pL < 0 || pL > 3)
			return CELL_INVALID;
		return mCell[(pR * 4 + pC)+(16*pL) ];
	}

private:
	///private version of above function (allows modifying cells)
	uint8_t &mutableAt(int pR, int pC, int pL) const
	{
		//this is a bit ugly, but is useful for the implementation of
		//findPossibleMoves. It won't affect in single-threaded programs
		//and you're not allowed to use threads anyway
		return const_cast<uint8_t&>(mCell[pR * 4 + pC +(16*pL)]);
	}

public:

	///returns the row corresponding to a cell index
	static int cellToRow(int pCell)
	{
		return (((pCell) >> 2)-(4*cellToLay(pCell)));
	}

	///returns the col corresponding to a cell index
	static int cellToCol(int pCell)
	{
		int lC = ((pCell) & 3);
		return lC;
	}
	
	///returns the lay corresponding to a cell index
	static int cellToLay(int pCell)
	{
		return pCell/16;
	}

	///returns the cell corresponding to a row and col

	///It doesn't check if it corresponds to a black square in the board,
	///or if it falls within the board.
	///
	///If it doesn't, the result is undefined, and the program is likely
	///to crash
	static int rowColLayToCell(int pRow, int pCol, int pLay)
	{
		return (pRow * 4 + pCol +16*pLay);
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
	* Checks if a move end up being a winning move *
	* \param pCell the cell where the move is tried
	* \param pPlayer says who is making the move
	
	*/
	
	int Special_Move(int pCell, Cell pPlayer) const
	{
		int lR=cellToRow(pCell)+1;
		int lC=cellToCol(pCell)+1;
		int lL=cellToLay(pCell)+1;
		//check if winning move:
		int checkWin3D[6][6][6];
		//fill check 3D array
		for (int x=0;x<4;x++) {
			 for (int y=0;y<4;y++) {
				 for (int z=0;z<4;z++) {
					 checkWin3D[x+1][y+1][z+1]=at(x,y,z);
					 }
				 }
         } 

		 for (int i=0;i<6;i++) {
			 for (int j=0;j<6;j++) {
				 checkWin3D[0][i][j]=7; checkWin3D[5][i][j]=7;
				 checkWin3D[i][j][0]=7; checkWin3D[i][j][5]=7;
				 checkWin3D[i][0][j]=7; checkWin3D[i][5][j]=7;}
		} 
		
		 //make the move temporaly:
		checkWin3D[lR][lC][lL]=pPlayer;
		
		int x[4],y[4],z[4];        //variables to store winning coordinates
		x[0]=lR;y[0]=lC;z[0]=lL;   //sets 0 of each to last move
     
		int Pcount;
     
     for (int a=1;a<14;a++){    //iterates through all possible directions in 3D space     
           Pcount=1;                      //reset counter     
           int lR2=lR; int lC2=lC; int lL2=lL;
           
           for (;checkWin3D[lR2][lC2][lL2]!=7;){
			
				//look in every derection until out of field ('7')
                switch (a) {
					case 1:lR2--;break;                
					case 2:lR2--;lC2--;lL2--;break;    
					case 3:lR2--;lC2--;break;          
					case 4:lR2--;lC2--;lL2++;break;    
					case 5:lR2--;lL2--;break;          
					case 6:lR2--;lL2++;break;          
					case 7:lR2--;lC2++;lL2--;break;    
					case 8:lR2--;lC2++;break;          
					case 9:lR2--;lC2++;lL2++;break;    
					case 10:lL2--;break;               
					case 11:lC2--;lL2--;break;         
					case 12:lC2--;break;               
					case 13:lC2--;lL2++;break;         
                }                        
                if (checkWin3D[lR2][lC2][lL2]==pPlayer) {
                           x[Pcount]=lR2;y[Pcount]=lC2;z[Pcount]=lL2;
                           Pcount++;}
                 
            }                 
            
			lR2=lR;lC2=lC;lL2=lL;   //reset placeholder variables
         
         
			for (;checkWin3D[lR2][lC2][lL2]!=7;) { 
              
              
				switch (a) {
					case 1:lR2++;break;                ///////////////////////
					case 2:lR2++;lC2++;lL2++;break;      //
					case 3:lR2++;lC2++;break;           // and
					case 4:lR2++;lC2++;lL2--;break;      // then back
					case 5:lR2++;lL2++;break;           //  in the
					case 6:lR2++;lL2--;break;           //  opposite 
					case 7:lR2++;lC2--;lL2++;break;      //  direction
					case 8:lR2++;lC2--;break;           //
					case 9:lR2++;lC2--;lL2--;break;      //
					case 10:lL2++;break;               //
					case 11:lC2++;lL2++;break;          //
					case 12:lC2++;break;               //
					case 13:lC2++;lL2--;break;          ///////////////////////
                }
                                   
				if (checkWin3D[lR2][lC2][lL2]==pPlayer) {
                      x[Pcount]=lR2;y[Pcount]=lC2;z[Pcount]=lL2;
                      Pcount++;
                }
            }
			   
            if (Pcount>3) {for (int b=0;b<4;b++) {checkWin3D[x[b]][y[b]][z[b]]=4;} 
			return 1;}
        }
		//Check Draw 
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
		return mLastMove.isDraw();
	}

private:
	uint8_t mCell[cSquares];
	uint8_t mNextPlayer;
	Move mLastMove;
};

/*namespace TICTACTOE3D*/}

#endif
