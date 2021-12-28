#include<stdio.h>

typedef vector<double, 2> point; 
typedef vector<double, 3> point3d; 

template<class T, int N> class vector { 
    T component[N];

    public:
        vector(const T&); 
        vector(const vector&); 
        const vector& operator=(const vector&); 
        const vector& operator=(const T&); 

        ~vector() { 
        }  

        const T& operator[](int i) const { 
            return component[i]
        }

        void set (int i, const T& a) { 
            component[i] = a;
        } // set ith component to a 

        const vector& operator=(const vector&); 
        const vector& operator=(const T&); 
        const vector& operator+=(const vector&);
        const vector& operator+=(const T&); 
        const vector& operator-=(const vector&);
        const vector& operator*=(const T&);
        const vector& operator/=(const T&);
};
