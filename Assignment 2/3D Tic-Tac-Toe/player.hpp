#ifndef _TICTACTOE_PLAYER_HPP_
#define _TICTACTOE_PLAYER_HPP_

#include "constants.hpp"
#include "deadline.hpp"
#include "gamestate.hpp"
#include "move.hpp"
#include <vector>

namespace TICTACTOE3D {
    const int MAXDEPTH = 1;
    const int SCORE[5][5] = {
        {0,         -10,    -1000,  -100000,    -10000000},
        {10,        0,      0,      0,          0},
        {1000,      0,      0,      0,          0},
        {100000,    0,      0,      0,          0},
        {10000000,  0,      0,      0,          0}
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
        int compLine(const GameState& pState);
        int compFaceDiag(const GameState& pState);
        int compSpaceDiag(const GameState& pState);
        Node alphabeta(const GameState& pState, int depth, int alpha, int beta, uint8_t player);

        ///perform a move
        ///\param pState the current state of the board
        ///\param pDue time before which we must have returned
        ///\return the next state the board is in after our move
        GameState play(const GameState &pState, const Deadline &pDue);
    };

/*namespace TICTACTOE*/ }

#endif
