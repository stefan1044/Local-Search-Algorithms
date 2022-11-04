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
			double tempMinimumFound = DBL_MAX;
			averageMinimums.push_back(averageMinimum); TaverageMinimums += averageMinimum;
			finalChanges.push_back(finalChange); TfinalChanges += finalChange;
			averageChanges.push_back(averageChange); TaverageChanges += averageChange;
			averageTimes.push_back(averageTime); TaverageTimes += averageTime;
			totalTimes.push_back(totalTime); TtotalTimes += totalTime;




			if (minimumFound < tempMinimumFound)
				tempMinimumFound = minimumFound;

			for (int i = 0; i < 15; i++) {
				in >> finalChange >> minimumFound >> averageChange >> averageMinimum >> totalTime >> averageTime;
				if (minimumFound < tempMinimumFound)
					tempMinimumFound = minimumFound;
				averageMinimums.push_back(averageMinimum); TaverageMinimums += averageMinimum;
				finalChanges.push_back(finalChange); TfinalChanges += finalChange;
				averageChanges.push_back(averageChange); TaverageChanges += averageChange;
				totalTimes.push_back(totalTime); TtotalTimes += totalTime;
				averageTimes.push_back(averageTime); TaverageTimes += averageTime;
			}

			timesRan += 2000;
			minimumsFound.push_back(tempMinimumFound); TminimumsFound += tempMinimumFound;

			if (tempMinimumFound < smallestMinimum)
				smallestMinimum = tempMinimumFound;
			if (tempMinimumFound > biggestMinimum)
				biggestMinimum = tempMinimumFound;

			if (totalTime < smallestTotalTime)
				smallestTotalTime = totalTime;
			if (totalTime > biggestTotalTime)
				biggestTotalTime = totalTime;
			absoluteTotalTime += totalTime;
		}


		in.close();
	}
	double mean = TminimumsFound / minimumsFound.size();
	int n = minimumsFound.size() - 1;
	double stdDev = 0;

	for (auto var : minimumsFound) {
		stdDev += (var - mean) * (var - mean);
	}
	stdDev /= n;
	stdDev = sqrt(stdDev);

	ofstream out("AnalyzedData/" + method + function + dimension + "Data");

	out << "Times ran: " << timesRan << '\n';
	out << "Average position of final chnage: " << TfinalChanges / finalChanges.size() << '\n';
	out << "Average of lowest minimums found: " << mean << "\nWith smallestMinimum: "
		<< smallestMinimum << " and biggestMinimum: " << biggestMinimum << '\n';
	out << "Standard deviation: " << stdDev << '\n';
	out << "Average averageChanges: " << TaverageChanges / averageChanges.size() << '\n';
	out << "Average averageMinimums: " << TaverageMinimums / averageMinimums.size() << '\n';
	out << "Average totalTimes: " << TtotalTimes / totalTimes.size() << " ( " << (TtotalTimes / totalTimes.size()) / 1000000
		<< " seconds )" << "\nWith smallestTotalTime: " << smallestTotalTime << " ( " << smallestTotalTime / 1000000 << " seconds )"
		<< "\nbiggestTotalTime: " << biggestTotalTime << " ( " << (biggestTotalTime / 1000000) << " seconds )" << "\nabsoluteTotalTime: "
		<< absoluteTotalTime << "( " << (absoluteTotalTime / 1000000) / 8 << " seconds )" << '\n';
	out << "Average averageTimes: " << (TaverageTimes / averageTimes.size()) / 1000000 << " seconds " << '\n';


}
