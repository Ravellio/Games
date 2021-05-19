#include "Graphics.hpp"

Graphics::Graphics(int windowWidth, int windowHeight) {
	SDL_Init(SDL_INIT_EVERYTHING);

	window = SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_SHOWN); 
	if (!window) { 
		throw "Could not initialize window object";
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!renderer) {
		throw "Could not initialize renderer object";
	}

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); 
	SDL_RenderClear(renderer);
	
	food.w = rectSize;
	food.h = rectSize;
	
	SDL_Rect head;
	head.w = rectSize;
	head.h = rectSize;
	head.x = windowWidth / 2;
	head.y = windowHeight / 2;
	snake.push_front(head);

	this->windowWidth = windowWidth;
	this->windowHeight = windowHeight;

	placeFood();
}

Graphics::~Graphics(void) {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}

void Graphics::printScore(void) {
	std::string s = "Snake:   " + std::to_string(snake.size() - 1);
	SDL_SetWindowTitle(window, s.c_str());
}

void Graphics::renderWindow(void) {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
	
	printScore();
}

void Graphics::renderSnake(int r, int g, int b) {
	for (size_t i = 0; i < snake.size(); i++) {
		if (i == 0) {
			SDL_SetRenderDrawColor(renderer, r, g, b, 255);
		}
		else {
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		}
		SDL_RenderFillRect(renderer, &snake[i]);
	}	
}

void Graphics::renderFood(void) {
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(renderer, &food);
}

void Graphics::placeFood(void) {
	bool success = false;
	int x, y;
	
	while (!success) {
		x = rand() % (windowWidth / rectSize);
		y = rand() % (windowHeight / rectSize);
		
		success = true;
		for (auto block : snake) {
			if (block.x == x && block.y == y) {
				success = false;
				break;
			}
		}
	}
	
	food.x = x * rectSize;
	food.y = y * rectSize;
}

bool Graphics::checkCollision(void) {
	for (size_t i = 1; i < snake.size(); i++) {
		if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
			snake[i].x = -1;
			snake[i].y = -1;
			return true;
		}
	} 
	return false;
}

void Graphics::moveSnake(const Dir& dir) {			
	if (((dir == Dir::Up || dir == Dir::Down) && snake[0].x == food.x && abs(snake[0].y - food.y) == rectSize) ||
		((dir == Dir::Left || dir == Dir::Right) && snake[0].y == food.y && abs(snake[0].x - food.x) == rectSize)) {
			snake.push_front(food);
			placeFood();
		}
	
	for (size_t i = snake.size() - 1; i > 0; i--) {
		snake[i].x = snake[i - 1].x;
		snake[i].y = snake[i - 1].y;
	}
	
	if (dir == Dir::Up) {
		snake[0].y -= rectSize;
	}
	else if (dir == Dir::Down) {
		snake[0].y += rectSize;
	}
	else if (dir == Dir::Left) {
		snake[0].x -= rectSize;
	}
	else if (dir == Dir::Right) {
		snake[0].x += rectSize;
	}
	
	snake[0].x = (windowWidth + (snake[0].x % windowWidth)) % windowWidth;
	snake[0].y = (windowHeight + (snake[0].y % windowHeight)) % windowHeight;
}

int Graphics::getSpeed(void) {
	return snake.size() + 5;
}

bool Graphics::run(const Dir& dir) {
    renderWindow();
	
	bool quit = checkCollision();
	if (quit) {
		renderSnake(255, 0, 0);
	}
	else {
		moveSnake(dir);
		renderSnake(0, 0, 255);
		renderFood();
	}
	
    SDL_RenderPresent(renderer);
	return quit;
}