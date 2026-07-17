//
// Created by Hansol Park on 2026. 7. 17..
//

#pragma once

namespace hs::ui
{
/// @brief 2D integer vector with dual naming conventions
/// @details Provides both (row, column) and (y, x) accessors for flexibility.
///          Supports arithmetic operations and compound assignments.
class Vec2i
{
public:
	/// @brief Union for flexible coordinate access
	union
	{
		/// @brief Grid-style coordinates (row, column)
		struct
		{
			int row;
			int column;
		};

		/// @brief Cartesian coordinates (y, x)
		struct
		{
			int y;
			int x;
		};

		/// @brief Array-style access
		struct
		{
			int value[2];
		};
	};

public:
	/// @brief Default constructor initializes to (0, 0)
	Vec2i();

	/// @brief Parameterized constructor
	/// @param row Initial row/y coordinate
	/// @param column Initial column/x coordinate
	Vec2i(int row, int column);

	/// @brief Destructor
	~Vec2i();

	/// @brief Check if coordinates are valid (non-negative)
	/// @return true if both row and column are >= 0
	bool IsValid() const { return row >= 0 && column >= 0; }

	/// @brief Addition operator (returns new Vec2i)
	/// @param rhs Right-hand side vector
	/// @return Sum of this and rhs
	Vec2i operator+(const Vec2i& rhs) const;

	/// @brief Subtraction operator (returns new Vec2i)
	/// @param rhs Right-hand side vector
	/// @return Difference of this and rhs
	Vec2i operator-(const Vec2i& rhs) const;

	/// @brief Scalar multiplication (returns new Vec2i)
	/// @param scalar Multiplication factor
	/// @return This vector scaled by scalar
	Vec2i operator*(int scalar) const;

	/// @brief Scalar division (returns new Vec2i)
	/// @param scalar Division factor
	/// @return This vector divided by scalar
	Vec2i operator/(int scalar) const;

	/// @brief Compound addition assignment
	/// @param rhs Right-hand side vector to add
	/// @return Reference to this after modification
	Vec2i& operator+=(const Vec2i& rhs);

	/// @brief Compound subtraction assignment
	/// @param rhs Right-hand side vector to subtract
	/// @return Reference to this after modification
	Vec2i& operator-=(const Vec2i& rhs);

	/// @brief Compound scalar multiplication assignment
	/// @param scalar Multiplication factor
	/// @return Reference to this after modification
	Vec2i& operator*=(int scalar);

	/// @brief Compound scalar division assignment
	/// @param scalar Division factor
	/// @return Reference to this after modification
	Vec2i& operator/=(int scalar);
};
} // namespace hs::ui
