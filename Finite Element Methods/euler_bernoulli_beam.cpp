#include "euler_bernoulli_beam.h"
#include "matrix_util.h"

std::vector<std::vector<std::vector<double>>> construct_local_stiffness() { 
    // index of current element being iterated over -- -1 from true number because of 0-indexation 
    std::vector<std::vector<std::vector<double>>> element_stiffness_matrices{};
    for (int curr_element = 0; curr_element < K; curr_element++) {
        double hk = HK[curr_element], constant = 6. * EI[curr_element] / (hk * hk); 
        // std::cout << hk << ' ' << constant << std::endl; 

        std::vector<std::vector<double>> curr_matrix = {
            {constant * 6. / hk, constant * 3.,  constant * -6. / hk, constant * 3.},
            {constant * 3., constant * 2. * hk, constant * (-3.), constant * hk},
            {constant * (-6.) / hk, constant * (-3.), constant * 6. / hk, constant * (-3.)},
            {constant * 3., constant * hk, constant * (-3.), constant * 2. * hk},
        };

        element_stiffness_matrices.push_back(curr_matrix); 
    }

    return element_stiffness_matrices; 
}

std::vector<std::vector<double>> construct_local_load() { 
    std::vector<std::vector<double>> element_load_vectors{}; 
    for (int curr_element = 0; curr_element < K; curr_element++) { 
        double hk = HK[curr_element], fk = FK[curr_element], constant = hk * fk / 12.; 

        std::vector<double> curr_load_vector = {
            constant * 6., constant * hk, constant * 6., constant * -hk
        };

        element_load_vectors.push_back(curr_load_vector);
    }

    return element_load_vectors; 
}

std::pair<std::vector<std::vector<double>>, std::vector<double>> construct_globals_raw(std::vector<std::vector<std::vector<double>>>& local_stiffness_matrices, std::vector<std::vector<double>>& local_load_vectors){
    // intialize NUM_NODESxNUM_NODES matrix with 0.'s 
    std::vector<std::vector<double>> global_stiffness(NUM_NODES, std::vector<double>(NUM_NODES));

    // NUM_NODESx1 vector, initialized to 0.'s 
    std::vector<double> global_loads(NUM_NODES, 0.); 

    // init indexation variables 
    int iglobal, jglobal; 
    
    // construct global stiffness and load vectors 
    for (int curr_element = 0; curr_element < K; curr_element++) { 
        for (int idx = 0; idx < 4; idx++) { 
            for (int jdx = 0; jdx < 4; jdx++) {
                iglobal = ELDOF[curr_element][idx] - 1;
                jglobal = ELDOF[curr_element][jdx] - 1; 
                global_stiffness[iglobal][jglobal] += local_stiffness_matrices[curr_element][idx][jdx];  
                //std::cout << idx << ' ' << jdx << std::endl; 
                //std::cout << iglobal << ' ' <<  jglobal << ' ' << global_stiffness[iglobal][jglobal] << std::endl;  
            }
            global_loads[iglobal] += local_load_vectors[curr_element][idx]; 
        }
    }

    // add point load contributions 
    for (int idx = 0; idx < NEU_DOF.size(); idx++) { 
        global_loads[std::get<0>(NEU_DOF[idx]) - 1] += std::get<1>(NEU_DOF[idx]);
    }

    return std::make_pair(global_stiffness, global_loads);  
}

std::pair<std::vector<std::vector<double>>, std::vector<double>> construct_globals(std::pair<std::vector<std::vector<double>>, std::vector<double>>& raw_globals) {
    // apply Dirichlet constraints
    for (int idx = 0; idx < DIR_DOF.size(); idx++) { 
        int dirichlet_index = DIR_DOF[idx] - 1; 

        for (int row_idx = 0; row_idx < NUM_NODES; row_idx++) { 
            std::get<0>(raw_globals)[dirichlet_index][row_idx] = 0.;
            std::get<0>(raw_globals)[row_idx][dirichlet_index] = 0.; 
            std::get<0>(raw_globals)[dirichlet_index][dirichlet_index] = 1.;
        }

        std::get<1>(raw_globals)[dirichlet_index] = 0.; 
    }

    return raw_globals; 
}

