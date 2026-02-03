#ifndef SIMULATION_H
#define SIMULATION_H

#include <vector>
#include <cmath>
#include <algorithm>
#include <random>

class NeuralNetwork
{
public:
    std::vector<std::vector<double>> inputsToHidden = std::vector<std::vector<double>>(3, std::vector<double>(5));
    std::vector<std::vector<double>> hiddenToOutput = std::vector<std::vector<double>>(5, std::vector<double>(1));

    std::vector<double> hiddenBiases = std::vector<double>(5);

    int inputSize = 3;
    int hiddenSize = 5; 
    double outputBias = 0;

    NeuralNetwork();

    double ForwardPass(const std::vector<double>& inputs);

    void Mutate(double mutationStrength = 0.1f);

private:
    double Sigmoid(double value);
};

class Agent
{
public:
    NeuralNetwork brain;
    float energy;
    float reputation;

    Agent() : energy(0.0f), reputation(0.5f) {}

    Agent(float startingEnergy);

    bool Decide(Agent& opponent, float foodAvailability);

    void Live(float livingCost);

    bool IsDead();

    void UpdateReputation(bool cooperated);

    Agent Spawn(float mutationRate);
};

class Simulation
{
public:
    std::mt19937 rng;

    std::vector<Agent> agents;
    float foodAvailability = 0.5f;

    float mutationStrength = 0.1f;
    float cooperationReward = 4.0f;
    float stealingReward = 6.0f;
    float livingCost = 0.5f;

    int niceCount = 0;
    int badCount = 0;
    int totalInteractions = 0;
    int mutualCooperations = 0;
    int thefts = 0;

    Simulation(int agentsNumber);
    void Step();

    size_t GetPopulationCount() { return agents.size(); }
    double GetAverageEnergy();
    double GetAverageReputation();
};

#endif