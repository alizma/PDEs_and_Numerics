#include "euler_bernoulli_beam.h"
#include "matrix_util.h"

double determinant(std::vector<std::vector<double>>& A) { 
    if (A.size() != A[0].size()) { 
        throw std::runtime_error("Matrix not square.");
    }

    int dimension = A.size(); 

    switch (dimension){
        case 0: return 1; 
        case 1: return A[0][0];
        case 2: return A[0][0] * A[1][1] - A[0][1] * A[1][0]; 
        default: 
            double D = 0; 
            int sgn = 1;
            for (int idx = 0; idx < dimension; idx++) {
                std::vector<std::vector<double>> subMatrix(dimension - 1, std::vector<double> (dimension-1));

                // copy values from main matrix to submatrix to prepare for recursive call (cofactor)
                for (int m = 1; m < dimension; m++) { 
                    int z = 0; 
                    for (int n = 0; n < dimension; n++) { 
                        if (n != idx) { 
                            subMatrix[m-1][z] = A[m][n]; 
                            z++; 
                        }
                    }
                }

                D = D + sgn * A[0][idx] * determinant(subMatrix);
                sgn = -sgn; 
            }

            return D; 
    }
}

std::vector<std::vector<double>> transpose(std::vector<std::vector<double>>& A) { 
    // to compute rectangular matrix transpose 
    int height = A.size(), width = A[0].size(); 

    std::vector<std::vector<double>> sol(width, std::vector<double> (height));

    for (int idx = 0; idx < height; idx++) { 
        for (int jdx = 0; jdx < width; jdx++) { 
            sol[jdx][idx] = A[idx][jdx]; 
        }
    }

    return sol;
}

std::vector<std::vector<double>> getCofactor(std::vector<std::vector<double>>& A) { 
    int dim = A.size(); 
    if (dim != A[0].size()) { 
        throw std::runtime_error("Matrix not square.");
    }

    std::vector<std::vector<double>> sol(dim, std::vector<double>(dim));
    std::vector<std::vector<double>> subMatrix(dim - 1, std::vector<double>(dim - 1)); 

    for (int idx = 0; idx < dim; idx++) { 
        for (int jdx = 0; jdx < dim; jdx++) { 
            int p = 0; 

            for (int x = 0; x < dim; x++) { 
                if (x == idx) { 
                    continue; 
                }

                int q = 0; 

                for (int y = 0; y < dim; y++) { 
                    if (y == jdx) { 
                        continue;
                    }

                    subMatrix[p][q] = A[x][y]; 
                    q++; 
                }
                p++; 
            }
            sol[idx][jdx] = pow(-1, idx+jdx) * determinant(subMatrix); 
        }
    } 

    return sol; 
}

std::vector<std::vector<double>> inverse(std::vector<std::vector<double>>& A) { 
    double D = determinant(A); 
    if (D == 0) { 
        std::runtime_error("Matrix not square.");
    }

    double d = 1. / D;

    std::vector<std::vector<double>> sol(A.size(), std::vector<double> (A.size()));

    for (int idx = 0; idx < A.size(); idx++) { 
        for (int jdx = 0; jdx < A.size(); jdx++) { 
            sol[idx][jdx] = A[idx][jdx]; 
        }
    }

    std::vector<std::vector<double>> cofac = getCofactor(sol); 
    sol = transpose(cofac); 

    for (int idx = 0; idx < sol.size(); idx++) { 
        for (int jdx = 0; jdx < sol.size(); jdx++) { 
            sol[idx][jdx] *= d; 
        }
    }

    return sol;  
}

std::vector<double> matrix_times_vector(std::vector<std::vector<double>>& A, std::vector<double>& v) {
    if (v.size() != A[0].size()) {
        std::runtime_error("Matrix and vector dimensions do not agree.");
    }
    
    std::vector<double> result(v.size()); 

    for (int idx = 0; idx < v.size(); idx++) { 
        for (int jdx = 0; jdx < A.size(); jdx++) { 
            result[idx] += A[idx][jdx] * v[jdx]; 
        } 
    }

    return result; 
}