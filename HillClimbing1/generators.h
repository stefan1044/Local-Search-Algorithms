#pragma once
#include "randomBool.h"
#include "utils.h"

void randomizeCandidate(vector<bool>& candidate, const unsigned& size, const unsigned& nodeLength);


void randomizeCandidate(vector<bool>& candidate, const unsigned& size, const unsigned& nodeLength) {
	for (unsigned i = 0; i < candidate.size();i++) 
		candidate[i] = randomBool();
}

