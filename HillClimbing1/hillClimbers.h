#pragma once
#include <chrono>
#include <cstdio>
#include <vector>
#include "generators.h"
#include "printerFunctions.h"
#include "utils.h"

#define HCBI 1
#define HCWI 2
#define HCFI 3

using namespace std;

struct HCValues
{
	const unsigned size, timesToRun;
	Function function;
	const int stepSize;
	HCValues(const unsigned size,const unsigned timesToRun, const Function function,const int stepSize) :
		size(size), timesToRun(timesToRun), function(function), stepSize(stepSize) {}
};


void hillClimbingBestImprovement(const vector<bool>& startNode, const unsigned& size, const unsigned& nodeLength, const Function& function, double&
	currentBestValue);
void hillClimbingFirstImprovement(vector<bool>& startNode, const unsigned& size, const unsigned& nodeLength, const Function& function, double&
	currentBestValue);
void hillClimbingWorstImprovement(const vector<bool>& startNode, const unsigned& size, const unsigned& nodeLength, const Function& function, double&
	currentBestValue);
void runHillClimber(const HCValues& values, const int hillClimberType, PrintUnit& printUnit);


void hillClimbingBestImprovement(const vector<bool>& startNode, const unsigned& size, const unsigned& nodeLength, const Function& function, double&
	currentBestValue) {
	//initial prints
	/*printf("Started new Hill Climbing from node:");
	printVector(startNode);*/

	//Evaluate local values
	double currentEval = evaluate(startNode, nodeLength, function);
	if (currentEval < currentBestValue)
		currentBestValue = currentEval;
	//printf("Current Eval:%lf\n", currentEval);

	//Generate all neighbours
	const vector<vector<bool>> neighbours = generateNeighbourhood(startNode);

	//print all neighbours
	/*{
		printf("Neighbours:\n");
		unsigned i = 0;
		for (auto& var : neighbours) {
			printf("neightbours[%d]:", i);
			printVector(var);
			i++;
		}
	}
	printf("Generated all neighbours\n");*/


	//Neighbourhood values
	auto neighbourValues = vector<double>();
	int index = 0;
	for (const auto& var : neighbours) {
		neighbourValues.push_back(evaluate(var, nodeLength, function));
	}

	/*printf("NeighbourValues:\n");
	printVector(neighbourValues);
	printf("\n");*/


	//Calculate bestImprovement
	int positionOfBestImprovement = -1;
	{
		int i = 0;
		for (const auto& var : neighbourValues) {
			if (var < currentEval) {
				currentEval = var;
				positionOfBestImprovement = i;
			}
			i++;
		}
	}

	//post-search results

	if (positionOfBestImprovement != -1) {
		hillClimbingBestImprovement(neighbours[positionOfBestImprovement], size, nodeLength, function, currentBestValue);
	}

}

void hillClimbingFirstImprovement(vector<bool>& startNode, const unsigned& size, const unsigned& nodeLength, const Function& function, double&
	currentBestValue) {
	//initial prints
	/*printf("Started new Hill Climbing from node:");
	printVector(startNode);*/


	const double currentEval = evaluate(startNode, nodeLength, function);
	if (currentEval < currentBestValue)
		currentBestValue = currentEval;

	bool local = false;
	unsigned index = 0;
	while (index < startNode.size()) {
		startNode[index] = !startNode[index];
		if (evaluate(startNode, nodeLength, function) < currentEval) {
			local = true;
			break;
		}

		startNode[index] = !startNode[index];
		index++;
	}


	if (local == true) {
		hillClimbingFirstImprovement(startNode, size, nodeLength, function, currentBestValue);
	}

}


void hillClimbingWorstImprovement(const vector<bool>& startNode, const unsigned& size, const unsigned& nodeLength, const Function& function, double&
	currentBestValue) {
	//initial prints
	/*printf("Started new Hill Climbing from node:");
	printVector(startNode);*/

	//Evaluate local values
	const double currentEval = evaluate(startNode, nodeLength, function);
	if (currentEval < currentBestValue)
		currentBestValue = currentEval;
	//printf("Current Eval:%lf\n", currentEval);

	//Generate all neighbours
	const vector<vector<bool>> neighbours = generateNeighbourhood(startNode);

	//print all neighbours
	/*{
		printf("Neighbours:\n");
		unsigned i = 0;
		for (auto& var : neighbours) {
			printf("neightbours[%d]:", i);
			printVector(var);
			i++;
		}
	}
	printf("Generated all neighbours\n");*/



	//Neighbourhood values
	auto neighbourValues = vector<double>();
	int index = 0;
	for (const auto& var : neighbours) {
		neighbourValues.push_back(evaluate(var, nodeLength, function));
	}

	/*printf("NeighbourValues:\n");
	printVector(neighbourValues);
	printf("\n");*/


	//Calculate bestImprovement
	int positionOfWorstImprovement = -1;
	{
		int i = 0;
		double worstImprovement = DBL_MIN;
		for (const auto& var : neighbourValues) {
			if (var < currentEval && var>worstImprovement) {
				worstImprovement = var;
				positionOfWorstImprovement = i;
			}
			i++;
		}
	}

	//post-search results
	if (positionOfWorstImprovement != -1) {
		hillClimbingWorstImprovement(neighbours[positionOfWorstImprovement], size, nodeLength, function, currentBestValue);
	}

}

