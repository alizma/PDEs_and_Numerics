#include <iostream> 
#include <vector> 
#include <utility> 
#include <array> 
#include <algorithm> 

/* DECLARATIONS OF PROBLEM-SPECIFIC STRUCTURES */ 
// map from node number to x-coord - nodes are 1, 2, 3: 1 at LHS boundary, 2 at midpoint, 3 at RHS boundary 
const std::vector<double> COORDS { 5. / 2, 5., 7. }; 
// map from element number to node number -- element I maps to nodes 1 & 2, element II maps to nodes 2 & 3
const std::vector<std::pair<int, int>> ELNODE = { std::make_pair(1, 3), std::make_pair(3, 2), std::make_pair(2, 4) };
// map from element number to numbers of degrees of freedom
const std::vector<std::array<int, 4>> ELDOF = { {1, 5, 2, 6}, {2, 6, 3, 7}, {3, 7, 4, 8} };  
// mapping from degree of freedom to essential boundary conditions (Dirichlet BCs)
const std::vector<int> DIR_DOF = { 1, 5, 3 };
// mapping from degrees of freedom to natural constraints (Neumann BCs)
const std::vector<std::pair<int, double>> NEU_DOF = { std::make_pair(2, -5.), std::make_pair(4, -3.)}; // in the 2-interval problem from Pset4, this is empty
// K - number of elements 
const int K = 3; 
// number of nodes on intervals of elements 
const int NUM_NODES = 2*(K+1); 
// fluxural rigidity of each element 
const std::array<double, K> EI = { 1., 1., 1. }; 
// step sizes between adjacent nodes 
const std::array<double, K> HK = { 5. / 2, 5. / 2, 2. };
// forces for each element segment 
const std::array<double, K> FK = { -1., -1., -1. };
// nodes to compute resultant forces at 
const std::vector<int> RESULTANT_FORCE_NODES = { 1, 2, 3, 4, 5, 6, 7, 8 }; 

/* DECLARATIONS OF FEM FUNCTIONS */
// computes local stiffness matrices and returns a vector of arrays
std::vector<std::vector<std::vector<double>>> construct_local_stiffness(); 
// computes local load vectors and returns a vector of vectors 
std::vector<std::vector<double>> construct_local_load();
// computes RAW global stiffness matrix and global load vector 
std::pair<std::vector<std::vector<double>>, std::vector<double>> construct_globals_raw(std::vector<std::vector<std::vector<double>>>& , std::vector<std::vector<double>>& );
// computes global stiffness matrix and global load vector accounting for Dirichlet BCs 
std::pair<std::vector<std::vector<double>>, std::vector<double>> construct_globals(std::pair<std::vector<std::vector<double>>, std::vector<double>>& ); 
// computes resultant forces at relevant nodes
std::vector<double> compute_resultant_forces(std::vector<double>& , std::vector<double>& );