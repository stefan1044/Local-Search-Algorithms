#pragma once

typedef double (*functionPointer)(double values[], const int& numberOfValues);

class Function
{
public:
	double lowerBound, upperBound;
	functionPointer func;
public:
	Function(const double a, const double b, const functionPointer c) {
		lowerBound = a;
		upperBound = b;
		func = c;
	}

};

double deJong1(double values[], const int& numberOfValues);


double deJong1(double values[], const int& numberOfValues) {
	double sum = 0;

	for (int i = 0; i < numberOfValues; i++) {
		sum += values[i] * values[i];
	}

	return sum;
}