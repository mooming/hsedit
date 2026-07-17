//
// Created by Hansol Park on 2026. 7. 17..
//

#pragma once


namespace hs::ui
{
class Cursor
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
	Cursor();
	Cursor(int row, int column);
	~Cursor();

	bool IsValid() const { return row >= 0 && column >= 0; }

	Cursor operator+(const Cursor& rhs) const;
	Cursor operator-(const Cursor& rhs) const;
	Cursor operator*(int scalar) const;
	Cursor operator/(int scalar) const;

	Cursor& operator+=(const Cursor& rhs);
	Cursor& operator-=(const Cursor& rhs);

	Cursor& operator*=(int scalar);
	Cursor& operator/=(int scalar);
};
} // namespace hs::ui
