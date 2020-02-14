#ifndef _CHECKERS_PLAYER_HPP_
#define _CHECKERS_PLAYER_HPP_

#include "constants.hpp"
#include "deadline.hpp"
#include "move.hpp"
#include "gamestate.hpp"
#include <bits/stdc++.h>
#include <cmath>
#include <cstdlib>
#include <climits>
#include <functional>
#include <random>
#include <map>
#include <unordered_map>
#include <vector>
using namespace std;


namespace checkers {
    const int MAXDEPTH = 9;
    const int STEPDEPTH = 3;
    const int PAWN_SCORE = 1;
    const int KING_SCORE = 2;
    const int WIN_SCORE = 2000;
    const int LOSE_SCORE = -2000;
    const int DRAW_SCORE = -1000;

    const int RED_PAWN = 0;
    const int RED_KING = 1;
    const int WHITE_PAWN = 2;
    const int WHITE_KING = 3;
    const int COUNT_TYPE = 4;

    struct Node {
        int v;
        int id;
    };

    struct StateNode {
        GameState pState;
        int v;
        int id;
    };

    class Player {
    private:
        uint8_t max;
        uint8_t min;
        uint_fast64_t zobristTable[2][GameState::cSquares][COUNT_TYPE];
        unordered_map<uint_fast64_t, int> stateMap;

        vector<StateNode> stepLeaves;

    public:
        int evaluation(const GameState& pState);
        int evalDefend(const GameState& pState);
        int evalEnd(const GameState& pState);

        void zobristInit();
        uint_fast64_t randomInt();
        uint_fast64_t zobrist(const GameState& pState);
        int cell2map(uint8_t cell);

        Node alphabeta(const GameState& pState, const Deadline& pDue, int depth, int alpha, int beta, uint8_t player);
        GameState ids(const GameState& pState, const Deadline& pDue);
        GameState play(const GameState &pState, const Deadline &pDue);

        Player() {
            cerr << "Initialize a player." << '\n';
            stateMap.clear();
            zobristInit();
        }

        virtual ~Player() { }
    };

    const int POSITION_SCORE[32] = {
            3,      3,      3,      3,
        3,      2,      2,      2,
            2,      1,      1,      3,
        3,      1,      0,      2,
            2,      0,      1,      3,
        3,      1,      1,      2,
            2,      2,      2,      3,
        3,      3,      3,      3
    };
    const int WHITE_CHARGE_SCORE[32] = {
            7,      7,      7,      7,
        6,      6,      6,      6,
            5,      5,      5,      5,
        4,      4,      4,      4,
            3,      3,      3,      3,
        2,      2,      2,      2,
            1,      1,      1,      1,
        0,      0,      0,      0
    };
    const int RED_CHARGE_SCORE[32] = {
            0,      0,      0,      0,
        1,      1,      1,      1,
            2,      2,      2,      2,
        3,      3,      3,      3,
            4,      4,      4,      4,
        5,      5,      5,      5,
            6,      6,      6,      6,
        7,      7,      7,      7
    };
    const int KING_POSITION_SCORE[32] = {
            0,      0,      0,      0,
        1,      1,      1,      1,
            2,      2,      2,      2,
        3,      3,      3,      3,
            3,      3,      3,      3,
        2,      2,      2,      2,
            1,      1,      1,      1,
        0,      0,      0,      0
    };
    const int lRows[9] = {5, 6, 7,  13, 14, 15, 21, 22, 23};
    const int rRows[9] = {8, 9, 10, 16, 17, 18, 24, 25, 26};

/*namespace checkers*/ }

#endif

/*
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
*/
