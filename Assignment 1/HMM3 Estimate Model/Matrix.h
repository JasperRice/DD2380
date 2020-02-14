#ifndef MATRIX_H
#define MATRIX_H
#include <vector>
using namespace std;

template <typename T> class myMatrix {
private:
	vector<vector<T> > mat;
	unsigned row;
	unsigned col;
public:
	myMatrix (unsigned r, unsigned c, const T& v);
	virtual ~myMatrix ();

	// --------------- Matrix Operation ---------------
	myMatrix<T>& operator=(const myMatrix<T>& MatB);
	myMatrix<T> operator*(const myMatrix<T>& MatB);

	// --------------- Scalar Operation ---------------
	// myMatrix<T> operator*(const T& ValueB);
	// myMatrix<T> operator/(const T& ValueB);

	// --------------- Elements ---------------
	T& operator()(const unsigned& r, const unsigned& c);
	const T& operator()(const unsigned& r, const unsigned& c) const;

	// --------------- Row and Column ---------------
	unsigned Get_Row() const;
	unsigned Get_Col() const;
};

#include "Matrix.cpp"

#endif
