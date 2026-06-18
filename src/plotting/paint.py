import matplotlib
import matplotlib.pyplot as plt


f = open ("../../data/coordinates.txt", 'r')
parameters = [float (i) for i in f.readline ().split ()]
x_Q1 = parameters[0]
x_Q2 = parameters[1]
x_R1 = parameters[2]
x_R2 = parameters[3]
x_K = parameters[4]
x_M = parameters[5]
x_P = parameters[6]
x_R3 = parameters[7]
f.close ()

# SAGE DATA

f = open ("../../results/sage_result_x.txt", 'r') 
x_Sage = [float (i) for i in f.readline ().split ()]
f.close()

f = open ("../../results/sage_res_Q.txt", 'r') 
Q_Sage = [float (i) for i in f.readline ().split ()]
f.close()

f = open ("../../results/sage_res_Moment.txt", 'r') 
M_Sage = [float (i) for i in f.readline ().split ()]
# print ("M_Sage = ", M_Sage)
f.close()

f = open ("../../results/sage_res_Theta.txt", 'r') 
Theta_Sage = [float (i) for i in f.readline ().split ()]
f.close()

f = open ("../../results/sage_res_W.txt", 'r')
W_Sage = [float (i) for i in f.readline ().split ()]
f.close()



# FEM DATA

f = open ("../../results/FEM_result_x.txt", 'r') 
x_FEM = [float (i) for i in f.readline ().split ()]
f.close ()

f = open ("../../results/FEM_res_Q.txt", 'r') 
Q_FEM = [float (i) for i in f.readline ().split ()]
f.close ()

f = open ("../../results/FEM_res_Moment.txt", 'r') 
M_FEM = [float (i) for i in f.readline ().split ()]
f.close ()

f = open ("../../results/FEM_res_Theta.txt", 'r') 
Theta_FEM = [float (i) for i in f.readline ().split ()]
f.close ()

f = open ("../../results/FEM_res_W.txt", 'r')
W_FEM = [float (i) for i in f.readline ().split ()]
f.close ()


expand = 1.3

Q_max = max (max (Q_FEM), max (Q_Sage)) * expand
Q_min = min (min (Q_FEM), min (Q_Sage)) * expand

# Q_max = max (Q_Sage) * expand
# Q_min = min (Q_Sage) * expand

M_max = max (max (M_FEM), max (M_Sage)) * expand
M_min = min (min (M_FEM), min (M_Sage)) * expand

# M_max = max (M_Sage) * expand
# M_min = min (M_Sage) * expand

Theta_max = max (max (Theta_FEM), max (Theta_Sage)) * expand
Theta_min = min (min (Theta_FEM), min (Theta_Sage)) * expand

W_max = max (max (W_FEM), max (W_Sage)) * expand
W_min = min (min (W_FEM), min (W_Sage)) * expand

fig, xy = plt.subplots (5, 1, figsize = (20, 15))
plt.subplots_adjust (wspace = 0.3, hspace = 0.6)

xy[0].scatter ([x_R1, x_R2, x_R3], [1, 1, 1], marker = '^', s = 100, facecolors = 'brown', label = 'R')
xy[0].scatter ([x_K], [1], marker = '^', s = 100, facecolors = 'red', label = 'K')
xy[0].scatter ([x_M], [1], marker = '^', s = 100, facecolors = 'orange', label = 'M')
xy[0].scatter ([x_P], [1], marker = '^', s = 100, facecolors = 'green', label = 'P')
xy[0].fill ([x_Q1, x_Q2, x_Q2, x_Q1], [0.5, 0.5, 15, 7], color = 'pink', alpha = 0.4, label = 'Q_lin')

xy[0].axis ([-1, 26, 0, 20])
xy[0].legend ()


###############
# (x, Q) plot #
###############

