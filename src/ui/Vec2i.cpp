//
// Created by Hansol Park on 2026. 7. 17..
//

#include "Vec2i.h"

namespace hs::ui
{

// ========================================================================
// CONSTRUCTORS
// ========================================================================

/// @brief Default constructor: initializes to origin (0, 0)
Vec2i::Vec2i()
	: row(0)
	, column(0)
{}

/// @brief Parameterized constructor
/// @param row Initial row/y coordinate
/// @param column Initial column/x coordinate
Vec2i::Vec2i(int row, int column)
	: row(row)
	, column(column)
{}

// ========================================================================
// DESTRUCTOR
// ========================================================================

/// @brief Default destructor (no resources to clean up)
Vec2i::~Vec2i()
{}

// ========================================================================
// ARITHMETIC OPERATORS (return new Vec2i)
// ========================================================================

/// @brief Vector addition
/// @param rhs Right-hand side vector
/// @return New vector with summed coordinates
Vec2i Vec2i::operator+(const Vec2i& rhs) const
{
	return Vec2i(row + rhs.row, column + rhs.column);
}

/// @brief Vector subtraction
/// @param rhs Right-hand side vector
/// @return New vector with differenced coordinates
Vec2i Vec2i::operator-(const Vec2i& rhs) const
{
	return Vec2i(row - rhs.row, column - rhs.column);
}

/// @brief Scalar multiplication
/// @param scalar Multiplication factor
/// @return New vector with scaled coordinates
Vec2i Vec2i::operator*(int scalar) const
{
	return Vec2i(row * scalar, column * scalar);
}

/// @brief Scalar division
/// @param scalar Division factor
/// @return New vector with divided coordinates
Vec2i Vec2i::operator/(int scalar) const
{
	return Vec2i(row / scalar, column / scalar);
}

// ========================================================================
// COMPOUND ASSIGNMENT OPERATORS (modify in-place)
// ========================================================================

/// @brief Add rhs to this vector
/// @param rhs Right-hand side vector to add
/// @return Reference to this modified vector
Vec2i& Vec2i::operator+=(const Vec2i& rhs)
{
	row += rhs.row;
	column += rhs.column;
	return *this;
}

/// @brief Subtract rhs from this vector
/// @param rhs Right-hand side vector to subtract
/// @return Reference to this modified vector
Vec2i& Vec2i::operator-=(const Vec2i& rhs)
{
	row -= rhs.row;
	column -= rhs.column;
	return *this;
}

/// @brief Multiply this vector by scalar
/// @param scalar Multiplication factor
/// @return Reference to this modified vector
Vec2i& Vec2i::operator*=(int scalar)
{
	row *= scalar;
	column *= scalar;
	return *this;
}

/// @brief Divide this vector by scalar
/// @param scalar Division factor
/// @return Reference to this modified vector
Vec2i& Vec2i::operator/=(int scalar)
{
	row /= scalar;
	column /= scalar;
	return *this;
}

} // namespace hs::ui
