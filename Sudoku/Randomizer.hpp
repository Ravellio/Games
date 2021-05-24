#pragma once

#include <vector>
#include <random>
#include <set>
#include <algorithm> 
#include <iostream>

class Randomizer {
private:	
	int nsteps;
	int level;
	int blockSize;
	
	std::vector<std::vector<int>> result;
	
	bool checkVector(const std::vector<int>& vec, int number);
	bool checkBoard(const std::vector<std::vector<int>>& matrix, int x, int y, int number);
	
public:
	Randomizer(void);
	Randomizer(int nsteps, int level);
	
	std::vector<std::vector<int>> randomize(int blockSize);
	bool checkAll(std::vector<std::vector<int>> matrix);
};