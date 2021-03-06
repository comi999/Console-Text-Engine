#pragma once
#include <Windows.h>

enum class ConsoleColour : char
{
	BLACK = 0x0000,
	DARK_BLUE = 0x0001,
	DARK_GREEN = 0x0002,
	DARK_RED = 0x0004,
	DARK_GREY = 0x0008,
	DARK_CYAN = DARK_BLUE | DARK_GREEN,
	DARK_MAGENTA = DARK_BLUE | DARK_RED,
	DARK_YELLOW = DARK_GREEN | DARK_RED,
	BLUE = DARK_GREY | DARK_BLUE,
	GREEN = DARK_GREY | DARK_GREEN,
	RED = DARK_GREY | DARK_RED,
	GREY = DARK_BLUE | DARK_GREEN | DARK_RED,
	CYAN = DARK_GREY | DARK_CYAN,
	MAGENTA = DARK_GREY | DARK_MAGENTA,
	YELLOW = DARK_GREY | DARK_YELLOW,
	WHITE = DARK_GREY | DARK_BLUE | DARK_GREEN | DARK_RED
};

struct CharInfo : private CHAR_INFO
{
	CharInfo() : CHAR_INFO() { }
	inline CharInfo( const CHAR_INFO& a_CharInfo ) { *reinterpret_cast< CHAR_INFO* >( this ) = a_CharInfo; }
	inline CHAR& Ascii() { return Char.AsciiChar; }
	inline WCHAR& Unicode() { return Char.UnicodeChar; }
	inline WORD& Attributes() { return CHAR_INFO::Attributes; }
	inline ConsoleColour GetForegroundColour() { return static_cast< ConsoleColour >( 0xF & CHAR_INFO::Attributes ); }
	inline ConsoleColour GetBackgroundColour() { return static_cast< ConsoleColour >( ( 0xF0 & CHAR_INFO::Attributes ) >> 4 ); }
	inline void SetForegroundColour( ConsoleColour a_ConsoleColour ) 
	{
		CHAR_INFO::Attributes &= 0xFFF0;
		CHAR_INFO::Attributes |= static_cast< WORD >( a_ConsoleColour ); 
	}
	inline void SetBackgroundColour( ConsoleColour a_ConsoleColour ) 
	{
		CHAR_INFO::Attributes &= 0xFF0F;
		CHAR_INFO::Attributes |= static_cast< WORD >( a_ConsoleColour ) << 4; 
	}
	inline operator CHAR_INFO& ( ) { return *reinterpret_cast< CHAR_INFO* >( this ); }
};

struct Vector2Int
{
	int X, Y;
};

struct Vector2
{
	float X, Y;
};

typedef Vector2Int Offset;
typedef Vector2Int Size;
typedef Vector2 Pivot;
typedef Vector2 Anchor;

struct Rect
{
	Offset Offset;
	Size Size;

	int GetArea() const { return Size.X * Size.Y; }
	int GetMaxX() const { return Offset.X + Size.X - 1; }
	int GetMaxY() const { return Offset.Y + Size.Y - 1; }
};
