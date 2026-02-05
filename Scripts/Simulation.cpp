#include "Simulation.h"
#include <vector>
#include <cstdlib> 
#include <ctime> 

//-----------NEURAL NETWROK FUNCTIONS-----------//
NeuralNetwork::NeuralNetwork() 
{
    inputsToHidden = std::vector<std::vector<double>>(3, std::vector<double>(5));
    hiddenToOutput = std::vector<std::vector<double>>(5, std::vector<double>(1));
    hiddenBiases = std::vector<double>(5);
    
    for(auto& row : inputsToHidden) for(auto& weight : row) weight = ((rand() % 200) - 100) / 100.0;
    for(auto& row : hiddenToOutput) for(auto& weight : row) weight = ((rand() % 200) - 100) / 100.0;
    for(auto& bias : hiddenBiases) bias = ((rand() % 200) - 100) / 100.0;
    outputBias = ((rand() % 200) - 100) / 100.0;
}

double NeuralNetwork::ForwardPass(const std::vector<double>& inputs)
{
        std::vector<double> hiddenLayer(5, 0.0f);

        for (int to = 0; to < hiddenSize; to++)
        {
            double sum = 0;

            for (int from = 0; from < inputSize; from++)
            {
                sum += inputs[from] * inputsToHidden[from][to];
            }

            hiddenLayer[to] = Sigmoid(sum + hiddenBiases[to]);
        }

        double finalSum = 0;
        for (int from = 0; from < hiddenSize; from++)
        {
            finalSum += hiddenLayer[from] * hiddenToOutput[from][0];
        }

        return Sigmoid(finalSum + outputBias);
}

void NeuralNetwork::Mutate(double mutationStrength)
{
    //Mutate Weights
    for (int to = 0; to < hiddenSize; to++)
    {
        for (int from = 0; from < inputSize; from++)
        {
            double change = ((rand() % 201) - 100) / 1000.0f;
            inputsToHidden[from][to] += change * mutationStrength;
        }
    }

    for (int from = 0; from < hiddenSize; from++)
    {
        double change = ((rand() % 201) - 100) / 1000.0f;
        hiddenToOutput[from][0] += change * mutationStrength;
    }

    //Mutate Biases
    for (int i = 0; i < hiddenSize; i++)
    {
        double change = ((rand() % 201) - 100) / 1000.0f;
        hiddenBiases[i] += change * mutationStrength;
    }

    outputBias += ((rand() % 201) - 100) / 1000.0 * mutationStrength;
}

double NeuralNetwork::Sigmoid(double value)
{
    return 1.0f / (1.0f + exp(-value));
}



//----------AGENT FUNCTIONS--------------//
Agent::Agent(float startingEnergy) 
{
    energy = startingEnergy;
    reputation = 0.5f;
}

bool Agent::Decide(Agent& opponent, float foodAvailability)
{
    std::vector<double> inputs = {
        energy / 50.0f, 
        opponent.reputation, 
        foodAvailability
    };

    double output = brain.ForwardPass(inputs);

    return (output > 0.5f);
}

void Agent::Live(float livingCost)
{
    float metabolism = livingCost + (energy * 0.05f); 
    energy -= metabolism;
}

bool Agent::IsDead()
{
    return (energy <= 0.0f);
}

void Agent::UpdateReputation(bool cooperated)
{
    float choiceValue = cooperated ? 1.0f : 0.0f;
    reputation = (reputation * 0.9f) + (choiceValue * 0.1f);
}

Agent Agent::Spawn(float mutationRate)
{
    this->energy /= 2;

    Agent child(this->energy);
    child.brain = this->brain;
    child.brain.Mutate(mutationRate);   
    return child;
}



//---------------SIMULATION FUNCTIONS---------------//
Simulation::Simulation(int agentsNumber) : rng(std::random_device{}())
{
    std::srand(static_cast<unsigned int>(std::time(0)));

    for (int i = 0; i < agentsNumber; i++)
    {
        Agent newAgent = Agent(10.0f);
        agents.push_back(newAgent);
    }
}

void Simulation::Step()
{
    std::shuffle(agents.begin(), agents.end(), rng);
    float crowdingPenalty = (agents.size() / 500.0f) * 2.0f; 
    float totalCost = livingCost + crowdingPenalty;

    for (int i = 0; i + 1 < agents.size(); i += 2)
    {
        totalInteractions++;
        Agent& agentA = agents[i];
        Agent& agentB = agents[i+1];

        //Get decisions
        bool aCooperates = agentA.Decide(agentB, foodAvailability);
        bool bCooperates = agentB.Decide(agentA, foodAvailability);

        //Logic
        if (aCooperates && bCooperates) 
        {
            //Cooperation
            mutualCooperations++;
            agentA.energy += cooperationReward; 
            agentB.energy += cooperationReward;
        } 
        else if (aCooperates && !bCooperates) 
        {
            //A steals
            thefts++;
            agentA.energy -= (livingCost * 2) + crowdingPenalty; 
            agentB.energy += stealingReward;
        } 
        else if (!aCooperates && bCooperates) 
        {
            //A steals
            thefts++;
            agentB.energy -= (livingCost * 2) + crowdingPenalty; 
            agentA.energy += stealingReward;
        } 
        else 
        {
            //Both steal
            agentA.energy += 0; agentB.energy += 0;
        }

        //Update Reputations
        agentA.UpdateReputation(aCooperates);
        agentB.UpdateReputation(bCooperates);
    }

    //Update Agents Metabolism
    std::vector<Agent> nextGeneration;
    for (auto& agent : agents) 
    {
        agent.Live(totalCost);

        if (!agent.IsDead()) 
        {
            nextGeneration.push_back(agent);

            //Reproduce given enough energy
            float birthThreshold = 30.0f + crowdingPenalty; 
            if (agent.energy > birthThreshold) 
            {
                nextGeneration.push_back(agent.Spawn(mutationStrength));
            }
        }
    }

    agents = nextGeneration;

    niceCount = 0;
    badCount = 0;
    for (const auto& agent : agents) 
    {
        if (agent.reputation >= 0.8f) niceCount++;
        else if (agent.reputation <= 0.2f) badCount++;
    }

    //Cap population
    if (agents.size() > 2000)
    {
        agents.erase(agents.begin() + 2000, agents.end());
    }
}

double Simulation::GetAverageEnergy()
{
    if (agents.empty()) return 0.0;

    double total = 0.0f;
    for (auto& agent : agents)
    {
        total += agent.energy;
    }

    return total / static_cast<double>(agents.size());
}

double Simulation::GetAverageReputation()
{
    if (agents.empty()) return 0.0;

    double total = 0.0f;
    for (auto& agent : agents)
    {
        total += agent.reputation;
    }

    return total / static_cast<double>(agents.size());
}