void runHillClimber(const HCValues& values, const int hillClimberType, PrintUnit& printUnit) {
	double currentBestValue = DBL_MAX;
	double overallBestValue = DBL_MAX;
	unsigned bestFoundAt;

	auto _N = abs((values.function.lowerBound - values.function.upperBound) * pow(10, values.stepSize));
	const auto nodeLength = static_cast<unsigned>(ceil(log2(_N)));

	auto startArray = vector<bool>(values.size * nodeLength, 0);

	//printf("_N is %lf, nodeLength is %d\n", _N, nodeLength);
	unsigned timesToRun = 1;
	randomizeCandidate(startArray, values.size, nodeLength);

	chrono::time_point<chrono::steady_clock> startTime;
	chrono::time_point<chrono::steady_clock> stopTime;
	startTime = chrono::high_resolution_clock::now();
	printUnit.timeChanges.push_back(startTime);
	if (hillClimberType == HCBI) {
		while (timesToRun <= values.timesToRun) {
			hillClimbingBestImprovement(startArray, values.size, nodeLength, values.function, currentBestValue);
			stopTime = chrono::high_resolution_clock::now();
			printUnit.timeChanges.push_back(stopTime);

			randomizeCandidate(startArray, values.size, nodeLength);

			if (currentBestValue < overallBestValue) {
				overallBestValue = currentBestValue;
				bestFoundAt = timesToRun;
				printUnit.madeChanges.push_back(timesToRun);
			}

			/*printf("Remaining timesToRun:%d with minimum found:%lf and overAllMinimum:%lf found at %dth run\n",
				timesToRun, currentHCBestValue, overallHCBestValue, bestFoundAtHC);*/
			printUnit.minimumsFound.push_back(currentBestValue);
			currentBestValue = DBL_MAX;
			timesToRun++;
		}
		//printf("Minimum found is:%lf at %dth run", overallHCBestValue, bestFoundAtHC);
		printUnit.minimumFound = overallBestValue; printUnit.finalChange = bestFoundAt;
	}
	else if (hillClimberType == HCWI) {
		while (timesToRun <= values.timesToRun) {
			hillClimbingWorstImprovement(startArray, values.size, nodeLength, values.function, currentBestValue);
			stopTime = chrono::high_resolution_clock::now();
			printUnit.timeChanges.push_back(stopTime);

			randomizeCandidate(startArray, values.size, nodeLength);

			if (currentBestValue < overallBestValue) {
				overallBestValue = currentBestValue;
				bestFoundAt = timesToRun;
				printUnit.madeChanges.push_back(timesToRun);
			}

			/*printf("Remaining timesToRun:%d with minimum found:%lf and overAllMinimum:%lf found at %dth run\n",
				timesToRun, currentHCBestValue, overallHCBestValue, bestFoundAtHC);*/
			printUnit.minimumsFound.push_back(currentBestValue);
			currentBestValue = DBL_MAX;
			timesToRun++;
		}
		//printf("Minimum found is:%lf at %dth run", overallHCBestValue, bestFoundAtHC);
		printUnit.minimumFound = overallBestValue; printUnit.finalChange = bestFoundAt;
	}
	else if (hillClimberType == HCFI) {
		while (timesToRun <= values.timesToRun) {
			hillClimbingFirstImprovement(startArray, values.size, nodeLength, values.function, currentBestValue);
			stopTime = chrono::high_resolution_clock::now();
			printUnit.timeChanges.push_back(stopTime);

			randomizeCandidate(startArray, values.size, nodeLength);

			if (currentBestValue < overallBestValue) {
				overallBestValue = currentBestValue;
				bestFoundAt = timesToRun;
				printUnit.madeChanges.push_back(timesToRun);
			}

			/*printf("Remaining timesToRun:%d with minimum found:%lf and overAllMinimum:%lf found at %dth run\n",
				timesToRun, currentHCBestValue, overallHCBestValue, bestFoundAtHC);*/
			printUnit.minimumsFound.push_back(currentBestValue);
			currentBestValue = DBL_MAX;
			timesToRun++;
		}
		/*printf("Minimum found is:%lf at %dth run", overallHCBestValue, bestFoundAtHC);*/
		printUnit.minimumFound = overallBestValue; printUnit.finalChange = bestFoundAt;
	}

	stopTime = chrono::high_resolution_clock::now();
	printUnit.timeChanges.push_back(stopTime);
}
