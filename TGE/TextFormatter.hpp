#pragma once
#include <string_view>
#include <list>

#include "Utilities.hpp"
#include "Buffer.hpp"

using namespace std;

enum class HorizontalTextAllignment : unsigned char
{
	Left, CentreLeft, CentreRight, Right
};

enum class VerticalTextAllignment : unsigned char
{
	Top, CentreTop, CentreBottom, Bottom
};

enum class HorizontalWrapMode : unsigned char
{
	Wrap,
	Truncate,
	Overflow
};

enum class VerticalWrapMode : unsigned char
{
	Truncate,
	Overflow
};

struct StringBundle : list< string_view >
{
	StringBundle() = default;
	StringBundle( const string& a_String )
	{
		emplace_back( a_String );
	}
};

class TextFormatter
{
private:

	TextFormatter()
		: HorizontalTextAllignment( HorizontalTextAllignment::Left ), 
		  VerticalTextAllignment  ( VerticalTextAllignment::Top    ), 
		  HorizontalWrapMode      ( HorizontalWrapMode::Wrap       ), 
		  VerticalWrapMode        ( VerticalWrapMode::Truncate     ) 
	{ }
	void Format( Vector2Int a_RectSize, StringBundle& o_StringBundle ) const
	{
		for ( auto iter = o_StringBundle.begin(); iter != o_StringBundle.end(); ++iter )
		{
			size_t index = iter->find_first_of( '\n' );

			if ( index == string_view::npos )
			{
				// Remove all whitespaces from start of string.
				if ( ( index = iter->find_first_not_of( ' ' ) ) != string_view::npos )
				{
					iter->remove_prefix( index );
				}

				// Remove all whitespaces from end of string.
				if ( ( index = iter->find_last_not_of( ' ' ) ) != string_view::npos )
				{
					iter->remove_suffix( iter->length() - index - 1 );
				}

				continue;
			}

			if ( index != iter->length() - 1 )
			{
				auto nextIter = iter;
				o_StringBundle.emplace( ++nextIter, iter->data() + index + 1, iter->length() - index - 1 );
			}

			iter->remove_suffix( iter->length() - index );

			// Remove all whitespaces from start of string.
			if ( ( index = iter->find_first_not_of( ' ' ) ) != string_view::npos )
			{
				iter->remove_prefix( index );
			}

			// Remove all whitespaces from end of string.
			if ( ( index = iter->find_last_not_of( ' ' ) ) != string_view::npos )
			{
				iter->remove_suffix( iter->length() - index - 1 );
			}
		}

		if ( HorizontalWrapMode != HorizontalWrapMode::Wrap )
		{
			if ( HorizontalWrapMode == HorizontalWrapMode::Truncate )
			{
				for ( auto iter = o_StringBundle.begin(); iter != o_StringBundle.end(); ++iter )
				{
					size_t index = 0;

					// Remove all whitespaces from start of string.
					if ( ( index = iter->find_first_not_of( ' ' ) ) != string_view::npos )
					{
						iter->remove_prefix( index );
					}

					// Remove all whitespaces from end of string.
					if ( ( index = iter->find_last_not_of( ' ' ) ) != string_view::npos )
					{
						iter->remove_suffix( iter->length() - index - 1 );
					}

					int offset = 0;

					switch ( HorizontalTextAllignment )
					{
					case HorizontalTextAllignment::CentreLeft:
					{
						offset = floorf( static_cast< int >( a_RectSize.X - iter->length() ) * 0.5f );
					}
					break;
					case HorizontalTextAllignment::CentreRight:
					{

						offset = ceilf( static_cast< int >( a_RectSize.X - iter->length() ) * 0.5f );
					}
					break;
					case HorizontalTextAllignment::Right:
					{
						offset = a_RectSize.X - iter->length();
					}
					break;
					}

					if ( offset < 0 )
					{
						iter->remove_prefix( -offset );
						offset = 0;
					}

					if ( iter->length() + offset > a_RectSize.X )
					{
						iter->remove_suffix( iter->length() + offset - a_RectSize.X );
					}

					// Remove all whitespaces from start of string.
					if ( ( index = iter->find_first_not_of( ' ' ) ) != string_view::npos )
					{
						iter->remove_prefix( index );
					}

					// Remove all whitespaces from end of string.
					if ( ( index = iter->find_last_not_of( ' ' ) ) != string_view::npos )
					{
						iter->remove_suffix( iter->length() - index - 1 );
					}
				}
			}

			return;
		}

		for ( auto iter = o_StringBundle.begin(); iter != o_StringBundle.end(); ++iter )
		{
			// Remove all whitespaces from start of string.
			size_t index = 0;

			if ( ( index = iter->find_first_not_of( ' ' ) ) != string_view::npos )
			{
				iter->remove_prefix( index );
			}

			// If string needs to be split.
			if ( iter->length() > a_RectSize.X )
			{
				auto nextIter = iter;

				// If split should occur wherever it's found.
				if ( SplitWordsOnWrap )
				{
					nextIter = o_StringBundle.emplace( ++nextIter, iter->data() + a_RectSize.X, iter->length() - a_RectSize.X );
					iter->remove_suffix( iter->length() - a_RectSize.X );
				}
				else
				{
					if ( ( index = iter->find_last_of( ' ', a_RectSize.X ) ) != string_view::npos )
					{
						nextIter = o_StringBundle.emplace( ++nextIter, iter->data() + index + 1, iter->length() - index - 1 );
						iter->remove_suffix( iter->length() - index );
					}
					else
					{
						iter->remove_suffix( iter->length() - a_RectSize.X );
					}
				}
			}

			// Remove all whitespaces from end of string.
			if ( ( index = iter->find_last_not_of( ' ' ) ) != string_view::npos )
			{
				iter->remove_suffix( iter->length() - index - 1 );
			}
		}

		if ( VerticalWrapMode == VerticalWrapMode::Truncate )
		{
			int offset = 0;

			switch ( VerticalTextAllignment )
			{
			case VerticalTextAllignment::CentreTop:
			{
				offset = floorf( static_cast< int >( a_RectSize.Y - o_StringBundle.size() ) * 0.5f );
			}
			break;
			case VerticalTextAllignment::CentreBottom:
			{
				offset = ceilf( static_cast< int >( a_RectSize.Y - o_StringBundle.size() ) * 0.5f );
			}
			break;
			case VerticalTextAllignment::Bottom:
			{
				offset = a_RectSize.Y - o_StringBundle.size();
			}
			break;
			}

			if ( offset < 0 )
			{
				while ( offset != 0 )
				{
					o_StringBundle.pop_front();
					++offset;
				}
			}

			while ( offset + o_StringBundle.size() > a_RectSize.Y )
			{
				o_StringBundle.pop_back();
			}
		}
	}

	friend class TextBlock;
	friend class Buffer;

public:

	HorizontalTextAllignment HorizontalTextAllignment : 3;
	VerticalTextAllignment VerticalTextAllignment : 3;
	HorizontalWrapMode HorizontalWrapMode : 2;
	VerticalWrapMode VerticalWrapMode : 1;
	bool SplitWordsOnWrap : 1;

};