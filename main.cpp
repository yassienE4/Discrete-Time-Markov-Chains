#include <iostream>
#include <vector>
#include <MarkovChain.h>
#include <cstdlib>
using namespace std;

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) || defined(_WIN64)
void clearScreenMain() {
    system("cls");
}
#else
void clearScreenMain() {
    system("clear");
}
#endif


int printmatrixcalled = 0;
void printMatrix(vector<vector<double>> matrix)
{
    printmatrixcalled++;
    for (const auto& row : matrix) {
        cout << "[";
        for (size_t j = 0; j < row.size(); j++) {
            cout << row[j];
            if (j < row.size() - 1) {
                cout << ", ";
            }
        }
        cout << "]" << endl;
    }
    char next;
    cout << "Press anything to view next step ";
    cin >> next;
    cout << endl;
}


vector<vector<double>> multiplyMatrix(const vector<vector<double>>& matrix1, const vector<vector<double>>& matrix2 )
{
    const int n1 = matrix1.size();
    const int m1 = matrix1[0].size();
    // matrix 1 is m x n
    const int n2 = matrix2.size();
    const int m2 = matrix2[0].size();
    // matrix 2 is nn x mm

    // to multiply the matrices m1 has to be equal to n2
    if (m1 != n2)
    {
        cout << "Matrix multiplication not possible" << endl;
        return {};
    }

    vector result(n1, vector<double>(m2, 0));

    for (int i = 0; i < n1; i++) {
        for (int j = 0; j < m2; j++) {
            for (int k = 0; k < m1; k++) {
                result[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }
    return result;
    
}

vector<vector<double>> transposeMatrix(const vector<vector<double>>& matrix)
{
    vector<vector<double>> transposed(matrix[0].size(), vector<double>(matrix.size(), 0));
    for (int i = 0; i < transposed.size(); i++) {
        for (int j = 0; j < transposed[i].size(); j++) {
            transposed[j][i] = matrix[i][j];
        }
    }
    return transposed;
}

bool isConvergent(vector<vector<double>>& matrix)
{
    for (int i = 0; i < 65; i++) {
        matrix = multiplyMatrix(matrix, matrix);
    }
    
    for(int i = 1; i < matrix.size(); i++){
        if(matrix[i-1] != matrix[i]){
            return false;
        }
    }
    return true;
}

vector<vector<double>> reduceMatrix(vector<vector<double>> matrix)
{
    if (matrix.empty() || matrix[0].empty()) {
        return {};
    }

    const int rows = matrix.size();
    const int cols = matrix[0].size();
    
    int lead = 0;

    cout << "before reduction:" << endl;
    printMatrix(matrix);
    
    for (int r = 0; r < rows; r++) {
        if (lead >= cols) {
            return matrix;
        }
        
        int i = r;
        
        // Find the row with the largest absolute value in the current column

        // we do this to avoid floating point errors if the smallest value is too small
        // and also to avoid division by zero
        for (int j = r + 1; j < rows; j++) {
            if (abs(matrix[j][lead]) > abs(matrix[i][lead])) {
                i = j;
            }
        }


        // Swap rows if needed
        if (i != r) {
            swap(matrix[r], matrix[i]);
        }
        cout<< "after swapping row " << r << " with row " << i << ":" << endl;
        printMatrix(matrix);
        
        // If the lead element is zero, move to next column
        if (abs(matrix[r][lead]) < 1e-10) {
            lead++; // increments lead to skip this column
            r--; // decrement r to stay on the same row for the next iteration
            continue;
        }

        cout << "making the pivot 1" << endl;
        // Scale the current row so that the lead element is 1
        double divisor = matrix[r][lead];
        for (int j = 0; j < cols; j++) {
            matrix[r][j] /= divisor;
        } // make sure the pivot is 1
        
        printMatrix(matrix);
        
        // Subtract multiples of the current row from all other rows
        for (int i = 0; i < rows; i++) {
            if (i != r) {
                double factor = matrix[i][lead];
                for (int j = 0; j < cols; j++) {
                    matrix[i][j] -= factor * matrix[r][j];
                }
            }
        }
        cout << "making sure rows under pivot are 0 by subtracting" << endl;
        printMatrix(matrix);
        
        lead++;
    }
    cout << "after reduction:" << endl;
    printMatrix(matrix);
    return matrix;
}

int nullity(const vector<vector<double>>& matrix)
{
    int rank = 0;
    for (int i = 0; i < matrix.size(); i++)
    {
       for (int j = 0; j < matrix[i].size(); j++)
       {
           if (matrix[i][j] == 1)
           {
                rank++;
           }
       }
      
    }
    return matrix.size() - rank;
}

vector<double> getStationaryDistribution(vector<vector<double>> transitionMatrix)
{
    const int n = transitionMatrix.size();
    
    // Step 1: Create (P - I)^T matrix
    // We transpose because we want to solve π(P - I) = 0
    // which is equivalent to (P - I)^T π^T = 0
    
    // First subtract identity matrix from P
    cout << "Subtracting identity matrix from transition matrix..." << endl;
    for (int i = 0; i < n; i++) {
        transitionMatrix[i][i] -= 1.0;  // P - I
    }
    printMatrix(transitionMatrix);
    
    // Transpose the matrix
    cout << "Transposing the matrix..." << endl;
    vector<vector<double>> augmented = transposeMatrix(transitionMatrix);
    printMatrix(augmented);
    
    // Step 2: Add constraint that probabilities sum to 1
    // Replace the last row with [1, 1, 1, ..., 1, 1]
    cout << "add constraint that probabilities sum to 1..." << endl;
    for (int j = 0; j < n; j++) {
        augmented[n-1][j] = 1.0;
    }
    printMatrix(augmented);

    cout << "augmented matrix after adding the constraint:" << endl;
    // Add augmented column (right-hand side)
    for (int i = 0; i < n; i++) {
        augmented[i].push_back(0.0);  // All zeros except last
    }
    augmented[n-1][n] = 1.0;  // Sum constraint = 1

    printMatrix(augmented);

    cout << "reducing the augmented matrix..." << endl;
    // Step 3: Solve using row reduction
    vector<vector<double>> reduced = reduceMatrix(augmented);
    
    // Step 4: Extract solution
    vector<double> stationary(n);
    for (int i = 0; i < n; i++) {
        stationary[i] = reduced[i][n];  // Last column contains solution
    }
    
    return stationary;
}



int main()
{
    cout << "Do you want to input a transition matrix? (y/n): ";
    char choice;
    cin >> choice;
    MarkovChain * markov_chain;
    if (choice == 'y')
    {
        cout << "Input the Size of the transition matrix (nxn): ";
        int size;
        cin >> size;
        vector<vector<double>> transitionMatrix1(size, vector<double>(size));

        for (int i = 0; i < size; i++) {
            bool validRow = false;
            while (!validRow) {
                double sum = 0.0;
                cout << "Enter values for row " << i << ":" << endl;
        
                for (int j = 0; j < size; j++) {
                    cout << "Enter the value for transitionMatrix[" << i << "][" << j << "]: ";
                    cin >> transitionMatrix1[i][j];
            
                    // Check for negative values
                    if (transitionMatrix1[i][j] < 0) {
                        cout << "Probabilities cannot be negative. Please re-enter this row." << endl;
                        break;
                    }
            
                    sum += transitionMatrix1[i][j];
            
                    // Check if sum exceeds 1 during input
                    if (sum > 1.0) {
                        cout << "The sum of the row exceeds 1. Please re-enter this row." << endl;
                        break;
                    }
                }
        
                // Check if the complete row sums to exactly 1
                if (abs(sum - 1.0) < 1e-9) {  // Using epsilon for floating-point comparison
                    validRow = true;
                } else {
                    cout << "Row sum is " << sum << ". Each row must sum to exactly 1. Please re-enter this row." << endl;
                }
            }
        }
        vector<double> initialState(size);
        for (int i = 0; i < size; i++)
        {
            initialState[i] = i;
        }
        markov_chain = new MarkovChain(transitionMatrix1, initialState);
    }
    else
    {
        vector<vector<double>> transitionMatrix1 = {
            {0.0, 0.9, 0.0, 0.1, 0.0, 0.0},
        {0.0, 0.0, 0.9, 0.0, 0.1, 0.0},
        {0.9, 0.0, 0.0, 0.0, 0.0, 0.1},
        {0.1, 0.0, 0.0, 0.0, 0.9, 0.0},
        {0.0, 0.1, 0.0, 0.0, 0.0, 0.9},
        {0.0, 0.0, 0.1, 0.9, 0.0, 0.0}
        };
        vector<double> initialState(6);
        for (int i = 0; i < 6; i++)
        {
            initialState[i] = i;
        }
        markov_chain = new MarkovChain(transitionMatrix1, initialState);
    }

    cout << "We will now use the Power Method to find the stationary distribution of the Markov Chain. enter anything to continue" << endl;
    cin >> choice;
    
    cout << "Finding the stationary distribution of the Markov Chain by Power Method: " << endl;
    markov_chain->simulate(10000); // Simulates until convergence or 10000 steps
    cout << "Finding the stationary distribution of the Markov Chain by Power Method: " << endl;
    cout << "After: " << markov_chain->getSteadyStateFromSimulation() << " Steps:" << endl;
    markov_chain->printCurrentState();
    
    cout << "After normalization: " << endl;
    markov_chain->normalize();
    markov_chain->printCurrentState();


    cout << "We will now use the stationary equation (input anything to continue): ";
    cin >> choice;
    
    cout << "Finding the stationary distribution of the Markov Chain by Stationary Equation: " << endl;
    vector<double> steadyState = getStationaryDistribution(markov_chain->getP());
    cout << "Calculated state: [";
    for (int i = 0; i < steadyState.size(); i++)
    {
        cout << steadyState[i];
        if (i < steadyState.size() - 1)
        {
            cout << ", ";
        }
    }
    cout << "]" << endl;

    cout << "Comparing the two states: " << endl;
    for (int i = 0; i < steadyState.size(); i++)
    {
        if (abs(steadyState[i] - markov_chain->getCurrent()[i]) > 1e-6)
        {
            cout << "The two states are not equal at index " << i << ": " << steadyState[i] << " vs " << markov_chain->getCurrent()[i] << endl;
            return 1; // Exit if the states are not equal
        }
        else
        {
            
        }
    }


    clearScreenMain();

    cout << "Finding the stationary distribution of the Markov Chain by Power Method: " << endl;
    cout << "After: " << markov_chain->getSteadyStateFromSimulation() << " Steps:" << endl;
    markov_chain->printCurrentState();
    cout << endl;
    cout << "Finding the stationary distribution of the Markov Chain by Stationary Equation: " << endl;
    cout << "After: " << printmatrixcalled << " Steps:" << endl;
    cout << "Calculated state: [";
    for (int i = 0; i < steadyState.size(); i++)
    {
        cout << steadyState[i];
        if (i < steadyState.size() - 1)
        {
            cout << ", ";
        }
    }
    cout << "]" << endl;

    cout << endl;
    
    cout << "Since the two states are  equal, our methods are valid " << endl;
   
    
    
    
    

    

    return 0;
}


