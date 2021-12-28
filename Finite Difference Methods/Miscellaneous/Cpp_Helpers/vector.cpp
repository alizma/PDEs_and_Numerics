#include "vector.h"

// constructor
template<class T, int N> vector<T, N>::vector(const T& a = 0) { 
    for (int i =  0; i < N; i++) { 
        component[i] = a; 
    }
}

// copy constructor 
template<class T, int N> vector<T, N>::vector(const vector<T, N>& v) { 
    for (int i = 0; i < N; i++) { 
        component[i] = v.component[i];
    }
}

// assignment operator
template<class T, int N> const vector<T, N>& vector<T, N>::operator=(const vector<T, N>& v) { 
    if (this != &v) { 
        for (int i = 0; i < N; i++) { 
            component[i] = v.component[i]; 
        }
    }
    return *this; 
}

// scalar assignment operator  
template<class T, int N> const vector<T, N>& vector<T, N>::operator=(const T& a){ 
    for (int i = 0; i < N; i++) { 
        component[i] = a; 
    }
    return *this; 
}

// scalar multiplication 
template<class T, int N> const vector<T, N>& vector<T, N>::operator*=(const T& a) {
    for (int i = 0; i < N; i++) { 
        component[i] *= a; 
    }

    return *this; 
}

// scalar addition 
template<class T, int N> const vector<T, N>& vector<T, N>::operator+=(const T& a) {
    for (int i = 0; i < N; i++) { 
        component[i] += a; 
    }

    return *this; 
}

// scalar division 
template<class T, int N> const vector<T, N>& vector<T, N>::operator/=(const T& a) {
    for (int i = 0; i < N; i++) { 
        component[i] /= a; 
    }

    return *this; 
}

// addition of vector to current vector object 
template<class T, int N> const vector<T, N>& vector<T, N>::operator+=(const vector<T, N> &v) { 
    for (int i = 0; i < N; i++) { 
        component[i] += v[i]; 
    }

    return *this; 
}

// addition of two vector<T, N> 
template<class T, int N> const vector<T, N>& operator+(const vector<T, N>& a, const vector<T,N>& b) { 
    return vector<T, N> (a) += b; 
}

/* unary operators */ 
// + v = v
template<class T, int N> const vector<T, N>& operator+(const vector<T, N> &a) { 
    return u; 
}

template<class T, int N> const vector<T, N>& operator-(const vector<T, N> &a) { 
    return vector<T,N>(a) *= -1; 
}

template<class T, int N> const T operator*(const vector<T, N> &a, const vector<T, N> &b) { 
    T sum = 0; 
    for (int i = 0; i < N; i++) { 
        sum += a[i] * b[i]; 
    }

    return sum; 
}

template<class T, int N> T squaredNorm(const vector<T, N> &a) { 
    return a*a; 
}

template<class T, int N> void print(const vector<T, N>&v) { 
    printf("("); 
    for (int i =0; i < N; i++) { 
        printf("v[&d]=", i); 
        print(v[i]); 
    }
    printf(")\n"); 
}