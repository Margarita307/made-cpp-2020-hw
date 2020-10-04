#pragma once
#include <vector>
#include <iostream>

const double EPS_ = double(1e-10);

using namespace std;

namespace task {

	vector<double> operator+ (const vector<double>& a)
	{
		return a;
	}

	vector<double> operator- (const vector<double>& a)
	{
		vector<double> b;
		for (size_t i = 0; i < a.size(); ++i)
		{
			b.push_back(-a[i]);
		}
		return b;
	}

	vector<double> operator+ (const vector<double>& a, const vector<double>& b)
	{
		vector<double> c;
		for (size_t i = 0; i < a.size(); ++i)
		{
			c.push_back(a[i] + b[i]);
		}
		return c;
	}

	vector<double> operator- (const vector<double>& a, const vector<double>& b)
	{
		return a + (-b);
	}

	double operator* (const vector<double>& a, const vector<double>& b)
	{
		double c = 0;
		for (size_t i = 0; i < a.size(); ++i)
		{
			c += a[i] * b[i];
		}
		return c;
	}

	vector<double> operator% (const vector<double>& a, const vector<double>& b)
	{
		vector<double> c;
		c.push_back(a[1] * b[2] - b[1] * a[2]);
		c.push_back(b[0] * a[2] - a[0] * b[2]);
		c.push_back(a[0] * b[1] - b[0] * a[1]);
		return c;
	}

	bool operator|| (const vector<double>& a, const vector<double>& b)
	{
		for (size_t i = 0; i < a.size() - 1; ++i)
		{
			if (abs(a[i] * b[i + 1] - b[i] * a[i + 1]) >= EPS_)
				return false;
		}
		return true;
	}

	bool operator&& (const vector<double>& a, const vector<double>& b)
	{
		return (a || b) && ((b[0] == 0 && a[0] >= 0) || a[0] / b[0] >= 0);
	}

	ostream& operator<< (ostream& os, const vector<double>& a)
	{
		for (size_t i = 0; i < a.size(); ++i)
		{
			os << a[i] << " ";
		}
		os << endl;
		return os;
	}

	istream& operator>> (istream& is, vector<double>& a)
	{
		size_t n;
		is >> n;

		a.clear();

		double el;
		for (size_t i = 0; i < n; ++i)
		{
			is >> el;
			a.push_back(el);
		}
		return is;
	}

	vector<double> operator>> (const size_t n, vector<double>& a)
	{
		a.resize(n);
		return a;
	}

	void reverse(vector<double>& a)
	{
		for (size_t i = 0; i < a.size() / 2; ++i)
		{
			swap(a[i], a[a.size() - 1 - i]);
		}
	}

	vector<int> operator& (const vector<int>& a, const vector<int>& b)
	{
		vector<int> c;
		for (size_t i = 0; i < a.size(); ++i)
		{
			c.push_back(a[i] & b[i]);
		}
		return c;
	}

	vector<int> operator| (const vector<int>& a, const vector<int>& b)
	{
		vector<int> c;
		for (size_t i = 0; i < a.size(); ++i)
		{
			c.push_back(a[i] | b[i]);
		}
		return c;
	}

}  // namespace task
