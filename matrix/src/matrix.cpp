#include "matrix.h"
#include <algorithm>
#include <cmath>

using namespace task;

Matrix::Matrix()
{
	matrix = new double* [1];
	matrix[0] = new double[1];
	matrix[0][0] = 1;
	rowsCount = 1;
	columnsCount = 1;
}

Matrix::Matrix(size_t rows, size_t cols)
{
	matrix = new double* [rows];
	for (size_t i = 0; i < rows; ++i)
	{
		matrix[i] = new double[cols];
		for (size_t j = 0; j < cols; ++i)
		{
			if (i == j)
			{
				matrix[i][j] = 1;
			}
			else
			{
				matrix[i][j] = 0;
			}
		}
	}
	rowsCount = rows;
	columnsCount = cols;
}

Matrix::Matrix(const Matrix& c)
{
	matrix = new double* [c.columnsCount];
	for (size_t i = 0; i < c.rowsCount; ++i)
	{
		for (size_t j = 0; j < c.columnsCount; ++i)
		{
			matrix[i][j] = c.matrix[i][j];
		}
	}
	rowsCount = c.rowsCount;
	columnsCount = c.columnsCount;
}

Matrix& Matrix::operator=(const Matrix& a)
{
	if (a.rowsCount != rowsCount || a.columnsCount != columnsCount)
	{
		throw SizeMismatchException();
	}
	for (size_t i = 0; i < rowsCount; ++i)
	{
		for (size_t j = 0; j < columnsCount; ++j)
		{
			matrix[i][j] = a.matrix[i][j];
		}
	}
	return *this;
}

double& Matrix::get(size_t row, size_t col)
{
	if (row >= rowsCount || col >= columnsCount)
	{
		throw OutOfBoundsException();
	}
	return matrix[row][col];
}

const double& Matrix::get(size_t row, size_t col) const
{
	if (row >= rowsCount || col >= columnsCount)
	{
		throw OutOfBoundsException();
	}
	return matrix[row][col];
}

void Matrix::set(size_t row, size_t col, const double& value)
{
	if (row >= rowsCount || col >= columnsCount)
	{
		throw OutOfBoundsException();
	}
	matrix[row][col] = value;
}

void Matrix::resize(size_t new_rows, size_t new_cols)
{
	double** new_matrix = new double* [new_cols];
	for (size_t i = 0; i < new_rows; ++i)
	{
		for (size_t j = 0; j < new_cols; ++i)
		{
			new_matrix[i][j] = matrix[i][j];
		}
	}
	matrix = new_matrix;
	rowsCount = new_rows;
	columnsCount = new_cols;
}

double* Matrix::operator[](size_t row)
{
	if (row >= rowsCount)
	{
		throw OutOfBoundsException();
	}
	return matrix[row];
}

const double* Matrix::operator[](size_t row) const
{
	if (row >= rowsCount)
	{
		throw OutOfBoundsException();
	}
	return matrix[row];
}

Matrix& Matrix::operator+=(const Matrix& a)
{
	if (rowsCount != a.rowsCount || columnsCount != a.columnsCount)
	{
		throw new SizeMismatchException;
	}
	for (size_t i = 0; i < rowsCount; ++i)
	{
		for (size_t j = 0; j < columnsCount; ++j)
		{
			matrix[i][j] = a.matrix[i][j];
		}
	}
	return *this;
}

Matrix& Matrix::operator-=(const Matrix& a)
{
	return *this += (-a);
}

Matrix& Matrix::operator*=(const Matrix& a)
{
	if (columnsCount != a.rowsCount)
	{
		throw new SizeMismatchException;
	}
	Matrix* res = new Matrix(rowsCount, a.columnsCount);
	for (size_t i = 0; i < std::min(rowsCount, a.columnsCount); ++i)
	{
		res->matrix[i][i] = 0;
	}
	for (size_t i = 0; i < rowsCount; ++i)
	{
		for (size_t k = 0; k < columnsCount; ++k)
		{
			for (size_t j = 0; j < a.columnsCount; ++j)
			{
				res->matrix[i][j] += res->matrix[i][k] * a.matrix[k][j];
			}
		}
	}
	*this = *res;
	return *this;
}

