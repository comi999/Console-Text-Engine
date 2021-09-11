#pragma once
#include <string>

#include "Utilities.hpp"
#include "Buffer.hpp"

using namespace std;

enum class TextAllignment : char
{
	Left, Centre, Right
};

enum class WrapMode : char
{
	Overflow,
	Truncate
};

class TextBlock : public Rect
{
public:

	string Text;
	int BorderWidth;
	CharInfo Border;
	CharInfo Centre;
	TextAllignment TextAllignment;
	WrapMode HorizontalWrapMode;
	WrapMode VerticalWrapMode;

};