#ifndef _TICTACTOE_CONSTANTS_HPP_
#define _TICTACTOE_CONSTANTS_HPP_

#include <string>

namespace TICTACTOE {

/**
 * This enumeration is used as the contents of squares in TTTBoard.
 * the CELL_X and CELL_O constants are also used to refer
 * to this and the other player
 */
enum Cell
{
    CELL_EMPTY = 0,     ///< the cell is empty
    CELL_X = 1,         ///< the cell belongs to the X player
    CELL_O = 2,         ///< the cell belongs to the O player
    CELL_INVALID = 3    ///< the cell is invalid
};

const std::string SIMPLE_TEXT[4] = {
		". ", // CELL_EMPTY
		"x ", // CELL_X
		"o ", // CELL_O
		"  ", // CELL_INVALID
};

const std::string UNICODE_TEXT[4] = {
		"― ", // CELL_EMPTY
		"✗ ", // CELL_X
		"𝑂 ", // CELL_O
		"  ", // CELL_INVALID
};

const std::string COLOR_TEXT[4] = {
		"\u001B[30m― \u001B[0m", // CELL_EMPTY
		"✗ ", // CELL_X
		"𝑂 ", // CELL_O
		"  ", // CELL_INVALID
};

const char MESSAGE_SYMBOLS[4] = {
		'.', // CELL_EMPTY
		'x', // CELL_X
		'o', // CELL_O
		'_', // CELL_INVALID
};

/*namespace TICTACTOE*/ }

#endif
