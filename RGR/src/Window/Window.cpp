#include "Window.h"

#include "../Tests/Test.h"
#include "../Tests/Test Menu/TestMenu.h"
#include "../Tests/Textured Cube/TestTexturedCube.h"
#include "../Tests/Clear Color/TestClearColor.h"


#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"


#include "../Camera/Camera.h"
#include "imgui/imgui_impl_opengl3.h"


//TODO: FIX this sh**
// Camera
static Camera camera;
static bool keys[1024];
static GLfloat lastX = 400;
static GLfloat lastY = 300;

static bool firstMouse = true;
static bool pause = false;

Window::Window(const std::string& title, int width, int height)
	: m_Width(width), m_Height(height), deltaTime(0), lastFrame(0)
{
	m_Window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

	if (!m_Window)
		throw std::runtime_error("Could not initialize window");

	SetContextCurrent();

	glfwSetKeyCallback(m_Window, key_callback);
	glfwSetCursorPosCallback(m_Window, mouse_callback);
	glfwSetScrollCallback(m_Window, scroll_callback);

	glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
		throw std::runtime_error("Could not initialize GLEW");

	GLCall(glEnable(GL_DEPTH_TEST));
	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	//ImGui initialization
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
	ImGui_ImplOpenGL3_Init("#version 330 core");
	ImGui::StyleColorsDark();
}

Window::~Window()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(m_Window);
}

void Window::SetContextCurrent() const
{
	glfwMakeContextCurrent(m_Window);
}

void Window::loop()
{
	Test::Test* CurrentTest = nullptr;
	Test::TestMenu* TestMenu = new Test::TestMenu(CurrentTest);
	CurrentTest = TestMenu;

	TestMenu->RegisterTest<Test::TestClearColor>("Clear Color");
	//TestMenu->RegisterTest<Test::TestTexturedCube>("Textured Cube");

	CurrentTest = new Test::TestTexturedCube(m_Width, m_Height, camera);


	while (!glfwWindowShouldClose(m_Window))
	{
		GLfloat CurrentFrame = glfwGetTime();
		deltaTime = CurrentFrame - lastFrame;
		lastFrame = CurrentFrame;

		if (CurrentTest)
		{
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			CurrentTest->OnUpdate(deltaTime);
			CurrentTest->OnRender();

			ImGui::Begin("Test");

			if (CurrentTest != TestMenu)
			{
				if (ImGui::Button("<-"))
				{
					delete CurrentTest;
					CurrentTest = TestMenu;
				}

				if (!pause)
				{
					do_movement();
				}
			}
			


			CurrentTest->OnImGuiRender();
			ImGui::End();

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		glfwSwapBuffers(m_Window);
		glfwPollEvents();
	}
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	//cout << key << endl;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
		{
			pause = true;
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
		else
		{
			pause = false;
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}

	}
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

void Window::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;  // Reversed since y-coordinates go from bottom to left

	lastX = xpos;
	lastY = ypos;

	if (!pause)
		camera.ProcessMouseMovement(xoffset, yoffset);

}

void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

void Window::do_movement() const
{
	// Camera controls
	if (keys[GLFW_KEY_W])
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		camera.ProcessKeyboard(RIGHT, deltaTime);
}
