#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

//////////////////////////
// Hermite shape functions
//////////////////////////

long double N1 (long double ksi) { return 1. - 3 * ksi * ksi + 2. * ksi * ksi * ksi; }
long double N2 (long double ksi, long double L) { return L * (ksi - 2. * ksi * ksi + ksi * ksi * ksi); }
long double N3 (long double ksi) { return 3. * ksi * ksi - 2. * ksi * ksi * ksi; }
long double N4 (long double ksi, long double L) { return L * (-ksi * ksi + ksi * ksi * ksi); }

// first derivatives for ANGLES
long double N1_x (long double ksi, long double L) { return (-6. * ksi + 6. * ksi * ksi) / L; }
long double N2_x (long double ksi) { return 1. - 4. * ksi + 3. * ksi * ksi; }
long double N3_x (long double ksi, long double L) { return (6. * ksi - 6. * ksi * ksi) / L; }
long double N4_x (long double ksi) { return -2. * ksi + 3. * ksi * ksi; }

// second derivatives for MOMEMTS
long double N1_xx (long double ksi, long double L) { return (-6. + 12. * ksi) / (L * L); }
long double N2_xx (long double ksi, long double L) { return (-4. + 6. * ksi) / L; }
long double N3_xx (long double ksi, long double L) { return (6. - 12. * ksi) / (L * L); }
long double N4_xx (long double ksi, long double L) { return (-2. + 6. * ksi) / L; }

// third derivatives for Q
long double N1_xxx (long double ksi, long double L) { return 12. / (L * L * L); }
long double N2_xxx (long double ksi, long double L) { return 6. / (L * L); }
long double N3_xxx (long double ksi, long double L) { return -12. / (L * L * L); }
long double N4_xxx (long double ksi, long double L) { return 6. / (L * L); }


void evaluate_parameters_in_x (
	long double x,
	long double &w,
	long double &theta,
	long double &M,
	long double &Q,
	long double *node_coord,
	long double *U,
	long double E,
	long double J,
	size_t N_elem) {

	long double ksi, L;
	size_t node_i, node_j;

	int element_number = -1;
	// searching element for coordinate x
	for (size_t i = 0; i < N_elem; ++i) {
		if (node_coord[i] - x < 1.e-9 && x - node_coord[i+1] < 1.e-9) {
			L = node_coord[i+1] - node_coord[i];
			ksi = (x - node_coord[i]) / L;
			node_i = i;
			node_j = i + 1;
			element_number = i;
		}
	}

	if (element_number == -1) {
		w = theta = M = Q = 0.;
	} else {
		// node values for element
		long double w_i = U[2 * node_i];
		long double theta_i = U[2 * node_i + 1];
		long double w_j = U[2 * node_j];
		long double theta_j = U[2 * node_j + 1];

		w = N1 (ksi) * w_i + N2 (ksi, L) * theta_i + N3 (ksi) * w_j + N4 (ksi, L) * theta_j;
		theta = N1_x (ksi, L) * w_i + N2_x (ksi) * theta_i + N3_x (ksi, L) * w_j + N4_x (ksi) * theta_j;
		// M = - E * J * (N1_xx (ksi, L) * w_i + N2_xx (ksi, L) * theta_i
		// 	+ N3_xx (ksi, L) * w_j + N4_xx (ksi, L) * theta_j);
		// Q = - E * J * (N1_xxx (ksi, L) * w_i + N2_xxx (ksi, L) * theta_i
		// 	+ N3_xxx (ksi, L) * w_j + N4_xxx (ksi, L) * theta_j);
		M = (N1_xx (ksi, L) * w_i + N2_xx (ksi, L) * theta_i
			+ N3_xx (ksi, L) * w_j + N4_xx (ksi, L) * theta_j);
		Q = (N1_xxx (ksi, L) * w_i + N2_xxx (ksi, L) * theta_i
			+ N3_xxx (ksi, L) * w_j + N4_xxx (ksi, L) * theta_j);
	}
}



