#pragma once
#include <vector>

using namespace std;

template < typename Base >
class Registry
{
private:

	struct INode {  };

	template < typename Type >
	struct Node : INode
	{
		struct _
		{
			virtual ~_();
		} m_;

		typedef bool( *CanCast )( void* );

		Node() 
			: m_Parent( nullptr ), m_CanCast( CanCast< Type > )
		{ }

		INode* m_Parent;
		vector< INode* > m_Children;
		vector< Type > m_Values;
		CanCast m_CanCast;

		template < typename Base >
		bool CanCast( void* a_Ptr )
		{
			return dynamic_cast< Base* >( a_Ptr );
		}

		template < typename Derived >
		static bool IsBaseOf()
		{
			Derived* derived = reinterpret_cast< Derived* >( m_ );
			return m_CanCast( derived );
		}
	};

public:

	Registry()
	{

	}

private:

	Node< Base > m_Root;

};