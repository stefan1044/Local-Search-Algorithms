#include <fstream>
#include <iostream>
#include <thread>

#include "hillClimbers.h"
#include "printerFunctions.h"
#include "rangeFunctions.h"
#include "readerFunctions.h"
#include "simulatedAnnealing.h"


using namespace std;

void runThread(const HCValues hcValues, const SAValues saValues, PrintUnit puValues, const int hillClimbingTypes, const int
               exercisesTypes, const unsigned timesToRun, const char threadNumber);


int main() {

	/*readData("BIHC","DeJong's","5");
	readData("BIHC", "DeJong's", "10");
	readData("BIHC", "Schwefel's", "5");
	readData("BIHC", "Schwefel's", "10");
	readData("BIHC", "Rastrigin's", "5");
	readData("BIHC", "Rastrigin's", "10");
	readData("BIHC", "Michalewicz's", "5");
	readData("BIHC", "Michalewicz's", "10");

	readData("WIHC","DeJong's","5");
	readData("WIHC", "DeJong's", "10");
	readData("WIHC", "Schwefel's", "5");
	readData("WIHC", "Schwefel's", "10");
	readData("WIHC", "Rastrigin's", "5");
	readData("WIHC", "Rastrigin's", "10");
	readData("WIHC", "Michalewicz's", "5");
	readData("WIHC", "Michalewicz's", "10");


	readData("WIHC", "Schwefel's", "30");

	readData("FIHC","DeJong's","5");
	readData("FIHC", "DeJong's", "10");
	readData("FIHC", "Schwefel's", "5");
	readData("FIHC", "Schwefel's", "10");
	readData("FIHC", "Rastrigin's", "5");
	readData("FIHC", "Rastrigin's", "10");
	readData("FIHC", "Michalewicz's", "5");
	readData("FIHC", "Michalewicz's", "10");


	exit(0);*/

	const Function exercise1(-5.12, 5.12, deJongs);
	const Function exercise2(-500, 500, schwefels);
	const Function exercise3(-5.12, 5.12, rastrings);
	const Function exercise4(0, PI, michalewiczs);


	unsigned amountOfAttempts, threadsToStart;
	while (fin >> amountOfAttempts >> threadsToStart) {
		int _stepSize;
		fin >> _stepSize;
		const int stepSize = _stepSize;

		vector<int> exercises; int exercise;
		vector<int> hillClimbingTypes; int hillClimbingType;
		vector<unsigned> vTimesToRun; unsigned timesToRun;
		vector<unsigned> vDimensions; unsigned dimensions;
		vector<double> temperatures; double temperature;
		vector<double> coolingConstants; double coolingConstant;
		vector<HCValues> hcValues;
		vector<SAValues> saValues;
		for (unsigned i = 0; i < threadsToStart; i++) {
			fin >> exercise >> hillClimbingType >> timesToRun >> dimensions >> temperature >> coolingConstant;
			exercises.push_back(exercise);
			hillClimbingTypes.push_back(hillClimbingType);
			vTimesToRun.push_back(timesToRun);
			vDimensions.push_back(dimensions);
			temperatures.push_back(temperature);
			coolingConstants.push_back(coolingConstant);

			if (exercise == 1) {
				const HCValues hc(dimensions, timesToRun, exercise1, stepSize);
				hcValues.push_back(hc);
				const SAValues sa(dimensions, timesToRun, exercise1, stepSize, temperature, coolingConstant);
				saValues.push_back(sa);
			}
			if (exercise == 2) {
				const HCValues hc(dimensions, timesToRun, exercise2, stepSize);
				hcValues.push_back(hc);
				const SAValues sa(dimensions, timesToRun, exercise2, stepSize, temperature, coolingConstant);
				saValues.push_back(sa);
			}
			if (exercise == 3) {
				const HCValues hc(dimensions, timesToRun, exercise3, stepSize);
				hcValues.push_back(hc);
				const SAValues sa(dimensions, timesToRun, exercise4, stepSize, temperature, coolingConstant);
				saValues.push_back(sa);
			}
			if (exercise == 4) {
				const HCValues hc(dimensions, timesToRun, exercise4, stepSize);
				hcValues.push_back(hc);
				const SAValues sa(dimensions, timesToRun, exercise4, stepSize, temperature, coolingConstant);
				saValues.push_back(sa);
			}
		}

		PrintUnit pu1; PrintUnit pu2; PrintUnit pu3; PrintUnit pu4; PrintUnit pu5; PrintUnit pu6; PrintUnit pu7; PrintUnit pu8;
		vector<PrintUnit> puValues; puValues.push_back(pu1); puValues.push_back(pu2); puValues.push_back(pu3); puValues.push_back(pu4);
		puValues.push_back(pu5); puValues.push_back(pu6); puValues.push_back(pu7); puValues.push_back(pu8);


		for (unsigned i = 0; i < amountOfAttempts; i++) {
			vector<thread> threads;
			for (unsigned j = 0; j < threadsToStart; j++)
				threads.emplace_back(runThread, hcValues[j], saValues[j], puValues[j], hillClimbingTypes[j], exercises[j], vTimesToRun[j], j + '0');
			for (unsigned j = 0; j < threadsToStart; j++)
				threads[j].join();
		}
	}
	return 0;
}


void runThread(const HCValues hcValues, const SAValues saValues, PrintUnit puValues, const int hillClimbingTypes, const int
               exercisesTypes, const unsigned timesToRun, const char threadNumber) {

	if (hillClimbingTypes == 0) {
		puValues.setHeader(hillClimbingTypes, saValues.size, timesToRun, exercisesTypes,
			saValues.temperature, saValues.coolingConstant);
		runSimulatedAnnealing(saValues, puValues);
		puValues.printResults(threadNumber);

	}
	else {
		puValues.setHeader(hillClimbingTypes, hcValues.size, timesToRun, exercisesTypes,
			0, 0);
		runHillClimber(hcValues, hillClimbingTypes, puValues);
		puValues.printResults(threadNumber);

	}
}