int main () {

	///////////////////////////////////
	// 1. Parameters (global constants)
	///////////////////////////////////

	ifstream file ("../../data/parameters.txt");
    string line;

	long double P;
	long double M;
	long double Q1;
	long double Q2;
	long double K_c;
	long double E;  // steel
	long double J;

    if (file.is_open ()) {
        file >> P;
        file >> M;
        file >> Q1;
        file >> Q2;
        file >> K_c;
        file >> E;
        file >> J;
        // cout << "E = " << E << endl;
        file.close ();
    } else {
        cerr << "Unable to open file" << endl;
    }

	const size_t N_elem = 8;
	const size_t N_nodes = N_elem + 1;



	////////////////////////////
	// 2. Build stiffness matrix
	////////////////////////////

	// coordinates of nodes
	long double node_coord [N_nodes] = {0., 6., 8., 10., 12., 15., 19., 22., 25.};

	const size_t N_DOF = 2 * N_nodes;
	// because (2 * N_nodes) is a total number of DOF's

	// K is a (2 * N_nodes)-by-(2 * N_nodes) stifness matrix
	long double** K = new long double* [N_DOF];
	for (size_t i = 0; i < N_DOF; ++i) {
		K[i] = new long double [N_DOF];
		for (size_t j = 0; j < N_DOF; ++j)
			K[i][j] = 0.;
	}

	// for each element
	for (size_t i = 0; i < N_elem; ++i) {

		size_t n1 = i, n2 = i + 1;  							// the start and end node of the element
		long double L = node_coord [i + 1] - node_coord [i]; 	// len of element
		long double coeff = E * J / pow (L, 3.); 				// coeff for stifness matrix
		long double K_l [4][4]; 								// K_l is a local stifness matrix

		K_l[0][0] = 12. * coeff;
        K_l[0][1] = 6. * L * coeff;
        K_l[0][2] = -12. * coeff;
        K_l[0][3] = 6. * L * coeff;
        
        K_l[1][0] = 6. * L * coeff;
        K_l[1][1] = 4. * L * L * coeff;
        K_l[1][2] = -6. * L * coeff;
        K_l[1][3] = 2. * L * L * coeff;
        
        K_l[2][0] = -12. * coeff;
        K_l[2][1] = -6. * L * coeff;
        K_l[2][2] = 12. * coeff;
        K_l[2][3] = -6. * L * coeff;
        
        K_l[3][0] = 6. * L * coeff;
        K_l[3][1] = 2. * L * L * coeff;
        K_l[3][2] = -6. * L * coeff;
        K_l[3][3] = 4. * L * L * coeff;


        // global indexes of DOF's
        // for node n_i: deflection = 2 * n, angle = 2 * n + 1
        size_t dof_n1_w = 2 * n1, dof_n1_theta = 2 * n1 + 1;
        size_t dof_n2_w = 2 * n2, dof_n2_theta = 2 * n2 + 1;

        size_t dofs [4] = {dof_n1_w, dof_n1_theta, dof_n2_w, dof_n2_theta};

        // embedding a local matrix K_l into a global matrix K
        for (size_t i = 0; i < 4; ++i)
                for (size_t j = 0; j < 4; ++j)
               		K[dofs[i]][dofs[j]] += K_l[i][j];
	}

	// adding elastic support at x = 12.
	// x = 12. is the 4th node
	K[2 * 4][2 * 4] += K_c; // it's node at x = 12.


	// cout << "K before boundary conditions" << endl;
	// for (size_t i = 0; i < N_DOF; ++i) {
	// 	for (size_t j = 0; j < N_DOF; ++j)
	// 		cout << K[i][j] << ' ';
	// 	cout << endl;
	// }
	// cout << endl;



	///////////////////////
	// 3. Build load vector
	///////////////////////

	long double* F = new long double [N_DOF]; // load vector
	long double* U = new long double [N_DOF]; // displacement vector

	for (size_t i = 0; i < N_DOF; ++i) {
		F[i] = 0.;
		U[i] = 0.;
	}


	// DISTRIBUTED LOAD

	// for each element
	for (size_t i = 0; i < N_elem; ++i) {
		const size_t n1 = i, n2 = i + 1;
		long double L = node_coord[n2] - node_coord[n1];

		long double q1 = 0., q2 = 0.; // loads at the start and end of element

		// load on [0., 8.]
		// it's 0th and 1st elements
		if (n1 < 2) {
			const long double x1 = node_coord[n1];
			const long double x2 = node_coord[n2];
			if (x1 < 8.) {
				q1 = Q1 + (Q2 - Q1) * (x1 - 0.) / 8.;
				if ((x2 < 8.) or (fabs (x2 - 8.) < 1.e-7)) {
					q2 = Q1 + (Q2 - Q1) * (x2 - 0.) / 8.;
				} else {
					q2 = Q2;
				}
			}
		}

		long double F_linear_load [4];
        F_linear_load[0] = L * (7. * q1 + 3. * q2) / 20.;
        F_linear_load[1] = L * L * (3. * q1 + 2. * q2) / 60.;
        F_linear_load[2] = L * (3. * q1 + 7. * q2) / 20.;
        F_linear_load[3] = -L * L * (2. * q1 + 3. * q2) / 60.;

        // global indexes of nodes
        const size_t dof1_w = 2 * n1, dof1_theta = 2 * n1 + 1;
        const size_t dof2_w = 2 * n2, dof2_theta = 2 * n2 + 1;
	
		// adding to global vector
		F[dof1_w] += F_linear_load[0];
        F[dof1_theta] += F_linear_load[1];
        F[dof2_w] += F_linear_load[2];
        F[dof2_theta] += F_linear_load[3];
	}

	// CONCENTRATED LOAD in x = 19. (6th node)
	F[2 * 6] += P;  // diflection (2) in elem 6

	// CONCENTRATED LOAD in x = 15. (5th node)
	F[5 * 2 + 1] += M;  // angle



	//////////////////////////////////////////
	// 4. Consideration of boundary conditions
	//////////////////////////////////////////

	// determine which degrees of freedom are fixed
	size_t *is_DOF_fixed = new size_t [N_DOF];
	for (size_t i = 0; i < N_DOF; ++i) {
		is_DOF_fixed[i] = 0;
	}

	// sealing in x = 0. (0th node)
	is_DOF_fixed[2 * 0] = 1; // diflection
	is_DOF_fixed[2 * 0 + 1] = 1; // angle

	// hinges in x = 6., 10., 22.
	// it's nodes 1, 3, 7
	is_DOF_fixed[2 * 1] = 1;
	is_DOF_fixed[2 * 3] = 1;
	is_DOF_fixed[2 * 7] = 1;

	// CONSIDERATION boundary conditions FOR K and F
	for (size_t i = 0; i < N_DOF; ++i) {
		if (is_DOF_fixed[i]) {
			// reset the row and column
			for (size_t j = 0; j < N_DOF; ++j) {
				K[i][j] = 0.;
				K[j][i] = 0.;
			}
			K[i][i] = 1.; // diagonal element
			F[i] = 0.;
		}
	}

	delete [] is_DOF_fixed;


	// cout << "K after boundary conditions" << endl;
	// for (size_t i = 0; i < N_DOF; ++i) {
	// 	for (size_t j = 0; j < N_DOF; ++j)
	// 		cout << K[i][j] << ' ';
	// 	cout << endl;
	// }
	// cout << endl;



	////////////////////////////////
	// 5. Solving system of equation
	////////////////////////////////

	// create copies for solving
	long double **A = new long double *[N_DOF];
	long double *B = new long double [N_DOF];
	long double *X = new long double [N_DOF];

	for (size_t i = 0; i < N_DOF; ++i) {
        A[i] = new long double [N_DOF];
        B[i] = F[i];
        for (size_t j = 0; j < N_DOF; ++j) {
            A[i][j] = K[i][j];
        }
    }

    // straight course Gauss method
    for (size_t i = 0; i < N_DOF; ++i) {
    	// searching max element in a row in a current line
    	size_t max_row = i;
    	for (size_t j = i + 1; j < N_DOF; ++j) {
    		if (fabs (A[j][i]) > fabs (A[max_row][i]))
    			max_row = j;
    	}

    	// changing lines if max element isn't in a current line
    	if (max_row != i) {
    		long double *tmp_row = A[i];
    		A[i] = A[max_row];
    		A[max_row] = tmp_row;

    		// changing lines in B
    		const long double tmp = B[i];
    		B[i] = B[max_row];
    		B[max_row] = tmp;
    	}

    	const long double A_ii = A[i][i];
    	if (fabs (A_ii) < 1.e-12) {
    		cout << "The matrix is degenerate" << endl;
    		// return -1;
    	}

    	// exception from the last lines
    	for (size_t j = i + 1; j < N_DOF; ++j) {
    		long double coeff = A[j][i] / A[i][i];
    		for (size_t k = i; k < N_DOF; ++k)
    			A[j][k] -= coeff * A[i][k];
    		B[j] -= coeff * B[i];
    	}
    }


    // cout << "Matrix A after straight course" << endl;
	// for (size_t i = 0; i < N_DOF; ++i) {
	// 	for (size_t j = 0; j < N_DOF; ++j)
	// 		cout << A[i][j] << ' ';
	// 	cout << endl;
	// }
	// cout << endl;


    // BACK COURSE Gauss method
    for (size_t i = N_DOF - 1; i >= 0; --i) {
    	X[i] = B[i];
    	for (size_t j = i + 1; j < N_DOF; ++j)
    		X[i] -= A[i][j] * X[j];
    	X[i] /= A[i][i];
    }



    ////////////////////
	// 6. Output results
	////////////////////

    const size_t num_points = 500;
    const long double dx = 25. / (num_points - 1);

    long double *x_grid = new long double [num_points];
    long double *W_grid = new long double [num_points];
    long double *T_grid = new long double [num_points];
    long double *M_grid = new long double [num_points];
    long double *Q_grid = new long double [num_points];

    for (size_t i = 0; i < num_points; ++i) {
    	const long double x = i * dx;
    	long double W, Theta, M, Q;

    	evaluate_parameters_in_x (x, W, Theta, M, Q, node_coord, X, E, J, N_elem);

    	x_grid[i] = x;
    	W_grid[i] = W;
    	T_grid[i] = Theta;
    	M_grid[i] = M;
    	Q_grid[i] = Q;
    }

    ofstream x_file ("FEM_result_x.txt");
    if (x_file.is_open ()) {
    	for (size_t i = 0; i < num_points; ++i)
    		x_file << x_grid[i] << ' ';
    }
    x_file.close ();

    ofstream w_file ("FEM_res_W.txt");
    if (w_file.is_open ()) {
    	for (size_t i = 0; i < num_points; ++i)
    		w_file << W_grid[i] << ' ';
    }
    w_file.close ();

    ofstream t_file ("FEM_res_Theta.txt");
    if (t_file.is_open ()) {
    	for (size_t i = 0; i < num_points; ++i)
    		t_file << T_grid[i] << ' ';
    }
    t_file.close ();

    ofstream m_file ("FEM_res_Moment.txt");
    if (m_file.is_open ()) {
    	for (size_t i = 0; i < num_points; ++i)
    		m_file << M_grid[i] << ' ';
    }
    m_file.close ();

    ofstream q_file ("FEM_res_Q.txt");
    if (q_file.is_open ()) {
    	for (size_t i = 0; i < num_points; ++i)
    		q_file << Q_grid[i] << ' ';
    }
    q_file.close ();


    delete [] x_grid;
    delete [] W_grid;
    delete [] T_grid;
    delete [] M_grid;
    delete [] Q_grid;



    //////////////////////////
    // 7. Freeing up resources
    //////////////////////////

	for (size_t i = 0; i < N_DOF; ++i)
		delete [] K[i];
	delete [] K;

	delete [] F;
	delete [] U;


	for (size_t i = 0; i < N_DOF; ++i)
		delete [] A[i];
	delete [] A;
	delete [] B;
	delete [] X;
}