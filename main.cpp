#include <iostream>
#include <vector>
#include <MarkovChain.h>
using namespace std;


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
        
        // If the lead element is zero, move to next column
        if (abs(matrix[r][lead]) < 1e-10) {
            lead++; // increments lead to skip this column
            r--; // decrement r to stay on the same row for the next iteration
            continue;
        }
        
        // Scale the current row so that the lead element is 1
        double divisor = matrix[r][lead];
        for (int j = 0; j < cols; j++) {
            matrix[r][j] /= divisor;
        } // make sure the pivot is 1
        
        // Subtract multiples of the current row from all other rows
        for (int i = 0; i < rows; i++) {
            if (i != r) {
                double factor = matrix[i][lead];
                for (int j = 0; j < cols; j++) {
                    matrix[i][j] -= factor * matrix[r][j];
                }
            }
        }
        
        lead++;
    }
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
    for (int i = 0; i < n; i++) {
        transitionMatrix[i][i] -= 1.0;  // P - I
    }
    
    // Transpose the matrix
    vector<vector<double>> augmented = transposeMatrix(transitionMatrix);
    
    // Step 2: Add constraint that probabilities sum to 1
    // Replace the last row with [1, 1, 1, ..., 1, 1]
    for (int j = 0; j < n; j++) {
        augmented[n-1][j] = 1.0;
    }
    
    // Add augmented column (right-hand side)
    for (int i = 0; i < n; i++) {
        augmented[i].push_back(0.0);  // All zeros except last
    }
    augmented[n-1][n] = 1.0;  // Sum constraint = 1
    
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
    //test
    vector<vector<double>> m1 = { {10.5, 0,0, 1.6}, {20, 2,3, 1} ,{35,33,33, 33}, {1,66,66,66} };
    vector<vector<double>> m2 = { {1, 1}, {2, 2} };

    vector<vector<double>> transitionMatrix = {
        {0.0, 0.9, 0.0, 0.1, 0.0, 0.0},
    {0.0, 0.0, 0.9, 0.0, 0.1, 0.0},
    {0.9, 0.0, 0.0, 0.0, 0.0, 0.1},
    {0.1, 0.0, 0.0, 0.0, 0.9, 0.0},
    {0.0, 0.1, 0.0, 0.0, 0.0, 0.9},
    {0.0, 0.0, 0.1, 0.9, 0.0, 0.0}
    };
    vector<double> initialState = {1, 2,3,4,5,6};  // Starting in state 0

    MarkovChain mc(transitionMatrix, initialState);


    mc.printCurrentState();

    
    mc.simulate(10000); // Simulates until convergence or 10000 steps
    cout << "After: " << mc.getSteadyStateFromSimulation() << " Steps:" << endl;
    mc.printCurrentState();
    cout << "After normalization: " << endl;
    mc.normalize();
    mc.printCurrentState();
    cout << "Steady state calculation: " << endl;
    vector<double> steadyState = getStationaryDistribution(mc.getP());
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
    
    

    

    return 0;
}


