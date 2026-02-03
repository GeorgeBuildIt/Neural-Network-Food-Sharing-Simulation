import simulation
import matplotlib.pyplot as plt
from matplotlib.widgets import Slider, Button

#Create 3 subplots for: 1. Pop/Success, 2. Reputation, 3. Social Classes
fig, (ax1, ax2, ax3) = plt.subplots(3, 1, figsize=(7, 8))
fig.canvas.manager.set_window_title('Stealing vs Cooperation Simulation')
plt.subplots_adjust(left=0.12, bottom=0.25, hspace=0.5, right=0.82)

#Create the twin axis for Success Rate
ax1_twin = ax1.twinx()

#UI Element Axes
ax_mutation = plt.axes([0.25, 0.18, 0.50, 0.02])
ax_cooperation = plt.axes([0.25, 0.15, 0.50, 0.02])
ax_steal = plt.axes([0.25, 0.12, 0.50, 0.02])
ax_cost = plt.axes([0.25, 0.09, 0.50, 0.02])
#ax_food = plt.axes([0.25, 0.06, 0.50, 0.02])

#Sliders
mutation_slider = Slider(ax_mutation, 'Mutation', 0.0, 0.5, valinit=0.05)
cooperation_reward_slider = Slider(ax_cooperation, 'Cooperation Reward', 0.0, 10.0, valinit=6.0)
steal_reward_slider = Slider(ax_steal, 'Steal Reward', 0.0, 10.0, valinit=6.5)
living_cost_slider = Slider(ax_cost, 'Living Cost', 0.0, 5.0, valinit=1.2)
#food_availability_slider = Slider(ax_food, 'Food Availability', 0.0, 1, valinit=0.5)

#Run Button
ax_button = plt.axes([0.4, 0.01, 0.2, 0.04])
run_button = Button(ax_button, "Run Simulation", color="skyblue")

def run_experiment(event):
    #Initialise Simulation From C++, 100 Population
    sim = simulation.Simulation(100)
    
    #Update Simulation Paramaters
    sim.mutation_strength = mutation_slider.val
    sim.coop_reward = cooperation_reward_slider.val
    sim.steal_reward = steal_reward_slider.val
    sim.living_cost = living_cost_slider.val
    #sim.food_availability = food_availability_slider.val
    
    #Data Storage For Plotting
    pop_hist, rep_hist, success_rates, share_hist, steal_hist = [], [], [], [], []

    #Run For 500 Steps
    for _ in range(500):
        sim.step()
        pop = sim.get_population()
        
        #Break if 0 population
        if pop == 0:
            pop_hist.append(0)
            break
            
        pop_hist.append(pop)
        rep_hist.append(sim.get_average_reputation())
        share_hist.append(sim.sharer_count)
        steal_hist.append(sim.stealer_count)

        if sim.total_interactions > 0:
            rate = (sim.mutual_cooperations / sim.total_interactions) * 100
            success_rates.append(rate)
        else:
            success_rates.append(0)

    #--------Reset Graphs--------
    ax1.cla()
    ax1_twin.cla()
    ax2.cla()
    ax3.cla()
    fig.texts.clear() 

    #-------Population and Success Rate--------
    ax1.set_title("Population and Success Rate of Interactions", fontsize=16)
    ax1.plot(pop_hist, color='blue', label="Total Population", linewidth=2)
    ax1.set_ylabel("Population", color='blue', fontweight='bold')
    ax1.tick_params(axis='y', labelcolor='blue')
    ax1.grid(True, alpha=0.3)

    ax1_twin.plot(success_rates, color='purple', label="Success Rate (%)", alpha=0.5, linestyle='--')
    ax1_twin.set_ylabel("Cooperation %", color='purple', fontweight='bold')
    ax1_twin.tick_params(axis='y', labelcolor='purple')
    ax1_twin.set_ylim(0, 105)
    ax1_twin.yaxis.set_label_position("right")
    
    #Combined Legend for Graph 1
    h1, l1 = ax1.get_legend_handles_labels()
    h2, l2 = ax1_twin.get_legend_handles_labels()
    ax1.legend(h1 + h2, l1 + l2, loc='upper left', fontsize='small')

    #-------Average Reputation----------
    ax2.set_title("Average Reputation", fontsize=16)
    ax2.plot(rep_hist, color='green', label="Avg Reputation", linewidth=2)
    ax2.axhline(0.5, color='red', linestyle=':', alpha=0.5, label="Default")
    ax2.set_ylim(0, 1.1)
    ax2.set_ylabel("Average Reputation", fontweight='bold')
    ax2.grid(True, alpha=0.3)
    ax2.legend(loc='upper left', fontsize='small')

    #---------Nice vs Bad People---------
    ax3.set_title("Popularity of Stealing vs Sharing Strategies", fontsize=16)
    ax3.plot(share_hist, color='gold', label="Sharers (Reputation > 0.8)", linewidth=2)
    ax3.plot(steal_hist, color='red', label="Stealers (Reputation < 0.2)", linewidth=2)
    ax3.set_ylabel("Strategies Count", fontweight='bold')
    ax3.set_xlabel("Generations")
    ax3.grid(True, alpha=0.3)
    ax3.legend(loc='upper left', fontsize='small')
    
    plt.draw()

run_button.on_clicked(run_experiment)
print("Simulation Visualiser Ready.")
plt.show()