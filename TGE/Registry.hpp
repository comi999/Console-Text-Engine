#pragma once
#include <vector>
#include <map>
#include <queue>
#include <typeinfo>

using namespace std;

template < typename Base >
class Registry
{
	static_assert( has_virtual_destructor_v< Base >, "Base is not a polymorphic type." );

private:

	template < typename Type >
	struct Node;

	struct INode
	{
		typedef bool( *IsBaseOf_T )( Base* );

		INode( IsBaseOf_T a_IsBaseOf, size_t a_HashCode, INode* a_Parent )
			: m_IsBaseOf( a_IsBaseOf ), m_HashCode( a_HashCode ), m_ValueCount( 0 ), m_Parent( a_Parent )
		{ }

		template < typename T >
		Node< T >* AsNode()
		{
			return static_cast< Node< T >* >( this );
		}

		IsBaseOf_T m_IsBaseOf;
		size_t m_HashCode;
		size_t m_ValueCount;
		INode* m_Parent;
		vector< INode* > m_Children;
	};

	template < typename Type >
	struct Node : public INode
	{
		Node( INode* a_Parent ) 
			: INode( IsBaseOf, typeid( Type ).hash_code(), a_Parent )
		{ }

		template < typename T >
		vector< T* > GetValuesAs() const
		{
			vector< T* > values;
			values.reserve( m_Values.size() );

			for ( auto& value : m_Values )
			{
				values.push_back( ( T* )&value );
			}

			return values;
		}

		static bool IsBaseOf( Base* a_Ptr )
		{
			return dynamic_cast< Type* >( a_Ptr );
		}

		vector< Type > m_Values;
	};

public:

	Registry()
		: m_Root( nullptr ), m_Count( 0 )
	{
		m_Nodes.emplace( make_pair( typeid( Base ).hash_code(), &m_Root ) );
	}

	~Registry()
	{
		for ( auto& node : m_Nodes )
		{
			delete node.second;
		}
	}
	
	template < typename T, typename... Args >
	typename enable_if< is_base_of_v< Base, T > && is_default_constructible_v< T >, T* >::
	type Emplace( size_t a_Index, Args... a_Args )
	{
		Node< T >* exact = nullptr;
		T* newObject = new T( a_Args... );
		auto iter = m_Nodes.find( typeid( T ).hash_code() );

		if ( iter != m_Nodes.end() )
		{
			exact = static_cast< Node< T >* >( iter->second );
		}
		else
		{
			queue< INode* > nodes;
			nodes.push( ( INode* )&m_Root );
			INode* closest = nullptr;

			do
			{
				INode* node = nodes.front();
				nodes.pop();

				if ( node->m_IsBaseOf( reinterpret_cast< Base* >( newObject ) ) )
				{
					closest = node;
					nodes = queue< INode* >();

					for ( auto& child : node->m_Children )
					{
						nodes.push( child );
					}
				}

			} while ( !nodes.empty() );

			exact = static_cast< Node< T >* >( m_Nodes.emplace( make_pair( typeid( T ).hash_code(), static_cast< INode* >( new Node< T >( closest ) ) ) ).first->second );
			list< INode* > peers;
			list< INode* > children;

			for ( auto& peer : closest->m_Children )
			{
				Base* value = &peer->AsNode< Base >()->m_Values.front();
				if ( !exact->m_IsBaseOf( value ) )
				{
					peers.push_back( peer );
				}
				else
				{
					children.push_back( peer );
					peer->m_Parent = exact;
				}
			}

			if ( peers.size() != closest->m_Children.size() )
			{
				exact->m_Children.reserve( children.size() );

				for ( auto& child : children )
				{
					exact->m_Children.push_back( child );
				}

				closest->m_Children = vector< INode* >();
				closest->m_Children.reserve( peers.size() + 1 );

				for ( auto& peer : peers )
				{
					closest->m_Children.push_back( peer );
				}
			}

			closest->m_Children.push_back( exact );
		}

		INode* parent = exact->m_Parent;
		while ( parent )
		{
			++parent->m_ValueCount;
			parent = parent->m_Parent;
		}

		auto where = exact->m_Values.begin();
		a_Index = a_Index > exact->m_Values.size() ? exact->m_Values.size() : a_Index;
		advance( where, a_Index );
		exact->m_Values.insert( where, *newObject );
		++exact->m_ValueCount;
		++m_Count;
		operator delete( newObject );
		return &exact->m_Values.at( a_Index );
	}

