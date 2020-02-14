#include "HiddenMarkovModel.hpp"
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <vector>
using namespace std;

namespace ducks {
	// Test Initialize with input of hmm3.
	void HMM::Test() {
		unsigned row, col;
		cin >> row >> col;
		N = row;
		A.resize(row);
		for (size_t i = 0; i < row; i++) {
			A[i].resize(col);
			for (size_t j = 0; j < col; j++) {
				cin >> A[i][j];
			}
		}

		cin >> row >> col;
		M = col;
		B.resize(row);
		for (size_t i = 0; i < row; i++) {
			B[i].resize(col);
			for (size_t j = 0; j < col; j++) {
				cin >> B[i][j];
			}
		}

		cin >> row >> col;
		Pi.resize(col);
		for (size_t i = 0; i < col; i++) {
			cin >> Pi[i];
		}
	}

	HMM::HMM() {
		N = HIDDEN_NODE;
		M = COUNT_MOVE;
		Pi.resize(N);
		A.resize(N);
		B.resize(N);
		double c = 0.0;
		for (size_t i = 0; i < N; i++) {
			// Pi[i] = 1./N + 0.3/N*((double) rand() / (RAND_MAX));
			Pi[i] = 1 + ((double) rand() / (RAND_MAX)) * 0.5;
			c += Pi[i];
		}
		c = 1.0 / c;
		for (size_t i = 0; i < N; i++) {
			Pi[i] *= c;
		}

		for (size_t i = 0; i < N; i++) {
			c = 0.0;
			A[i].resize(N);
			for (size_t j = 0; j < N; j++) {
				// A[i][j] = 1.0/N + 0.3/N*((double) rand() / (RAND_MAX));
				A[i][j] = 1 + ((double) rand() / (RAND_MAX))*10;
                if (i == j) {
                    A[i][j] = A[i][j] + 10;
                }
				c += A[i][j];
			}
			c = 1.0 / c;
			for (size_t j = 0; j < N; j++) {
				A[i][j] *= c;
			}

			c = 0.0;
			B[i].resize(M);
			for (size_t j = 0; j < M; j++) {
				// B[i][j] = 1./M+0.3/M*((double) rand() / (RAND_MAX));
				B[i][j] = 1 + ((double) rand() / (RAND_MAX))*0.5;
                if (i == j) {
                    B[i][j] = B[i][j] + 1;
                }
				c += B[i][j];
			}
			c = 1.0 / c;
			for (size_t j = 0; j < M; j++) {
				B[i][j] *= c;
			}
		}
	}

	HMM::HMM(unsigned tN, unsigned tM) {
		N = tN;
		M = tM;
		Pi.resize(N);
		A.resize(N);
		B.resize(N);
		double c = 0.0;
		for (size_t i = 0; i < N; i++) {
			// Pi[i] = 1./N + 0.3/N*((double) rand() / (RAND_MAX));
			Pi[i] = 1 + ((double) rand() / (RAND_MAX))*0.5;
			c += Pi[i];
		}
		c = 1.0 / c;
		for (size_t i = 0; i < N; i++) {
			Pi[i] *= c;
		}

		for (size_t i = 0; i < N; i++) {
			c = 0.0;
			A[i].resize(N);
			for (size_t j = 0; j < N; j++) {
				// A[i][j] = 1.0/N + 0.3/N*((double) rand() / (RAND_MAX));
				A[i][j] = 1 + ((double) rand() / (RAND_MAX))*10;
                if (i == j) {
                    A[i][j] = A[i][j] + 10 ;
                }
				c += A[i][j];
			}
			c = 1.0 / c;
			for (size_t j = 0; j < N; j++) {
				A[i][j] *= c;
			}

			c = 0.0;
			B[i].resize(M);
			for (size_t j = 0; j < M; j++) {
				// B[i][j] = 1./M+0.3/M*((double) rand() / (RAND_MAX));
				B[i][j] = 1 + ((double) rand() / (RAND_MAX))*0.5;
                if (i == j) {
                    B[i][j] = B[i][j] + 1;
                }
				c += B[i][j];
			}
			c = 1.0 / c;
			for (size_t j = 0; j < M; j++) {
				B[i][j] *= c;
			}
		}
	}

