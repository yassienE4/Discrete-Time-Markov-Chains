//
// Created by Yassien Ebrahim  on 15/05/2025.
//

#ifndef MARKOVCHAIN_H
#define MARKOVCHAIN_H

#include <vector>
#include <string>
using namespace std;
using Matrix = vector<vector<double>>;

class MarkovChain 
{
public:
    MarkovChain(Matrix transitionMatrix, vector<double> initialState);
    vector<double> step(); // perform a step
    void simulate(int steps); // simulate n steps
    void printCurrentState();

    // if we think we need more we can add a function that detects if
    // the distribution converges to a stationary distribution
    // basically after each step we compare the difference between them and if
    // it is small enough then we can say it converged
    // bool hasConverged();
    bool hasCoverged();
    int getSteadyStateFromSimulation();
    Matrix getP();
    void normalize();
    bool compareStates(const vector<double>& state1, const vector<double>& state2);

    
    
private:
    Matrix P;
    vector<double> previousState;
    vector<double> state;
    int stepCount = 0;
};



#endif //MARKOVCHAIN_H
