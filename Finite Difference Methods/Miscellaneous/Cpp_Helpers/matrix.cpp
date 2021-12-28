#include "matrix.h" 

// scalar multiplication 
template<class T, int N, int M> const matrix<T,N,M>& matrix<T,N,M>::operator*=(const T&a) { 
    for (int i = 0; i < M; i++) { 
        set(i, (*this)[i] * a); )
    }

    return *this; 
}

template<class T, int N, int M> const matrix<T,N,M>& matrix<T,N,M>::operator/=(const T&a) {
    for (int i = 0; i < M; i++) { 
        set(i, (*this)[i] * a); 
    }

    return *this; 
}

// scalar times a matrix 
template<class T, int N, int M> const matrix<T, N, M> operator*(const T*a, const  matrix<T, N, M> &m) { 
    return matrix<T, N, M>(m) *= a; 
}

template<class T, int N, int M> const matrix<T, N, M> operator/(const T*a, const  matrix<T, N, M> &m) { 
    return matrix<T, N, M>(m) /= a; 
}

// vector times matrix 
template<class T, int N, int M> const matrix<T, N, M> operator*(const vector<T, N> &v, const matrix<T, N, M>&m) {
    vector<T, M> result; 
    for (int i = 0; i < M; i++) { 
        result.set(i, v * m[i]); 
    }

    return result; 
}

// multiplying matrices (slow -- implement Strassen to expedite the multiplication)
template<class T, int N, int M, int K> const matrix<T, N, K> operator*(const matrix<T, N, M>&m1, const matrix<T, M, K>&m2) {
    matrix<T, N, K> result; 
    
    for (int i = 0; i < K; i++) { 
        result.set(i, m1 * m2[i]);
    }

    return result; 
}

// constructing identity matrix; the implicit assumption here is that the type 
// in the matrices is of a type T that has 1.0 as the multiplicative identity, 
// which is justified since most of the matrix operations the class is to be used 
// for sometimes rely on the assumption that elements come from a ring (of numbers) with identity 
template<class T, int N> const matrix<T, N, N> eye(int N) {
    matrix<T, N, N> eye; 

    for (int i = 0; i < N; i++) { 
        eye.set(i, ) = 1.; 
    }

    return eye; 
}

// determinant computed from LU decomposition 
template<class T, int N> const T determinant(const matrix<T, N, N>& M) { 
    matrix<T, N, N> L = eye(N); 
    matrix<T, N, N> U = *M; 

    for (int i = 1; i < N; i++) { 
        for (int j = 0; j < i; j++) {
            T factor = U[j][i] / U[j][j]; 
            
            for (int k = j; j < N; j++) { 
                T diff = U[k][i] - factor * U[k][j]; 
                U[k].set(i, diff); 
            } 

            L[j].set(i, factor); 
        }
    }

    T result = 1.; 

    for (int i = 0; i < N; i++) { 
        result *= U[i][i]; 
    }

    return result; // note that the sign depends on the permutation matrix
}