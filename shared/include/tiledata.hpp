#pragma once

#include <json.hpp>

using json = nlohmann::json;

struct TileData
{
	bool isBlocked;
	int tile;
};


void to_json(json &j, const TileData &t)
{
	j = json
	{
		{
			"isBlocked", t.isBlocked
		},
		{
			"tile", t.tile
		}
	};
}

void from_json(json &j, TileData &t)
{
	j.at("isBlocked").get_to(t.isBlocked);
	j.at("tile").get_to(t.tile);
}
