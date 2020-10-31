#pragma once

#include <string>
#include <fstream>


class FileIO
{
public:
	static bool Exists(std::string fileName)
	{
		std::ifstream file(fileName, std::ios::in);

		if (file)
		{
			return true;
		}

		return false;
	}

	static std::string Read(std::string fileName)
	{
		std::ifstream file(fileName, std::ios::in);
		std::string contents;

		if (file)
		{
			file.seekg(0, std::ios::end);
			contents.resize(file.tellg());

			file.seekg(0, std::ios::beg);
			file.read(&contents[0], contents.size());
			file.close();
			return contents;
		}

		return contents;
	}

	static bool Write(std::string fileName, std::string text)
	{
		std::ofstream file(fileName, std::ios::out);

		if (file)
		{
			file.write(text.c_str(), text.size());
			file.close();
			return true;
		}

		return false;
	}

private:

};

template <class T>
T GetFileToObject(std::string filename)
{
	using json = nlohmann::json;

	json root = json::parse(FileIO::Read(filename));
	return root.get<T>();
}
