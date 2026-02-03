#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "Simulation.h"

namespace py = pybind11;

PYBIND11_MODULE(simulation, m) 
{
    py::class_<Agent>(m, "Agent")
        .def_readonly("reputation", &Agent::reputation)
        .def_readonly("energy", &Agent::energy);

    py::class_<Simulation>(m, "Simulation")
        .def(py::init<int>())
        .def("step", &Simulation::Step)
        .def_readwrite("mutation_strength", &Simulation::mutationStrength)
        .def_readwrite("coop_reward", &Simulation::cooperationReward)
        .def_readwrite("steal_reward", &Simulation::stealingReward)
        .def_readwrite("living_cost", &Simulation::livingCost)
        .def_readwrite("food_availability", &Simulation::foodAvailability)
        .def_readonly("mutual_cooperations", &Simulation::mutualCooperations)
        .def_readonly("total_interactions", &Simulation::totalInteractions)
        .def_readonly("thefts", &Simulation::thefts)
        .def_readonly("sharer_count", &Simulation::niceCount)
        .def_readonly("stealer_count", &Simulation::badCount)
        .def("get_population", &Simulation::GetPopulationCount)
        .def("get_average_energy", &Simulation::GetAverageEnergy)
        .def("get_average_reputation", &Simulation::GetAverageReputation);
}