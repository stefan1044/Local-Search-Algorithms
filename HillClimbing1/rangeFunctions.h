#pragma once
#include <vector>

using namespace std;

constexpr double PI = 3.14159265358979323846;

typedef double (*functionPointer)(const vector<double>& values);

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

double deJongs(const vector<double>& values);
double schwefels(const vector<double>& values);
double rastrings(const vector<double>& values);
double michalewiczs(const vector<double>& values);


double deJongs(const vector<double>& values) {
	double sum = 0;

	for (const auto var : values)
		sum += var * var;

	return sum;
}
double schwefels(const vector<double>& values) {
	double sum = 0;

	for (const auto var : values)
		sum += (-var) * (sin(sqrt(abs(var))));

	return sum;
}
double rastrings(const vector<double>& values) {
	double sum = 0;
	const double n = values.size();

	for (const auto var : values)
		sum += var * var - 10 * cos(2 * PI * var);
	sum += 10 * n;

	return sum;
}
double michalewiczs(const vector<double>& values) {
	double sum = 0;
	unsigned i = 1;

	for (const auto var : values) {
		sum -= sin(var) * pow(sin((i * var * var) / PI), 20);
		i++;
	}

	return sum;
}