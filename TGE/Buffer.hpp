#pragma once
#include <Windows.h>
#include <list>
#include "Utilities.hpp"
#include "TextBlock.hpp"

class Buffer
{
public:

	Buffer( int a_Width, int a_Height, int a_PixelWidth, int a_PixelHeight );
	~Buffer();

	void ResetBuffer();
	void SetBuffer( CharInfo a_CharInfo );
	void UpdateBuffer();
	void UpdateBuffer( const Rect& a_Rect );
	void SetCharacter( CharInfo a_CharInfo, Vec2 a_Position );
	void SetLine( const char* a_String, size_t a_Count, Vec2 a_Postion );
	void SetLine( const char* a_String, size_t a_Count, Vec2 a_Postion, CharInfo a_CharInfo );
	void DrawTextBlock( const TextBlock& a_TextBlock );

private:

	int m_Width, m_Height;
	int m_PixelWidth, m_PixelHeight;
	CharInfo* m_Buffer;
	HANDLE m_Handle;
	PSMALL_RECT m_WriteRegion;
	list< TextBlock > m_TextBlocks;
};