	template < typename T, typename... Args >
	typename enable_if< is_base_of_v< Base, T > && is_default_constructible_v< T >, T* >::
	type EmplaceFront( Args... a_Args )
	{
		Node< T >* exact = nullptr;
		T* newObject = new T( a_Args... );
		auto iter = m_Nodes.find( typeid( T ).hash_code() );

		if ( iter != m_Nodes.end() )
		{
			exact = static_cast< Node< T >* >( iter->second );
		}
		else
		{
			queue< INode* > nodes;
			nodes.push( ( INode* )&m_Root );
			INode* closest = nullptr;

			do
			{
				INode* node = nodes.front();
				nodes.pop();

				if ( node->m_IsBaseOf( reinterpret_cast< Base* >( newObject ) ) )
				{
					closest = node;
					nodes = queue< INode* >();

					for ( auto& child : node->m_Children )
					{
						nodes.push( child );
					}
				}

			} while ( !nodes.empty() );

			exact = static_cast< Node< T >* >( m_Nodes.emplace( make_pair( typeid( T ).hash_code(), static_cast< INode* >( new Node< T >( closest ) ) ) ).first->second );
			list< INode* > peers;
			list< INode* > children;
			
			for ( auto& peer : closest->m_Children )
			{
				Base* value = &reinterpret_cast< Node< Base >* >( peer )->m_Values.front();
				if ( !exact->m_IsBaseOf( value ) )
				{
					peers.push_back( peer );
				}
				else
				{
					children.push_back( peer );
					peer->m_Parent = exact;
				}
			}

			if ( peers.size() != closest->m_Children.size() )
			{
				exact->m_Children.reserve( children.size() );

				for ( auto& child : children )
				{
					exact->m_Children.push_back( child );
				}

				closest->m_Children = vector< INode* >();
				closest->m_Children.reserve( peers.size() + 1 );

				for ( auto& peer : peers )
				{
					closest->m_Children.push_back( peer );
				}
			}

			closest->m_Children.push_back( exact );
		}

		INode* parent = exact->m_Parent;
		while ( parent )
		{
			++parent->m_ValueCount;
			parent = parent->m_Parent;
		}

		auto where = exact->m_Values.insert( exact->m_Values.begin(), *newObject );
		++exact->m_ValueCount;
		++m_Count;
		operator delete( newObject );
		return &*where;
	}

