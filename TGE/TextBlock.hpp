#pragma once
#include <string>

#include "Utilities.hpp"
#include "Buffer.hpp"
#include "TextFormatter.hpp"

using namespace std;

class TextBlock : public Rect
{
public:

	string Text;
	int BorderWidth = 0;
	CharInfo Border;
	CharInfo Centre;
	TextFormatter Formatting;

};