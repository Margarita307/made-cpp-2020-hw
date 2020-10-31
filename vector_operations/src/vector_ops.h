#pragma once
#include <vector>
#include <iostream>

const double EPS_ = 1e-6;

using std::vector;

namespace task {

    vector<double> operator+(const vector<double>& a) {
        return a;
    }

    vector<double> operator-(const vector<double>& a) {
        vector<double> b(a.size());
        for (size_t i = 0; i < a.size(); ++i) {
            b[i] = -a[i];
        }
        return b;
    }

    vector<double> operator+(const vector<double>& a, const vector<double>& b) {
        vector<double> c(a.size());
        for (size_t i = 0; i < a.size(); ++i) {
            c[i] = a[i] + b[i];
        }
        return c;
    }

    vector<double> operator-(const vector<double>& a, const vector<double>& b) {
        return a + (-b);
    }

    double operator*(const vector<double>& a, const vector<double>& b) {
        double c = 0;
        for (size_t i = 0; i < a.size(); ++i) {
            c += a[i] * b[i];
        }
        return c;
    }

    vector<double> operator%(const vector<double>& a, const vector<double>& b) {
        vector<double> c(a.size());
        c[0] = a[1] * b[2] - b[1] * a[2];
        c[1] = b[0] * a[2] - a[0] * b[2];
        c[2] = a[0] * b[1] - b[0] * a[1];
        return c;
    }

    bool operator||(const vector<double>& a, const vector<double>& b) {
        for (size_t i = 0; i < a.size() - 1; ++i) {
            if (abs(a[i] * b[i + 1] - b[i] * a[i + 1]) >= EPS_)
                return false;
        }
        return true;
    }

    bool operator&&(const vector<double>& a, const vector<double>& b) {
        return (a || b) && (a[0] * b[0] >= 0);
    }

    std::ostream& operator<<(std::ostream& os, const vector<double>& a) {
        for (size_t i = 0; i < a.size(); ++i) {
            os << a[i] << " ";
        }
        os << std::endl;
        return os;
    }

    std::istream& operator>>(std::istream& is, vector<double>& a) {
        size_t n;
        is >> n;

    	while (a.size() > n) {
            a.pop_back();
    	}

        for (size_t i = 0; i < n; ++i) {
            is >> a[i];
        }
        return is;
    }

    void reverse(vector<double>& a) {
        for (size_t i = 0; i < a.size() / 2; ++i) {
            std::swap(a[i], a[a.size() - 1 - i]);
        }
    }

    vector<int> operator&(const vector<int>& a, const vector<int>& b) {
        vector<int> c(a.size());
        for (size_t i = 0; i < a.size(); ++i) {
            c[i] = a[i] & b[i];
        }
        return c;
    }

    vector<int> operator|(const vector<int>& a, const vector<int>& b) {
        vector<int> c(a.size());
        for (size_t i = 0; i < a.size(); ++i) {
            c[i] = a[i] | b[i];
        }
        return c;
    }

}  // namespace task
