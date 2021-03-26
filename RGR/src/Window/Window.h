#pragma once

#include "../Help.h"
#include "../Camera/Camera.h"


class Window
{
private:
	GLFWwindow* m_Window;
	int m_Width;
	int m_Height;

	// Deltatime
	GLfloat deltaTime;	// Time between current frame and last frame
	GLfloat lastFrame;  	// Time of last frame

public:

	Camera& m_Camera;
	void do_movement() const;
	
	Window(const std::string& title, int width, int height);
	~Window();

	void SetContextCurrent() const;

	void loop();

	inline int GetWindowWidth() const { return m_Width; }
	inline int GetWindowHeight() const { return m_Height; }

private:
	// Is called whenever a key is pressed/released via GLFW
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
	
	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);

	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	
	
};
