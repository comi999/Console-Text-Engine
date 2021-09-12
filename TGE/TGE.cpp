#include <iostream>
#include "Buffer.hpp"

int main()
{
    Buffer buffer( 50, 50, 8, 12);

    CharInfo charInfo;
    charInfo.Ascii() = 'X';
    charInfo.SetBackgroundColour( ConsoleColour::GREEN );
    charInfo.SetForegroundColour( ConsoleColour::RED );

    TextBlock textBlock;
    textBlock.BorderWidth = 0;
    textBlock.Border = charInfo;
    charInfo.Ascii() = ' ';
    charInfo.SetBackgroundColour( ConsoleColour::WHITE );
    charInfo.SetForegroundColour( ConsoleColour::BLACK );
    textBlock.Centre = charInfo;
    textBlock.Text = "Hi, my name is Len and it is nice to meet you!!";
    textBlock.TextAllignment = TextAllignment::Right;

    textBlock.Pos = { 1, 1 };
    textBlock.Size = { 30, 10 };

    buffer.DrawTextBlock( textBlock );

    buffer.UpdateBuffer();
}