//
// Created by Yassien Ebrahim  on 15/05/2025.
//

#include "MarkovChain.h"

#include <iostream>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) || defined(_WIN64)
void clearScreen() {
    system("cls");
}
#else
void clearScreen() {
    system("clear");
}
#endif

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
        if (!hasCoverged())
        {
            step();
            printCurrentState();
            clearScreen();
            stepCount++;
        }
        else
        {
            return;
        }
    }
}

void MarkovChain::printCurrentState()
{
    cout << "Current state: [";
    for (int i = 0; i < state.size(); i++)
    {
        cout << state[i];
        if (i < state.size() - 1)
        {
            cout << ", ";
        }
    }
    cout << "]" << endl;
}

bool MarkovChain::compareStates(const vector<double>& state1, const vector<double>& state2)
{
    if (state1.size() != state2.size())
    {
        return false; // States are of different sizes
    }

    for (int i = 0; i < state1.size(); i++)
    {
        if (abs(state1[i] - state2[i]) > 1e-6) // Using a small epsilon for floating-point comparison
        {
            return false; // States are not equal
        }
    }
    return true; // States are equal
}



bool MarkovChain::hasCoverged()
{
    if (previousState.empty())
    {
        previousState = state; // Initialize previous state
        return false; // Not converged yet
    }
    else
    {
        if (compareStates(previousState, state))
        {
            return true;
        }
        else
        {
            previousState = state; // Update previous state
            return false; // Not converged yet
        }
    }
}

int MarkovChain::getSteadyStateFromSimulation()
{
    return stepCount;
}

void MarkovChain::normalize()
{
    double sum = 0;
    for (int i = 0; i < state.size(); i++)
    {
        sum += state[i];
    }
    for (int i = 0; i < state.size(); i++)
    {
        state[i] /= sum;
    }
}



Matrix MarkovChain::getP()
{
    return P;
}