	void HMM::Initialize(unsigned tN, unsigned tM){
		N = tN;
		M = tM;
		Pi.resize(N);
		A.resize(N);
		B.resize(N);
		double c = 0.0;
		for (size_t i = 0; i < N; i++) {
			// Pi[i] = 1./N + 0.3/N*((double) rand() / (RAND_MAX));
			Pi[i] = 1 + ((double) rand() / (RAND_MAX))*0.5;
			c += Pi[i];
		}
		c = 1.0 / c;
		for (size_t i = 0; i < N; i++) {
			Pi[i] *= c;
		}

		for (size_t i = 0; i < N; i++) {
			c = 0.0;
			A[i].resize(N);
			for (size_t j = 0; j < N; j++) {
				// A[i][j] = 1.0/N + 0.3/N*((double) rand() / (RAND_MAX));
				A[i][j] = 1 + ((double) rand() / (RAND_MAX))*10;
                if (i == j) {
                    A[i][j] = A[i][j] + 10 ;
                }
				c += A[i][j];
			}
			c = 1.0 / c;
			for (size_t j = 0; j < N; j++) {
				A[i][j] *= c;
			}

			c = 0.0;
			B[i].resize(M);
			for (size_t j = 0; j < M; j++) {
				// B[i][j] = 1./M+0.3/M*((double) rand() / (RAND_MAX));
				B[i][j] = 1 + ((double) rand() / (RAND_MAX))*0.5;
                if (i == j) {
                    B[i][j] = B[i][j] + 1;
                }
				c += B[i][j];
			}
			c = 1.0 / c;
			for (size_t j = 0; j < M; j++) {
				B[i][j] *= c;
			}
		}
	}

	HMM::~HMM () { }

	HMM& HMM::operator=(const HMM& Model) {
		if (this == &Model) {
			return *this;
		}
		N = Model.GetN();
		M = Model.GetM();
		Pi.resize(N);
		A.resize(N);
		B.resize(N);
		for (size_t i = 0; i < N; i++) {
			Pi[i] = Model.GetPi(i);
			A[i].resize(N);
			for (size_t j = 0; j < N; j++) {
				A[i][j] = Model.GetA(i, j);
			}
			B[i].resize(M);
			for (size_t j = 0; j < M; j++) {
				B[i][j] = Model.GetB(i, j);
			}
		}
		return *this;
	}

	vector<double> HMM::NextStateDistribution(const vector<double>& CSD) {
		vector<double> NSD(N, 0.0);
		for (size_t j = 0; j < N; j++) {
			for (size_t i = 0; i < N; i++) {
				NSD[j] += CSD[i] * A[i][j];
			}
		}
		return NSD;
	}

	vector<double> HMM::NextObservationDistribution(const vector<double>& NSD) {
		vector<double> NOD(M, 0.0);
		for (size_t j = 0; j < M; j++) {
			for (size_t i = 0; i < N; i++) {
				NOD[j] += NSD[i] * B[i][j];
			}
		}
		return NOD;
	}

	double HMM::ForwardPass(const vector<unsigned>& O) {
		unsigned T = O.size();
		vector<vector<double> > Alpha;
		// vector<double> c;
		Alpha.resize(T);
		// c.resize(T, 0.0);
		for (size_t t = 0; t < T; t++) {
			Alpha[t].resize(N, 0.0);
		}
		// --------------- Forward Algorithm without Scaling--------------- //
		// Compute Alpha(0, i)
		// c[0] = 0.0;
		for (size_t i = 0; i < N; i++) {
			Alpha[0][i] = B[i][O[0]] * Pi[i];
			// c[0] += Alpha[0][i];
		}
		// Scale the Alpha(0, i)
		// c[0] = 1 / c[0];
		// for (size_t i = 0; i < N; i++) {
		// 	Alpha[0][i] *= c[0];
		// }
		// Compute Alpha(t, i)
		for (size_t t = 1; t < T; t++) {
			// c[t] = 0.0;
			for (size_t i = 0; i < N; i++) {
				Alpha[t][i] = 0.0;
				for (size_t j = 0; j < N; j++) {
					Alpha[t][i] += A[j][i] * Alpha[t-1][j];
				}
				Alpha[t][i] *= B[i][O[t]];
				// c[t] += Alpha[t][i];
			}
			// Scale Alpha(t, i)
			// c[t] = 1 / c[t];
			// for (size_t i = 0; i < N; i++) {
			// 	Alpha[t][i] *= c[t];
			// }
		}

		// Return P(O|Lambda)
		double prob = 0.0;
		for (size_t i = 0; i < N; i++) {
	        prob += Alpha[T-1][i];
	    }

		return prob;
	}

