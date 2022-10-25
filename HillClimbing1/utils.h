#pragma once
#include "rangeFunctions.h"

void addBitsWithoutOverflow(int array[], const int& startPos, const int& endPos, int valueToAdd[]);
int* generateNeighbourhood(int node[], const int& nodeStart, const int& nodeEnd);
int nodeToNumber(int node[], const int& startPos, const int& endPos);
double convertToDomain(const int& value, const Function& function, const int& bitSize);

void addBitsWithoutOverflow(int array[], const int& startPos, const int& endPos, int valueToAdd[]) {
	int carry = 0;

	for (int i = endPos - 1; i > startPos; i--) {
		array[i] += carry + valueToAdd[i];
		if(array[i] == 0)
			continue;
		else if (array[i] == 1) 
			carry = 0;
		else if (array[i] == 2) {
			array[i] = 0;
			carry = 1;
		}
		else {
			array[i] = 1;
			carry = 1;
		}
	}

	if (carry == 1 || valueToAdd[startPos] == 1)
		array[startPos] = 1;
	
}

int* generateNeighbourhood(int node[], const int& nodeStart, const int& nodeEnd) {
	const int nodeLength = nodeEnd - nodeStart;
	const auto neighbourhood = new int[nodeLength * nodeLength];

	for (int i = 0; i < nodeLength; i++) {
		for (int j = 0; j < nodeLength; j++) {
			if (i != j)
				neighbourhood[i * nodeLength + j] = node[nodeStart + j];
			else
				neighbourhood[i * nodeLength + j] = (node[nodeStart + j] ^ 1);
		}
	}

	return neighbourhood;
}

int nodeToNumber(int node[], const int& startPos, const int& endPos) {
	int power = 1, number = 0;
	for (int i = endPos - 1; i >= startPos; i--) {
		number += node[i] * power;
		power *= 2;
	}

	return number;
}

double convertToDomain(const int& value, const Function& function, const int& bitSize) {
	const double a = pow(2, bitSize) - 1;
	const double average = function.upperBound - function.lowerBound;

	return (value / a) * average + function.lowerBound;
}
