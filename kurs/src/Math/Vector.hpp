#pragma once

#include "kurspch.hpp"

namespace kurs
{
	struct Vector2f
	{
		float CoordX = 0.0f;
		float CoordY = 0.0f;
	
		explicit Vector2f() = default;
	
		Vector2f(float coordX, float coordY);
	
		Vector2f operator+() const;
		Vector2f operator+(const Vector2f& other) const;
		Vector2f& operator+=(const Vector2f& other);

		Vector2f operator-() const;
		Vector2f operator-(const Vector2f& other) const;
		Vector2f& operator-=(const Vector2f& other);
	
		Vector2f operator*(float scalar) const;
		Vector2f& operator*=(float scalar);
	
		Vector2f operator/(float scalar) const;
		Vector2f& operator/=(float scalar);

		float Dot(const Vector2f& other) const;

		float GetSquareLength() const;
		float GetLength() const;
	};
}
