/********************************************************
* CS 103 Zombie-pocalypse PA
* Name: Nicolas Neven
* USC email: nneven@usc.edu
* Comments (you want us to know):
*
*
********************************************************/

// Add other #includes if you need
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <climits>
#include <cmath>
using namespace std;

const int MAXPOP = 100000;

int main()
{
	// Array with enough entries for the maximum population size possible.
	bool pop[MAXPOP];
	int N, k, M, seed, zombies, r, night, minimum = INT_MAX, maximum = INT_MIN;
	double total = 0;
	
	// Your code here
	cout << "Enter the following: N k M seed" << endl;
	cin >> N >> k >> M >> seed;
	srand(seed);
    
    // Runs the amount of total simulations
	for (int run = 0; run < M; run++) {

		// Sets up array to represent starting zombie condition
        for (int i = 0; i < N; i++) {
            if (i < k) {
		        pop[i] = true;
            } else {
                pop[i] = false;
            }
	    }
        
        zombies = k;
        night = 0;
        
        // Keeps simulating nights until everyone is a zombie
		while (zombies < N) {
			night++;

			// Zombies infect humans
			for (int i = 0; i < zombies; i++) {
				r = rand() % N;
				pop[r] = true;
			}

			zombies = 0;

			// Counts total zombies at the end of each night
			for (int i = 0; i < N; i++) {
				if (pop[i]) {
					zombies++;
				}
			}
		}
        
		minimum = min(minimum, night);
		maximum = max(maximum, night);
		total += night;
	}

	// Print out your average, max, and min number of nights here
	// Be sure to follow the format shown in the assignment writeup.
	cout << "Average Nights: " << total / M << endl;
	cout << "Max Nights: " << maximum << endl;
	cout << "Min Nights: " << minimum << endl;
	
	return 0;
}