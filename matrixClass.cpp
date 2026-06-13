#include "matrixClass.h"
#include <sstream>
#include <fstream>

Matrix Matrix::eye(const Fraction& n) {

	BigInteger numerator = n.getNominator();
	std::string str = to_string(numerator);
	int size = std::stoi(str);

	Matrix edinichka(size, size);
	for (int i = 0; i < size; ++i) {
		edinichka.values[i][i] = Fraction(1, 1);
	}
	return edinichka;
}

Matrix Matrix::ones(const Fraction& n, const Fraction& m) {
	BigInteger n_ = n.getNominator();
	BigInteger m_ = m.getNominator();

	int rows = std::stoi(to_string(n_));
	int cols = std::stoi(to_string(m_));

	Matrix allOnes(rows, cols);
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			allOnes.values[i][j] = Fraction(1, 1);
		}
	}

	return allOnes;
}

Matrix Matrix::zeros(const Fraction& n, const Fraction& m) {
	BigInteger n_ = n.getNominator();
	BigInteger m_ = m.getNominator();

	int rows = std::stoi(to_string(n_));
	int cols = std::stoi(to_string(m_));

	Matrix allZeros(rows, cols);
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			allZeros.values[i][j] = Fraction(0, 1);
		}
	}

	return allZeros;
}

Matrix Matrix::sum() {
	Fraction resSum(0, 1);
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			resSum += values[i][j];
		}
	}
	return Matrix(resSum);
}

Matrix Matrix::Multiply(const Matrix& other) {
	if (!isValidForMultiple(other)) {
		throw std::invalid_argument("Invalid for multiplying");

	}

	Matrix Result = Matrix::zeros(rows, other.cols);

	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < other.cols; ++j) {
			for (int k = 0; k < cols; ++k) {
				Result.values[i][j] += values[i][k] * other.values[k][j];
			}
		}
	}

	return Result;
}

Matrix& Matrix::operator +=(const Matrix& other) {
	if (rows != other.rows || cols != other.cols) {
		throw std::invalid_argument("size1 != size2");
	}
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			values[i][j] += other.values[i][j];
		}
	}
	return *this;

}

Matrix& Matrix::operator -=(const Matrix& other) {
	if (rows != other.rows || cols != other.cols) {
		throw std::invalid_argument("size1 != size2");
	}
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			values[i][j] -= other.values[i][j];
		}
	}
	return *this;

}

Matrix& Matrix::operator *=(const Matrix& other) {
	if (rows != other.rows || cols != other.cols) {
		throw std::invalid_argument("size1 != size2");
	}
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			values[i][j] *= other.values[i][j];
		}
	}
	return *this;

}

Matrix& Matrix::operator /=(const Matrix& other) {
	if (rows != other.rows || cols != other.cols) {
		throw std::invalid_argument("size1 != size2");
	}
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			values[i][j] /= other.values[i][j];
		}
	}
	return *this;

}

Matrix Matrix::operator -(const Matrix& other) {

	Matrix C = *this;
	C -= other;
	return C;

}

Matrix Matrix::operator-() const {
	Matrix C = *this;
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			C.values[i][j] = -values[i][j];
		}
	}
	return C;
}

bool Matrix::isSameSize(const Matrix& other) const{
	return rows == other.rows && cols == other.cols;
}

bool Matrix::isValidForMultiple(const Matrix& other) const {
	if ((rows == 1 && cols == 1) || (other.rows == 1 && other.cols == 1)) {
		return true;
	}
	return cols == other.rows;
}

bool Matrix::isNxN(const Matrix& other) const {
	return rows == cols;
}

