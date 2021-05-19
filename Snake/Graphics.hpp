#pragma once

#include <deque>
#include <string>
#include <iostream>

#include "SDL2/SDL.h"
#undef main

class Graphics {
public:
	enum Dir {Up, Left, Down, Right};
	
private:
	const int rectSize = 20;

	int windowWidth;
	int windowHeight;
	
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	
	SDL_Rect food;
	std::deque<SDL_Rect> snake;
	
	void printScore(void);
	void renderWindow(void);
	void renderSnake(int r, int g, int b);
	void renderFood(void);
	void placeFood(void);
	bool checkCollision(void);
	void moveSnake(const Dir& dir);
	
public:
	Graphics(int windowWidth, int windowHeight);
	~Graphics(void);
	
	int getSpeed(void);
	bool run(const Dir& dir);
};