	void HMM::BaumWelch(const vector<unsigned>& O) {
		unsigned T = O.size();
		unsigned maxIters = 100;
		double denom, numer, logProb;
		double oldLogProb = -INFINITY;
		vector<vector<double> > Alpha;
		vector<vector<double> > Beta;
		vector<vector<vector<double> > > Gamma;
		vector<vector<double> > DiGamma;
		vector<double> c;
		Alpha.resize(T);
		Beta.resize(T);
		Gamma.resize(T);
		DiGamma.resize(T);
		c.resize(T, 0.0);
		for (size_t t = 0; t < T; t++) {
			Alpha[t].resize(N, 0.0);
			Beta[t].resize(N, 0.0);
			Gamma[t].resize(N);
			DiGamma[t].resize(N, 0.0);
			for (size_t i = 0; i < N; i++) {
				Gamma[t][i].resize(N, 0.0);
			}
		}

		for (size_t Iters = 0; Iters < maxIters; Iters++) {
			// --------------- Forward Algorithm --------------- //
			// Compute Alpha(0, i)
			c[0] = 0.0;
			for (size_t i = 0; i < N; i++) {
				Alpha[0][i] = B[i][O[0]] * Pi[i];
				c[0] += Alpha[0][i];
			}
			// Scale the Alpha(0, i)
			c[0] = 1.0 / c[0];
			for (size_t i = 0; i < N; i++) {
				Alpha[0][i] *= c[0];
			}
			// Compute Alpha(t, i)
			for (size_t t = 1; t < T; t++) {
				c[t] = 0;
				for (size_t i = 0; i < N; i++) {
					Alpha[t][i] = 0.0;
					for (size_t j = 0; j < N; j++) {
						Alpha[t][i] += A[j][i] * Alpha[t-1][j];
					}
					Alpha[t][i] *= B[i][O[t]];
					c[t] += Alpha[t][i];
				}
				// Scale Alpha(t, i)
				c[t] = 1.0 / c[t];
				for (size_t i = 0; i < N; i++) {
					Alpha[t][i] *= c[t];
				}
			}

			// --------------- Backward Algorithm --------------- //
			// Let Beta(T-1, i) = 1, scaled by c[T-1]
			for (size_t i = 0; i < N; i++) {
				Beta[T-1][i] = c[T-1];
			}
			// Beta-pass
			for (int t = T-2; t > -1; t--) {
				for (size_t i = 0; i < N; i++) {
					Beta[t][i] = 0.0;
					for (size_t j = 0; j < N; j++) {
						Beta[t][i] += Beta[t+1][j] * B[j][O[t+1]] * A[i][j];
					}
					// Scale Beta(t, i) with the same scale factor as Alpha(t, i)
					Beta[t][i] *= c[t];
				}
			}

			// --------------- Compute Gamma and Di-gamma --------------- //
			for (size_t t = 0; t < T - 1; t++) {
				denom = 0.0;
				for (size_t i = 0; i < N; i++) {
					for (size_t j = 0; j < N; j++) {
						denom += Alpha[t][i] * A[i][j] * B[j][O[t+1]] * Beta[t+1][j];
					}
				}
				for (size_t i = 0; i < N; i++) {
					DiGamma[t][i] = 0;
					for (size_t j = 0; j < N; j++) {
						Gamma[t][i][j] = Alpha[t][i] * A[i][j] * B[j][O[t+1]] * Beta[t+1][j] / denom;
						DiGamma[t][i] += Gamma[t][i][j];
					}
				}
			}
			// Special case for Di-gamma(T-1, i)
			denom = 0.0;
			for (size_t i = 0; i < N; i++) {
				denom += Alpha[T-1][i];
			}
			for (size_t i = 0; i < N; i++) {
				DiGamma[T-1][i] = Alpha[T-1][i] / denom;
			}

			// --------------- Re-estimate A, B and Pi --------------- //
			// Re-estimate Pi
			for (size_t i = 0; i < N; i++) {
				Pi[i] = DiGamma[0][i];
			}
			// Re-estimate A
			for (size_t i = 0; i < N; i++) {
				for (size_t j = 0; j < N; j++) {
					numer = 0.0;
					denom = 0.0;
					for (size_t t = 0; t < T-1; t++) {
						numer += Gamma[t][i][j];
						denom += DiGamma[t][i];
					}
					A[i][j] = numer / denom;
				}
			}
			// Re-estimate B
			for (size_t i = 0; i < N; i++) {
				for (size_t j = 0; j < M; j++) {
					numer = 0.0;
					denom = 0.0;
					for (size_t t = 0; t < T; t++) {
						if (O[t] == j) {
							numer += DiGamma[t][i];
						}
						denom += DiGamma[t][i];
					}
					B[i][j] = numer / denom;
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
				break;
			}
		}

		/// Make sure there's no zero.
		double toAdd = 1.0 / (8.0 * N);
		double newRowSum = 1.0 + toAdd * N;
		for(int i = 0; i< N; i++){
			for(int j = 0; j<N; j++){
				A[i][j] += toAdd;
				A[i][j] = A[i][j] / newRowSum;
			}
		}
		newRowSum = 1.0 + toAdd * M;
		for(int i = 0; i< N; i++){
			for(int j = 0; j<M; j++){
				B[i][j] += toAdd;
				B[i][j] = B[i][j] / newRowSum;
			}
		}
		newRowSum = 1.0 + toAdd * N;
		for(int i = 0; i< N; i++){
			Pi[i] += toAdd;
			Pi[i] = Pi[i] / newRowSum;
		}

	}
}
