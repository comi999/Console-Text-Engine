#pragma once
#include <string>

#include "crc32.hpp"

using namespace std;

struct Name
{
public:

    Name() : m_String( nullptr ), m_Length( 0 ), m_Hash( 0 )
    {
    }

    template < size_t N >
    constexpr Name( const char( &C )[ N ] )
        : m_String( C ), m_Length( N - 1 ), m_Hash( crc32_cpt< N >( C ) )
    {
    }

    constexpr Name( const char* C, size_t N, crc32_t H )
        : m_String( C ), m_Length( int32_t( N ) ), m_Hash( H )
    {
    }

    Name( char* a_String )
        : m_String( a_String ), m_Length( ( int32_t( strlen( a_String ) ) ) ),
        m_Hash( crc32_impl< sizeof( const char* ) >( a_String ) )
    {
    }

    Name( char* a_String, size_t a_Length )
        : m_String( a_String ), m_Length( int32_t( a_Length ) ), m_Hash( crc32_impl< sizeof( const char* ) >( a_String ) )
    {
    }

    Name( const string& a_String )
        : m_String( a_String.c_str() ), m_Length( int32_t( a_String.length() ) ),
        m_Hash( crc32_impl< sizeof( const char* ) >( m_String ) )
    {
    }

    operator string() const
    {
        return string( m_String, m_Length );
    }
    operator crc32_t() const
    {
        return m_Hash;
    }
    operator const char* ( ) const
    {
        return m_String;
    }
    constexpr char operator []( size_t a_Pos )
    {
        return m_String[ a_Pos ];
    }

    constexpr const char* Data() const
    {
        return m_String;
    }
    constexpr int32_t Length() const
    {
        return m_Length;
    }
    constexpr crc32_t Hash() const
    {
        return m_Hash;
    }

private:

    const char* m_String;
    int32_t m_Length;
    crc32_t m_Hash;
};

constexpr Name
operator "" _N( const char* C, size_t N )
{
    return Name( C, N, crc32_impl2( C, N - 1 ) ^ 0xFFFFFFFF );
}