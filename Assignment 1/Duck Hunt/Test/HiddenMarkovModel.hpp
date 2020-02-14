#ifndef _DUCKS_HIDDENMARKOVMODEL_HPP_
#define _DUCKS_HIDDENMARKOVMODEL_HPP_

#include "Constants.hpp"
#include <cmath>
#include <vector>
using namespace std;

namespace ducks {
    // Try different numbers here
    const unsigned MIN_OBSERVATION_TIME = 25;
    const unsigned HIDDEN_NODE = 10;
    const double GUESS_THRESHOLD = 0.95;
    const double SHOOT_THRESHOLD = 0.7;
    const double SHOOT_ANYWAY = 0.9;

    class HMM {
    private:
        unsigned N;
        unsigned M;
        vector<double> Pi;
        vector<vector<double> > A;
        vector<vector<double> > B;
        friend class Player;
    public:
        // Test Initialize with input of hmm3.
        void Test();

        HMM();
        HMM(unsigned tN, unsigned tM);
        void Initialize(unsigned tN, unsigned tM);
		~HMM();

        HMM& operator=(const HMM& Model);

        vector<double> NextStateDistribution(const vector<double>& CSD);
        vector<double> NextObservationDistribution(const vector<double>& NSD);
        double ForwardPass(const vector<unsigned>& O);
        void BaumWelch(const vector<unsigned>& O);

        unsigned GetN() const { return N; }
        unsigned GetM() const { return M; }
        double& GetPi(const unsigned& i) { return Pi[i]; }
        double& GetA(const unsigned& i, const unsigned& j) { return A[i][j]; }
        double& GetB(const unsigned& i, const unsigned& j) { return B[i][j]; }
        const double& GetPi(const unsigned& i) const { return Pi[i]; }
        const double& GetA(const unsigned& i, const unsigned& j) const { return A[i][j]; }
        const double& GetB(const unsigned& i, const unsigned& j) const { return B[i][j]; }

        // Test
        void PrintTransition();
        void PrintEmission();
        void PrintPi();
    };
}

#endif
