#include "euler_bernoulli_beam.h"
#include "matrix_util.h"

std::vector<std::vector<std::vector<double>>> construct_local_stiffness() { 
    // index of current element being iterated over -- -1 from true number because of 0-indexation 
    std::vector<std::vector<std::vector<double>>> element_stiffness_matrices{};
    for (int curr_element = 0; curr_element < K; curr_element++) {
        double hk = HK[curr_element], constant = 6. * EI[curr_element] / (hk * hk); 
        std::cout << hk << ' ' << constant << std::endl; 

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

std::pair<std::vector<std::vector<double>>, std::vector<double>> construct_globals(std::vector<std::vector<std::vector<double>>>& local_stiffness_matrices, std::vector<std::vector<double>>& local_load_vectors){ 
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
                std::cout << idx << ' ' << jdx << std::endl; 
                std::cout << iglobal << ' ' <<  jglobal << ' ' << global_stiffness[iglobal][jglobal] << std::endl;  
            }
            global_loads[iglobal] += local_load_vectors[curr_element][idx]; 
        }
    }

    // add point load contributions 
    for (int idx = 0; idx < NEU_DOF.size(); idx++) { 
        global_loads[std::get<0>(NEU_DOF[idx]) - 1] += std::get<1>(NEU_DOF[idx]);
    }

    // apply Dirichlet constraints
    for (int idx = 0; idx < DIR_DOF.size(); idx++) { 
        int dirichlet_index = DIR_DOF[idx] - 1; 

        for (int row_idx = 0; row_idx < NUM_NODES; row_idx++) { 
            global_stiffness[dirichlet_index][row_idx] = 0.;
            global_stiffness[row_idx][dirichlet_index] = 0.; 
            global_stiffness[dirichlet_index][dirichlet_index] = 1.;
        }

        global_loads[dirichlet_index] = 0.; 
    }

    return std::make_pair(global_stiffness, global_loads);  
}

int main() { 
    std::vector<std::vector<std::vector<double>>> element_stiffness_matrices = construct_local_stiffness(); 
    std::vector<std::vector<double>> element_load_vectors = construct_local_load(); 
    std::pair<std::vector<std::vector<double>>, std::vector<double>> globals = construct_globals(element_stiffness_matrices, element_load_vectors); 

    for (int curr_element = 0; curr_element < 2; curr_element++) { 
        for (int idx = 0; idx < 4; idx++) { 
            std::cout << element_load_vectors[curr_element][idx] << ' '; 
        }
        std::cout << std::endl; 
    }

    for (int curr_element = 0; curr_element < 2; curr_element++) { 
        for (int idx = 0; idx < 4; idx++) { 
            for (int jdx = 0; jdx < 4; jdx++) { 
                    std::cout << element_stiffness_matrices[curr_element][idx][jdx] << ' '; 
            }
            std::cout << std::endl; 
        }
        std::cout << std::endl;
    }

    for (int idx = 0; idx < NUM_NODES; idx++) { 
        std::cout << std::get<1>(globals)[idx] << std::endl; 
    }

    for (int idx = 0; idx < NUM_NODES; idx++) { 
        for (int jdx = 0; jdx < NUM_NODES; jdx++) { 
            std::cout << std::get<0>(globals)[idx][jdx] << ' ';    
        }
        std::cout << std::endl; 
    }

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

    for (int idx = 0; idx < NUM_NODES; idx++) { 
        for (int jdx = 0; jdx < NUM_NODES; jdx++) { 
            std::cout << std::get<0>(globals)[idx][jdx] << ' ';    
        }
        std::cout << std::endl; 
    }

    std::vector<std::vector<double>> inv_matrix = inverse(std::get<0>(globals));

    for (int idx = 0; idx < NUM_NODES; idx++) { 
        for (int jdx = 0; jdx < NUM_NODES; jdx++) { 
            std::cout << inv_matrix[idx][jdx] << ' ';    
        }
        std::cout << std::endl; 
    }

   std::vector<double> alpha_vec = matrix_times_vector(inv_matrix, std::get<1>(globals)); 

    for (int idx = 0; idx < alpha_vec.size(); idx++) { 
        std::cout << alpha_vec[idx] << std::endl; 
    }

    return 0; 
}