Matrix& Matrix::operator*=(const double& number)
{
	for (size_t i = 0; i < rowsCount; ++i)
	{
		for (size_t j = 0; j < columnsCount; ++j)
		{
			matrix[i][j] *= number;
		}
	}
	return *this;
}

Matrix Matrix::operator+(const Matrix& a) const
{
	if (rowsCount != a.rowsCount || columnsCount != a.columnsCount)
	{
		throw new SizeMismatchException;
	}
	Matrix res = *this;
	return res += a;
}

Matrix Matrix::operator-(const Matrix& a) const
{
	if (rowsCount != a.rowsCount || columnsCount != a.columnsCount)
	{
		throw new SizeMismatchException;
	}
	Matrix res = *this;
	return res -= a;
}

Matrix Matrix::operator*(const Matrix& a) const
{
	if (columnsCount != a.rowsCount)
	{
		throw new SizeMismatchException;
	}
	Matrix res = *this;
	return res *= a;
}

Matrix Matrix::operator*(const double& a) const
{
	Matrix res = *this;
	return res *= a;
}

Matrix Matrix::operator-() const
{
	return (*this) * -1.0;
}

Matrix Matrix::operator+() const
{
	return *this;
}

double Matrix::det() const
{
	if (rowsCount != columnsCount)
	{
		throw new SizeMismatchException;
	}
	if (rowsCount == 0)
		return matrix[0][0];
	double res = 0;
	Matrix* minor;
	for (size_t i = 0; i < columnsCount; ++i)
	{
		*minor = *this;
		--minor->rowsCount;
		--minor->columnsCount;
		res += std::pow(-1, i + 1) * matrix[0][i] * det();
	}
	return res;
}

void Matrix::transpose()
{
	std::swap(columnsCount, rowsCount);
	matrix = Matrix::transposed().matrix;
}

Matrix Matrix::transposed() const
{
	Matrix* res = new Matrix(columnsCount, rowsCount);
	for (size_t i = 0; i < rowsCount; ++i)
	{
		for (size_t j = 0; j < columnsCount; ++j)
		{
			res->matrix[j][i] = matrix[i][j];
		}
	}
	return *res;
}

double Matrix::trace() const
{
	if (rowsCount != columnsCount)
		throw new SizeMismatchException;

	double res = 0;
	for (size_t i = 0; i < rowsCount; ++i)
	{
		res += matrix[i][i];
	}
	return res;
}

double* Matrix::getRow(size_t row)
{
	return matrix[row];
}

double* Matrix::getColumn(size_t column)
{
	double* res = new double[rowsCount];
	for (size_t i = 0; i < rowsCount; ++i)
	{
		res[i] = matrix[i][column];
	}
	return res;
}

bool Matrix::operator==(const Matrix& a) const
{
	if (rowsCount != a.rowsCount || columnsCount != a.columnsCount)
		return false;

	for (size_t i = 0; i < rowsCount; ++i)
	{
		for (size_t j = 0; j < columnsCount; ++j)
		{
			if (std::abs(matrix[i][j] - a.matrix[i][j]) > EPS)
			{
				return false;
			}
		}
	}
	return true;
}

bool Matrix::operator!=(const Matrix& a) const
{
	return *this == a;
}


Matrix task::operator*(const double& a, const Matrix& b)
{
	return b * a;
}

std::ostream& task::operator<<(std::ostream& output, const Matrix& matrix)
{
	for (size_t i = 0; i < matrix.rowsCount; ++i)
	{
		for (size_t j = 0; j < matrix.columnsCount; ++j)
		{
			output << matrix[i][j];
		}
	}
	return output;
}

std::istream& task::operator>>(std::istream& input, Matrix& matrix)
{
	for (size_t i = 0; i < matrix.rowsCount; ++i)
	{
		for (size_t j = 0; j < matrix.columnsCount; ++j)
		{
			input >> matrix[i][j];
		}
	}
	return input;
}