void Matrix::display(const std::string& varName, std::ostream& outFile) {

	if (!varName.empty()) {
		if (rows == 1 && cols == 1) {
			outFile << varName << " = " << values[0][0] << std::endl;
			return;
		}
		outFile << varName << " = [\n";
	}

	std::vector<std::vector<std::string>> strValues(rows, std::vector<std::string>(cols));
	for (int j = 0; j < cols; ++j) {
		for (int i = 0; i < rows; ++i) {
			std::ostringstream oss;
			oss << values[i][j];
			strValues[i][j] = oss.str();
		}
	}

	std::vector<int> colLength(cols, 0);
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			int len = static_cast<int>(strValues[i][j].length());
			if (len > colLength[j]) {
				colLength[j] = len;
			}
		}
	}

	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			int numberOfSpaces = colLength[j] - static_cast<int>(strValues[i][j].length());
			for (int s = 0; s < numberOfSpaces; ++s) {
				outFile << " ";
			}
			outFile << strValues[i][j];

			if (j < cols - 1) {
				outFile << " ";
			}
		}
		outFile << '\n';
	}

	if (!varName.empty()) {
		outFile << "]\n";
	}
}

Matrix Matrix::transpose() {
	Matrix other(cols, rows);
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			other.values[j][i] = values[i][j];
		}
	}
	return other;
}

Matrix Matrix::add(const Matrix& other) {

	Matrix C = *this;
	C += other;

	return C;
}

Matrix Matrix::minus(const Matrix& other) {

	Matrix C = *this;
	C -= other;

	return C;
}

Matrix Matrix::elementalMultiply(const Matrix& other) {
	Matrix C = *this;
	C *= other;
	return C;
}

Matrix Matrix::elementalDiv(const Matrix& other) {
	Matrix C = *this;
	C /= other;
	return C;
}

Matrix Matrix::prod() {
	Fraction resProd(1, 1);
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			resProd *= values[i][j];
		}
	}
	return Matrix(resProd);
}

Matrix Matrix::minimalElementMatrix(const Matrix& other) {
	Matrix result = Matrix::zeros(rows, cols);
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			result.values[i][j] = std::min(values[i][j], other.values[i][j]);
		}
	}
	return result;
}

Matrix Matrix::maximalElementMatrix(const Matrix& other) {
	Matrix result = Matrix::zeros(rows, cols);
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			result.values[i][j] = std::max(values[i][j], other.values[i][j]);
		}
	}
	return result;
}

Matrix Matrix::unarMinus() const {
	return -(*this);
}

Matrix Matrix::linspace(const Fraction& x0, const Fraction& x1, const Fraction& n) {

	int x0_int = std::stoi(to_string(x0.getNominator()));
	int x1_int = std::stoi(to_string(x1.getNominator()));
	int n_int = std::stoi(to_string(n.getNominator()));

	Matrix res(1, n_int);

	if (n_int == 1) {
		res.values[0][0] = Fraction(x1_int, 1);
		return res;
	}

	Fraction step(x1_int - x0_int, n_int - 1);

	for (int i = 0; i < n_int; ++i) {
		res.values[0][i] = Fraction(x0_int, 1) + Fraction(i, 1) * step;
	}

	return res;
}

Matrix Matrix::det() const {

	if (rows != cols) {
		throw std::invalid_argument("Rows != Cols");
	}

	Fraction determinant(1, 1);

	int n = rows;

	Matrix tmp(n, n);

	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			tmp.values[i][j] = this->values[i][j];
		}
	}

	for (int i = 0; i < n; ++i) {

		if (tmp.values[i][i] == Fraction(0, 1)) {
			bool detExist = false;
			for (int j = i + 1; j < n; ++j) {
				if (tmp.values[j][i] != Fraction(0, 1)) {
					for (int k = 0; k < n; ++k) {
						std::swap(tmp.values[i][k], tmp.values[j][k]);
					}
					determinant *= Fraction(-1, 1);
					detExist = true;
					break;
				}
			}
			if (!detExist) {
				return Fraction(0, 1);
			}
		}
		//приведем к верхнетреугольной матрице
		for (int j = i + 1; j < n; ++j) {
			Fraction multiplier = tmp.values[j][i] / tmp.values[i][i];
			for (int k = i; k < n; ++k) {
				tmp.values[j][k] -= multiplier * tmp.values[i][k];
			}
		}

		determinant *= tmp.values[i][i];
	}
	return determinant;
}

