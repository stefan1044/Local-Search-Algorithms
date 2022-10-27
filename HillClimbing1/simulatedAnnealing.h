#pragma once
#include <chrono>
#include <cmath>
#include "generators.h"
#include "printerFunctions.h"
#include "randomBool.h"
#include "rangeFunctions.h"
#include "utils.h"


struct SAValues
{
	const unsigned size, timesToRun;
	Function function;
	const int stepSize;
	const double temperature;
	const double coolingConstant;
	SAValues(const unsigned size, const unsigned timesToRun, const Function function, const int stepSize, const double temperature, const double coolingConstant) :
		size(size), timesToRun(timesToRun), function(function), stepSize(stepSize), temperature(temperature), coolingConstant(coolingConstant) {}
};

struct T
{
	double temperature, coolingConstant;
};

random_device rd;
mt19937 gen(rd());
uniform_real_distribution<> dis(0, 1.0);


void simulatedAnnealingFIHC(vector<bool>& startNode, const unsigned& size, const unsigned& nodeLength, const Function& function, double&
                            currentBestValue, T& temp);
void runSimulatedAnnealing(const SAValues& values, PrintUnit& printUnit);


void simulatedAnnealingFIHC(vector<bool>& startNode, const unsigned& size, const unsigned& nodeLength, const Function& function, double&
                            currentBestValue, T& temp) {
	//initial prints
	/*printf("Started new Hill Climbing from node:");
	printVector(startNode);*/
	const double currentEval = evaluate(startNode, nodeLength, function);
	if (currentEval < currentBestValue)
		currentBestValue = currentEval;

	bool local = false;
	unsigned index = 0;
	double localEval;
	while (index < startNode.size()) {
		startNode[index] = !startNode[index];
		localEval = evaluate(startNode, nodeLength, function);
		if (localEval < currentEval) {
			local = true;
			break;
		}
		auto a = dis(gen);
		auto b = exp(-abs(localEval - currentEval) / temp.temperature);
		if (a < b) {
			local = true;
			break;
		}

		startNode[index] = !startNode[index];
		index++;
	}


	temp.temperature *= temp.coolingConstant;
	//printf("%lf ", temperature);
	if (local == true) {
		simulatedAnnealingFIHC(startNode, size, nodeLength, function, currentBestValue, temp);
	}
}

void runSimulatedAnnealing(const SAValues& values, PrintUnit& printUnit) {

	double currentBestValue = DBL_MAX;
	double overallBestValue = DBL_MAX;
	unsigned bestFoundAt;
	T temp; temp.temperature = values.temperature; temp.coolingConstant = values.coolingConstant;

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
	while (timesToRun <= values.timesToRun) {
		temp.temperature = values.temperature;
		simulatedAnnealingFIHC(startArray, values.size, nodeLength, values.function, currentBestValue, temp);
		stopTime = chrono::high_resolution_clock::now();
		printUnit.timeChanges.push_back(stopTime);

		randomizeCandidate(startArray, values.size, nodeLength);

		if (currentBestValue < overallBestValue) {
			overallBestValue = currentBestValue;
			bestFoundAt = timesToRun;
			printUnit.madeChanges.push_back(timesToRun);
		}
		/*printf("Remaining timesToRun:%d with minimum found:%lf and overAllMinimum:%lf found at %dth run\n",
			timesToRun, currentSABestValue, overallSABestValue, bestFoundAtSA);*/
		printUnit.minimumsFound.push_back(currentBestValue);
		currentBestValue = DBL_MAX;
		timesToRun++;
	}
	//printf("Minimum found is:%lf at %dth run", overallSABestValue, bestFoundAtSA);
	printUnit.minimumFound = overallBestValue; printUnit.finalChange = bestFoundAt;

}