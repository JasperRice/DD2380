#ifndef _TICTACTOE_PLAYER_HPP_
#define _TICTACTOE_PLAYER_HPP_

#include "constants.hpp"
#include "deadline.hpp"
#include "gamestate.hpp"
#include "move.hpp"
#include <vector>

namespace TICTACTOE {
    const int MAXDEPTH = 5;
    const int SCORE[5][5] = {
        {0,     -10,    -100,   -1000,  -10000},
        {10,    0,      0,      0,      0},
        {100,   0,      0,      0,      0},
        {1000,  0,      0,      0,      0},
        {10000, 0,      0,      0,      0}
    };
    struct Node {
        int v;
        int id;
    };

    class Player {
    private:
        uint8_t max;
        uint8_t min;
    public:
        int evaluation(const GameState& pState);
        int compRows(const GameState& pState);
        int compCols(const GameState& pState);
        int compDiag(const GameState& pState);
        Node alphabeta(const GameState& pState, int depth, int alpha, int beta, uint8_t player);

        ///perform a move
        ///\param pState the current state of the board
        ///\param pDue time before which we must have returned
        ///\return the next state the board is in after our move
        GameState play(const GameState &pState, const Deadline &pDue);
    };

/*namespace TICTACTOE*/ }

#endif
