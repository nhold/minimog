#pragma once

#include <json.hpp>
#include <tiledata.hpp>

using json = nlohmann::json;

struct TileMapData
{
	TileData* data;
    int tilesetID;
    int width;
    int height;
};

void to_json(json &j, const TileMapData &t)
{
	j = json
	{
		{
			"width", t.width
		},
		{
			"height", t.height
		},
		{
			"tilesetID", t.tilesetID
		},
	};

	auto data = json::array();

	for (int index = 0; index < t.width * t.height; index++)
	{
		json j = t.data[index];
		data[index] = j;
	}

	j["data"] = data;
}

void from_json(json &j, TileMapData& t) 
{
	t.width = j["width"];
	t.height = j["width"];
	t.tilesetID = j["tilesetID"];
	t.data = new TileData[t.width*t.height];

	for (int index = 0; index < t.width * t.height; index++)
	{
		auto data = j["data"];
		json jsonObj = data[index];
		from_json(jsonObj, t.data[index]);
	}
}