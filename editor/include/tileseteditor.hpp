#ifndef TILESETEDITOR_HPP
#define TILESETEDITOR_HPP

#include <tilesetdata.hpp>
#include <imgui.h>
#include <portable-file-dialogs.h>
#include <SFML\Graphics.hpp>

struct TileSetDataEditor
{
	int index = 0;
	sf::Texture image;
};


class TileSetEditor
{
public:
	TileSetEditor(TileSetLibrary& library) : library(library)
	{
		current = nullptr;
	}

	~TileSetEditor()
	{
		editorDataList.clear();
	}

	void Render()
	{
		if (!isOpen)
			return;

		ImGui::Begin("Tileset", &isOpen, ImGuiWindowFlags_MenuBar);

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Add Tileset", "Ctrl+O"))
				{
					auto selection = pfd::open_file("Select a file", ".",
						{ "Image Files", "*.png *.jpg *.jpeg *.bmp" },
						false).result();

					for (auto const &filename : selection)
					{
						TileSetDataEditor editorData;
						TileSetData data;
						data.fileName = filename;
						if (editorData.image.loadFromFile(data.fileName))
						{
							editorData.index = library.tileSetData.size();
							library.tileSetData.push_back(data);
							editorDataList.push_back(editorData);
						}
					}
				}

				if (ImGui::MenuItem("Close", "Ctrl+W")) { isOpen = false; }
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		ImGui::BeginChild("Scrolling");
		for (auto const &tsd : editorDataList)
		{
			if (ImGui::ImageButton(tsd.image, sf::Vector2f(100,100)))
			{

			}
		}

		ImGui::EndChild();
		ImGui::End();
	}

	bool isOpen;
private:
	TileSetLibrary& library;
	TileSetData* current;
	std::vector<TileSetDataEditor> editorDataList;
};

#endif // !TILEMAPEDITOR_HPP
