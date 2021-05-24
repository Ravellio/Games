#include "Graphics.hpp"
#include <iostream>

int main() {
	const int fps = 20;
	const int windowWidth = 1000;
	const int windowHeight = 800;
	
	const int level = 2;
	
	Graphics g(windowWidth, windowHeight, level);
	
	bool quit = false;
	SDL_Event e;
	int timeout = SDL_GetTicks() + 1000 / fps;
	int x, y;
	while (!quit) {
		while (!SDL_TICKS_PASSED(SDL_GetTicks(), timeout)) {
			while (SDL_PollEvent(&e)) {
				switch (e.type) { 
					case SDL_QUIT: 
						quit = true;
						break;
					case SDL_MOUSEBUTTONDOWN:
						SDL_GetMouseState(&x, &y);
						g.select(x, y);
						break;
					case SDL_KEYDOWN:
						int key = e.key.keysym.sym;
						if (key == SDLK_BACKSPACE) {
							g.clean();
						}
						else if (key == SDLK_RETURN) {
							quit = g.verify();
						}
						else {
							g.placeNumber(e.key.keysym.sym);
						}
						break;
				}
			}
		}
		timeout += 1000 / fps;
	}
	
	SDL_Delay(1000);
	return 0;
}
	
