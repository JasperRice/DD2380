#ifndef _CHECKERS_CONSTANTS_HPP_
#define _CHECKERS_CONSTANTS_HPP_

#include <string>

namespace checkers {

/**
 * This enumeration is used as the contents of squares in CBoard.
 * the CELL_WHITE and CELL_RED constants are also used to refer
 * to this and the other player
 */
enum Cell
{
    CELL_EMPTY    = 0,        ///< the cell is empty
    CELL_RED      = (1<<0),   ///< the cell belongs to the red player
    CELL_WHITE    = (1<<1),   ///< the cell belongs to the white player
    CELL_KING     = (1<<2),   ///< the cell is a king
    CELL_INVALID  = (1<<3)    ///< the cell is invalid
};

const std::string SIMPLE_TEXT[9] = {
		". ", // CELL_EMPTY
		"r ", // CELL_RED
		"w ", // CELL_WHITE
		"? ", // Unused
		"? ", // Unused
		"R ", // CELL_RED | CELL_KING
		"W ", // CELL_WHITE | CELL_KING
		"? ", // Unused
		"  ", // CELL_INVALID
};

const std::string UNICODE_TEXT[9] = {
		". ", // CELL_EMPTY
		"⛂ ", // CELL_RED
		"⛀ ", // CELL_WHITE
		"? ", // Unused
		"? ", // Unused
		"⛃ ", // CELL_RED | CELL_KING
		"⛁ ", // CELL_WHITE | CELL_KING
		"? ", // Unused
		"  ", // CELL_INVALID
};

const std::string COLOR_TEXT[9] = {
		". ", // CELL_EMPTY
		"\e[31m⛂ \e[0m", // CELL_RED
		"⛀ ", // CELL_WHITE
		"? ", // Unused
		"? ", // Unused
		"\e[31m⛃ \e[0m", // CELL_RED | CELL_KING
		"⛁ ", // CELL_WHITE | CELL_KING
		"? ", // Unused
		"  ", // CELL_INVALID
};

const char MESSAGE_SYMBOLS[9] = {
		'.', // CELL_EMPTY
		'r', // CELL_RED
		'w', // CELL_WHITE
		'?', // Unused
		'?', // Unused
		'R', // CELL_RED | CELL_KING
		'W', // CELL_WHITE | CELL_KING
		'?', // Unused
		'_', // CELL_INVALID
};

/*namespace checkers*/ }

#endif