	template < typename T, typename... Args >
	typename enable_if< is_base_of_v< Base, T > && is_default_constructible_v< T >, T* >::
	type EmplaceBack( const Args&... a_Args )
	{
		Node< T >* exact = nullptr;
		T* newObject = new T( a_Args... );
		auto iter = m_Nodes.find( typeid( T ).hash_code() );

		if ( iter != m_Nodes.end() )
		{
			exact = static_cast< Node< T >* >( iter->second );
		}
		else
		{
			queue< INode* > nodes;
			nodes.push( ( INode* )&m_Root );
			INode* closest = nullptr;

			do
			{
				INode* node = nodes.front();
				nodes.pop();

				if ( node->m_IsBaseOf( reinterpret_cast< Base* >( newObject ) ) )
				{
					closest = node;
					nodes = queue< INode* >();

					for ( auto& child : node->m_Children )
					{
						nodes.push( child );
					}
				}

			} while ( !nodes.empty() );

			exact = static_cast< Node< T >* >( m_Nodes.emplace( make_pair( typeid( T ).hash_code(), static_cast< INode* >( new Node< T >( closest ) ) ) ).first->second );
			list< INode* > peers;
			list< INode* > children;
			
			for ( auto& peer : closest->m_Children )
			{
				Base* value = &reinterpret_cast< Node< Base >* >( peer )->m_Values.front();
				if ( !exact->m_IsBaseOf( value ) )
				{
					peers.push_back( peer );
				}
				else
				{
					children.push_back( peer );
					peer->m_Parent = exact;
				}
			}

			if ( peers.size() != closest->m_Children.size() )
			{
				exact->m_Children.reserve( children.size() );

				for ( auto& child : children )
				{
					exact->m_Children.push_back( child );
				}

				closest->m_Children = vector< INode* >();
				closest->m_Children.reserve( peers.size() + 1 );

				for ( auto& peer : peers )
				{
					closest->m_Children.push_back( peer );
				}
			}

			closest->m_Children.push_back( exact );
		}

		INode* parent = exact->m_Parent;
		while ( parent )
		{
			++parent->m_ValueCount;
			parent = parent->m_Parent;
		}

		exact->m_Values.push_back( *newObject );
		++exact->m_ValueCount;
		++m_Count;
		operator delete( newObject );
		return &exact->m_Values.back();
	}

	template < typename T >
	typename enable_if< is_base_of_v< Base, T > && is_default_constructible_v< T >, T* >::
	type PushFront( const T& a_Object )
	{
		Node< T >* exact = nullptr;
		auto iter = m_Nodes.find( typeid( T ).hash_code() );

		if ( iter != m_Nodes.end() )
		{
			exact = static_cast< Node< T >* >( iter->second );
		}
		else
		{
			queue< INode* > nodes;
			nodes.push( ( INode* )&m_Root );
			INode* closest = nullptr;

			do
			{
				INode* node = nodes.front();
				nodes.pop();

				if ( node->m_IsBaseOf( reinterpret_cast< Base* >( &a_Object ) ) )
				{
					closest = node;
					nodes = queue< INode* >();

					for ( auto& child : node->m_Children )
					{
						nodes.push( child );
					}
				}

			} while ( !nodes.empty() );

			exact = static_cast< Node< T >* >( m_Nodes.emplace( make_pair( typeid( T ).hash_code(), static_cast< INode* >( new Node< T >( closest ) ) ) ).first->second );
			list< INode* > peers;
			list< INode* > children;

			for ( auto& peer : closest->m_Children )
			{
				Base* value = &reinterpret_cast< Node< Base >* >( peer )->m_Values.front();
				if ( !exact->m_IsBaseOf( value ) )
				{
					peers.push_back( peer );
				}
				else
				{
					children.push_back( peer );
					peer->m_Parent = exact;
				}
			}

			if ( peers.size() != closest->m_Children.size() )
			{
				exact->m_Children.reserve( children.size() );

				for ( auto& child : children )
				{
					exact->m_Children.push_back( child );
				}

				closest->m_Children = vector< INode* >();
				closest->m_Children.reserve( peers.size() + 1 );

				for ( auto& peer : peers )
				{
					closest->m_Children.push_back( peer );
				}
			}

			closest->m_Children.push_back( exact );
		}

		INode* parent = exact->m_Parent;
		while ( parent )
		{
			++parent->m_ValueCount;
			parent = parent->m_Parent;
		}

		exact->m_Values.insert( exact->m_Values.begin(), a_Object );
		++exact->m_ValueCount;
		++m_Count;
		return &exact->m_Values.front();
	}

