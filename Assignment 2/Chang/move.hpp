#ifndef _CHECKERS_MOVE_HPP_
#define _CHECKERS_MOVE_HPP_

#include "constants.hpp"
#include <stdint.h>
#include <vector>
#include <string>
#include <sstream>
#include <cassert>

namespace checkers {

/**
 * Encapsulates a move
 *
 * In general, you should regard this as an opaque structure that exists
 * in the GameState you obtain from findPossibleMoves.
 *
 * The functions IsNormal(), IsJump() and IsEOG(), might be useful.
 *
 * You can probably ignore the rest of the interface.
 */
class Move
{
public:
    enum MoveType
    {
        MOVE_JUMP=1,   ///< a single jump (numbers above that will represent multiple jumps)
        MOVE_NORMAL=0, ///< a normal move
        MOVE_BOG=-1,   ///< beginning of game
        MOVE_RW=-2,   ///< red wins => end of game
        MOVE_WW=-3,   ///< white wins => end of game
        MOVE_DRAW=-4,   ///< draw => end of game
        MOVE_NULL=-5   ///< a null move
    };

public:
    ///constructs a special type move
    
    ///\param pType should be one of MOVE_BOG, MOVE_RW, MOVE_WW or MOVE_DRAW
    explicit Move(MoveType pType=MOVE_BOG)
        :   mType(pType)
    {
    }

    ///constructs a normal move (not a jump)
    
    ///\param p1 the source square
    ///\param p2 the destination square
    Move(uint8_t p1,uint8_t p2)
        :	mType(MOVE_NORMAL)
    {
    	mData.resize(2);
    	mData[0] = p1;
    	mData[1] = p2;
    }

    ///constructs a jump move
    
    ///\param pData a series of squares that form the sequence of jumps
    ///\param pLen the number of squares in pData
    Move(uint8_t *pData,std::size_t pLen)
        :	mType(pLen-1)
        ,   mData(pData, pData + pLen)
    {
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
            if (lCell<0 || lCell>31)
            {
                mType=MOVE_NULL;
                break;
            }
            
            mData[i]=lCell;
        }

        if(!lStream)
            mType=MOVE_NULL;
    }

    Move reversed() const
    {
    	Move result = *this;
    	if (isRedWin())
    		result.mType = MOVE_WW;
    	else if (isWhiteWin())
    		result.mType = MOVE_RW;

    	for (unsigned i=0; i < mData.size(); ++i)
			result.mData[i] = 31 - mData[i];

    	return result;
    }

    ///returns true if the movement is null or invalid
    bool isNull() const { return (mType==MOVE_NULL); }
    ///returns true if the movement marks beginning of game
    bool isBOG() const { return (mType==MOVE_BOG); }
    ///returns true if the movement marks end of game
    bool isEOG() const { return (mType<MOVE_BOG); }
    ///returns true if the game ended in red win
    bool isRedWin() const { return (mType==MOVE_RW); }
    ///returns true if the game ended in white win
    bool isWhiteWin() const { return (mType==MOVE_WW); }
    ///returns true if the game ended in draw
    bool isDraw() const { return (mType==MOVE_DRAW); }
    ///returns true if the movement is a jump
    bool isJump() const { return (mType>0); }
    ///returns true if the movement is a normal move
    bool isNormal() const { return (mType==MOVE_NORMAL); }

    ///returns the type of the move
    int getType() const { return mType; }
    
    ///returns (for normal moves and jumps) the number of squares
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
    	if (mType == MOVE_WW)
    		return "WW";
    	if (mType == MOVE_RW)
    		return "RW";
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
            lStream << delimiter << (int)mData[i];

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

/*namespace checkers*/ }

#endif
