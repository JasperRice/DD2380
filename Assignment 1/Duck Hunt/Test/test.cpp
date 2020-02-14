#include <cmath>
#include <iostream>
#include <vector>
#include "HiddenMarkovModel.hpp"
using namespace std;
using namespace ducks;

// int start_s=clock();

// int stop_s=clock();
// cout << "time: " << (stop_s-start_s)/double(CLOCKS_PER_SEC)*1000 << endl;

int main(int argc, char const *argv[])
{
	bool test = true;	// true: Test with cases in HMM3
						// false: Test with Bird Emissions

	HMM TestHMM;
	vector<unsigned> tO;

	if (test) {
		TestHMM.Test();
		int T;
		cin >> T;
		tO.resize(T);
		for (size_t i = 0; i < tO.size(); ++i)
		{
			cin >> tO[i];
		}
		TestHMM.BaumWelch(tO);
		TestHMM.PrintTransition();
		TestHMM.PrintEmission();
		TestHMM.PrintPi();
	}
	else {
		int trash;
		cin >> trash >> trash;

		vector<unsigned> allO(101,0);
		for (size_t i = 0; i < allO.size(); i++)
		{
			cin >> allO[i];
		}

		vector<double> NSD(HIDDEN_NODE, 0.0);
		vector<double> NOD;

		for (size_t length = 1; length <= allO.size(); length++) {
			TestHMM.Initialize(HIDDEN_NODE, COUNT_MOVE);
			tO.resize(length);
			for (size_t i = 0; i < tO.size(); ++i)
			{
				tO[i] = allO[i];
			}
			TestHMM.BaumWelch(tO);
			// TestHMM.PrintTransition();
			// TestHMM.PrintEmission();
			// TestHMM.PrintPi();

			for (unsigned i = 0; i < NSD.size(); ++i)
			{
				NSD[i] = TestHMM.GetPi(i);
			}

			for (unsigned i = 0; i < tO.size(); ++i)
			{
				NSD = TestHMM.NextStateDistribution(NSD);
			}
			NOD = TestHMM.NextObservationDistribution(NSD);

			////test
			double sum = 0.0;
			cout << "# Time length " << length << ". Next Observation Distribution: ";
			for (size_t i = 0; i < NOD.size(); i++) {
				sum += NOD[i];
				cout << NOD[i];
				if (i != NOD.size()-1) {
					cout << '+';
				}
			}
			cout << "=" << sum;
			cout << "\n\n------------------------------\n";
		}
	}

	return 0;
}
