#include "Player.hpp"
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <vector>
using namespace std;

namespace ducks {
	Player::Player() {
		GuessModel.resize(COUNT_SPECIES);
		for (int i = 0; i < COUNT_SPECIES; i++) {
			GuessModel[i].clear();
		}
		RevealedSpecies.resize(COUNT_SPECIES, false);
	}

	// -------------------- Shoot -------------------- //
	Action Player::shoot(const GameState &pState, const Deadline &pDue) {
		// Initialization of observation at the beginning of each round
		if (pState.getBird(0).getSeqLength() == 1) {
			AllObservation.resize(pState.getNumBirds());
			for (size_t i = 0; i < pState.getNumBirds(); i++) {
				AllObservation[i].clear();
			}
			ShootModel.resize(pState.getNumBirds());
		}

		for (size_t i = 0; i < pState.getNumBirds(); i++) {
			if (pState.getBird(i).isAlive()) {
				AllObservation[i].push_back(pState.getBird(i).getLastObservation());
			}
		}

		// Don't shoot before enough observations
		if (pState.getBird(0).getSeqLength() < MIN_OBSERVATION_TIME) {
			return cDontShoot;
		}

		// Initialize ShootModel
		for (size_t i = 0; i < pState.getNumBirds(); i++) {
			ShootModel[i].Initialize(HIDDEN_NODE, COUNT_MOVE);
		}

		// Training of shooting models and decision on shooting
		int ShootIndex;
		int ShootMove;
		double MaxBirdNOD;
		vector<double> ShootCSD;
		vector<double> ShootNOD;
		double Black_Pro;
		bool ifBlackBird;

		ShootIndex = -1;
		ShootMove = MOVE_DEAD;
		MaxBirdNOD = 0.0;

		for (size_t i = 0; i < pState.getNumBirds(); i++) {
			ifBlackBird = false;
			if (pState.getBird(i).isAlive()) {
				for (size_t i = 0; i < GuessModel[SPECIES_BLACK_STORK].size(); i++) {
					Black_Pro = GuessModel[SPECIES_BLACK_STORK][i].ForwardPass(AllObservation[i]);
					if (Black_Pro > Black_Bird) {
						ifBlackBird = true;
						std::cerr << "It's a black bird." << '\n';
						break;
					}
				}

				if (! ifBlackBird) {
					ShootModel[i].BaumWelch(AllObservation[i]);
					ShootCSD = ShootModel[i].Pi;
					for (int t = 0; t < pState.getBird(i).getSeqLength(); t++) {
						ShootCSD = ShootModel[i].NextStateDistribution(ShootCSD);
					}
					ShootNOD = ShootModel[i].NextObservationDistribution(ShootCSD);

					// Choose movement of bird i with biggest next observation distribution
					for (int j = 0; j < COUNT_MOVE; j++) {
						if (ShootNOD[j] > SHOOT_THRESHOLD && ShootNOD[j] > MaxBirdNOD) {
							ShootIndex = i;
							ShootMove = j;
							MaxBirdNOD = ShootNOD[j];
						}
					}
				}
			}
		}

		////test
		if (ShootIndex != -1) {
			std::cerr << "Shoot at bird " << ShootIndex << ' ' << ShootMove << '\n';
		}

		return Action(ShootIndex, static_cast<EMovement>(ShootMove));
	}


	// -------------------- Guess -------------------- //
	std::vector<ESpecies> Player::guess(const GameState &pState, const Deadline &pDue) {
		// Initialization of guess
		vector<ESpecies> lGuesses(pState.getNumBirds(), SPECIES_UNKNOWN);
		IfGuess.resize(pState.getNumBirds(), false);
		SpeciesGuess.resize(pState.getNumBirds(), SPECIES_UNKNOWN);

		// Guess randomly in the first round
		if (pState.getRound() == 0) {
			for (int i = 0; i < pState.getNumBirds(); i++) {
				lGuesses[i] = static_cast<ESpecies>((rand() % static_cast<int>(COUNT_SPECIES + 1)));
			}
		}
		else {
			vector<double> BirdProb(COUNT_SPECIES);
			double average, birdMax;
			int birdMaxInd;
			for(int i = 0; i< pState.getNumBirds(); i++){ //BirdProb
				birdMax = 0.0;
				for(int j = 0; j < COUNT_SPECIES; j++){
					average = 0.0;
					for(int k = 0; k < GuessModel[j].size(); k++){
						average += GuessModel[j][k].ForwardPass(AllObservation[i]);
					}
					if (!GuessModel[j].empty()) {
						average = average / GuessModel[j].size();
					}
					BirdProb[j] = average; // Probability that the bird i belongs to the jth species
				}

				// Extract the best species for bird i
				for(int j = 0; j< COUNT_SPECIES; j++){
					if(BirdProb[j] > birdMax){
						birdMax = BirdProb[j];
						lGuesses[i] = static_cast<ESpecies>(j);
					}
				}
			}
		}

		return lGuesses;
	}

	void Player::hit(const GameState &pState, int pBird, const Deadline &pDue) {
		std::cerr << "HIT BIRD!!!" << std::endl;
	}

	void Player::reveal(const GameState &pState, const vector<ESpecies> &pSpecies, const Deadline &pDue) {
		for (size_t i = 0; i < pState.getNumBirds(); i++) {
			if (pSpecies[i] != SPECIES_UNKNOWN) {
				RevealedSpecies[pSpecies[i]] = true;
				HMM AddSpecies(HIDDEN_NODE, COUNT_MOVE);
				AddSpecies.BaumWelch(AllObservation[i]);
				GuessModel[pSpecies[i]].push_back(AddSpecies);
			}
		}
	}

}
