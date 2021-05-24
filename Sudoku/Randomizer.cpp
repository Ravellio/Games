#include "Randomizer.hpp"

Randomizer::Randomizer(void) {}

Randomizer::Randomizer(int nsteps, int level) {
	for (int i = 0; i < nsteps; i++) {
		std::vector<int> row;
		for (int j = 0; j < nsteps; j++) {
			row.push_back(0);
		}
		result.push_back(row);
	}
	this->nsteps = nsteps;
	this->level = std::max(1, std::min(level, nsteps));
	this->blockSize = static_cast<int>(sqrt(nsteps));
}

//bool Randomizer::checkVector(const std::vector<int>& vec, int number) {
//	if (std::find(std::begin(vec), std::end(vec), number) != std::end(vec)) {
//		return false;
//	}
//	return true;
//}

//bool Randomizer::checkBoard(const std::vector<std::vector<int>>& matrix, int x, int y, int number) {
//	std::vector<int> row = matrix[x];
//	if (!checkVector(row, number)) {
//		return false;
//	}
//	
//	std::vector<int> col;
//	for (int i = 0; i < nsteps; i++) {
//		col.push_back(matrix[i][y]);
//	}
//	if (!checkVector(col, number)) {
//		return false;
//	}
//	
//	std::vector<int> block;
//	int xx = blockSize * (x / blockSize);
//	int yy = blockSize * (y / blockSize);
//	
//	for (int i = xx; i < xx + blockSize; i++) {
//		for (int j = yy; j < yy + blockSize; j++) {
//			block.push_back(matrix[i][j]);
//		}
//	}
//	if (!checkVector(block, number)) {
//		return false;
//	}
//	
//	return true;
//}

std::vector<std::vector<int>> Randomizer::randomize(int nsteps) {
	std::random_device rd;
    std::mt19937 gen(rd());
    //std::uniform_int_distribution<> numDistr(1, nsteps);
	std::uniform_int_distribution<> probDistr(1, 10);
	
	// Fix me: temporary solution
	std::vector<std::vector<int>> result = 
	{{5, 3, 4, 6, 7, 8, 9, 1, 2},
	 {6, 7, 2, 1, 9, 5, 3, 4, 8},
	 {1, 9, 8, 3, 4, 2, 5, 6, 7},
	 {8, 5, 9, 7, 6, 1, 4, 2, 3},
	 {4, 2, 6, 8, 5, 3, 7, 9, 1},
	 {7, 1, 3, 9, 2, 4, 8, 5, 6},
	 {9, 6, 1, 5, 3, 7, 2, 8, 4},
	 {2, 8, 7, 4, 1, 9, 6, 3, 5},
	 {3, 4, 5, 2, 8, 6, 1, 7, 9}};
	 
	for (int i = 0; i < nsteps; i++) {
		for (int j = 0; j < nsteps; j++) {
			int p = probDistr(gen);
			if (p < level) {
				result[i][j] = 0;
			}
		}
	}
	
	return result;
}

bool Randomizer::checkAll(std::vector<std::vector<int>> matrix) {
	for (int i  = 0; i < nsteps; i++) {
		std::set<int> s(matrix[i].begin(), matrix[i].end());
		if (nsteps != (int)s.size()) {
			return false;
		}
	}
	
	for (int i = 0; i < nsteps; i++) {
		std::vector<int> col;
		for (int j = 0; j < nsteps; j++) {
			col.push_back(matrix[j][i]);
		}
		std::set<int> s(col.begin(), col.end());
		if (nsteps != (int)s.size()) {
			return false;
		}
	}
	
	for (int x = 0; x < nsteps; x += blockSize) {
		for (int y = 0; y < nsteps; y += blockSize) {
			std::vector<int> block;
			for (int i = x; i < x + blockSize; i++) {
				for (int j = y; j < y + blockSize; j++) {
					block.push_back(matrix[i][j]);
				}
			}
			std::set<int> s(block.begin(), block.end());
			if (nsteps != (int)s.size()) {
				return false;
			}
		}
	}

	return true;
}