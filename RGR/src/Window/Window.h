#pragma once

#include "../Help.h"


class Window
{
private:
	GLFWwindow* mWindow;
	
public:
	
	Window(const std::string& title, int width, int height);
	~Window();

	void SetContextCurrent() const;

	void loop();
};
