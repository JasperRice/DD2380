#include <iostream>
#include "Matrix.h"
using namespace std;

int main(int argc, char const *argv[]) {
    unsigned r, c, N, T;

    // --------------- Initialize Matries --------------- //
    cin >> r >> c;
    N = r;
    myMatrix<double> Transition(r, c, 0.0);
    for (size_t i = 0; i < r; i++) {
        for (size_t j = 0; j < c; j++) {
            cin >> Transition(i, j);
        }
    }

    cin >> r >> c;
    myMatrix<double> Emission(r, c, 0.0);
    for (size_t i = 0; i < r; i++) {
        for (size_t j = 0; j < c; j++) {
            cin >> Emission(i, j);
        }
    }

    cin >> r >> c;
    myMatrix<double> Pi(r, c, 0.0);
    for (size_t i = 0; i < r; i++) {
        for (size_t j = 0; j < c; j++) {
            cin >> Pi(i, j);
        }
    }

    cin >> T;
    unsigned* Observation = new unsigned[T];
    for (size_t i = 0; i < T; i++) {
        cin >> Observation[i];
    }

    // --------------- Viterbi Algorithm --------------- //
    myMatrix<double> Delta(T, N, 0.0);
    myMatrix<int> Index_Delta(T, N, -1);
    double delta, new_delta;
    for (size_t t = 0; t < T; t++) {
        if (t == 0) {
            for (size_t i = 0; i < N; i++) {
                Delta(t, i) = Emission(i, Observation[t]) * Pi(0, i);
            }
        }
        else {
            for (size_t i = 0; i < N; i++) {
                new_delta = delta = -1.0;
                for (size_t j = 0; j < N; j++) {
                    new_delta = Transition(j, i) * Delta(t-1, j) * Emission(i, Observation[t]);
                    if (new_delta > delta) {
                        delta = new_delta;
                        Index_Delta(t, i) = j;
                    }
                }
                Delta(t, i) = delta;
            }
        }
    }

    unsigned* X = new unsigned[T];
    delta = -1.0;
    for (size_t j = 0; j < N; j++) {
        if (Delta(T-1, j) > delta) {
            delta = Delta(T-1, j);
            X[T-1] = j;
        }
    }

    for (int t = T-2; t > -1; t--) {
        X[t] = Index_Delta(t+1, X[t+1]);
    }

    // --------------- Print Output --------------- //
    for (size_t t = 0; t < T; t++) {
        cout << X[t] << " ";
    }
    cout << endl;

    // Delete Allocation
    delete [] X;
    delete [] Observation;
    return 0;
}
