#include <imgui.h>
#include <imgui-SFML.h>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/CircleShape.hpp>

#include <tilesetdata.hpp>
#include <datatypes.hpp>

// Win User for expanding the window at startup.
#include <windows.h>

#include <tilemapeditor.hpp>
#include <tileseteditor.hpp>

void EditorMenuBar();
void AboutWindow();

TileMapEditor tileMapEditor;
TileSetLibrary library;
TileSetEditor tileSetEditor(library);

int main()
{
	sf::RenderWindow window(sf::VideoMode(640, 480), "");
	ShowWindow(window.getSystemHandle(), 3);
	window.setVerticalSyncEnabled(true);
	ImGui::SFML::Init(window);

	char windowTitle[255] = "MiniMOG Editor";

	window.setTitle(windowTitle);
	window.resetGLStates(); // call it if you only draw ImGui. Otherwise not needed.
	sf::Clock deltaClock;

	WorldData worldData;
	bool t = true;
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			ImGui::SFML::ProcessEvent(event);

			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}

		ImGui::SFML::Update(window, deltaClock.restart());

		EditorMenuBar();
		AboutWindow();

		tileMapEditor.Render();
		tileSetEditor.Render();

		window.clear();
		ImGui::SFML::Render(window);
		window.display();
	}

	ImGui::SFML::Shutdown();
}

bool aboutWindowActive = false;

void EditorMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Window"))
		{
			if (ImGui::MenuItem("TileSet", "CTRL+T"))
			{
				tileSetEditor.isOpen = true;
			}

			if (ImGui::MenuItem("TileMap", "CTRL+M"))
			{
				tileMapEditor.isOpen = true;
			}

			if (ImGui::MenuItem("About", "CTRL+A"))
			{
				aboutWindowActive = true;
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

void AboutWindow()
{
	if (!aboutWindowActive)
		return;

	ImGui::Begin("About");
	std::string s = "SFML Version: " + std::to_string(SFML_VERSION_MAJOR) + "." + std::to_string(SFML_VERSION_MINOR) + "." + std::to_string(SFML_VERSION_PATCH);
	std::string b = ImGui::GetVersion();
	s += "ImGui Version:" + b;

	ImGui::Text(s.c_str());

	if (ImGui::Button("Okay"))
	{
		aboutWindowActive = false;
	}

	ImGui::End();
}