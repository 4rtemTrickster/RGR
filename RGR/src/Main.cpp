#include "Help.h"

#include "GLFW/GLFW.h"
#include "Window/Window.h"

int main(void)
{
	try
	{
		GLFW glfw;

		Window window("RGR", 640, 480);
		window.SetContextCurrent();
		window.loop();
 
	}
	catch (std::runtime_error& e)
	{
		std::cout << "EXCEPTION::" << e.what() << std::endl;
		return -1;
	}

    return 0;
}
