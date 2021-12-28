#include "vector.h"

typedef matrix<double, 2,2> matrix2d; 
typedef matrix<double, 3,3> matrix3d; 

// T - type of elements 
// N - number of rows 
// M - number of columns; N x M matrix objects
template<class T, int N, int M> class matrix : public vector<vector<T, N>, M> { 
    public:
    
        matrix() {}

        matrix(const vector<T, N>&u, const vector<T, N>&v) {
            set(0, u); 
            set(1, v); 
        }

        // copy constructor 
        matrix(const matrix<T, N, M> &m1) { 
            for (int i = 0; i < M; i++) {
                set(i, m1[i]);
            }
        }

        // return the (i, j)th matrix entry 
        const T& operator() (int i, int j) const { 
            return (*this)[j][i];
        }

        const matrix& operator*=(const T&); 
        const matrix& operator/=(const T&); 
};