#pragma once

#include <algorithm>

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

	inline float Magnitude() const
	{
		return sqrt(SqrMagnitude());
	}

	inline float SqrMagnitude() const
	{
		return x * x + y * y;
	}

	inline Vector2 Normalise() const
	{
		auto mag = Magnitude();
		Vector2 vec(x / mag, y / mag);
		return vec;
	}

	Vector2 operator/ (float num) 
	{
		return Vector2(x / num, y / num);
	}

	Vector2 operator+ (Vector2 vector) 
	{
		return Vector2(x + vector.x, y + vector.y);
	}

	Vector2 operator- (Vector2 vector) 
	{
		return Vector2(x - vector.x, y - vector.y);
	}

	Vector2 operator* (float scaler) 
	{
		return Vector2(x * scaler, y * scaler);
	}

	float DotProduct(const Vector2& a_vec) const
	{
		return x * a_vec.x + y * a_vec.y;
	}

	float AngleBetween(const Vector2& vector) const
	{
		float angle = Normalise().DotProduct(vector.Normalise());
		float clampedAngle = std::clamp<float>(angle, -1, 1);

		angle = acos(angle);
		return angle;
	}
};