#pragma once
#include <iostream>
#include <vector>
#include "rangeFunctions.h"

using namespace std;


double convertToDomain(const unsigned& value, const Function& function, const unsigned& bitSize);
double evaluate(const vector<bool>& candidate, const unsigned& nodeLength, const Function& function);
vector<vector<bool>> generateNeighbourhood(const vector<bool>& candidate);
unsigned nodeToNumber(const vector<bool>& node, const unsigned& currentNode, const unsigned& nodeLength);
template<class T> void printVector(vector<T> array);


double convertToDomain(const unsigned& value, const Function& function, const unsigned& bitSize) {
	const double a = pow(2, bitSize) - 1;
	const double average = function.upperBound - function.lowerBound;

	return (value / a) * average + function.lowerBound;
}

double evaluate(const vector<bool>& candidate, const unsigned& nodeLength, const Function& function) {

	const unsigned length = candidate.size() / nodeLength;
	vector<double> values;

	for (unsigned i = 0; i < length; i++) {
		values.push_back(convertToDomain(nodeToNumber(candidate, i, nodeLength), function, nodeLength));
	}

	return function.func(values);
}

vector<vector<bool>> generateNeighbourhood(const vector<bool>& candidate) {
	vector<vector<bool>> neighbourhood;

	for (unsigned i = 0; i < candidate.size(); i++) {
		neighbourhood.push_back(candidate);
		neighbourhood.back()[i] = !neighbourhood.back()[i];
	}

	return neighbourhood;
}

unsigned nodeToNumber(const vector<bool>& node, const unsigned& currentNode, const unsigned& nodeLength) {
	unsigned power = 1, number = 0;
	const unsigned _temp = currentNode * nodeLength;

	for (unsigned i = _temp; i < _temp + nodeLength; i++) {
		number += node[i] * power;
		power *= 2;
	}

	return number;
}

template<class T>
void printVector(vector<T> array) {
	for (const auto var : array)
		cout << var << ' ';
	cout << '\n';
}