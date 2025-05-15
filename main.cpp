#include <iostream>
#include <vector>
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


int main()
{
    //test
    vector<vector<double>> m1 = { {1, 1}, {2, 2} };
    vector<vector<double>> m2 = { {1, 1}, {2, 2} };

    auto result = multiplyMatrix(m1, m2);

    for (const auto& row : result)
    {
        for (int val : row)
        {
            cout << val << "\t";
        }
        cout << endl;
    }
    return 0;
}


