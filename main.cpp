/*********************************************************************
** Filename: 	main.cpp
** Author: 		Group 8 - Niza Volair, Sara Hashem, Matt Palmer
** Date: 		07/12/16
** Description: Main function for coin change algorithms program
** Input: 		From file
** Output: 		Test results from functions to standard output and file
*********************************************************************/
// #include "algorithms.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cstdio>
#include <ctime>
#include <fstream>
#include <sstream>
#include <vector>
#include <deque>
#include <string>



// Function Prototypes
int changeGreedy(std::vector<int> coinValues, int desiredChange, std::ofstream &file);
int changeDP(std::vector<int> coinValues, int desiredChange, std::ofstream &file);



int main(int argc, char *argv[]){
    // Check number of arguments 
    if(argc < 2){
	std::cout << "usage coin input_file" << std::endl;
	exit(1);
    }

    // Create clock variable and double to track time
    std::clock_t start;
    double duration;

	// Initialize random number generator with time seed
	time_t seed;
	srand((unsigned) time(&seed));

	// Set up arrays
	std::vector<int> coinValues;
	
	/* Testing deques to store results
	std::deque<double> greedyValues;
	std::deque<double> dpValues;
	*/

	// int testArray[100];
	int amount;
	
	// Set up file objects and parsers
	std::ifstream infile;
	std::ofstream outfile;
	std::string delim, line, number, inFileName, outFileName, fileExtension;


	// Open file for reading
	inFileName = argv[1];
	unsigned int extensionIndex = inFileName.find_last_of('.');
	outFileName = inFileName.substr(0, extensionIndex) + "change" + inFileName.substr(extensionIndex);
	infile.open( inFileName.c_str() );
	outfile.open( outFileName.c_str() );


	// Parse each line from file
	while(getline(getline(infile, delim, '['), line, ']')){
		std::stringstream linestream(line);
		std::string value;

		// Extract integer values from string
		while(getline(linestream, value, ',')){
			// Convert strings to integers (must compile with -std=c++11)
			coinValues.push_back(stoi(value));

			// // Push vector elements to array
			// for(int i = 0; i < coinValues.size(); i++){
			// 	testArray[i] = coinValues[i];
			// }
		}

		// Extract amount of change to be made
		getline(infile, number);	// Skip newline directly after array
		getline(infile, number);
		amount = stoi(number);		// Convert to integer


		std::cout << "[";
		for(int i = 0; i < coinValues.size(); i++){
			std::cout << coinValues[i];
			if(i < (coinValues.size()-1)){
				std::cout << ", ";
			}
		}
		std::cout << "]" << std::endl;

		std::cout << amount << std::endl;


		// Greedy Algorithm
		start = std::clock();
		int coins = 0;

		coins = changeGreedy(coinValues, amount, outfile);
		
	    duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;

	    std::cout << "Greedy coins: " << coins << " Time: " << duration << std::endl << std::endl;
	    //Testing code- greedyValues.push_back(duration);

		// Dynamic Programming Algorithm
		start = std::clock();
		coins = 0;
		
		coins = changeDP(coinValues, amount, outfile);
		
	    duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;

	    std::cout << "Dynamic Programming: " << coins << " Time: " << duration << std::endl << std::endl;
	    // Testing code- dpValues.push_back(duration);

	    // Clear vector for next line (array) in file
	    coinValues.clear();

	}

	// Close files
	outfile.close();
	infile.close();


	/* Testing code to find coin and time results
	while(!(greedyValues.empty())){
	    std::cout << greedyValues.front() << "\n";
	    greedyValues.pop_front();
	}

	std::cout << std::endl << "DP coin values:" << std::endl;
	while(!(dpValues.empty())){
	    std::cout << dpValues.front() << "\n";
	    dpValues.pop_front();
	}
	*/

	return 0;
}

// Function Implementations

