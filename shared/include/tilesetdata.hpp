#pragma once

#include <string>
#include <vector>

struct TileSetData
{
    std::string fileName;
    int tileWidth;
    int tileHeight;
};

class TileSetLibrary
{
public:
	std::vector<TileSetData> tileSetData;
};