Matrix Matrix::inv() const {
	if (rows != cols) {
		throw std::invalid_argument("Rows != Cols");
	}
	int n = rows;
	Matrix tmp(n, 2 * n);

	// обычная матрица + единичная
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			tmp.values[i][j] = this->values[i][j];
		}
		for (int j = n; j < 2 * n; ++j) {
			tmp.values[i][j] = (i == j - n) ? Fraction(1, 1) : Fraction(0, 1);
		}
	}

	// верхнетреугольный вид
	for (int i = 0; i < n; ++i) {
		// поиск ненулевого элемента
		if (tmp.values[i][i] == Fraction(0, 1)) {
			for (int k = i + 1; k < n; ++k) {
				if (tmp.values[k][i] != Fraction(0, 1)) {
					for (int j = 0; j < 2 * n; ++j) {
						std::swap(tmp.values[i][j], tmp.values[k][j]);
					}
					break;
				}
			}
			
		}

		// приведение к 1 на главной диагонали
		Fraction to_one = tmp.values[i][i];
		for (int j = 0; j < 2 * n; ++j) {
			tmp.values[i][j] /= to_one;
		}

		// обнуление элементов под диагональю
		for (int k = i + 1; k < n; ++k) {
			Fraction factor = tmp.values[k][i];
			for (int j = 0; j < 2 * n; ++j) {
				tmp.values[k][j] -= factor * tmp.values[i][j];
			}
		}
	}

	// обнуление элементов над диагональю
	for (int i = n - 1; i >= 0; --i) {
		for (int k = i - 1; k >= 0; --k) {
			Fraction factor = tmp.values[k][i];
			for (int j = 0; j < 2 * n; ++j) {
				tmp.values[k][j] -= factor * tmp.values[i][j];
			}
		}
	}

	Matrix result(n, n);
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			result.values[i][j] = tmp.values[i][j + n];
		}
	}

	return result;
}

Matrix Matrix::del(const Matrix& other) {
	return Multiply(other.inv());
}

Matrix Matrix::horzcat(const Matrix& other) const {
	if (this->rows != other.rows) {
		throw std::invalid_argument("rows1 != rows2");
	}

	Matrix horzcat_matrix(this->rows, this->cols + other.cols);

	for (int i = 0; i < this->rows; ++i) {
		for (int j = 0; j < this->cols; ++j) {
			horzcat_matrix.values[i][j] = this->values[i][j];
		}
	}

	for (int i = 0; i < other.rows; ++i) {
		for (int j = 0; j < other.cols; ++j) {
			horzcat_matrix.values[i][j + this->cols] = other.values[i][j];
		}
	}

	return horzcat_matrix;
}

Matrix Matrix::vertcat(const Matrix& other) const {
	if (this->cols != other.cols) {
		throw std::invalid_argument("cols1 != cols2");
	}

	Matrix vertcat_matrix(this->rows + other.rows, this->cols);

	for (int i = 0; i < this->rows; ++i) {
		for (int j = 0; j < this->cols; ++j) {
			vertcat_matrix.values[i][j] = this->values[i][j];
		}
	}

	for (int i = 0; i < other.rows; ++i) {
		for (int j = 0; j < other.cols; ++j) {
			vertcat_matrix.values[i + this->rows][j] = other.values[i][j];
		}
	}

	return vertcat_matrix;
}

Matrix Matrix::horzcat_all(const std::vector<Matrix>& allMatrix) {

	if (allMatrix.size() == 1) {
		return allMatrix[0];
	}

	Matrix res = allMatrix[0];

	for (int i = 1; i < allMatrix.size(); ++i) {
		res = res.horzcat(allMatrix[i]);
	}

	return res;
}

Matrix Matrix::vertcat_all(const std::vector<Matrix>& allMatrix) {

	if (allMatrix.size() == 1) {
		return allMatrix[0];
	}

	Matrix res = allMatrix[0];

	for (int i = 1; i < allMatrix.size(); ++i) {
		res = res.vertcat(allMatrix[i]);
	}

	return res;
}

Fraction Matrix::toNumber() const {
	return values[0][0];
}
