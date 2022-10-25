#pragma once
#include <algorithm>
#include <cstdio>
#include "utils.h"
#include "generators.h"


#define HCBI 1


using namespace std;

struct HCValues
{

	const int size, length;
	Function function;
	const double stepSize;
	HCValues(const int size,const int length,const Function function,const double stepSize):
		size(size),length(length),function(function),stepSize(stepSize) {}
};

double bestValue = DBL_MAX;

void hillClimbingBestImprovement(int startNode[], const int& size, const int& nodeLength, const Function& function);
void runHillClimber(const HCValues& values, const int& hillClimberType);


void hillClimbingBestImprovement(int startNode[], const int& size, const int& nodeLength, const Function& function) {
	//initial prints
	/*
	printf("Started new hiilClimbing with startNode: ");
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < nodeLength; j++)
			printf("%d", startNode[i * nodeLength + j]);
		printf(" ");
	}
	printf("\n");
	*/
	//***************************************************************

	//Generate all neighbours
	int** neighbours = new int* [size];
	for (int i = 0; i < size; i++)
		neighbours[i] = new int[nodeLength * nodeLength];

	for (int i = 0; i < size; i++) {
		int* temp = new int[nodeLength * nodeLength];
		temp = generateNeighbourhood(startNode, i * nodeLength, (i + 1) * nodeLength);
		copy(temp, temp + nodeLength * nodeLength, neighbours[i]);
		/*
		printf("N[%d]:", i);
		for (int j = 0; j < nodeLength; j++) {
			for (int k = 0; k < nodeLength; k++) {
				printf("%d", neighbours[i][j * nodeLength + k]);
			}
			printf(" ");
		}
		printf("\n");
		*/
	}

	//Evaluate local values
	bool local = false;
	auto values = new double[size];
	for (int i = 0; i < size; i++) {
		values[i] = convertToDomain(nodeToNumber(startNode, i * nodeLength, (i + 1) * nodeLength), function, nodeLength);
	}
	double currentEval = function.func(values, size);
	if (currentEval < bestValue)
		bestValue = currentEval;

	//local value
	/*
	for (int i = 0; i < size; i++) {
		printf("V[%d]:%lf ", i, values[i]);
	}
	printf("Eval: %lf\n", currentEval);
	*/
	//************************************************************************


	double** neighbourValues = new double* [nodeLength];

	for (int i = 0; i < nodeLength; i++) {
		neighbourValues[i] = new double[size];
		for (int j = 0; j < size; j++) {
			neighbourValues[i][j] = convertToDomain(nodeToNumber(neighbours[j], i * nodeLength, (i + 1) * nodeLength), function, nodeLength);
		}
	}
	//neighbourhood values
	/*
	printf("Neighbourhood Values\n");
	for (int i = 0; i < nodeLength; i++) {
		for (int j = 0; j < size; j++) {
			printf("N[%d][%d]:%lf ", i, j, neighbourValues[i][j]);
		}
		printf("\n");
	}
	*/
	//************************************************************************************************

	int positionOfSmallestNeighbour = -1;
	for (int i = 0; i < nodeLength; i++) {
		double neighbourEval = function.func(neighbourValues[i], size);

		//printf("Eval[%d]:%lf\n", i, neighbourEval);
		if (neighbourEval < currentEval) {
			currentEval = neighbourEval;
			positionOfSmallestNeighbour = i;
		}
	}
	//post-search results
	//printf("positionOfSmallestNeighbour:%d\n", positionOfSmallestNeighbour);
	//**************************************************************************************************

	if (positionOfSmallestNeighbour == -1) {
		local = true;
	}
	else {
		int* temp = new int[size * nodeLength];
		for (int i = 0; i < size; i++) {
			copy(neighbours[i] + positionOfSmallestNeighbour * nodeLength,
				neighbours[i] + (positionOfSmallestNeighbour + 1) * nodeLength,
				temp + i * nodeLength);
		}
		/*
		printf("Temp is:");
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < nodeLength; j++)
				printf("%d", temp[i * nodeLength + j]);
			printf(" ");
		}
		printf("\n");

		printf("\n");
		*/
		hillClimbingBestImprovement(temp, size, nodeLength, function);
	}

}

void runHillClimber(const HCValues& values, const int& hillClimberType) {

	if(hillClimberType == HCBI) {
		const auto startArray = new int[values.size * values.length];
		memset(startArray, 0, values.size * values.length);

		const auto step = generateStep(values.function, values.stepSize, values.length);

		auto timesToRun = fabs(values.function.lowerBound) + fabs(values.function.upperBound);
		timesToRun /= values.stepSize;
		timesToRun = static_cast<int>(timesToRun);

		while (timesToRun > 0) {
			hillClimbingBestImprovement(startArray, values.size, values.length, values.function);
			generateProgressive(startArray, values.size, values.length, step);

			timesToRun--;
		}

		printf("Minimum found is:%lf", bestValue);
	}
}
