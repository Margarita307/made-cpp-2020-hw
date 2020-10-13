#include "matrix.h"
#include <algorithm>
#include <cmath>

using namespace task;

Matrix::Matrix()
{
	rows.push_back(*new std::vector<double>(1, 1));
	rowsCount = 1;
	columnsCount = 1;
}

Matrix::Matrix(size_t rows, size_t cols)
{
	std::vector<double>* row = new std::vector<double>(cols);
	this->rows = *new std::vector<std::vector<double>>(rows, *row);
	for (size_t i = 0; i < std::min(rows, cols); ++i)
	{
		this->rows[i][i] = 1;
	}
	rowsCount = rows;
	columnsCount = cols;
}

Matrix::Matrix(const Matrix& c)
{
	Matrix res;
	std::vector<double>* row = new std::vector<double>(columnsCount);
	for (size_t i = 0; i < c.rowsCount; ++i)
	{
		for (size_t j = 0; j < c.columnsCount; ++j)
		{
			(*row)[j] = rows[i][j];
		}
		res.rows.push_back(*row);
	}
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
			rows[i][j] = a.rows[i][j];
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
	return rows[row][col];
}

const double& Matrix::get(size_t row, size_t col) const
{
	if (row >= rowsCount || col >= columnsCount)
	{
		throw OutOfBoundsException();
	}
	return rows[row][col];
}

void Matrix::set(size_t row, size_t col, const double& value)
{
	if (row >= rowsCount || col >= columnsCount)
	{
		throw OutOfBoundsException();
	}
	rows[row][col] = value;
}

void Matrix::resize(size_t new_rows, size_t new_cols)
{
	while (rowsCount > new_rows)
	{
		rows.pop_back();
		--rowsCount;
	}
	std::vector<double>* row = new std::vector<double>(columnsCount, 0);
	while (rowsCount < new_rows)
	{
		rows.push_back(*row);
		--rowsCount;
	}
	while (columnsCount > new_cols)
	{
		for (size_t i = 0; i < rowsCount; ++i)
		{
			rows[i].pop_back();
		}
		--columnsCount;
	}
	while (columnsCount < new_cols)
	{
		for (size_t i = 0; i < rowsCount; ++i)
		{
			rows[i].push_back(0);
		}
		++columnsCount;
	}
}

std::vector<double>& Matrix::operator[](size_t row)
{
	if (row >= rowsCount)
	{
		throw OutOfBoundsException();
	}
	return rows[row];
}

const std::vector<double>& Matrix::operator[](size_t row) const
{
	if (row >= rowsCount)
	{
		throw OutOfBoundsException();
	}
	return rows[row];
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
			rows[i][j] = a.rows[i][j];
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
		this->rows[i][i] = 0;
	}
	for (size_t i = 0; i < rowsCount; ++i)
	{
		for (size_t k = 0; k < columnsCount; ++k)
		{
			for (size_t j = 0; j < a.columnsCount; ++j)
			{
				rows[i][j] += rows[i][k] * a.rows[k][j];
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
			rows[i][j] *= number;
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
		return rows[0][0];
	double res = 0;
	Matrix* minor;
	for (size_t i = 0; i < columnsCount; ++i)
	{
		*minor = *this;
		--minor->rowsCount;
		--minor->columnsCount;
		res += std::pow(-1, i + 1) * rows[0][i] * det();
	}
	return res;
}

void Matrix::transpose()
{
	std::swap(columnsCount, rowsCount);
	rows = Matrix::transposed().rows;
}

Matrix Matrix::transposed() const
{
	Matrix* res = new Matrix(columnsCount, rowsCount);
	for (size_t i = 0; i < rowsCount; ++i)
	{
		for (size_t j = 0; j < columnsCount; ++j)
		{
			res->rows[j][i] = rows[i][j];
		}
	}
	return *res;
}

double Matrix::trace() const
{
	if (rowsCount != columnsCount)
		throw new SizeMismatchException;

	double res = 0;
	for (size_t i = 0; i < rows.size(); ++i)
	{
		res += rows[i][i];
	}
	return res;
}

std::vector<double> Matrix::getRow(size_t row)
{
	return rows[row];
}

std::vector<double> Matrix::getColumn(size_t column)
{
	std::vector<double> res;
	for (size_t i = 0; i < rows.size(); ++i)
	{
		res.push_back(rows[i][column]);
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
			if (std::abs(rows[i][j] - a.rows[i][j]) > EPS)
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