std::vector<double> compute_resultant_forces(std::pair<std::vector<double>, std::vector<double>>& raw_globals) {
    int num_resultant_forces = RESULTANT_FORCE_NODES.size(); 
    std::vector<double> resultant_forces(num_resultant_forces); 

    for (int node_idx = 0; node_idx < num_resultant_forces; node_idx++) { 
        int curr_node_num = RESULTANT_FORCE_NODES[node_idx] - 1; 
        resultant_forces[curr_node_num] += std::get<0>(raw_globals)[curr_node_num] - std::get<1>(raw_globals)[curr_node_num];
    }

    return resultant_forces; 
}

int main() { 
    std::vector<std::vector<std::vector<double>>> element_stiffness_matrices = construct_local_stiffness(); 
    std::vector<std::vector<double>> element_load_vectors = construct_local_load(); 
    std::pair<std::vector<std::vector<double>>, std::vector<double>> raw_globals = construct_globals_raw(element_stiffness_matrices, element_load_vectors); 
    std::pair<std::vector<std::vector<double>>, std::vector<double>> globals = construct_globals(raw_globals); 

    std::cout << "==== GLOBAL LOAD VECTOR ===" << std::endl; 
    for (int idx = 0; idx < NUM_NODES; idx++) { 
        std::cout << std::get<1>(globals)[idx] << std::endl; 
    }
    std::cout << std::endl; 

    // note that the nodes that are shared by two adjacent elements are counted exactly 3 times
    for (int curr_element = 0; curr_element < K; curr_element++) { 
        int dof1 = ELDOF[curr_element][2] - 1, dof2 = ELDOF[curr_element][3] - 1;  
        if (std::get<0>(globals)[dof1][dof1] != 1.) { 
            std::get<0>(globals)[dof1][dof1] /= 3.; 
        }

        if (std::get<0>(globals)[dof2][dof2] != 1.) { 
            std::get<0>(globals)[dof2][dof2] /= 3.; 
        }
    }

    std::cout << "=== GLOBAL STIFFNESS MATRIX ===" << std::endl; 
    for (int idx = 0; idx < NUM_NODES; idx++) { 
        for (int jdx = 0; jdx < NUM_NODES; jdx++) { 
            std::cout << std::get<0>(globals)[idx][jdx] << ' ';    
        }
        std::cout << std::endl; 
    }
    std::cout << std::endl; 

    std::vector<std::vector<double>> inv_matrix = inverse(std::get<0>(globals));

    std::cout << "=== INVERSE STIFFNESS MATRIX ===" << std::endl; 
    for (int idx = 0; idx < NUM_NODES; idx++) { 
        for (int jdx = 0; jdx < NUM_NODES; jdx++) { 
            std::cout << inv_matrix[idx][jdx] << ' ';    
        }
        std::cout << std::endl; 
    }
    std::cout << std::endl;  

   std::vector<double> alpha_vec = matrix_times_vector(inv_matrix, std::get<1>(globals)); 

    std::cout << "=== ALPHA VECTOR OF COEFFICIENTS ===" << std::endl; 
    for (int idx = 0; idx < alpha_vec.size(); idx++) { 
        std::cout << alpha_vec[idx] << std::endl; 
    }
    std::cout << std::endl; 

    std::vector<double> s_raw_alpha = matrix_times_vector(std::get<0>(raw_globals), alpha_vec);
    std::pair<std::vector<double>, std::vector<double>> raw_vectors = std::make_pair(s_raw_alpha, std::get<1>(raw_globals)); 

    std::vector<double> resultant_forces = compute_resultant_forces(raw_vectors); 

    std::cout << "=== RESULTANT FORCES AT FIXED POINTS OR PIVOTS ===" <<  std::endl;
    for (int idx = 0; idx < resultant_forces.size(); idx++) { 
        std::cout << resultant_forces[idx] << std::endl;
    }

    return 0; 
}