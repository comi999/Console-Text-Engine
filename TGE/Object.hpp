#pragma once
#include <vector>

#include "Name.hpp"
#include "RectTransform.hpp"
#include "Component.hpp"

using namespace std;

class Object
{
public:

	const Name& GetName() const;
	RectTransform& GetRectTransform() const;

	template < typename T >
	typename enable_if< is_base_of_v< Component, T >, T* >::
		type AddComponent()
	{

	}

	template < typename T >
	typename enable_if< is_base_of_v< Component, T >, T* >::
		type AddComponent( const T& a_Component )
	{

	}

	template < typename T >
	typename enable_if< is_base_of_v< Component, T >, T* >::
		type GetComponent()
	{

	}

	template < typename T >
	typename enable_if< is_base_of_v< Component, T >, T* >::
		type GetComponentInChild()
	{

	}

	template < typename T >
	typename enable_if< is_base_of_v< Component, T >, vector< T* > >::
		type GetComponents()
	{

	}

	template < typename T >
	typename enable_if< is_base_of_v< Component, T >, vector< T* > >::
		type GetComponentsInChildren()
	{

	}

	template < typename T >
	typename enable_if< is_base_of_v< Component, T >, bool >::
		type TryGetComponent( T*& o_Component )
	{

	}

	template < typename T >
	typename enable_if< is_base_of_v< Component, T >, bool >::
		type HasComponent()
	{

	}

	template < typename T >
	typename enable_if< is_base_of_v< Component, T >, bool >::
		type DestroyComponent()
	{

	}

private:

	Name m_Name;
	uint32_t m_Id;

};