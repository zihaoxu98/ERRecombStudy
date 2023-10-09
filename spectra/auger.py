import numpy as np
import re
from copy import deepcopy


class Auger():
    def __init__(self):
        self.reset()

    def reset(self):
        self.orbits = {}
        self._transition_table = {}
        self._vacancy_list = []
        self.load_data()

    def load_data(self):
        current_subshell_index = None
        with open('./data/xenon_auger.dat', 'r') as file:
            for line in file:
                line = line.strip()
                if re.match(r'^\d+ - subshell index$', line):
                    current_subshell_index = int(line.split()[0])
                    self._transition_table[current_subshell_index] = {}
                elif re.match(r'\d+ - number of electrons when neutral', line):
                    self.orbits[current_subshell_index] = int(line.split()[0])
                elif re.match(r'^\d+\s+\d+\s+\d+\.\d+E[+-]\d+\s+\d+\.\d+E[+-]\d+$', line):
                    values = line.split()
                    secondary_subshell = int(values[0])
                    tertiary_subshell = int(values[1])
                    transition_energy = float(values[2])
                    transition_probability = float(values[3])

                    index = (secondary_subshell, tertiary_subshell)
                    value = [transition_energy, transition_probability]
                    self._transition_table[current_subshell_index][index] = value
        self._transition_table[0] = {}

    @property
    def is_stable(self):
        return np.all([len(self._transition_table[vacancy]) == 0 for vacancy in self.vacancy_list])

    @property
    def vacancy_list(self):
        return self._vacancy_list
    
    @vacancy_list.setter
    def vacancy_list(self, value):
        backup = deepcopy(self._vacancy_list)
        self._vacancy_list = value
        if np.any(np.array(list(self.occupancy.values())) < 0):
            self._vacancy_list = backup
            raise ValueError("Exceed number of electrons in orbits!")

    @property
    def occupancy(self):
        occupancy = deepcopy(self.orbits)
        occupancy[0] = np.inf
        for vacancy in self.vacancy_list:
            occupancy[vacancy] -= 1
        return occupancy

    def transition_is_allowed(self, transition):
        if isinstance(transition, (list, tuple)):
            trans, count = np.unique(transition, return_counts=True)
            return np.all([self.occupancy[t] >= n for t, n in zip(trans, count)])
        else:
            return self.occupancy[transition] > 0

    def get_allowed_transition_probabilities(self, vacancy):
        table = self._transition_table[vacancy]
        allowed_transition = []
        energy = []
        prob = []
        for transition in table.keys():
            if self.transition_is_allowed(transition):
                allowed_transition.append(transition)
                energy.append(table[transition][0])
                prob.append(table[transition][1])
        return allowed_transition, np.array(prob) / np.sum(prob), np.array(energy)

    def simulate(self):
        products = []
        while not self.is_stable:
            vacancy = self.vacancy_list.pop(0)
            allowed_transition, prob, energy = self.get_allowed_transition_probabilities(vacancy)
            if len(allowed_transition) == 0:
                continue
            else:
                i = np.random.choice(np.arange(len(allowed_transition)), p=prob)
                self.vacancy_list.extend(allowed_transition[i])
                if allowed_transition[i][1] == 0:
                    products.append(("gamma", energy[i]))
                else:
                    products.append(("e-", energy[i]))
        return products
