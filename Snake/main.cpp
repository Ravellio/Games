#include "Graphics.hpp"
#include "SDL2/SDL.h"

int main() {
	const int windowWidth = 800;
	const int windowHeight = 600;
	
	srand(time(0));
	Graphics g(windowWidth, windowHeight);
	Graphics::Dir dir = static_cast<Graphics::Dir>(rand() % 4);
	
	bool quit = false;
	SDL_Event e;
	
	int fps;
    int timeout = SDL_GetTicks() + 1000 / fps;
	while (!quit) {
		quit = g.run(dir);
		fps = g.getSpeed();
		while (!SDL_TICKS_PASSED(SDL_GetTicks(), timeout)) {
			while (SDL_PollEvent(&e)) { 
				switch (e.type) { 
					case SDL_QUIT: 
						quit = true;
						break;
					case SDL_KEYDOWN: 
						switch (e.key.keysym.sym) { 
							case SDLK_w: 
								if (dir != Graphics::Dir::Down) {
									dir = Graphics::Dir::Up; 
								}
								break;
							case SDLK_a:
								if (dir != Graphics::Dir::Right) {
									dir = Graphics::Dir::Left; 
								}
								break;
							case SDLK_d: 
								if (dir != Graphics::Dir::Left) {
									dir = Graphics::Dir::Right;
								}
								break;
							case SDLK_s:
								if (dir != Graphics::Dir::Up) {
									dir = Graphics::Dir::Down; 
								}
								break;
							default:
								break;
						}
				}
			}
		}
		timeout += 1000 / fps;
	}
	return 0;
}
