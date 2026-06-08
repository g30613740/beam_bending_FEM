import os
import numpy as np


'''
# Parameters
P = 10000.
M = 5000.
Q1 = 2000.
Q2 = 5000.
K = 1.0e6
E = 2.1e11  # steel
J = 1.0e-4

# Coordinates
x_Q1 = 0.
x_Q2 = 8.
x_R1 = 6.
x_R2 = 10.
x_K = 12.
x_M = 15.
x_P = 19.
x_R3 = 22.
'''


f = open ("../../data/parameters.txt", 'r')
parameters = [float (i) for i in f.readline ().split ()]
P = parameters[0]
M = parameters[1]
Q1 = parameters[2]
Q2 = parameters[3]
K = parameters[4]
E = parameters[5]  # steel
J = parameters[6]
f.close ()

f = open ("../../data/coordinates.txt", 'r')
coordinates = [float (i) for i in f.readline ().split ()]
x_Q1 = coordinates[0]
x_Q2 = coordinates[1]
x_R1 = coordinates[2]
x_R2 = coordinates[3]
x_K = coordinates[4]
x_M = coordinates[5]
x_P = coordinates[6]
x_R3 = coordinates[7]
f.close ()

# print ("parameters = ", parameters)
# print ("coordinates = ", coordinates)


# Variables
x = var ('x')
M_0 = var ('M_0')
Q_0 = var ('Q_0')
R1 = var ('R1')
R2 = var ('R2')
R3 = var ('R3')
R_K = var ('R_K')


# The deflection equation
tan = (Q2 - Q1) / (x_Q2 - x_Q1)
W = (1 / (E * J)) * \
	(- M_0 * (x ^ 2.) / 2. \
	- Q_0 * (x ^ 3.) / 6.  \
	- (M * ((x - x_M) ^ 2.) / 2.) * heaviside (x - x_M) \
	+ (P * ((x - x_P) ^ 3.) / 6.) * heaviside (x - x_P) \
	- (R1 * ((x - x_R1) ^ 3.) / 6.) * heaviside (x - x_R1) \
	- (R2 * ((x - x_R2) ^ 3.) / 6.) * heaviside (x - x_R2) \
	- (R3 * ((x - x_R3) ^ 3.) / 6.) * heaviside (x - x_R3) \
	- (R_K * ((x - x_K) ^ 3.) / 6.) * heaviside (x - x_K) \
	+ (Q1 * ((x - x_Q1) ^ 4.) / 24. + tan * ((x - x_Q1) ^ 5.) / 120.) * heaviside (x - x_Q1) \
	- (Q2 * ((x - x_Q2) ^ 4.) / 24. + tan * ((x - x_Q2) ^ 5.) / 120.) * heaviside (x - x_Q2))

# print ("W = ", W, '\n')
# print (diff (W, x))

zero (x) = 0
Theta = diff (W, x).substitute_function (dirac_delta, zero)
Moment = diff (Theta, x).substitute_function (dirac_delta, zero)
Q = diff (Moment, x).substitute_function (dirac_delta, zero)

# print ("Moment = ", Moment, '\n')
# print ("Q = ", Q, '\n')

sol = solve ([
	Moment (x = 25.) == 0., Q (x = 25.) == 0.,
	W (x = x_R1) == 0., W (x = x_R2) == 0., W (x = x_R3) == 0.,
	W (x = x_K) * K == - R_K],
	R1, R2, R3, R_K, Q_0, M_0, solution_dict = True)

# print (sol)
# print (sol[0])
# print (type (sol[0][R1].substitute_function (heaviside, zero)))
# print (sol[0][R1])
# print (sol[0][R1].substitute_function (heaviside, zero))
# R1 = float (sol[0][R1].substitute_function (heaviside, zero))

R1 = float (sol[0][R1])
R2 = float (sol[0][R2])
R3 = float (sol[0][R3])
R_K = float (sol[0][R_K])
Q_0 = float (sol[0][Q_0])
M_0 = float (sol[0][M_0])

# print ("R1 = ", R1)
# print ("R2 = ", R2)
# print ("R3 = ", R3)
# print ("W = ", W)

s_W (x) = W (R1 = R1, R2 = R2, R3 = R3, R_K = R_K, Q_0 = Q_0, M_0 = M_0)
s_Theta (x) = Theta (R1 = R1, R2 = R2, R3 = R3, R_K = R_K, Q_0 = Q_0, M_0 = M_0)
s_Moment (x) = Moment (R1 = R1, R2 = R2, R3 = R3, R_K = R_K, Q_0 = Q_0, M_0 = M_0)
s_Q (x) = Q (R1 = R1, R2 = R2, R3 = R3, R_K = R_K, Q_0 = Q_0, M_0 = M_0)

# print ("s_W = ", s_W, '\n')
# print ("s_Theta = ", s_Theta, '\n')
# print ("s_Moment = ", s_Moment, '\n')
# print ("s_Q = ", s_Q, '\n')

# print ("s_W (x = 1) = ", s_W (x = 1), '\n')

grid = np.linspace (0., 25., 500)

# print (s_Moment (x = grid[5]))
# print (s_Moment (x = grid[5]).substitute_function (heaviside, zero))
# s_W_grid = [s_W (x = x_grid) for x_grid in grid]

s_W_grid = [s_W (x = x_grid) for x_grid in grid]
s_Theta_grid = [s_Theta (x = x_grid) for x_grid in grid]
s_Moment_grid = [s_Moment (x = x_grid) for x_grid in grid]
s_Q_grid = [s_Q (x = x_grid) for x_grid in grid]

# print (s_W_grid)



# Export results

file = open ("../../results/sage_res_x.txt", 'w')
for i in range (len (grid)):
    file.write (str (grid[i]))
    file.write (' ')
file.close ()

file = open ("../../results/sage_res_W.txt", 'w')
for i in range (len (grid)):
    file.write (str (s_W_grid[i]))
    file.write (' ')
file.close ()

file = open ("../../results/sage_res_Theta.txt", 'w')
for i in range (len (grid)):
    file.write (str (s_Theta_grid[i]))
    file.write (' ')
file.close ()

file = open ("../../results/sage_res_Moment.txt", 'w')
for i in range (len (grid)):
    file.write (str (s_Moment_grid[i]))
    file.write (' ')
file.close ()

file = open ("../../results/sage_res_Q.txt", 'w')
for i in range (len (grid)):
    file.write (str (s_Q_grid[i]))
    file.write (' ')
file.close ()