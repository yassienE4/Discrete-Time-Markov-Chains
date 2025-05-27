# How to Build

For the Visualization to work, Run the code from the terminal, not your IDE.

First build the project, (this can be done using your ide).

Then Navigate to "cmake-build-debug" and then click on the executable "DiscreteTimeMarkovChains".

# Code Purpose and Logic

We are trying to verify methods of finding the Stationary Distrubition.

We do this using 2 different methods: 

## 1. Simulating N steps (Power Method)

After inputting a Transition Matrix M and an Initial State, we repeatedly multiply M by the current state S, saving the current state and multiplying again,
until we reach a steady state.

We defined a steady state as a state where the abs(previousState - currentState) < 1e-6,

Once we reached this condition we say that the state has converged, and we normalize the vector and output it as long with the number of steps n.

## 2. The Stationary Equation

How we implemented it:

First we had to form an equation which we could easily solve.
1. Let Stationary Distrubtion =  π, and Transition Matrix = P, we have that the Stationary Equation for a Markov Chain is π = πP
2. By Transposing the equation we get $P^T  π^T = π^T$
3. This shows that the stationary distribution (as a column vector) is a left eigenvector of the transition matrix P with eigenvalue 1.
IE. finding the stationary distribution is equivalent to finding the eigenvector corresponding to eigenvalue 1 of the transposed transition matrix.
4. To solve the equation we get, $P^T  π^T - π^T = 0$, and we factor $π^T$ to get $(P^T-I)π^T=0$, which by the transpose property is equivalent to $π(P - I) = 0$.

In the code,

We first subtracted the identity matrix from P, before finding the transpose $(P-I)^T$.

We then needed to augment the matrix, so we replaced the last row with [1,1,1....1], so that we could add a constraint that probabilities sum to 1.

We then add the augmented column [0,0,0....1], such that the sum constraint is now 1.

We then reduce the matrix using Gaussian Elimination, and we are left with the solution located in the last column of the matrix.

## 3. The results

After testing a collection of Transition Matrices, we found that there is no effective difference between the 2 methods, and both are valid ways of calculating the steady state.



