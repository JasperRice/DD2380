#ifndef _TICTACTOE3D_MOVE_HPP_
#define _TICTACTOE3D_MOVE_HPP_

#include "constants.hpp"
#include <stdint.h>
#include <vector>
#include <string>
#include <sstream>
#include <cassert>
#include <iostream>

namespace TICTACTOE3D {

/**
 * Encapsulates a move
 *
 * In general, you should regard this as an opaque structure that exists
 * in the GameState you obtain from findPossibleMoves.
 *
 * The functions IsNormal() and IsEOG(), might be useful.
 *
 * You can probably ignore the rest of the interface.
 */
class Move
{
public:
    enum MoveType
    {        
        MOVE_NORMAL=0, ///< a normal move
        MOVE_BOG=-1,   ///< beginning of game
        MOVE_XW=-2,   ///< X wins => end of game
        MOVE_OW=-3,   ///< O wins => end of game
        MOVE_DRAW=-4,   ///< draw => end of game
        MOVE_NULL=-5   ///< a null move
    };

public:
    ///constructs a special type move
    
    ///\param pType should be one of MOVE_BOG, MOVE_XW, MOVE_OW or MOVE_DRAW
    explicit Move(MoveType pType=MOVE_BOG)
        :   mType(pType)
    {
    }

    ///constructs a normal move 
    ///\param p1 the destination square
	///\param p2 is the player symbol
    Move(uint8_t p1,Cell p2)
        :	mType(MOVE_NORMAL)
    {
    	mData.resize(2);
    	mData[0] = p1;
		mData[1] = p2;
    }
	
	///constructs a Special move (Win or Draw) for player
    ///\param p1 the destination square
	///\param p2 is the player symbol
    Move(uint8_t p1,Cell p2,int SpecialMove)        
    {
    	mData.resize(2);
    	mData[0] = p1;
		mData[1] = p2;
		if(SpecialMove==2)
		{
			mType=MOVE_DRAW;
		}
		if(SpecialMove==1)
		{
			if(p2==CELL_O)
				mType=MOVE_OW;
			if(p2==CELL_X)
				mType=MOVE_XW;
		}
		
    }

      
    ///reconstructs the move from a string
    
    ///\param pString a string, which should have been previously generated
    ///by ToString(), or obtained from the server
    Move(const std::string &pString)
    {
        std::istringstream lStream(pString);        
        lStream >> mType;
        
        int lLen=0;
        
        if (mType==MOVE_NORMAL)
            lLen=2;
		else if(mType>0)
            lLen = mType+1;
		
		if (mType==MOVE_OW)
            lLen=2;
		
		if (mType==MOVE_XW)
            lLen=2; 
		
		if (mType==MOVE_DRAW)
            lLen=2;
            
        if (lLen>12 || mType<MOVE_NULL)
        {
            mType=MOVE_NULL;
            return;
        }
            
        mData.resize(lLen);
            
        for (int i=0; i<lLen; ++i)
        {
            int lCell;
            lStream.ignore(1, cDelimiter);
            lStream >> lCell;            
            mData[i]=lCell;
        }

        if(!lStream)
            mType=MOVE_NULL;
    }

   

    ///returns true if the movement is null or invalid
    bool isNull() const { return (mType==MOVE_NULL); }
    ///returns true if the movement marks beginning of game
    bool isBOG() const { return (mType==MOVE_BOG); }
    ///returns true if the movement marks end of game
    bool isEOG() const { return (mType<MOVE_BOG); }
    ///returns true if the game ended in red win
    bool isXWin() const { return (mType==MOVE_XW); }
    ///returns true if the game ended in white win
    bool isOWin() const { return (mType==MOVE_OW); }
    ///returns true if the game ended in draw
    bool isDraw() const { return (mType==MOVE_DRAW); }
    ///returns true if the movement is a normal move
    bool isNormal() const { return (mType==MOVE_NORMAL); }

    ///returns the type of the move
    int getType() const { return mType; }
    
    ///returns (for normal moves) the number of squares
    std::size_t length() const { return mData.size(); }
    ///returns the pNth square in the sequence
    uint8_t operator[](int pN) const { return mData[pN]; }

    ///converts the move to a string so that it can be sent to the other player
    std::string toMessage() const
    {
        std::ostringstream lStream;
        lStream << mType;
        for(unsigned i=0;i<mData.size();++i)
        {
            lStream << cDelimiter << (int)mData[i];
        }
        
        return lStream.str();
    }

    ///converts the move to a human readable string so that it can be printed
    std::string toString() const
    {
    	if (mType == MOVE_OW)
    		return "OW";
    	if (mType == MOVE_XW)
    		return "XW";
    	if (mType == MOVE_DRAW)
    		return "DRAW";
    	if (mType == MOVE_BOG)
    		return "BOG";
    	if (isNull())
    		return "NULL";

        std::ostringstream lStream;
    	char delimiter = isNormal() ? '-' : 'x';
    	assert(mData.size() > 0);

    	// Concatenate all the cell numbers
		lStream << (int)mData[0];
        for(unsigned i=1; i<mData.size(); ++i)
		{
            lStream << delimiter << (int)mData[i];
		}
        return lStream.str();
    }

    ///returns true if the two objects represent the same move
    bool operator==(const Move &pRH) const
    {
        if (mType != pRH.mType) return false;
        if (mData.size() != pRH.mData.size()) return false;
        
        for (unsigned i=0; i<mData.size(); ++i)
            if (mData[i] != pRH.mData[i]) return false;
        return true;
    }
    
private:
    int mType;
    std::vector<uint8_t> mData;
    static const char cDelimiter = '_';
};

/*namespace TICTACTOE3D*/ }

#endif
