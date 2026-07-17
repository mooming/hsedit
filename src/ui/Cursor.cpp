//
// Created by Hansol Park on 2026. 7. 17..
//

#include "Cursor.h"

namespace hs::ui
{

Cursor::Cursor()
	: row(0)
	, column(0)
{}

Cursor::Cursor(int row, int column)
	: row(row)
	, column(column)
{}

Cursor::~Cursor()
{}

Cursor Cursor::operator+(const Cursor& rhs) const
{
	return Cursor(row + rhs.row, column + rhs.column);
}

Cursor Cursor::operator-(const Cursor& rhs) const
{
	return Cursor(row - rhs.row, column - rhs.column);
}

Cursor Cursor::operator*(int scalar) const
{
	return Cursor(row * scalar, column * scalar);
}

Cursor Cursor::operator/(int scalar) const
{
	return Cursor(row / scalar, column / scalar);
}

Cursor& Cursor::operator+=(const Cursor& rhs)
{
	row += rhs.row;
	column += rhs.column;
	return *this;
}

Cursor& Cursor::operator-=(const Cursor& rhs)
{
	row -= rhs.row;
	column -= rhs.column;
	return *this;
}

Cursor& Cursor::operator*=(int scalar)
{
	row *= scalar;
	column *= scalar;
	return *this;
}

Cursor& Cursor::operator/=(int scalar)
{
	row /= scalar;
	column /= scalar;
	return *this;
}

} // namespace hs::ui
