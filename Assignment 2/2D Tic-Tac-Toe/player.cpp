#include "player.hpp"
#include <cstdlib>
#include <cmath>
using namespace std;

namespace TICTACTOE {
    /* ------------------------- Player::evaluation -------------------------*/
    int Player::evaluation(const GameState &pState) {
        int h = 0;
        h += compRows(pState);
        h += compCols(pState);
        h += compDiag(pState);
        return h;
    }
    int Player::compRows(const GameState& pState){
        int sum = 0;
        int countMax;
        int countMin;
        for (size_t i = 0; i < 4; i++) { // Row
            countMax = 0;
            countMin = 0;
            for (size_t j = 0; j < 4; j++) { // Col
                if (pState.at(i, j) == max) countMax++;
                if (pState.at(i, j) == min) countMin++;
            }
            sum += SCORE[countMax][countMin];
        }
        return sum;
    }
    int Player::compCols(const GameState& pState){
        int sum = 0;
        int countMax;
        int countMin;
        for (size_t j = 0; j < 4; j++) { // Col
            countMax = 0;
            countMin = 0;
            for (size_t i = 0; i < 4; i++) { // Row
                if (pState.at(i, j) == max) countMax++;
                if (pState.at(i, j) == min) countMin++;
            }
            sum += SCORE[countMax][countMin];
        }
        return sum;
    }
    int Player::compDiag(const GameState& pState){
        int sum = 0;
        int countMax = 0;
        int countMin = 0;
        for (size_t i = 0; i < 4; i++) {
            if (pState.at(i, i) == max) countMax++;
            if (pState.at(i, i) == min) countMin++;
        }
        sum += SCORE[countMax][countMin];

        countMax = 0;
        countMin = 0;
        for (size_t i = 0; i < 4; i++) {
            if (pState.at(3-i, i) == max) countMax += 1;
            if (pState.at(3-i, i) == min) countMin += 1;
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
        if (lNextStates.size() == 1) return lNextStates[0];

        max = pState.getNextPlayer();
        min = (max != CELL_X) ? CELL_X : CELL_O;

        Node node = alphabeta(pState, MAXDEPTH, static_cast<int>(-INFINITY), static_cast<int>(INFINITY), max);

        return lNextStates[node.id];
    }

/*namespace TICTACTOE*/ }
