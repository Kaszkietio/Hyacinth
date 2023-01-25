#include <hyacinth/include.h>
#include <iostream>
#include <hyacinth/window.h>



int main()
{
	std::cout << "Hello world" << std::endl;

	Hyacinth::Window* window = Hyacinth::Window::GetInstance();
	window->Run();
	delete window;
	return 0;
}