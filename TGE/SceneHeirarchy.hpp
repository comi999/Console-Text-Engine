#pragma once
#include <vector>

#include "RectTransform.hpp"

using namespace std;

class SceneHeirarchy
{
public:

	struct Node
	{
		Node* m_Parent;
		vector< Node > m_Children;
	};

private:

	vector< RectTransform > m_Transforms;

};