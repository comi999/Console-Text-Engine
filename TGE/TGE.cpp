#include <iostream>
#include <thread>

#include "Buffer.hpp"
#include "Registry.hpp"

struct Component
{
    virtual ~Component() { }
};

struct Graphic abstract : Component { };
struct Sprite        : Graphic { };
struct Shape         : Graphic { };
struct TextBox       : Graphic { };
struct Transform     : Component { };
struct RectTransform : Transform { };
struct Region        : Component { };
struct CollisionBox  : Region { };
struct Raycastable   : CollisionBox { };
struct Interactable  : Raycastable { };
struct Button        : Interactable { };
struct Rigidbody     : Region { };


int main()
{

















    Buffer buffer( 50, 50, 8, 12 );

    CharInfo charInfo;
    charInfo.Ascii() = 'X';
    charInfo.SetBackgroundColour( ConsoleColour::GREEN );
    charInfo.SetForegroundColour( ConsoleColour::RED );

    TextBlock textBlock;
    textBlock.BorderWidth = 1;
    textBlock.Border = charInfo;
    charInfo.Ascii() = ' ';
    charInfo.SetBackgroundColour( ConsoleColour::WHITE );
    charInfo.SetForegroundColour( ConsoleColour::BLACK );
    textBlock.Centre = charInfo;
    textBlock.Text = "Hi, my name is Len and it is nice to meet you!!\nWhat is your name?\nActually, don't tell me I don't wanna know lol?\nI am selling these items:\nApple - $2\nPear - $3";
    textBlock.Formatting.HorizontalTextAllignment = HorizontalTextAllignment::Left;
    textBlock.Formatting.VerticalTextAllignment = VerticalTextAllignment::Top;
    textBlock.Formatting.HorizontalWrapMode = HorizontalWrapMode::Overflow;
    textBlock.Offset = { 1, 1 };
    
    for ( int i = 0; i < 28; ++i )
    {
        buffer.ResetBuffer();

        textBlock.Size = { 30 - i, 10 };

        buffer.DrawTextBlock( textBlock );

        buffer.UpdateBuffer();

        Sleep( 100 );
    }
}

// Functions: Buffer
//    - SetRectLine.
//    - SetRect
//    - SetChar
//    - SetLine

// Utils:
//    - Rect:
//      + Specifies origin and size.
//    - CharInfo:
//      + Defines a character and character attributes.
//    - Formatter:
//      + Will apply formatting options to a string to fit it into a rect.
//      + Options include:
//        = Horizontal allignment: Left, CentreLeft, CentreRight, Right.
//        = Vertical allignment: Top, CentreTop, CentreBottom, Bottom.
//        = Horizontal Wrap Mode: Extend, truncate, Wrap.
//        = Vertical Wrap Mode: Extend, truncate.
//        = Split words on Wrap
//        = Consume whitespaces.
//    - InputManager:
//      + Will manage all keyboard input.
//      + Will manage all mouse input.

// Component:
//    - RectTransform = an origin and size delta.
//      + Stores a Size, AnchoredPosition, and PivotPosition. Also stored a reference to a parent transform
//
//    - RectOutline = draws a border of set thickness inside of rect.
//
//    - RectFill = fills a rect in with certain character
//
//    - TextBlock = fits a string into a rectangle
//      + Can set horizontal and vertical allignment
//      + Can set horizontal and vertical wrap modes for text
//      + Can enable split words on wrap
//      + Can enable end space consumption
//
//    - TextGrid = displays list of string_view's in a scrollable list
//      + Has AnchoredPosition that is relative to RectTransforms pivot position.
//      + Can set cell size.
//      + Can set row and column count.
//      + Stores formatter that controls word layout in each cell.
//      + Will only display what's inside of rectTransform.
//
//    - Raycastable = a Rect that tracks whether or not a mouse is inside of it. Uses RectTransform.
//      + Has OnPointerEnter, OnPointerExit, OnPointerStay events.
//
//    - Clickable = A version of Raycastable that is clickable.
//      + Has OnPointerPressed, OnPointerClicked, OnPointerReleased, OnPointerDown
//
//    - TextReader = Reads input keys and sends to string.