	template < typename T >
	typename enable_if< is_base_of_v< Base, T > && is_default_constructible_v< T >, T* >::
	type PushBack( const T& a_Object )
	{
		Node< T >* exact = nullptr;
		auto iter = m_Nodes.find( typeid( T ).hash_code() );

		if ( iter != m_Nodes.end() )
		{
			exact = static_cast< Node< T >* >( iter->second );
		}
		else
		{
			queue< INode* > nodes;
			nodes.push( ( INode* )&m_Root );
			INode* closest = nullptr;

			do
			{
				INode* node = nodes.front();
				nodes.pop();

				if ( node->m_IsBaseOf( reinterpret_cast< Base* >( &a_Object ) ) )
				{
					closest = node;
					nodes = queue< INode* >();

					for ( auto& child : node->m_Children )
					{
						nodes.push( child );
					}
				}

			} while ( !nodes.empty() );

			exact = static_cast< Node< T >* >( m_Nodes.emplace( make_pair( typeid( T ).hash_code(), static_cast< INode* >( new Node< T >( closest ) ) ) ).first->second );
			list< INode* > peers;
			list< INode* > children;

			for ( auto& peer : closest->m_Children )
			{
				Base* value = &reinterpret_cast< Node< Base >* >( peer )->m_Values.front();
				if ( !exact->m_IsBaseOf( value ) )
				{
					peers.push_back( peer );
				}
				else
				{
					children.push_back( peer );
					peer->m_Parent = exact;
				}
			}

			if ( peers.size() != closest->m_Children.size() )
			{
				exact->m_Children.reserve( children.size() );

				for ( auto& child : children )
				{
					exact->m_Children.push_back( child );
				}

				closest->m_Children = vector< INode* >();
				closest->m_Children.reserve( peers.size() + 1 );

				for ( auto& peer : peers )
				{
					closest->m_Children.push_back( peer );
				}
			}

			closest->m_Children.push_back( exact );
		}

		INode* parent = exact->m_Parent;
		while ( parent )
		{
			++parent->m_ValueCount;
			parent = parent->m_Parent;
		}

		exact->m_Values.insert( exact->m_Values.end(), a_Object );
		++exact->m_ValueCount;
		++m_Count;
		return &exact->m_Values.back();
	}

	template < typename T >
	typename enable_if< is_base_of_v< Base, T > && is_default_constructible_v< T >, bool >::
	type PopFront()
	{

	}
	
	template < typename T >
	typename enable_if< is_base_of_v< Base, T > && is_default_constructible_v< T >, bool >::
	type PopBack()
	{

	}

	template < typename T >
	typename enable_if< is_base_of_v< Base, T > && is_default_constructible_v< T >, T* >::
	type Insert( const T& a_Object, size_t a_Index )
	{
		Node< T >* exact = nullptr;
		auto iter = m_Nodes.find( typeid( T ).hash_code() );

		if ( iter != m_Nodes.end() )
		{
			exact = static_cast< Node< T >* >( iter->second );
		}
		else
		{
			queue< INode* > nodes;
			nodes.push( ( INode* )&m_Root );
			INode* closest = nullptr;

			do
			{
				INode* node = nodes.front();
				nodes.pop();

				if ( node->m_IsBaseOf( reinterpret_cast< Base* >( &a_Object ) ) )
				{
					closest = node;
					nodes = queue< INode* >();

					for ( auto& child : node->m_Children )
					{
						nodes.push( child );
					}
				}

			} while ( !nodes.empty() );

			exact = static_cast< Node< T >* >( m_Nodes.emplace( make_pair( typeid( T ).hash_code(), static_cast< INode* >( new Node< T >( closest ) ) ) ).first->second );
			list< INode* > peers;
			list< INode* > children;

			for ( auto& peer : closest->m_Children )
			{
				Base* value = &reinterpret_cast< Node< Base >* >( peer )->m_Values.front();
				if ( !exact->m_IsBaseOf( value ) )
				{
					peers.push_back( peer );
				}
				else
				{
					children.push_back( peer );
					peer->m_Parent = exact;
				}
			}

			if ( peers.size() != closest->m_Children.size() )
			{
				exact->m_Children.reserve( children.size() );

				for ( auto& child : children )
				{
					exact->m_Children.push_back( child );
				}

				closest->m_Children = vector< INode* >();
				closest->m_Children.reserve( peers.size() + 1 );

				for ( auto& peer : peers )
				{
					closest->m_Children.push_back( peer );
				}
			}

			closest->m_Children.push_back( exact );
		}

		INode* parent = exact->m_Parent;
		while ( parent )
		{
			++parent->m_ValueCount;
			parent = parent->m_Parent;
		}

		auto where = exact->m_Values.begin();
		a_Index = a_Index > exact->m_Values.size() ? exact->m_Values.size() : a_Index;
		advance( where, a_Index );
		exact->m_Values.insert( where, a_Object );
		++exact->m_ValueCount;
		++m_Count;
		return &exact->m_Values.at( a_Index );
	}

