// TODO: IDS

#include "player.hpp"
using namespace std;

namespace checkers {
    mt19937 mt(01234567);

    /* ------------------------- Player::evaluation -------------------------*/
    int Player::evaluation(const GameState &pState) {
        uint_fast64_t key = zobrist(pState);
        // if (stateMap.find(key) != stateMap.end()) return stateMap[key];

        int h = 0;
        for (size_t i = 0; i < pState.cSquares; i++) {
            if (pState.at(i) == max) h += (
                PAWN_SCORE
                // + POSITION_SCORE[i]
                // + 2*((max==CELL_RED)?RED_CHARGE_SCORE[i]:WHITE_CHARGE_SCORE[i])
            );
            if (pState.at(i) == min) h -= (
                PAWN_SCORE
                // + POSITION_SCORE[i]
                // + 2*((min==CELL_RED)?RED_CHARGE_SCORE[i]:WHITE_CHARGE_SCORE[i])
            );
            if (pState.at(i) == (max|CELL_KING)) h += (
                KING_SCORE
                // + POSITION_SCORE[i]
                // + KING_POSITION_SCORE[i]
            );
            if (pState.at(i) == (min|CELL_KING)) h -= (
                100 * KING_SCORE
                // + POSITION_SCORE[i]
                // + KING_POSITION_SCORE[i]
            );
        }
        h += evalDefend(pState);
        h += evalEnd(pState);
        stateMap[key] = h;
        return h;
    }

    int Player::evalDefend(const GameState& pState) {
        int countRed = 0;
        int countWhite = 0;
        for (int i: lRows) {
            if (pState.at(i) == CELL_RED) {
                if (pState.at(i-4) == CELL_RED) countRed++;
                if (pState.at(i-5) == CELL_RED) countRed++;
            }
            else if (pState.at(i) == CELL_WHITE) {
                if (pState.at(i+4) == CELL_WHITE) countWhite++;
                if (pState.at(i+5) == CELL_WHITE) countWhite++;
            }
        }

        for (int i: rRows) {
            if (pState.at(i) == CELL_RED) {
                if (pState.at(i-3) == CELL_RED) countRed++;
                if (pState.at(i-4) == CELL_RED) countRed++;
            }
            else if (pState.at(i) == CELL_WHITE) {
                if (pState.at(i+3) == CELL_WHITE) countWhite++;
                if (pState.at(i+4) == CELL_WHITE) countWhite++;
            }
        }

        if (max == CELL_RED) return (countRed - countWhite);
        else return (countWhite - countRed);
    }

    int Player::evalEnd(const GameState& pState) {
        int h;
        if (pState.isRedWin()) h = (max == CELL_RED) ? WIN_SCORE : LOSE_SCORE;
        else if (pState.isWhiteWin()) h = (max == CELL_RED) ? LOSE_SCORE : WIN_SCORE;
        else if (pState.isDraw()) h = DRAW_SCORE;
        return h;
    }


    /* ------------------------- Player::hash -------------------------*/
    void Player::zobristInit() {
        for (size_t k = 0; k < 2; k++) {
            for (size_t i = 0; i < GameState::cSquares; i++) {
                for (size_t j = 0; j < 4; j++) {
                    zobristTable[k][i][j] = randomInt();
                }
            }
        }
    }

    uint_fast64_t Player::randomInt() {
        uniform_int_distribution<uint_fast64_t> dist(0, UINT64_MAX);
        return dist(mt);
    }

    uint_fast64_t Player::zobrist(const GameState& pState) {
        uint_fast64_t key = 0;
        int k;
        if (pState.getNextPlayer() == max) k = 0;
        else k = 1;

        for (size_t i = 0; i < pState.cSquares; i++) {
            if (pState.at(i) != CELL_EMPTY) {
                key ^= zobristTable[k][i][cell2map(pState.at(i))];
            }
        }
        return key;
    }

    int Player::cell2map(uint8_t cell) {
        int j;
        switch (cell) {
            case CELL_RED: {
                j = RED_PAWN;
                break;
            }
            case (CELL_RED|CELL_KING): {
                j = RED_KING;
                break;
            }
            case CELL_WHITE: {
                j = WHITE_PAWN;
                break;
            }
            case (CELL_WHITE|CELL_KING): {
                j = WHITE_KING;
                break;
            }
            default: {
                cerr << "Error: unexpected cell type: " << static_cast<int>(cell) << ".\n";
                exit(-1);
            }
        }
        return j;
    }


    /* ------------------------- Player::minimax -------------------------*/
    Node Player::alphabeta(const GameState& pState, const Deadline& pDue, int depth, int alpha, int beta, uint8_t player) {
        vector<GameState> lNextStates;
        pState.findPossibleMoves(lNextStates);

        Node node;
        int v;
        int id = -1;
        if (depth == 0 || lNextStates.empty()) v = evaluation(pState);
        else if (player == max) {
            v = static_cast<int>(-INFINITY);
            for (size_t i = 0; i < lNextStates.size(); i++) {
                // if (lNextStates.size() == 1) depth++;
                Node t = alphabeta(lNextStates[i], pDue, depth-1, alpha, beta, min);
                if (v < t.v) {
                    v = t.v;
                    id = i;
                }
                if (alpha < v) alpha = v;
                if (beta <= alpha || pDue.now() > pDue - 0.005) {
                    break;
                }
            }
        }
        else { // player == min
            v = static_cast<int>(INFINITY);
            for (size_t i = 0; i < lNextStates.size(); i++) {
                // if (lNextStates.size() == 1) depth++;
                Node t = alphabeta(lNextStates[i], pDue, depth-1, alpha, beta, max);
                if (v > t.v) {
                    v = t.v;
                    id = i;
                }
                if (beta > v) beta = v;
                if (beta <= alpha || pDue.now() > pDue - 0.005) {
                    break;
                }
            }
        }
        node.v = v;
        node.id = id;
        return node;
    }

    GameState Player::ids(const GameState& pState, const Deadline& pDue) {
        vector<GameState> lNextStates;
        pState.findPossibleMoves(lNextStates);
        if (lNextStates.empty()) return GameState(pState, Move());
        if (lNextStates.size() == 1) return lNextStates[0];

        int alpha = - static_cast<int>(INFINITY);
        int beta = static_cast<int>(INFINITY);
        int depth = MAXDEPTH - STEPDEPTH;
        Node node;

        while (pDue.now() <= pDue - 0.01) {
            node = alphabeta(pState, pDue, MAXDEPTH, alpha, beta, max);
            depth += STEPDEPTH;
        }
        return lNextStates[node.id];
    }


    /* ------------------------- Player::play -------------------------*/
    GameState Player::play(const GameState& pState, const Deadline& pDue) {
        max = pState.getNextPlayer();
        min = (max != CELL_RED) ? CELL_RED : CELL_WHITE;
        return ids(pState, pDue);
    }

/*namespace checkers*/ }
