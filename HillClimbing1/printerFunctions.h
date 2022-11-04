#pragma once
#include <fstream>
#include <iomanip>


fstream fin("input.in");

class PrintUnit
{
public:
	string header;
	string outputFileName;

	vector<chrono::time_point<chrono::steady_clock>> timeChanges;
	vector<unsigned> madeChanges;
	vector<double> minimumsFound;

	unsigned finalChange;
	double minimumFound;

	void setHeader(const unsigned hillClimbingType, const unsigned size, const unsigned timesToRun, const unsigned exercise,
		const double temperature, const double coolingConstant) {

		char fileName[150];

		char typeOfFunc[100];
		if (exercise == 1) {
			sprintf_s(typeOfFunc, "DeJong's\0");
		}
		else if (exercise == 2) {
			sprintf_s(typeOfFunc, "Schwefel's\0");
		}
		else if (exercise == 3) {
			sprintf_s(typeOfFunc, "Rastrigin's\0");
		}
		else if (exercise == 4) {
			sprintf_s(typeOfFunc, "Michalewicz's\0");
		}
		else {
			sprintf_s(typeOfFunc, "Error\0");
		}

		char text[1000];
		if (hillClimbingType == 0) {
			sprintf_s(text, "Running Simulated Annealing %d times with %d dimensions on function %s with temperature %lf and cooling constant %lf\n\0",
				timesToRun, size, typeOfFunc, temperature, coolingConstant);
			sprintf_s(fileName, "%s%d%d%s%d\0", "SAFI",(int)(temperature), (int)(coolingConstant*100) ,typeOfFunc, size);
		}
		else if (hillClimbingType == 1) {
			sprintf_s(text, "Running Best-Improvement Hill-Climbing %d times with %d dimensions on function %s\n\0",
				timesToRun, size, typeOfFunc);
			sprintf_s(fileName, "%s%s%d\0", "BIHC", typeOfFunc, size);
		}
		else if (hillClimbingType == 2) {
			sprintf_s(text, "Running Worst-Improvement Hill-Climbing %d times with %d dimensions on function %s\n\0",
				timesToRun, size, typeOfFunc);
			sprintf_s(fileName, "%s%s%d\0", "WIHC", typeOfFunc, size);
		}
		else if (hillClimbingType == 3) {
			sprintf_s(text, "Running First-Improvement Hill-Climbing %d times with %d dimensions on function %s\n\0",
				timesToRun, size, typeOfFunc);
			sprintf_s(fileName, "%s%s%d\0", "FIHC", typeOfFunc, size);
		}

		header = text;
		outputFileName = fileName;

	}

	void printResults(const char thread) {

		string thr;
		thr = thr+ thread;

		ofstream fout("tests/" + thr + outputFileName, ios::app);

		//fout << header;

		fout /*<< "Final change: "*/ << finalChange << '\n';
		fout /*<< "Minimum found: " */<< fixed << setprecision(5) << minimumFound << '\n';

		double averageChange = 0;
		unsigned lastChange = 0;
		//fout << "All changes: ";
		for (const auto var : madeChanges) {
			//fout << var << ' ';
			averageChange += var - lastChange;
			lastChange = var;
		}
		averageChange /= madeChanges.size();
		//fout << '\n';
		fout /*<< "Average change: "*/ << fixed << setprecision(5) << averageChange << '\n';

		double averageMinimum = 0;
		//fout << "All minimums found: ";
		for (const auto var : minimumsFound) {
			//fout << var << ' ';
			averageMinimum += var;
		}
		averageMinimum /= minimumsFound.size();
		//fout << '\n';
		fout /*<< "Average minimum: " */<< fixed << setprecision(5) << averageMinimum << '\n';

		const auto totalTime = chrono::duration_cast<chrono::microseconds>(timeChanges.back() - timeChanges.front()).count();

		fout/* << "Time taken: " */<< totalTime << ' ' /*<< "Average time: " */<< totalTime / timeChanges.size()<<'\n';

		fout.close();
	}
};