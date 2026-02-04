# Neural-Network-Food-Sharing-Simulation
A multi agent simulation where agents compete for food using the "Prisoner's Dilemma" ruleset where agents may share or steal food from the other which provokes the evolution of different strategies over time due to a survival of the fittest system.

#-----SYSTEM ARCHITECTURE-----#                       
The prisoner's dilemma reward matrix vital to the simulation:

<img width="766" height="565" alt="image" src="https://github.com/user-attachments/assets/45135f78-d116-4418-85f4-a92a26daf997" />


There are a number of environmental factors which dictate the evolution of the system namely the: mutation strength, cooperation reward, stealing reward, living cost and food availablity.
The mutation strength affects the likelihood of a bias/ weight within the neural network being modified from the parent agents network (by ofsetting it by a small value between -0.02 and 0.02) when a child agent is created.
The cooperation reward is the amount of energy that both agents recieve when they successfully cooperate in an interaction (when they both choose to share the food).
The stealing reward is the amount of energy that the agent who chooses to steal gains when the opposing agent chooses to share (this is usually higher than the cooperation reward as the agent keeps all of the food for itself).
The living cost is the amount of energy that the agent takes up per step in the simulation (due to metabolism).
The food availability represents how harsh the environment is where it does not specifically affect the agents energy but will affect its descision making.

The system uses neural networks with three input neurons (the energy of the agent normalised, the opponents reputation and the food availability), 5 neurons on the hidden layer and one output neuron (wether the agent will steal or share). Each neuron within the neural network has a bias and each connection between neurons has a weight, for this network the sigmoid activation function is used as it outputs a number which can easily be used to represent binary values.

Each agent is composed of a brain (the neural network) as well as attributes for the energy and reputation (stemming from past interactions where the base value is 0.5 which is defined by this formula R_new = R_old * 0.9 + action * 0.1, where the action is either 0 or 1).

The step of the simulation, whilst stepping the simulation, it randomly pairs two agents together where they are each told to decide wether they will steal or share based off of the inputs previously stated and depending on the outcome they will get a specified amount of energy according to the simulation variable of each reward type. This system makes it so that the agents may gain or loose energy based on their descisions where if their energy falls below 0 they will die, whereas if their energy exceeds 30 they will produce a child, at the end of each step every adult agent will die and the child agents will take their places, simulating decay or growth of the population.

A number of values are recorded per step of the simulation such as the total population, percentage of interactions that were successfull, the average reputation as well as the number of sharers (agents with a reputation over 0.8) and stealers (agents with a reputation under 0.2) which are all displayed on graphs.

I used C++ for the simulation logic which requires heavy computation and used pybind11 to build the C++ code into a format compatible with python which means that i can combine the performance of C++ and the ease of use of graphical tools like matplotlib in python.

#-----OBSERVED BEHAVIOUR-----#               
Stealer population rises after the sharer population rises (the stealers exploit the increased amount of sharing):

<img width="696" height="828" alt="Untitled" src="https://github.com/user-attachments/assets/71602e74-01da-49f4-8c6d-42d096263d33" />


Sharer population stabilises the total population and when most of the sharers die off the population begins to lower rapidy:

<img width="698" height="826" alt="Untitled2" src="https://github.com/user-attachments/assets/3fb6c0ba-507b-46ff-824e-50cb86b7734d" />


Indication of neural network evolution as there are no specialised sharers / stealers until a few steps into the simulation:

<img width="697" height="825" alt="image" src="https://github.com/user-attachments/assets/e5bae7f9-cb74-447f-ba64-3e5002d36677" />


No Sharers evolve under harsh conditions where there is high living cost where loss of food from an interaction is more costly:

<img width="697" height="871" alt="image" src="https://github.com/user-attachments/assets/2c1368b2-70c0-41a8-9e9d-1fefff18df84" />


Sharers benefit from low populations with other sharers leading to exponential population growth:

<img width="692" height="824" alt="image" src="https://github.com/user-attachments/assets/4f5b5f93-3a56-48a6-9cac-189c3bd38f75" />

Natural population cap as the metabolism cost is proportional to the total population due to the crowding penalty, preventing exponential growth:

<img width="696" height="829" alt="image" src="https://github.com/user-attachments/assets/94b958a8-7953-4376-a107-1461f34d819e" />


#-----BUILD PROCESS-----#                  
Using pybind11 as previously mentioned i created the Main.cpp file which communicates to the setup.py file where i run this command in the terminal (python setup.py build_ext --inplace) to build the C++ code into a .pyd file which can then be read by python in the visualiser.py file where import simulation is used to call all simulation functions and access the variables.                      
I then used Pyinstaller to compile the entire project into the exe file which i have uploaded into the releases using this command:                    
python -m PyInstaller --noconfirm --onefile --windowed --collect-all matplotlib --paths "." --add-binary "simulation.cp312-win_amd64.pyd;." Visualiser.py.
