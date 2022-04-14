#include "Shape.h"
#include "Scene.h"

#include <SDL.h>
#undef main

//#define JPORTA

bool main(void) {
	// set up scene
	Scene scene;
	scene.SetBackgroundColor({ 89, 116, 117, 255 });
	scene.Load();

#ifndef JPORTA
	// create new shapes:
	Vec2 origo, point;
	Mouse::OnClick(MouseButton::RIGHT, [&]() {
		origo = Mouse::GetPosition();

		Mouse::OnRelease(MouseButton::RIGHT, [&]() {
			point = Mouse::GetPosition();

			scene.AddShape(new Triangle(origo, point));
			}, false);
		});

	// delete shape
	Mouse::OnClick(MouseButton::MIDDLE, [&]() {
		scene.ForEachShape([&](Shape* shape) {
			if (shape->IsPointInside(Mouse::GetPosition())) {
				scene.RemoveShape(shape);
			}
			});
		});
#endif // JPORTA

	// main loop
	while (scene.IsRunning()) {
		scene.Update();

#ifndef JPORTA
		// show the shape that is currently being created
		if (Mouse::IsButtonDown(MouseButton::RIGHT)) {
			Vec2 mousePos = Mouse::GetPosition();

			Vec2 a, b, c;
			a = mousePos;
			b = (mousePos - origo).Rotate(M_PI * 2 / 3) + origo;
			c = (mousePos - origo).Rotate(M_PI * 4 / 3) + origo;

			SDL_SetRenderDrawColor(scene.GetRenderer(), 255, 255, 255, 255);
			SDL_RenderDrawLine(scene.GetRenderer(), a.x, a.y, b.x, b.y);
			SDL_RenderDrawLine(scene.GetRenderer(), b.x, b.y, c.x, c.y);
			SDL_RenderDrawLine(scene.GetRenderer(), c.x, c.y, a.x, a.y);

			SDL_SetRenderDrawColor(scene.GetRenderer(), 179, 120, 255, 255);
			SDL_RenderDrawLine(scene.GetRenderer(), origo.x, origo.y, mousePos.x, mousePos.y);
		}
#endif // !JPORTA
	}

	scene.Save();

	return true;
}
