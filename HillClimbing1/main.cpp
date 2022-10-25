#include <iostream>
#include "rangeFunctions.h"
#include "hillClimbers.h"


using namespace std;




int main() {

	const Function exercise1(-5.12, 5.12, deJong1);
	int _size, _length;
	double _stepSize;
	cin >> _size >> _length >> _stepSize;

	const int size = _size, length = _length;
	const double stepSize = _stepSize;


	const HCValues test1(size,length,exercise1,stepSize);

	runHillClimber(test1, HCBI);


	return 0;
}







