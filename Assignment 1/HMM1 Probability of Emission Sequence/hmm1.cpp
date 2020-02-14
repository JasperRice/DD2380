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

    // --------------- Forward Algorithm --------------- //
    myMatrix<double> Alpha(T, N, 0.0);
    for (size_t t = 0; t < T; t++) {
        if (t == 0) {
            for (size_t i = 0; i < N; i++) {
                Alpha(t, i) = Emission(i, Observation[t]) * Pi(0, i);
            }
        }
        else {
            for (size_t i = 0; i < N; i++) {
                for (size_t j = 0; j < N; j++) {
                    Alpha(t, i) += Transition(j, i) * Alpha(t-1, j);
                }
                Alpha(t, i) *= Emission(i, Observation[t]);
            }
        }
    }

    double P = 0.0;
    for (size_t j = 0; j < N; j++) {
        P += Alpha(T-1, j);
    }

    // --------------- Print Output --------------- //
    cout << P << '\n';

    // Delete Allocation
    delete [] Observation;
    return 0;
}
