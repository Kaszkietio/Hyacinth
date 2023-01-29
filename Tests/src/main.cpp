#include <filesystem>
#include <Hyacinth/ObjParser.h>
#include <Hyacinth/Scene.h>

int main()
{
	std::cout << std::filesystem::current_path().string() << std::endl;
	std::string path = "..\\Resources\\";
	Hyacinth::ObjParser parser;

	parser.Parse(path + "triangle.obj", "yayks");

	Hyacinth::Scene dupa;


	return 0;
}