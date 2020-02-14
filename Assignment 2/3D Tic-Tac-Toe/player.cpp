#include "player.hpp"
#include <cmath>
#include <cstdlib>
using namespace std;

namespace TICTACTOE3D {

    /* ------------------------- Player::evaluation -------------------------*/
    int Player::evaluation(const GameState &pState) {
        int h = 0;
        h += compLine(pState);
        h += compFaceDiag(pState);
        h += compSpaceDiag(pState);
        return h;
    }
    int Player::compLine(const GameState& pState){
        int sum = 0;
        int countMax;
        int countMin;
        for (size_t y = 0; y < 4; y++) {
            for (size_t z = 0; z < 4; z++) {
                countMax = 0;
                countMin = 0;
                for (size_t x = 0; x < 4; x++) {
                    if (pState.at(x, y, z) == max) countMax++;
                    if (pState.at(x, y, z) == min) countMin++;
                }
                sum += SCORE[countMax][countMin];
            }
        }
        for (size_t z = 0; z < 4; z++) {
            for (size_t x = 0; x < 4; x++) {
                countMax = 0;
                countMin = 0;
                for (size_t y = 0; y < 4; y++) {
                    if (pState.at(x, y, z) == max) countMax++;
                    if (pState.at(x, y, z) == min) countMin++;
                }
                sum += SCORE[countMax][countMin];
            }
        }
        for (size_t x = 0; x < 4; x++) {
            for (size_t y = 0; y < 4; y++) {
                countMax = 0;
                countMin = 0;
                for (size_t z = 0; z < 4; z++) {
                    if (pState.at(x, y, z) == max) countMax++;
                    if (pState.at(x, y, z) == min) countMin++;
                }
                sum += SCORE[countMax][countMin];
            }
        }
        return sum;
    }
    int Player::compFaceDiag(const GameState& pState){
        int sum = 0;
        int countMax;
        int countMin;
        for (size_t x = 0; x < 4; x++) {
            countMax = 0;
            countMin = 0;
            for (size_t i = 0; i < 4; i++) {
                if (pState.at(x, i, i) == max) countMax++;
                if (pState.at(x, i, i) == min) countMin++;
            }
            sum += SCORE[countMax][countMin];

            countMax = 0;
            countMin = 0;
            for (size_t i = 0; i < 4; i++) {
                if (pState.at(x, 3-i, i) == max) countMax++;
                if (pState.at(x, 3-i, i) == min) countMin++;
            }
            sum += SCORE[countMax][countMin];
        }
        for (size_t y = 0; y < 4; y++) {
            countMax = 0;
            countMin = 0;
            for (size_t i = 0; i < 4; i++) {
                if (pState.at(i, y, i) == max) countMax++;
                if (pState.at(i, y, i) == min) countMin++;
            }
            sum += SCORE[countMax][countMin];

            countMax = 0;
            countMin = 0;
            for (size_t i = 0; i < 4; i++) {
                if (pState.at(3-i, y, i) == max) countMax++;
                if (pState.at(3-i, y, i) == min) countMin++;
            }
            sum += SCORE[countMax][countMin];
        }
        for (size_t z = 0; z < 4; z++) {
            countMax = 0;
            countMin = 0;
            for (size_t i = 0; i < 4; i++) {
                if (pState.at(i, i, z) == max) countMax++;
                if (pState.at(i, i, z) == min) countMin++;
            }
            sum += SCORE[countMax][countMin];

            countMax = 0;
            countMin = 0;
            for (size_t i = 0; i < 4; i++) {
                if (pState.at(3-i, i, z) == max) countMax++;
                if (pState.at(3-i, i, z) == min) countMin++;
            }
            sum += SCORE[countMax][countMin];
        }
        return sum;
    }
    int Player::compSpaceDiag(const GameState& pState){
        int sum = 0;
        int countMax = 0;
        int countMin = 0;
        for (size_t i = 0; i < 4; i++) {
            if (pState.at(i, i, i) == max) countMax++;
            if (pState.at(i, i, i) == min) countMin++;
        }
        sum += SCORE[countMax][countMin];

        countMax = 0;
        countMin = 0;
        for (size_t i = 0; i < 4; i++) {
            if (pState.at(3-i, i, i) == max) countMax++;
            if (pState.at(3-i, i, i) == min) countMin++;
        }
        sum += SCORE[countMax][countMin];

        countMax = 0;
        countMin = 0;
        for (size_t i = 0; i < 4; i++) {
            if (pState.at(i, 3-i, i) == max) countMax++;
            if (pState.at(i, 3-i, i) == min) countMin++;
        }
        sum += SCORE[countMax][countMin];

        countMax = 0;
        countMin = 0;
        for (size_t i = 0; i < 4; i++) {
            if (pState.at(i, i, 3-i) == max) countMax++;
            if (pState.at(i, i, 3-i) == min) countMin++;
        }
        sum += SCORE[countMax][countMin];
        return sum;
    }

    /* ------------------------- Player::alphabeta -------------------------*/
    Node Player::alphabeta(const GameState &pState, int depth, int alpha, int beta, uint8_t player) {
        vector<GameState> nextStates;
        pState.findPossibleMoves(nextStates);

        Node node;
        int v;
        int id = -1;
        if (depth == 0 || nextStates.empty()) v = evaluation(pState);
        else if (player == max) {
            v = static_cast<int>(-INFINITY);
            for (size_t i = 0; i < nextStates.size(); i++) {
                Node t = alphabeta(nextStates[i], depth-1, alpha, beta, min);
                if (v < t.v) {
                    v = t.v;
                    id = i;
                }
                if (alpha < v) alpha = v;
                if (beta <= alpha) break;
            }
        }
        else { // player == min
            v = static_cast<int>(INFINITY);
            for (size_t i = 0; i < nextStates.size(); i++) {
                Node t = alphabeta(nextStates[i], depth-1, alpha, beta, max);
                if (v > t.v) {
                    v = t.v;
                    id = i;
                }
                if (beta > v) beta = v;
                if (beta <= alpha) break;
            }
        }

        node.v = v;
        node.id = id;
        return node;
    }

    /* ------------------------- Player::play -------------------------*/
    GameState Player::play(const GameState& pState, const Deadline& pDue) {
        vector<GameState> lNextStates;
        pState.findPossibleMoves(lNextStates);
        if (lNextStates.empty()) return GameState(pState, Move());

        max = pState.getNextPlayer();
        min = (max != CELL_X) ? CELL_X : CELL_O;

        Node node = alphabeta(pState, MAXDEPTH, static_cast<int>(-INFINITY), static_cast<int>(INFINITY), max);

        return lNextStates[node.id];
    }

/*namespace TICTACTOE*/ }
