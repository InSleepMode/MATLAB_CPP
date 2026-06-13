#ifndef MATRIXCLASS_H
#define MATRIXCLASS_H

#include "biginteger.hpp"
#include "classDrobi.h"
#include <vector>
#include <iostream>
#include <fstream>

class Matrix {

private:
	int rows;
	int cols;
	std::vector<std::vector<Fraction>> values;

public:
	Matrix() : rows(0), cols(0) {}
	
	Matrix(int rows_, int cols_) : rows(rows_), cols(cols_), values(rows, std::vector<Fraction>(cols_, Fraction(0,1))) {} 
	Matrix(int rows_, int cols_, const Fraction& fillValue) : rows(rows_), cols(cols_), values(rows_, std::vector<Fraction>(cols_, fillValue)) {}

	Matrix(const Fraction& fillValue) : rows(1), cols(1) {
		values = std::vector<std::vector<Fraction>>(1, std::vector<Fraction>(1, fillValue));
	}

	Matrix(const Matrix& other)	: rows(other.rows), cols(other.cols) {
		values = other.values;
	}

	~Matrix() = default;

	Matrix& operator +=(const Matrix& other);

	Matrix& operator -=(const Matrix& other);

	Matrix& operator *=(const Matrix& other);

	Matrix& operator /=(const Matrix& other);
	
	Matrix operator -(const Matrix& other);

	Matrix operator-() const;
	
	bool isSameSize(const Matrix& other) const;

	bool isValidForMultiple(const Matrix& other) const;

	bool isNxN(const Matrix& other) const;
	
	void display(const std::string& name, std::ostream& outFile);
	
	Matrix transpose();

	static Matrix eye(const Fraction& n);

	Matrix add(const Matrix& other);

	Matrix minus(const Matrix& other);

	Matrix elementalMultiply(const Matrix& other);

	Matrix elementalDiv(const Matrix& other);
	
	static Matrix zeros(const Fraction& n, const Fraction& m);

	Matrix Multiply(const Matrix& other);

	static Matrix ones(const Fraction& n, const Fraction& m);

	Matrix sum();

	Matrix prod();

	Matrix minimalElementMatrix(const Matrix& other);
	
	Matrix maximalElementMatrix(const Matrix& other);
	
	Matrix unarMinus() const;

	static Matrix linspace(const Fraction& x0, const Fraction& x1, const Fraction& n);

	Matrix det() const;

	Matrix inv() const;

	Matrix del(const Matrix& other);

	Matrix horzcat(const Matrix& other) const;

	Matrix vertcat(const Matrix& other) const;

	static Matrix horzcat_all(const std::vector<Matrix>& allMatrix);

	static Matrix vertcat_all(const std::vector<Matrix>& allMatrix);

	Fraction toNumber() const;
};

#endif
