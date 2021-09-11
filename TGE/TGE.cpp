#include <iostream>
#include "Buffer.hpp"

int main()
{
    Buffer buffer( 50, 50, 8, 12);

    CharInfo charInfo;
    charInfo.Ascii() = 'X';
    charInfo.SetBackgroundColour( ConsoleColour::GREEN );
    charInfo.SetForegroundColour( ConsoleColour::YELLOW );

    TextBlock textBlock;
    textBlock.BorderWidth = 2;
    textBlock.Border = charInfo;
    charInfo.SetBackgroundColour( ConsoleColour::RED );
    textBlock.Centre = charInfo;

    textBlock.Pos = { 1, 1 };
    textBlock.Size = { 9, 9 };

    buffer.DrawTextBlock( textBlock );

    buffer.UpdateBuffer();
}