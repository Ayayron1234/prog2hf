#pragma once
#include <fstream>
#include "Shape.h"
#include "Input.h"

class Window {
	SDL_Window* window;
	SDL_Renderer* renderer;

	SDL_Color backgroundColor;
	bool* isRunning;

	void initSDL() {
		int flags = SDL_WINDOW_RESIZABLE | SDL_RENDERER_PRESENTVSYNC | SDL_WINDOW_ALLOW_HIGHDPI;

		// Initialise SDL
		if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		{
			SDL_Log("SDL cound not be initialised: %s\n", SDL_GetError());
			return;
		}

		// Create window
		window = SDL_CreateWindow("test window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 720, 480, flags);
		if (window == NULL)
		{
			SDL_Log("SDL couldn't open %s window: %s\n", "test window", SDL_GetError());
			return;
		}

		// Create rendering context
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		if (renderer == NULL)
		{
			SDL_Log("SDL couldn't create rendering context: %s", SDL_GetError());
			return;
		}
		Shape::debugRenderer = renderer;
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		SDL_RenderClear(renderer);
	}

	void handleSDLEvents() {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type)
			{
			case SDL_QUIT: {
				*isRunning = false;
			} break;

				// handle mouse events
			case SDL_MOUSEBUTTONDOWN: {
				Mouse::setButton((int)event.button.button);
			}
			case SDL_MOUSEBUTTONUP: {
				if (event.button.type == SDL_MOUSEBUTTONUP)
					Mouse::clearButton(event.button.button);
			} break;

				// handle keyboard events
			case SDL_KEYDOWN: {

			} break;
			case SDL_KEYUP: {

			}
			default:
				break;
			}
		}
	}

	void render() {
		SDL_RenderPresent(renderer);

		SDL_SetRenderDrawColor(renderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
		SDL_RenderClear(renderer);
	}
public:
	Window(bool* runStatePointer) : backgroundColor({ 80, 80, 80, 255 }), isRunning(runStatePointer) {
		initSDL();
	}

	void Update() {
		render();
		handleSDLEvents();
	}

	~Window() {
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
	}

	friend class Scene;
};

class Scene {
	Window window;
	bool isRunning;

	Vector<Shape*> shapes;

public:
	Scene() : window(&isRunning), isRunning(true) {}

	bool IsRunning() {
		return isRunning;
	}

	void SetBackgroundColor(SDL_Color color) {
		window.backgroundColor = color;
	}

	SDL_Renderer* GetRenderer() {
		return window.renderer;
	}

	void AddShape(Shape* shape) {
		shapes.push_back(shape);
	}

	void RemoveShape(Shape* shape) {
		Shape* tmp = shape;
		ForEachShape([&](Shape* _shape, unsigned i) {
			if (shape == _shape) {
				shapes.splice(i, 1);
				return;
			}
			});
		delete tmp;
	}

	void Quit() {
		isRunning = false;
	}

	void ForEachShape(std::function<void(Shape*)> callback) {
		for (unsigned i = 0; i < shapes.size(); i++)
			callback(shapes.at(i));
	}
	void ForEachShape(std::function<void(Shape*, unsigned)> callback) {
		for (unsigned i = 0; i < shapes.size(); i++)
			callback(shapes.at(i), i);
	}

	void Save() {
		std::ofstream file("save.txt");
		
		for (size_t i = 0; i < shapes.size(); i++)
			shapes.at(i)->Save(file);
	}

	void Load() {
		std::ifstream file("save.txt");

		size_t type;
		Vec2 origo, point;
		while (file >> type >> origo.x >> origo.y >> point.x >> point.y) {
			switch (type)
			{
			case 1: { 
				std::cout << "Square(";
				AddShape(new Square(origo, point)); 
			} break;
			case 2: {
				std::cout << "Triangle(";
				AddShape(new Triangle(origo, point));
			} break;
			default:
				break;
			}
			std::cout << "origo(" << origo.x << ", " << origo.y << "), point(" << point.x << ", " << point.y << "));" << std::endl;
		}
	}

	void Update() {
		ForEachShape([&](Shape* shape) {
			shape->Update();
			shape->draw(GetRenderer());
			});

		window.Update();
	}

	~Scene() {
		for (size_t i = 0; i < shapes.size(); i++) {
			delete shapes.at(i);
		}
	}
};
