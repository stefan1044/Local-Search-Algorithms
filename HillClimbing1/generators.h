#pragma once
#include <cstdlib>
#include <ctime>
#include "utils.h"


int* generateTest(const int& size, const int& nodeLength);
void generateProgressive(int array[], const int& size, const int& nodeLength, int step[]);
int* generateStep(const Function& function, const double& stepSize, const int& bitSize);


int* generateTest(const int& size, const int& nodeLength) {
	std::srand(std::time(0));

	const auto temp = new int[size * nodeLength];

	for (int i = 0; i < size * nodeLength; i++) {
		temp[i] = rand() % 2;
	}

	return temp;
}

void generateProgressive(int array[], const int& size, const int& nodeLength, int step[]) {

	for (int i = 0; i < size; i++) {
		addBitsWithoutOverflow(array, i * nodeLength, (i + 1) * nodeLength, step);
	}
}

int* generateStep(const Function& function, const double& stepSize, const int& bitSize) {

	const auto  startArray = new int[bitSize];
	const auto	_1 = new int[bitSize];
	for(int i = 0;i<bitSize;i++) {
		startArray[i] = 0;
		_1[i] = 0;
	}
	_1[bitSize - 1] = 1;

	auto stepValue = convertToDomain(nodeToNumber(startArray, 0, bitSize), function, bitSize);
	while(stepValue<stepSize) {
		generateProgressive(startArray, 1, bitSize, _1);
		stepValue = convertToDomain(nodeToNumber(startArray, 0, bitSize), function, bitSize);
	}

	return startArray;
}
