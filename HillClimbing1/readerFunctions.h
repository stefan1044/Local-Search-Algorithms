#pragma once
#include <string>
#include <fstream>
#include <vector>

using namespace std;


void readData(string method, string function, string dimension) {

	vector<unsigned> finalChanges;
	vector<double> minimumsFound, averageChanges, averageMinimums;
	vector<long long> totalTimes, averageTimes;

	double smallestMinimum = DBL_MAX, biggestMinimum = -DBL_MAX;
	long long absoluteTotalTime = 0, smallestTotalTime = LLONG_MAX, biggestTotalTime = LLONG_MIN;
	unsigned timesRan = 0;

	double TfinalChanges = 0, TminimumsFound = 0, TaverageChanges = 0, TaverageMinimums = 0;
	long long TtotalTimes = 0, TaverageTimes = 0;
	for (int i = 0; i < 8; i++) {
		string temp;
		temp += (i + '0');
		ifstream in("tests/" + temp + method + function + dimension);



		unsigned finalChange;
		double minimumFound, averageChange, averageMinimum;
		long long totalTime, averageTime;
		while (in >> finalChange >> minimumFound >> averageChange >> averageMinimum >> totalTime >> averageTime) {
			timesRan += 125;
			finalChanges.push_back(finalChange); TfinalChanges += finalChange;
			minimumsFound.push_back(minimumFound); TminimumsFound += minimumFound;
			averageChanges.push_back(averageChange); TaverageChanges += averageChange;
			averageMinimums.push_back(averageMinimum); TaverageMinimums += averageMinimum;
			totalTimes.push_back(totalTime); TtotalTimes += totalTime;
			averageTimes.push_back(averageTime); TaverageTimes += averageTime;

			if (minimumFound < smallestMinimum)
				smallestMinimum = minimumFound;
			if (minimumFound > biggestMinimum)
				biggestMinimum = minimumFound;

			if (totalTime < smallestTotalTime)
				smallestTotalTime = totalTime;
			if (totalTime > biggestTotalTime)
				biggestTotalTime = totalTime;
			absoluteTotalTime += totalTime;
		}
		in.close();
	}

	ofstream out("AnalyzedData/" + method + function + dimension + "Data");

	out << "Times ran: " << timesRan << '\n';
	out << "Average position of final chnage: " << TfinalChanges / finalChanges.size() << '\n';
	out << "Average of lowest minimums found: " << TminimumsFound / minimumsFound.size() << "\nWith smallestMinimum: "
		<< smallestMinimum << " and biggestMinimum: " << biggestMinimum << '\n';
	out << "Average averageChanges: " << TaverageChanges / averageChanges.size() << '\n';
	out << "Average averageMinimums: " << TaverageMinimums / averageMinimums.size() << '\n';
	out << "Average totalTimes: " << TtotalTimes / totalTimes.size() << " ( " << (TtotalTimes / totalTimes.size()) / 1000000
		<< " seconds )" << "\nWith smallestTotalTime: " << smallestTotalTime << " ( " << smallestTotalTime / 1000000 << " seconds )"
		<< "\nbiggestTotalTime: " << biggestTotalTime << " ( " << (biggestTotalTime / 1000000) << " seconds )" << "\nabsoluteTotalTime: "
		<< absoluteTotalTime << "( " << (absoluteTotalTime / 1000000) / 8 << " seconds )" << '\n';
	out << "Average averageTimes: " << TaverageTimes / averageTimes.size() << '\n';


}
