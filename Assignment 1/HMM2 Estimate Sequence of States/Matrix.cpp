#ifndef MATRIX_CPP
#define MATRIX_CPP

#include <iostream>
#include "Matrix.h"
using namespace std;

template <typename T>
myMatrix<T>::myMatrix(unsigned r, unsigned c, const T& v) {
	mat.resize(r);
	for (unsigned i = 0; i < mat.size(); i++) {
		mat[i].resize(c, v);
	}
	row = r;
	col = c;
}

template <typename T>
myMatrix<T>::~myMatrix() {}

template<typename T>
myMatrix<T>& myMatrix<T>::operator=(const myMatrix<T>& MatB) {
	if (&MatB == this) {
		return *this;
	}

	row = MatB.Get_Row();
	col = MatB.Get_Col();
	mat.resize(row);
	for (unsigned i = 0; i < row; i++) {
		mat[i].resize(col);
		for (unsigned j = 0; j < col; j++) {
			mat[i][j] = MatB(i, j);
		}
	}
	return *this;
}

// --------------- Multiplication ---------------
template<typename T>
myMatrix<T> myMatrix<T>::operator*(const myMatrix<T>& MatB) {
	unsigned rowA = this->row;
	unsigned colA = this->col;
	unsigned rowB = MatB.Get_Row();
	unsigned colB = MatB.Get_Col();
	myMatrix result(rowA, colB, 0.0);

	for (unsigned i = 0; i < rowA; i++) {
		for (unsigned j = 0; j < colB; j++) {
			for (unsigned k = 0; k < rowB; k++) {
				result(i,j) += this->mat[i][k] * MatB(k,j);
			}
		}
	}
	return result;
}

// --------------- Scalar Multiplication ---------------
/*
template<typename T>
myMatrix<T> myMatrix<T>::operator*(const T& ValueB) {
	myMatrix result(row, col, 0.0);

	for (unsigned i = 0; i < row; i++) {
		for (unsigned j = 0; j < col; j++) {
			result(i,j) = this->mat[i][j] * ValueB;
		}
	}

	return result;
}
*/

// --------------- Scalar Division ---------------
/*
template<typename T>
myMatrix<T> myMatrix<T>::operator/(const T& ValueB) {
	myMatrix result(row, col, 0.0);

	for (unsigned i = 0; i < row; i++) {
		for (unsigned j = 0; j < col; j++) {
			result(i,j) = this->mat[i][j] / ValueB;
		}
	}

	return result;
}
*/

// --------------- Elements ---------------
template<typename T>
T& myMatrix<T>::operator()(const unsigned& r, const unsigned& c) {
	return this->mat[r][c];
}

template<typename T>
const T& myMatrix<T>::operator()(const unsigned& r, const unsigned& c) const {
	return this->mat[r][c];
}

// --------------- Number of Row and Col ---------------
template<typename T>
unsigned myMatrix<T>::Get_Row() const {
	return this->row;
}

template<typename T>
unsigned myMatrix<T>::Get_Col() const {
	return this->col;
}
#endif