/*********************************************************************
** Function: 		changegreedy
** Description:		This function calculates change for a given amount
							based on the largest value coin possible.
** Parameters: 		int vector, int, file pointer
** Pre-Conditions:	Vector initializaed with at least 1 positive int, 
							integer initialized with positive value, file open 
							for writing
** Post-Conditions:	Number of coins returned, coin count vector 
							printed to standard output and output file
*********************************************************************/
int changeGreedy(std::vector<int> coinValues, int desiredChange, std::ofstream &file){
	// std::vector<int> c;
	std::deque<int> coinCount(coinValues.size(), 0);
	int tempCoinCount = 0;
	int totalCoinCount = 0;

	int i = coinValues.size() - 1;
	while(desiredChange != 0 && i > -1){
		if(desiredChange >= coinValues[i]){
			// Subtract the value of the coin under consideration
			desiredChange -= coinValues[i];
			
			// Add the that coin to the array of the count of each coin
			coinCount[i] = coinCount[i] + 1;
			
			totalCoinCount++;
		}
		else{
			i--;
		}
	}

	std::cout << "[";
	for(int i = 0; i < coinCount.size(); i++){
		std::cout << coinCount[i];
		if(i < (coinCount.size()-1)){
			std::cout << ", ";
		}
	}
	std::cout << "]" << std::endl;

	std::cout << totalCoinCount << std::endl;
	
	// Print to output file
	file << "[";
	for(int i = 0; i < coinCount.size(); i++){
		file << coinCount[i];
		if(i < (coinCount.size()-1)){
			file << ", ";
		}
	}
	file << "]" << std::endl;

	file << totalCoinCount << std::endl;

	return totalCoinCount;
}

/*********************************************************************
** Function: 			changeDP
** Description:		This function calculates change for a given amount
							based on the most optimum combination of each coin.
** Parameters: 		int vector, int, file pointer
** Pre-Conditions:	Vector initializaed with at least 1 positive int, 
							integer initialized with positive value, file open 
							for writing
** Post-Conditions:	Number of coins returned, coin count vector 
							printed to standard output and output file
*********************************************************************/
int changeDP(std::vector<int> coinValues, int desiredChange, std::ofstream &file){
	int i, j, n;
	
	// The memo array
	int coinCount[desiredChange + 1][coinValues.size() + 1];
	
	// The coin values arrays
	std::vector<std::vector <int> > prevCoinCountValues(desiredChange + 1, std::vector<int>(coinValues.size(), 0));
	std::vector<int> finalCoinValues(coinValues.size(), 0);
	
	// Initialize the memo array and prev coin values array
	for(i = 0; i < coinValues.size() + 1; i++){
		coinCount[0][i] = 0;
	}
	for(i = 0; i < desiredChange + 1; i++){
		coinCount[i][0] = 0;
		coinCount[i][1] = i / coinValues[0];
		prevCoinCountValues[i][0] = i / coinValues[0];
	}
	// Fill in the memo array by incrementally finding the 
	// minimum count of coins per given change amount
	
	for(i = 2, n = 2; i < coinValues.size() + 1; i++, n++){
		std::vector<std::vector <int> > currentCoinCointValues(desiredChange + 1, std::vector<int>(coinValues.size(), 0));
		for(j = 1; j < desiredChange + 1; j++){			
			// Check to see if the coin can be used
			if(coinValues[i - 1] <= j){
				// Find all possible cominations of all possible uses of the coin 
				// under consideration
				int iterations = j / coinValues[i - 1];
				
				// Keep track of the lowest number of coins needed for 
				// the desired amount of change
				int minCount = coinCount[j][i - 1];
				currentCoinCointValues[j] = prevCoinCountValues[j];
				for(int m = 1; m <= iterations; m++){
					int coinTotalValue = coinValues[i - 1] * m;
					
					// Look for the min total coins used with every combination
					// of the coin under consideration
					if( ( m + coinCount[(j - coinTotalValue)][i-1] ) < minCount ){
						minCount = m + coinCount[(j - coinTotalValue)][i-1];
						
						// Copy the lower coin count values
						currentCoinCointValues[j] = prevCoinCountValues[j - coinTotalValue];
						
						// Store the count of the coin under consideration
						currentCoinCointValues[j][i - 1] = m;
					}
				}
				coinCount[j][i] = minCount;
			}
			else{
				// The coin under consideration cannot be used				
				// Use the previous min coin count
				coinCount[j][i] = coinCount[j][i - 1];
				
				// Use the previous coin count array
				currentCoinCointValues[j] = prevCoinCountValues[j];
			}
		}
		
		// Copy the previous coin count values
		prevCoinCountValues = currentCoinCointValues;
	}
	
	// Copy the final coin count value array
	finalCoinValues = prevCoinCountValues[desiredChange];
	
	// Print out the array to the console
	std::cout << "[";
	for(int i = 0; i < finalCoinValues.size(); i++){
		std::cout << finalCoinValues[i];
		if(i < (finalCoinValues.size()-1)){
			std::cout << ", ";
		}
	}
	std::cout << "]" << std::endl;
	
	// Print to output file
	file << "[";
	for(int i = 0; i < finalCoinValues.size(); i++){
		file << finalCoinValues[i];
		if(i < (finalCoinValues.size()-1)){
			file << ", ";
		}
	}
	file << "]" << std::endl;

	file << coinCount[desiredChange][coinValues.size()] << std::endl;
	
	return coinCount[desiredChange][coinValues.size()];
}