	template < typename T >
	typename enable_if< is_base_of_v< Base, T > && is_default_constructible_v< T >, T* >::
	type Front() const
	{
		auto iter = m_Nodes.find( typeid( T ).hash_code() );
		if ( iter != m_Nodes.end() )
		{
			return &iter->AsNode< T >()->m_Values.front();
		}

		return nullptr;
	}

	template < typename T >
	typename enable_if< is_base_of_v< Base, T > && is_default_constructible_v< T >, T* >::
	type Back() const
	{
		auto iter = m_Nodes.find( typeid( T ).hash_code() );
		if ( iter != m_Nodes.end() )
		{
			return &iter->AsNode< T >()->m_Values.back();
		}

		return nullptr;
	}

	template < typename T >
	typename enable_if< is_base_of_v< Base, T > && is_default_constructible_v< T >, T* >::
	type At( size_t a_Index ) const
	{
		auto iter = m_Nodes.find( typeid( T ).hash_code() );
		if ( iter != m_Nodes.end() )
		{
			Node< T >* node = iter->AsNode< T >();

			if ( a_Index >= node->m_Values.size() )
			{
				return &node->m_Values.at( a_Index );
			}
			else
			{
				return nullptr;
			}
		}

		return nullptr;
	}

	template < typename T >
	typename enable_if< is_base_of_v< Base, T > && is_default_constructible_v< T >, T* >::
	type FindOfType() const
	{
		auto iter = m_Nodes.find( typeid( T ).hash_code() );

		if ( iter != m_Nodes.end() )
		{
			return &iter->second->AsNode< T >()->m_Values.front();
		}
		
		T newObject = T();
		queue< INode* > nodes;
		nodes.push( ( INode* )&m_Root );
		list< INode* > children;

		do
		{
			INode* node = nodes.front();
			nodes.pop();

			for ( auto& child : node->m_Children )
			{
				nodes.push( child );

				Base* value = &child->AsNode< Base >()->m_Values.front();
				if ( dynamic_cast< T* >( value ) )
				{
					children.push_back( child );
					break;
				}
			}

			if ( children.size() > 0 )
			{
				break;
			}

		} while ( !nodes.empty() );

		if ( children.size() > 0 )
		{
			return &children.front()->AsNode< T >()->m_Values.front();
		}
		
		return nullptr;
	}

