#include <string_view>

#include "Buffer.hpp"

using namespace std;

Buffer::Buffer( int a_Width, int a_Height, int a_PixelWidth, int a_PixelHeight )
    : m_PixelWidth( a_PixelWidth ), m_PixelHeight( a_PixelHeight )
{
    m_Handle = GetStdHandle( STD_OUTPUT_HANDLE );
    HWND consoleHwnd = GetConsoleWindow();
    CONSOLE_FONT_INFOEX consoleFontInfo;
    consoleFontInfo.cbSize = sizeof( CONSOLE_FONT_INFOEX );
    consoleFontInfo.nFont = 0;
    consoleFontInfo.dwFontSize.X = m_PixelWidth;
    consoleFontInfo.dwFontSize.Y = m_PixelHeight;
    consoleFontInfo.FontFamily = FF_DONTCARE;
    consoleFontInfo.FontWeight = FW_NORMAL;
    wcscpy_s( consoleFontInfo.FaceName, L"Terminal" );
    SetCurrentConsoleFontEx( m_Handle, 0, &consoleFontInfo );
    COORD largestWindow = GetLargestConsoleWindowSize( m_Handle );
    m_Width  = min( largestWindow.X, ( short )a_Width  );
    m_Height = min( largestWindow.Y, ( short )a_Height );
    SetConsoleScreenBufferSize( m_Handle, { ( short )m_Width, ( short )m_Height } );
    SMALL_RECT windowArea = { 0, 0, ( short )m_Width - 1, ( short )m_Height - 1 };
    SetConsoleWindowInfo( m_Handle, TRUE, &windowArea );
    CONSOLE_SCREEN_BUFFER_INFOEX screenBufferInfo = CONSOLE_SCREEN_BUFFER_INFOEX();
    GetConsoleScreenBufferInfoEx( m_Handle, &screenBufferInfo );
    SetConsoleScreenBufferSize( m_Handle, { 
        ( SHORT )( screenBufferInfo.srWindow.Right - screenBufferInfo.srWindow.Left + 1 ), 
        ( SHORT )( screenBufferInfo.srWindow.Bottom - screenBufferInfo.srWindow.Top + 1 ) } );
    SetWindowLongW( consoleHwnd, GWL_STYLE, WS_CAPTION | DS_MODALFRAME | WS_MINIMIZEBOX | WS_SYSMENU );
    SetWindowPos( consoleHwnd, NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_SHOWWINDOW );
    int size = m_Width * m_Height;
    m_Buffer = new CharInfo[ size ];
    m_WriteRegion = new SMALL_RECT{ 0, 0, ( short )m_Width - 1, ( short )m_Height - 1 };
    ResetBuffer();
}

Buffer::~Buffer()
{
    delete[] m_Buffer;
}

void Buffer::ResetBuffer()
{
    CharInfo blank;
    blank.Ascii() = ' ';
    blank.SetBackgroundColour( ConsoleColour::BLACK );
    blank.SetForegroundColour( ConsoleColour::BLACK );
    SetBuffer( blank );
}

void Buffer::SetBuffer( CharInfo a_CharInfo )
{
    int size = 4 * m_Width * m_Height;
    memset( m_Buffer, 0, size );
}

void Buffer::SetCharacter( CharInfo a_CharInfo, Vec2 a_Position )
{
    m_Buffer[ m_Width * a_Position.Y + a_Position.X ] = a_CharInfo;
}

void Buffer::SetLine( const char* a_String, size_t a_Count, Vec2 a_Postion )
{
    if ( a_Count < 0 )
    {
        a_Count = strlen( a_String );
    }

    CharInfo* begin = m_Buffer + static_cast< size_t >( m_Width ) * a_Postion.Y + a_Postion.X;
    //auto iter = 
}

void Buffer::SetLine( const char* a_String, size_t a_Count, Vec2 a_Postion, CharInfo a_CharInfo )
{

}

void Buffer::DrawTextBlock( const TextBlock& a_TextBlock )
{
    if ( a_TextBlock.BorderWidth > 0 )
    {
        CharInfo* top = m_Buffer + static_cast< size_t >( m_Width ) * a_TextBlock.Pos.Y + a_TextBlock.Pos.X;
        CharInfo* bottom = top + m_Width * ( static_cast< size_t >( a_TextBlock.Size.Y ) - a_TextBlock.BorderWidth );

        for ( int i = 0; i < a_TextBlock.BorderWidth; ++i )
        {
            for ( int j = 0; j < a_TextBlock.Size.X; ++j )
            {
                *( top + j ) = a_TextBlock.Border;
                *( bottom + j ) = a_TextBlock.Border;
            }

            top += m_Width;
            bottom += m_Width;
        }

        bottom = top + a_TextBlock.Size.X - a_TextBlock.BorderWidth;
        
        for ( int i = 0; i < a_TextBlock.Size.Y - 2 * a_TextBlock.BorderWidth; ++i )
        {
            for ( int j = 0; j < a_TextBlock.BorderWidth; ++j )
            {
                *( top + j ) = a_TextBlock.Border;
                *( bottom + j ) = a_TextBlock.Border;
            }

            top += m_Width;
            bottom += m_Width;
        }
    }

    auto iter = a_TextBlock.Text.begin();

    while ( iter != a_TextBlock.Text.end() )
    {

    }
}

void Buffer::UpdateBuffer()
{
    WriteConsoleOutput( m_Handle, reinterpret_cast< CHAR_INFO* >( m_Buffer ), { ( short )m_Width, ( short )m_Height }, { 0, 0 }, m_WriteRegion );
}

void Buffer::UpdateBuffer( const Rect& a_Rect )
{
    SMALL_RECT rect = { a_Rect.Pos.X, a_Rect.Pos.Y, a_Rect.GetMaxX(), a_Rect.GetMaxY() };
    WriteConsoleOutput( m_Handle, reinterpret_cast< CHAR_INFO* >( m_Buffer ), { ( short )m_Width, ( short )m_Height }, { 0, 0 }, &rect );
}
