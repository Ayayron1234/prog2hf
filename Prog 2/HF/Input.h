#pragma once
#include <functional>
#include <SDL.h>
#undef main

#include "Vec2.h"
#include "Vector.h"

enum class MouseButton
{
	LEFT = 1,
	MIDDLE = 2,
	RIGHT = 3,
	X1 = 4,
	X2 = 5,
};

struct MouseEventListener {
private:
	static unsigned firstUnusedID;
public:
	unsigned ID;

	bool type;								/// if button is pressed true, else false
	MouseButton button;
	bool persistent;						/// if false, the listener will be destroyed after it is activated. 
	std::function<void()> callback;

	MouseEventListener(bool _type = true, MouseButton _button = MouseButton::LEFT, bool _persistent = true, std::function<void()> _callback = [=]() {}) 
		: ID(firstUnusedID++),
		type(_type), 
		button(_button), 
		persistent(_persistent), 
		callback(_callback) {}
};

class Mouse {
	static Vec2 position;
	static uint8_t buttons;

	static Vector<MouseEventListener> eventListeners;

	static void setButton(int button) {
		if (checkButton(button)) return;
		Mouse::buttons |= 1UL << (button - 1);

		for (size_t i = 0; i < eventListeners.size(); i++) {
			auto listener = eventListeners.at(i);
			if (!listener.callback) {
				eventListeners.splice(i);
				i--;
				continue;
			}

			if (listener.type == true && listener.button == (MouseButton)button) {
				listener.callback();
				if (!listener.persistent) {
					eventListeners.splice(i);
					i--;
				}
			}
		}
	}
	static void clearButton(int button) {
		if (!checkButton(button)) return;
		Mouse::buttons &= ~(1UL << (button - 1));

		for (size_t i = 0; i < eventListeners.size(); i++) {
			auto listener = eventListeners.at(i);
			if (!listener.callback) {
				eventListeners.splice(i);
				i--;
				continue;
			}

			if (listener.type == false && listener.button == (MouseButton)button) {
				listener.callback();
				if (!listener.persistent) {
					eventListeners.splice(i);
					i--;
				}
			}
		}
	}

	static bool checkButton(int button) {
		return ((Mouse::buttons >> (button - 1)) & 1U) == true ? true : false;
	}
public:
	Mouse() {}

	static Vec2 GetPosition() {
		int x, y;
		SDL_GetMouseState(&x, &y);
		return Vec2(x, y);
	}

	static bool IsButtonDown(MouseButton button) {
		return checkButton((int)button);
	}

	static void RemoveEventListener(unsigned listenerID) {
		for (unsigned i = 0; i < eventListeners.size(); i++) {
			if (eventListeners.at(i).ID == listenerID) {
				eventListeners.splice(i, 1);
				return;
			}
		}
	}

	static unsigned OnClick(MouseButton button, std::function<void()> callback, bool isPresistent = true) {
		MouseEventListener listener(true, button, isPresistent, callback);
		eventListeners.push_back(listener);
		return listener.ID;
	}
	static unsigned OnClick(std::function<void()> callback, bool isPresistent = true) {
		MouseEventListener listener(true, MouseButton::LEFT, isPresistent, callback);
		eventListeners.push_back(listener);
		return listener.ID;
	}

	static unsigned OnRelease(MouseButton button, std::function<void()> callback, bool isPersistent = true) {
		MouseEventListener listener(false, button, isPersistent, callback);
		eventListeners.push_back(listener);
		return listener.ID;
	}

	friend class Window;
};