	template < typename T >
	typename enable_if< is_base_of_v< Base, T > && is_default_constructible_v< T >, vector< T* > >::
	type FindAllOfType() const
	{
		queue< INode* > nodes;
		size_t foundCount = 0;
		auto iter = m_Nodes.find( typeid( T ).hash_code() );

		if ( iter != m_Nodes.end() )
		{
			nodes.push( iter->second );
			foundCount += iter->second->m_ValueCount;
		}
		else
		{
			T newObject = T();
			nodes.push( ( INode* )&m_Root );
			list< INode* > children;

			do
			{
				INode* node = nodes.front();
				nodes.pop();

				for ( auto& child : node->m_Children )
				{
					nodes.push( child );

					Base* value = &child->AsNode< Base >()->m_Values.front();
					if ( dynamic_cast< T* >( value ) )
					{
						children.push_back( child );
						foundCount += child->m_ValueCount;
					}
				}

				if ( children.size() > 0 )
				{
					break;
				}

			} while ( !nodes.empty() );

			for ( auto& child : children )
			{
				nodes.push( child );
			}
		}

		if ( foundCount > 0 )
		{
			vector< T* > found;
			found.reserve( foundCount );

			do
			{
				INode* node = nodes.front();
				nodes.pop();
				vector< T* >&& values = node->AsNode< T >()->GetValuesAs< T >();

				for ( T* ptr : values )
				{
					found.push_back( ptr );
				}

				for ( auto& child : node->m_Children )
				{
					nodes.push( child );
				}

			} while ( !nodes.empty() );

			return found;
		}

		return vector< T* >();
	}

	template < typename T >
	typename enable_if< is_base_of_v< Base, T > && is_default_constructible_v< T >, vector< T* > >::
	type FindAllOfExactType() const
	{
		auto iter = m_Nodes.find( typeid( T ).hash_code() );

		if ( iter != m_Nodes.end() )
		{
			return iter->second->AsNode< T >()->GetValuesAs< T >();
		}

		return vector< T* >();
	}

	template < typename T >
	typename enable_if< is_base_of_v< Base, T > && is_default_constructible_v< T >, bool >::
	type DestroyAllOfType()
	{

	}

	template < typename T >
	typename enable_if< is_base_of_v< Base, T > && is_default_constructible_v< T >, bool >::
	type DestroyAllOfExactType()
	{

	}

	bool Clear()
	{
		if ( !m_Count )
		{
			return false;
		}

		for ( auto& node : m_Nodes )
		{
			delete node->second;
		}

		m_Nodes.clear();
		m_Count = 0;
		return true;
	}

	template < typename T >
	typename enable_if< is_base_of_v< Base, T > && is_default_constructible_v< T >, bool >::
	type ClearAllOfType()
	{

	}

	template < typename T >
	typename enable_if< is_base_of_v< Base, T > && is_default_constructible_v< T >, bool >::
	type ClearAllOfExactType()
	{

	}

	bool Empty() const
	{
		return !m_Count;
	}

	size_t Count() const
	{
		return m_Count;
	}

	template < typename T >
	typename enable_if< is_base_of_v< Base, T > && is_default_constructible_v< T >, size_t >::
	type CountAllOfType() const
	{
		auto iter = m_Nodes.find( typeid( T ).hash_code() );

		if ( iter != m_Nodes.end() )
		{
			return iter->second->m_ValueCount;
		}
		else
		{
			T newObject = T();
			queue< INode* > nodes;
			nodes.push( ( INode* )&m_Root );
			size_t foundCount = 0;

			do
			{
				INode* node = nodes.front();
				nodes.pop();

				for ( auto& child : node->m_Children )
				{
					nodes.push( child );

					Base* value = &child->AsNode< Base >()->m_Values.front();
					if ( dynamic_cast< T* >( value ) )
					{
						foundCount += child->m_ValueCount;
					}
				}

				if ( foundCount > 0 )
				{
					return foundCount;
				}

			} while ( !nodes.empty() );

			return foundCount;
		}
	}

	template < typename T >
	typename enable_if< is_base_of_v< Base, T > && is_default_constructible_v< T >, size_t >::
	type CountAllOfExactType() const
	{
		auto iter = m_Nodes.find( typeid( T ).hash_code() );

		if ( iter != m_Nodes.end() )
		{
			return iter->second->AsNode< T >()->m_Values.size();
		}

		return 0;
	}

private:

	Node< Base > m_Root;
	map< size_t, INode* > m_Nodes;
	size_t m_Count;

};