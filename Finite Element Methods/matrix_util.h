#include <stdexcept> 
#include <math.h> 

//  compute square matrix determinant 
double determinant(std::vector<std::vector<double>>& ); 
// compute transpose of rectangular matrix 
std::vector<std::vector<double>> transpose(std::vector<std::vector<double>>& ); 
//  compute cofactor of square matrix 
std::vector<std::vector<double>> getCofactor(std::vector<std::vector<double>>& ); 
// compute inverse of square matrix 
std::vector<std::vector<double>> inverse(std::vector<std::vector<double>>& ); 
// compute multiplication of vector by matrix 
std::vector<double> matrix_times_vector(std::vector<std::vector<double>>& , std::vector<double>& );