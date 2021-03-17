#pragma once

#include "../Help.h"


class Window
{
private:
	GLFWwindow* mWindow;
	int m_Width;
	int m_Height;
	
public:
	
	Window(const std::string& title, int width, int height);
	~Window();

	void SetContextCurrent() const;

	void loop();
};
