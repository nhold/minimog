#ifndef TILEMAPEDITOR_HPP
#define TILEMAPEDITOR_HPP

#include <tilemapdata.hpp>
#include <imgui.h>

class TileMapEditor
{
public:
	TileMapEditor() 
	{
	}

	~TileMapEditor() 
	{
	}

	void Render() 
	{
		if (!isOpen)
			return;

		ImGui::Begin("Tile Map Editor");


		if (ImGui::Button("Okay"))
		{
			isOpen = false;
		}

		ImGui::End();
	}

	bool isOpen;
private:
	TileMapData tileMapData;
	
};

#endif // !TILEMAPEDITOR_HPP
