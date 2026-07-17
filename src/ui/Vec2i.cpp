//
// Created by Hansol Park on 2026. 7. 17..
//

#include "Vec2i.h"

namespace hs::ui
{

Vec2i::Vec2i()
	: row(0)
	, column(0)
{}

Vec2i::Vec2i(int row, int column)
	: row(row)
	, column(column)
{}

Vec2i::~Vec2i()
{}

Vec2i Vec2i::operator+(const Vec2i& rhs) const
{
	return Vec2i(row + rhs.row, column + rhs.column);
}

Vec2i Vec2i::operator-(const Vec2i& rhs) const
{
	return Vec2i(row - rhs.row, column - rhs.column);
}

Vec2i Vec2i::operator*(int scalar) const
{
	return Vec2i(row * scalar, column * scalar);
}

Vec2i Vec2i::operator/(int scalar) const
{
	return Vec2i(row / scalar, column / scalar);
}

Vec2i& Vec2i::operator+=(const Vec2i& rhs)
{
	row += rhs.row;
	column += rhs.column;
	return *this;
}

Vec2i& Vec2i::operator-=(const Vec2i& rhs)
{
	row -= rhs.row;
	column -= rhs.column;
	return *this;
}

Vec2i& Vec2i::operator*=(int scalar)
{
	row *= scalar;
	column *= scalar;
	return *this;
}

Vec2i& Vec2i::operator/=(int scalar)
{
	row /= scalar;
	column /= scalar;
	return *this;
}

} // namespace hs::ui
