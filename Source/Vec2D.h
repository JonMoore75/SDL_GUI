#ifndef Vec2D_h__
#define Vec2D_h__
#pragma once

class Vec2D
{
public:
	Vec2D(double _x = 0.0, double _y = 0.0);
	~Vec2D() noexcept {}

	Vec2D& operator=(const Vec2D& rhs);

	Vec2D operator-();

	void operator+=(const Vec2D& rhs);
	void operator-=(const Vec2D& rhs);
	void operator*=(double s);
	void operator/=(double s);
	bool operator==(const Vec2D& rhs) const;
	bool operator!=(const Vec2D& rhs) const;

	int intX() const { return static_cast<int>(x); }
	int intY() const { return static_cast<int>(y); }

	double Length() const;
	Vec2D& Normalize();

	double dot(const Vec2D& rhs) const;

	Vec2D& Reflect(const Vec2D& normal);

	// Data members.
	double x;
	double y;

private:
};


// Define with left hand and right hand sides reversed
// so we can write both v*s and s*v.
Vec2D operator*(const Vec2D& v, double s);
Vec2D operator*(double s, const Vec2D& v);

Vec2D operator/(const Vec2D& v, double s);

Vec2D operator+(const Vec2D& lhs, const Vec2D& rhs);
Vec2D operator-(const Vec2D& lhs, const Vec2D& rhs);


#endif Vec2D_h__

