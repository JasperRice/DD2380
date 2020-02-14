#include <iostream>
#include <cmath>
#include "Matrix.h"
using namespace std;

int main(int argc, char const *argv[]) {
    unsigned row, col, N, K, T;
    unsigned maxIters = 50000;

    // --------------- Initialize Matries --------------- //
    cin >> row >> col;
    N = row;
    myMatrix<double> Transition(row, col, 0.0);
    for (size_t i = 0; i < row; i++) {
        for (size_t j = 0; j < col; j++) {
            cin >> Transition(i, j);
        }
    }

    cin >> row >> col;
    K = col;
    myMatrix<double> Emission(row, col, 0.0);
    for (size_t i = 0; i < row; i++) {
        for (size_t j = 0; j < col; j++) {
            cin >> Emission(i, j);
        }
    }

    cin >> row >> col;
    myMatrix<double> Pi(row, col, 0.0);
    for (size_t i = 0; i < row; i++) {
        for (size_t j = 0; j < col; j++) {
            cin >> Pi(i, j);
        }
    }

    cin >> T;
    unsigned* Observation = new unsigned[T];
    for (size_t i = 0; i < T; i++) {
        cin >> Observation[i];
    }

    myMatrix<double> Alpha(T, N, 0.0);
    myMatrix<double> Beta(T, N, 0.0);
    // Allocation of Gamma
    double*** Gamma = new double**[T];
    for (size_t t = 0; t < T; t++) {
        Gamma[t] = new double*[N];
        for (size_t i = 0; i < N; i++) {
            Gamma[t][i] = new double[N];
        }
    }
    myMatrix<double> Di_Gamma(T, N, 0.0);
    // Allocation of c
    double* c = new double[T];
	double denom, numer, logProb, oldLogProb;
	oldLogProb = - INFINITY;

    for (size_t Epoch = 0; Epoch < maxIters; Epoch++) {
        // --------------- Forward Algorithm --------------- //
        // Compute Alpha(0, i)
        c[0] = 0.0;
        for (size_t i = 0; i < N; i++) {
            Alpha(0, i) = Emission(i, Observation[0]) * Pi(0, i);
            c[0] += Alpha(0, i);
        }
        // Scale the Alpha(0, i)
        c[0] = 1 / c[0];
        for (size_t i = 0; i < N; i++) {
            Alpha(0, i) *= c[0];
        }
        // Compute Alpha(t, i)
        for (size_t t = 1; t < T; t++) {
            c[t] = 0;
            for (size_t i = 0; i < N; i++) {
                Alpha(t, i) = 0.0;
                for (size_t j = 0; j < N; j++) {
                    Alpha(t, i) += Transition(j, i) * Alpha(t-1, j);
                }
                Alpha(t, i) *= Emission(i, Observation[t]);
                c[t] += Alpha(t, i);
            }
            // Scale Alpha(t, i)
            c[t] = 1 / c[t];
            for (size_t i = 0; i < N; i++) {
                Alpha(t, i) *= c[t];
            }
        }

        // --------------- Backward Algorithm --------------- //
        // Let Beta(T-1, i) = 1, scaled by c[T-1]
        for (size_t i = 0; i < N; i++) {
            Beta(T-1, i) = c[T-1];
        }
        // Beta-pass
        for (int t = T-2; t > -1; t--) {
            for (size_t i = 0; i < N; i++) {
                Beta(t, i) = 0.0;
                for (size_t j = 0; j < N; j++) {
                    Beta(t, i) += Beta(t+1, j) * Emission(j, Observation[t+1]) * Transition(i, j);
                }
				// Scale Beta(t, i) with the same scale factor as Alpha(t, i)
				Beta(t, i) *= c[t];
			}
        }

		// --------------- Compute Gamma and Di-gamma --------------- //
        for (size_t t = 0; t < T - 1; t++) {
			denom = 0.0;
            for (size_t i = 0; i < N; i++) {
                for (size_t j = 0; j < N; j++) {
					denom += Alpha(t,i) * Transition(i, j) * Emission(j, Observation[t+1]) * Beta(t+1, j);
                }
            }
			for (size_t i = 0; i < N; i++) {
				Di_Gamma(t, i) = 0;
				for (size_t j = 0; j < N; j++) {
					Gamma[t][i][j] = Alpha(t,i) * Transition(i, j) * Emission(j, Observation[t+1]) * Beta(t+1, j) / denom;
					Di_Gamma(t, i) += Gamma[t][i][j];
				}
			}
        }
		// Special case for Di-gamma(T-1, i)
		denom = 0.0;
		for (size_t i = 0; i < N; i++) {
			denom += Alpha(T-1, i);
		}
		for (size_t i = 0; i < N; i++) {
			Di_Gamma(T-1, i) = Alpha(T-1, i) / denom;
		}

		// --------------- Re-estimate Transition, Emission and Pi --------------- //
		// Re-estimate Pi
		for (size_t i = 0; i < N; i++) {
			Pi(0, i) = Di_Gamma(0, i);
		}
		// Re-estimate Transition
		for (size_t i = 0; i < N; i++) {
			for (size_t j = 0; j < N; j++) {
				numer = 0.0;
				denom = 0.0;
				for (size_t t = 0; t < T-1; t++) {
					numer += Gamma[t][i][j];
					denom += Di_Gamma(t, i);
				}
				Transition(i, j) = numer / denom;
			}
		}
		// Re-estimate Emission
		for (size_t i = 0; i < N; i++) {
			for (size_t j = 0; j < K; j++) {
				numer = 0.0;
				denom = 0.0;
				for (size_t t = 0; t < T; t++) {
					if (Observation[t] == j) {
						numer += Di_Gamma(t, i);
					}
					denom += Di_Gamma(t, i);
				}
				Emission(i, j) = numer / denom;
			}
		}

		// --------------- Compute log[P(O|Lambda)] --------------- //
		logProb = 0.0;
		for (size_t i = 0; i < T; i++) {
			logProb += log(c[i]);
		}
		logProb = -logProb;

		// --------------- Test Convergence --------------- //
		if (logProb > oldLogProb) {
			oldLogProb = logProb;
		}
		else {
            cout << "Convergence iteration: " << Epoch << '\n';
			break;
		}
    }

    // --------------- Print Output --------------- //
    cout << N << " " << N << " ";
    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < N; j++) {
            cout << Transition(i,j) << " ";
        }
    }
    cout << endl;

    cout << N << " " << K << " ";
    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < K; j++) {
            cout << Emission(i,j) << " ";
        }
    }
    cout << endl;

    // HMM C
    cout << 1 << " " << N << " ";
    for (size_t i = 0; i < N; i++) {
        cout << Pi(0, i) << " ";
    }
    cout << endl;

    // Deallocation
    for (size_t t = 0; t < T; t++) {
        for (size_t i = 0; i < N; i++) {
            delete[] Gamma[t][i];
        }
        delete[] Gamma[t];
    }
    delete[] Gamma;
    delete [] Observation;
    delete[] c;
    return 0;
}
