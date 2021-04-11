#include "Help.h"

#include "GLFW/GLFW.h"
#include "Window/Window.h"
#include "Logger/Log.h"

int main(void)
{
	try
	{
		GLFW glfw;
		Log::Init();

		Window window("RGR", 1280, 720);
		window.SetContextCurrent();
		window.loop();
	}
	catch (std::runtime_error& e)
	{
		//std::cout << "EXCEPTION::" << e.what() << std::endl;
		LOG_ERROR("{0}",e.what());
		return -1;
	}

    return 0;
}
