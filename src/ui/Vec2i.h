//
// Created by Hansol Park on 2026. 7. 17..
//

#pragma once

namespace hs::ui
{
class Vec2i
{
public:
	union
	{
		struct
		{
			int row;
			int column;
		};

		struct
		{
			int y;
			int x;
		};

		struct
		{
			int value[2];
		};
	};

public:
	Vec2i();
	Vec2i(int row, int column);
	~Vec2i();

	bool IsValid() const { return row >= 0 && column >= 0; }

	Vec2i operator+(const Vec2i& rhs) const;
	Vec2i operator-(const Vec2i& rhs) const;
	Vec2i operator*(int scalar) const;
	Vec2i operator/(int scalar) const;

	Vec2i& operator+=(const Vec2i& rhs);
	Vec2i& operator-=(const Vec2i& rhs);

	Vec2i& operator*=(int scalar);
	Vec2i& operator/=(int scalar);
};
} // namespace hs::ui
