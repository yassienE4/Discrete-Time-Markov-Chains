//
// Created by Yassien Ebrahim  on 15/05/2025.
//

#include "MarkovChain.h"
#include <iostream>

MarkovChain::MarkovChain(Matrix transitionMatrix, vector<double> initialState)
{
    P = transitionMatrix;
    state = initialState;
}

vector<double> MarkovChain::step()
{
    // Calculate next state by multiplying current state with transition matrix
    vector<double> nextState(P[0].size(), 0);

    for (int j = 0; j < P[0].size(); j++)
    {
        for (int i = 0; i < state.size(); i++)
        {
            nextState[j] += state[i] * P[i][j];
        }
    }

    state = nextState;
    return state;
}

void MarkovChain::simulate(int steps)
{
    for (int i = 0; i < steps; i++)
    {
        step();
    }
}

void MarkovChain::printCurrentState()
{
    std::cout << "Current state: [";
    for (size_t i = 0; i < state.size(); i++)
    {
        std::cout << state[i];
        if (i < state.size() - 1)
        {
            std::cout << ", ";
        }
    }
    std::cout << "]" << std::endl;
}
