#include "Graphics.hpp"

Graphics::Graphics(int windowWidth, int windowHeight, int level) {
	SDL_Init(SDL_INIT_EVERYTHING);
	
	TTF_Init();
    font = TTF_OpenFont(fontPath, fontSize);
	if (!font) {
		throw "Could not intialize font object";
	}

	window = SDL_CreateWindow("Sudoku", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_SHOWN); 
	if (!window) { 
		throw "Could not initialize window object";
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
	if (!renderer) {
		throw "Could not initialize renderer object";
	}

	setBackgroundColor();
	SDL_RenderClear(renderer);

	this->windowWidth = windowWidth;
	this->windowHeight = windowHeight;
	
	xOffset = (windowWidth - stepSize * nsteps) / 2;
	yOffset = (windowHeight - stepSize * nsteps) / 2;
	
	selectedRect.w = stepSize - 1;
	selectedRect.h = stepSize - 1;
	
	randomizer = Randomizer(nsteps, level);
	
	drawBoard();
	initNumbers();
	
	SDL_Rect instructionRect;
	placeText(instructionRect, "Press Enter to Submit", stepSize / 20, 4 * stepSize);
}

Graphics::~Graphics(void) {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}

void Graphics::drawBoard(void) {
	int length = stepSize * nsteps + (blockSize + 1);
	int count = 0;
	int i = xOffset;

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	while (count <= nsteps) {
		SDL_RenderDrawLine(renderer, i, yOffset, i, yOffset + length);
		if (count++ % blockSize == 0) {
			i++;
			SDL_RenderDrawLine(renderer, i, yOffset, i, yOffset + length);
		}
		hgrid.push_back(i + 1);
		i += stepSize;
	}
	
	count = 0;
	i = yOffset;
	
	while (count <= nsteps) {
		SDL_RenderDrawLine(renderer, xOffset, i, xOffset + length, i);
		if (count++ % blockSize == 0) {
			i++;
			SDL_RenderDrawLine(renderer, xOffset, i, xOffset + length, i);
		}
		vgrid.push_back(i + 1);
		i += stepSize;
	}

	SDL_RenderPresent(renderer);
}

void Graphics::initNumbers(void) {
	for (int i = 0; i < nsteps; i++) {
		std::vector<int> row;
		for (int j = 0; j < nsteps; j++) {
			row.push_back(0);
		}
		result.push_back(row);
	}

	auto numbers = randomizer.randomize(nsteps);
	for (int i = 1; i <= nsteps; i++) {
		for (int j = 1; j <= nsteps; j++) {
			if (numbers[i - 1][j - 1] != 0) {
				insert(j, i, numbers[i - 1][j - 1], initTextColor);
				result[i - 1][j - 1] = numbers[i - 1][j - 1];
				initIdx.push_back({j - 1, i - 1});
			}
		}
	}
}

void Graphics::placeText(SDL_Rect& rect, const char* text, int y, int w) {
	SDL_Surface *textSurface = TTF_RenderText_Solid(font, text, initTextColor); 
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
	
	rect.w = w;
	rect.h = stepSize / 2;
	rect.x = (windowWidth - rect.w) / 2;
	rect.y = y;
	
	SDL_RenderCopy(renderer, textTexture, NULL, &rect);
	SDL_FreeSurface(textSurface);
	SDL_RenderPresent(renderer);
	SDL_DestroyTexture(textTexture);
}

void Graphics::insert(int x, int y, int number, SDL_Color textColor) {
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, std::to_string(number).c_str(), textColor); 
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

	int delta = stepSize / 10;
    SDL_Rect textRect;
    textRect.x = hgrid[x - 1] + delta;
    textRect.y = vgrid[y - 1] + delta;
    textRect.w = stepSize - 1 - 2 * delta;
    textRect.h = stepSize - 1 - 2 * delta;
	
	unselect(textRect, true);
	
	SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
	SDL_FreeSurface(textSurface);
	SDL_RenderPresent(renderer);
	SDL_DestroyTexture(textTexture);
}

void Graphics::select(int x, int y) {
	unselect(messageRect, true);
	unselect(selectedRect, false);
	selectedRect.x = INT16_MAX;
	
	if (x > hgrid.back() || x < xOffset || y > vgrid.back() || y < yOffset) {
		return;
	}
	
	int xx, yy;
	
	for (int i = hgrid.size() - 1; i >= 0; i--) {
		if (x >= hgrid[i]) {
			xx = hgrid[i];
			break;
		}
	}
	
	for (int i = hgrid.size() - 1; i >= 0; i--) {
		if (y >= vgrid[i]) {
			yy = vgrid[i];
			break;
		}
	}
	
	int xxx = (xx - xOffset) / stepSize;
	int yyy = (yy - yOffset) / stepSize;
	
	if(std::find(initIdx.begin(), initIdx.end(), std::make_pair(xxx, yyy)) != initIdx.end()) {
		return; 
	} 
	
	selectedRect.x = xx;
	selectedRect.y = yy;
	
	SDL_SetRenderDrawColor(renderer, 215, 215, 0, 255);
	SDL_RenderDrawRect(renderer, &selectedRect);
	SDL_RenderPresent(renderer);
}

void Graphics::clean(void) {
	unselect(messageRect, true);
	if (selectedRect.x != INT16_MAX) {
		int x = (selectedRect.x - xOffset) / stepSize;
		int y = (selectedRect.y - yOffset) / stepSize;
		
		result[y][x] = 0;
		unselect(selectedRect, true);
	}	
}

bool Graphics::verify(void) {
	for (auto row : result) {
		if (std::find(row.begin(), row.end(), 0) != row.end()) {
			placeText(messageRect, "Not Complete!", stepSize / 2, 2 * stepSize);
			return false;
		}
	}
	
	bool isCorrect = randomizer.checkAll(result);
	if (isCorrect) {
		placeText(messageRect, "Correct!", stepSize / 2, 2 * stepSize);
	}
	else {
		placeText(messageRect, "Incorrect!", stepSize / 2, 2 * stepSize);
	}
	return isCorrect;
}

void Graphics::setBackgroundColor(void) const {
	SDL_SetRenderDrawColor(renderer, 104, 164, 128, 255); 
}

void Graphics::unselect(const SDL_Rect& rect, bool fill) {
	setBackgroundColor();
	if (fill) {
		SDL_RenderFillRect(renderer, &rect);
	}
	else {
		SDL_RenderDrawRect(renderer, &rect);
	}
	SDL_RenderDrawRect(renderer, &rect);
	SDL_RenderPresent(renderer);
}

void Graphics::placeNumber(int number) {
	int a = number - 48;
	if (a >= 1 && a <= nsteps && selectedRect.x != INT16_MAX) {
		int x = 1 + (selectedRect.x - xOffset) / stepSize;
		int y = 1 + (selectedRect.y - yOffset) / stepSize;
		insert(x, y, a, insertedTextColor);
		result[y - 1][x - 1] = a;
	}	
}