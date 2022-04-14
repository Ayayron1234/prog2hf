#include "Input.h"

unsigned MouseEventListener::firstUnusedID = 0;

Vec2 Mouse::position = Vec2();
uint8_t Mouse::buttons = 0x00;

Vector<MouseEventListener> Mouse::eventListeners = Vector<MouseEventListener>();