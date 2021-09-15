#pragma once
#include <vector>
#include <map>

#include "RectTransform.hpp"
#include "crc32.hpp"

using namespace std;

class Scene
{
public:



private:

	vector< RectTransform > m_Nodes;
	map< crc32_t, uint32_t > m_NodeIndices;

};