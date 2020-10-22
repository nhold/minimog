#pragma once

#include <json.hpp>
#include <vector>
#include <string>
#include <tilemapdata.hpp>
#include <tilesetdata.hpp>
#include <Meta.h>

struct ObjectPlacementData
{
    int id;
    float x;
    float y;
};

namespace meta
{
	template<>
	inline auto registerMembers<ObjectPlacementData>()
	{
		return members(
		member("id", &ObjectPlacementData::id),
		member("x", &ObjectPlacementData::x),
		member("y", &ObjectPlacementData::y));
	}
}

struct RegionData
{
	std::string name;
	float minX;
	float minY;
	float maxX;
	float maxY;
};

struct WorldData
{
    TileMapData tileMapData;
	std::vector<ObjectPlacementData> objects;
	std::vector<RegionData> regions;
	std::vector<TileSetData> tileSets;
	int version;
};