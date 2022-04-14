#pragma once
#include <iostream>

#define M_PI 3.14159265358979323846

struct Vec2 {
	double x;
	double y;

	Vec2(const double _x = 0, const double _y = 0) : x(_x), y(_y) {}

	long double GetAngle() const {
		return (y > 0) ? acos(x / GetLength()) : M_PI * 2 - acos(x / GetLength());
	}
	void SetAngle(long double angle) {
		x = cos(angle) * GetLength();
		y = sin(angle) * GetLength();
	}
	Vec2 Rotate(long double angle) const {
		double newX = x * cos(angle) - y * sin(angle);
		double newY = x * sin(angle) + y * cos(angle);
		return Vec2(newX, newY);
	}
	static long double Angle(const Vec2& a, const Vec2& b) {
		return atan2(a.Det(b), a.Dot(b));
	}

	long double GetLength() const {
		return sqrt(x * x + y * y);
	}
	void SetLength(long double length) {
		x = cos(GetAngle()) * length;
		y = sin(GetAngle()) * length;
	}

	long double Dot(const Vec2& vec) const {
		return x * vec.x + y * vec.y;
	}
	static long double Dot(const Vec2 a, const Vec2 b) {
		return a.x * b.x + a.y * b.y;
	}

	long double Det(const Vec2& vec) const {
		return x * vec.y - y * vec.x;
	}
	static long double Det(const Vec2 a, const Vec2 b) {
		return a.x * b.x - a.y * b.y;
	}

	void Print(std::ostream& os = std::cout) const {
		os << "Vec(" << x << y << ")";
	}

	Vec2& operator=(const Vec2& vec) {
		x = vec.x;
		y = vec.y;
		return *this;
	}

	Vec2 operator+(const Vec2& vec) const {
		return Vec2(x + vec.x, y + vec.y);
	}
	void operator+=(const Vec2& vec) {
		x += vec.x;
		y += vec.y;
	}

	Vec2 operator-(const Vec2& vec) const {
		return Vec2(x - vec.x, y - vec.y);
	}
	void operator-=(const Vec2& vec) {
		x -= vec.x;
		y -= vec.y;
	}

	bool operator>(const Vec2& vec) const {
		return GetLength() > vec.GetLength();
	}
	bool operator>=(const Vec2& vec) const {
		return GetLength() >= vec.GetLength();
	}

	bool operator<(const Vec2& vec) const {
		return GetLength() < vec.GetLength();
	}
	bool operator<=(const Vec2& vec) const {
		return GetLength() <= vec.GetLength();
	}

	Vec2 operator*(const Vec2& vec) const {
		return Vec2(x * vec.x, y * vec.y);
	}
	void operator*=(const Vec2& vec) {
		x *= vec.x;
		y *= vec.y;
	}

	Vec2 operator*(const long double d) const {
		return Vec2(x * d, y * d);
	}
	void operator*=(const long double d) {
		x *= d;
		y *= d;
	}
};
