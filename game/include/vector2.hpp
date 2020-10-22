#pragma once

class Vector2
{
public:
	float x;
	float y;

	Vector2()
	{
	}

	Vector2(float x, float y)
	{
		this->x = x;
		this->y = y;
	}

	inline float Magnitude()
	{
		return sqrt(x * x + y * y);
	}

	inline Vector2 Normalise()
	{
		auto mag = Magnitude();
		Vector2 vec(x / mag, y / mag);
		return vec;
	}

	Vector2 operator/ (float num) {
		return Vector2(x / num, y / num);
	}

	Vector2 operator+ (Vector2 vector) {
		return Vector2(x + vector.x, y + vector.y);
	}

	Vector2 operator- (Vector2 vector) {
		return Vector2(x - vector.x, y - vector.y);
	}

	Vector2 operator* (float scaler) {
		return Vector2(x * scaler, y * scaler);
	}
};