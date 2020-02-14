#include <iostream>
#include "Matrix.h"
using namespace std;

int main(int argc, char const *argv[]) {
    unsigned r, c;
    double v;

    // --------------- Initialize Matries --------------- //
    cin >> r >> c;
    myMatrix<double> Transition(r, c, 0.0);
    for (size_t i = 0; i < r; i++) {
        for (size_t j = 0; j < c; j++) {
            cin >> v;
            Transition(i, j) = v;
        }
    }

    cin >> r >> c;
    myMatrix<double> Emission(r, c, 0.0);
    for (size_t i = 0; i < r; i++) {
        for (size_t j = 0; j < c; j++) {
            cin >> v;
            Emission(i, j) = v;
        }
    }

    cin >> r >> c;
    myMatrix<double> Pi(r, c, 0.0);
    for (size_t i = 0; i < r; i++) {
        for (size_t j = 0; j < c; j++) {
            cin >> v;
            Pi(i, j) = v;
        }
    }

    myMatrix<double> Result(1, 1, 0.0);
    Result = Pi * Transition;
    Result = Result * Emission;
    cout << Result.Get_Row() << " " << Result.Get_Col() << " ";
    for (size_t i = 0; i < Result.Get_Row(); i++) {
        for (size_t j = 0; j < Result.Get_Col(); j++) {
            std::cout << Result(i, j) << " ";
        }
    }
    cout << '\n';

    return 0;
}
