#include "Math/Vector.hpp"

namespace kurs
{
	Vector2f::Vector2f(float coordX, float coordY)
		: CoordX(coordX)
		, CoordY(coordY)
	{
	}

	Vector2f Vector2f::operator+() const
	{
		return *this;
	}

	Vector2f Vector2f::operator+(const Vector2f& other) const
	{
		return Vector2f(CoordX + other.CoordX, CoordY + other.CoordY);
	}

	Vector2f& Vector2f::operator+=(const Vector2f& other)
	{
		return *this = *this + other;
	}

	Vector2f Vector2f::operator-() const
	{
		return Vector2f(-CoordX, -CoordY);
	}

	Vector2f Vector2f::operator-(const Vector2f& other) const
	{
		return *this + -other;
	}

	Vector2f& Vector2f::operator-=(const Vector2f& other)
	{
		return *this = *this - other;
	}

	Vector2f Vector2f::operator*(float scalar) const
	{
		return Vector2f(CoordX * scalar, CoordY * scalar);
	}

	Vector2f& Vector2f::operator*=(float scalar)
	{
		return *this = *this * scalar;
	}

	Vector2f Vector2f::operator/(float scalar) const
	{
		return Vector2f(CoordX / scalar, CoordY / scalar);
	}

	Vector2f& Vector2f::operator/=(float scalar)
	{
		return *this = *this / scalar;
	}
	
	float Vector2f::Dot(const Vector2f& other) const
	{
		return CoordX * other.CoordX + CoordY * other.CoordY;
	}
	
	float Vector2f::GetSquareLength() const
	{
		return Dot(*this);
	}
	
	float Vector2f::GetLength() const
	{
		return std::sqrt(GetSquareLength());
	}
}
