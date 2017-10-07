#include "Vec2D.h"

#include <cmath>
#include <SDL_assert.h>

Vec2D::Vec2D(double _x, double _y) : x(_x), y(_y)
{

}

Vec2D& Vec2D::operator=(const Vec2D& rhs)
{
	if (this != &rhs)
	{
		x = rhs.x;
		y = rhs.y;
	}
		
	return *this;
}

Vec2D Vec2D::operator-()
{
	Vec2D result;
	result.x = -x;
	result.y = -y;

	return result;
}

void Vec2D::operator+=(const Vec2D& rhs)
{
	x += rhs.x;
	y += rhs.y;
}

void Vec2D::operator-=(const Vec2D& rhs)
{
	x -= rhs.x;
	y -= rhs.y;
}

void Vec2D::operator/=(double s)
{
	if (s == 0)
		return;

	x /= s;
	y /= s;
}

void Vec2D::operator*=(double s)
{
	x *= s;
	y *= s;
}

bool Vec2D::operator==(const Vec2D& rhs) const
{
	return (x == rhs.x && y == rhs.y);
}

bool Vec2D::operator!=(const Vec2D& rhs) const
{
	return (x != rhs.x || y != rhs.y);
}

double Vec2D::Length() const
{
	return sqrt(x*x + y*y);
}

Vec2D& Vec2D::Normalize()
{
	double len = Length();
	SDL_assert(len != 0);
	if (len != 0)
	{
		x /= len;
		y /= len;
	}

	return *this;
}

double Vec2D::dot(const Vec2D& rhs) const
{
	return x*rhs.x + y*rhs.y;
}

Vec2D& Vec2D::Reflect(const Vec2D& normal)
{
	Vec2D I = *this;

	return *this = -2.0f * dot(normal) * normal + I;
}

// ------------------------------------------------------------------

Vec2D operator*(const Vec2D& v, double s)
{
	Vec2D result;

	result.x = v.x * s;
	result.y = v.y * s;

	return result;
}

Vec2D operator*(double s, const Vec2D& v)
{
	return v*s;
}

Vec2D operator/(const Vec2D& v, double s)
{
	Vec2D result;

	double len = v.Length();
	SDL_assert(len != 0);
	if (len != 0)
	{
		result.x = v.x / s;
		result.y = v.y / s;
	}

	return result;
}

Vec2D operator+(const Vec2D& lhs, const Vec2D& rhs)
{
	Vec2D result;
	result.x = lhs.x + rhs.x;
	result.y = lhs.y + rhs.y;

	return result;
}

Vec2D operator-(const Vec2D& lhs, const Vec2D& rhs)
{
	Vec2D result;
	result.x = lhs.x - rhs.x;
	result.y = lhs.y - rhs.y;

	return result;
}
