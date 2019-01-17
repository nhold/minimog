#pragma once

#include <json.hpp>
#include <vector>
#include <string>
#include <tilemapdata.hpp>
#include <tilesetdata.hpp>

struct ObjectPlacementData
{
    int id;
    float x;
    float y;
};

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