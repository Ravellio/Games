#pragma once

#include <vector>
#include <iostream>

#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "Randomizer.hpp"

class Graphics {
private:
	const char* fontPath = "/home/vlad/Documents/Projects/Games/Sudoku/arial.ttf";// /
	const int fontSize = 92;
	
	const SDL_Color initTextColor = {0, 0, 0, 255};
	const SDL_Color insertedTextColor = {255, 0, 0, 255};

	const int stepSize = 70;
	const int nsteps = 9; // Fix me: should be set as a parameter
	const int blockSize = static_cast<int>(sqrt(nsteps));

	int windowWidth;
	int windowHeight;
	
	int xOffset;
	int yOffset;
	
	std::vector<int> hgrid;
	std::vector<int> vgrid;
	
	std::vector<std::vector<int>> result;
	std::vector<std::pair<int, int>> initIdx;

	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	TTF_Font *font = nullptr;
	
	SDL_Rect selectedRect;
	SDL_Rect messageRect;
	
	Randomizer randomizer;
	
	void drawBoard(void);
	void initNumbers(void);
	void placeText(SDL_Rect& rect, const char* text, int y, int w);
	void insert(int x, int y, int number, SDL_Color textColor);
	void setBackgroundColor(void) const;
	void unselect(const SDL_Rect& rect, bool fill);
	
public:
	Graphics(int windowWidth, int windowHeight, int level);
	~Graphics(void);
	
	void select(int x, int y);
	void clean(void);
	bool verify(void);
	void placeNumber(int number);
};