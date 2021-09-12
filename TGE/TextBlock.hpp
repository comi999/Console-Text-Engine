#pragma once
#include <string>

#include "Utilities.hpp"
#include "Buffer.hpp"

using namespace std;

enum class TextAllignment : char
{
	Left, CentreLeft, CentreRight, Right
};

enum class WrapMode : char
{
	Wrap,
	Truncate
};

class TextBlock : public Rect
{
public:

	string Text;
	int BorderWidth = 0;
	CharInfo Border;
	CharInfo Centre;
	TextAllignment TextAllignment = TextAllignment::Left;
	WrapMode WrapMode = WrapMode::Wrap;
	bool SplitWords : 1;

};