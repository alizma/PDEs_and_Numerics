#include <iostream> 
#include <vector> 
#include <utility> 
#include <array> 


// map from node number to x-coord - nodes are 1, 2, 3: 1 at LHS boundary, 2 at midpoint, 3 at RHS boundary 
const std::vector<double> COORDS { 0, 0.5, 1 }; 
// map from element number to node number -- element I maps to nodes 1 & 2, element II maps to nodes 2 & 3
const std::vector<std::pair<int, int>> ELNODE = { std::make_pair(1, 2), std::make_pair(2, 3) };
// map from element number to numbers of degrees of freedom
const std::vector<std::array<int, 4>> ELDOF = { {1, 4, 2, 5}, {2, 5, 3, 6} };  
// mapping from degree of freedom to essential boundary conditions (Dirichlet BCs)
const std::vector<int> DIR_DOF = { 1, 4, 3, 6 };
// mapping from degrees of freedom to natural constraints (Neumann BCs)
const std::vector<std::pair<int, double>> NEU_DOF; // in the 2-interval problem from Pset4, this is empty
// K - number of elements 
const int K = 2; 
// number of nodes on intervals of elements 
const int NUM_NODES = 2*(K+1); 
// fluxural rigidity of each element 
const std::array<double, K> EI = { 1., 1. }; 
// step sizes between adjacent nodes 
const std::array<double, K> HK = { 0.5, 0.5 };
// forces for each element segment 
const std::array<double, K> FK = { 1., 1. };

// TODO: WRITE FUNCTION PROTOTYPE DECLARATIONS