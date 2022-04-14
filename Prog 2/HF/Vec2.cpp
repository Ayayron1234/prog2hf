#include "Vec2.h"

std::istream& operator>>(std::istream& is, Vec2& vec) {
	return is >> vec.x >> vec.y;
}