xy[1].axis ([-1, 26, Q_min, Q_max])
xy[1].axhline (y = 0 , color = 'black', linestyle = '-')
xy[1].set_xlabel ("X")
xy[1].set_ylabel ("Q")
xy[1].grid (True)

xy[1].plot (x_Sage, Q_Sage, color = 'black', lw = 2.4)
xy[1].plot (x_FEM, Q_FEM, color = 'blue', lw = 1.5)

xy[1].vlines ([x_R1, x_R2, x_R3], Q_min, Q_max, color = 'brown' , linestyle = '--')
xy[1].vlines ([x_M], Q_min, Q_max, color = 'orange', linestyle = '--')
xy[1].vlines ([x_P], Q_min, Q_max, color = 'green', linestyle = '--')
xy[1].vlines ([x_K], Q_min, Q_max, color = 'red', linestyle = '--')
xy[1].vlines ([x_Q1, x_Q2], Q_min, Q_max, color = 'pink')


###############
# (x, M) plot #
###############

xy[2].axis ([-1, 26, M_min, M_max])
xy[2].axhline (y = 0 , color = 'black', linestyle = '-')
xy[2].set_xlabel ('X')
xy[2].set_ylabel ('M')
xy[2].grid (True)

xy[2].plot (x_Sage, M_Sage, color = 'black', lw = 2.4)
xy[2].plot (x_FEM, M_FEM, color = 'blue', lw = 1.5)

xy[2].vlines ([x_R1, x_R2, x_R3], M_min, M_max, color = 'brown' , linestyle = '--')
xy[2].vlines ([x_M], M_min, M_max, color = 'orange', linestyle = '--')
xy[2].vlines ([x_P], M_min, M_max, color = 'green', linestyle = '--')
xy[2].vlines ([x_K], M_min, M_max, color = 'red', linestyle = '--')
xy[2].vlines ([x_Q1, x_Q2], M_min, M_max, color = 'pink')


###################
# (x, Theta) plot #
###################

xy[3].axis ([-1, 26, Theta_min, Theta_max])
xy[3].axhline (y = 0 , color = 'black', linestyle = '-')
xy[3].set_xlabel ('X')
xy[3].set_ylabel ("Theta")
xy[3].grid (True)

xy[3].plot (x_Sage, Theta_Sage, color = 'black', lw = 2.4)
xy[3].plot (x_FEM, Theta_FEM, color = 'blue', lw = 1.5)

xy[3].vlines ([x_R1, x_R2, x_R3], Theta_min, Theta_max, color = 'brown', linestyle = '--')
xy[3].vlines ([x_M], Theta_min, Theta_max, color = 'orange', linestyle = '--')
xy[3].vlines ([x_P], Theta_min, Theta_max, color = 'green', linestyle = '--')
xy[3].vlines ([x_K], Theta_min, Theta_max, color = 'red', linestyle = '--')
xy[3].vlines ([x_Q1, x_Q2], Theta_min, Theta_max, color = 'pink')



###############
# (x, W) plot #
###############

xy[4].axis ([-1, 26, W_min, W_max])
xy[4].axhline (y = 0 , color = 'black', linestyle = '-')
xy[4].set_xlabel ('X')
xy[4].set_ylabel ('W')
xy[4].grid (True)

xy[4].plot (x_Sage, W_Sage, color = 'black', lw = 2.4)
xy[4].plot (x_FEM, W_FEM, color = 'blue', lw = 1.5)

xy[4].vlines ([x_R1, x_R2, x_R3], W_min, W_max, color = 'brown', linestyle = '--')
xy[4].vlines ([x_M], W_min, W_max, color = 'orange', linestyle = '--')
xy[4].vlines ([x_P], W_min, W_max, color = 'green', linestyle = '--')
xy[4].vlines ([x_K], W_min, W_max, color = 'red', linestyle = '--')
xy[4].vlines ([x_Q1, x_Q2], W_min, W_max, color = 'pink')

fig.savefig ("../../results/result_plot.png")
plt.show ()