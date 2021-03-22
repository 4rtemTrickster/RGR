#include "Window.h"

#include "../GL/VertexArray/VertexArray.h"
#include "../GL/IndexBuffer/IndexBuffer.h"
#include "../GL/VertexArray/VertexBufferLayout.h"
#include "../GL/VertexBuffer/VertexBuffer.h"
#include "../GL/Shader/Shader.h"
#include "../Renderer/Renderer.h"
#include "../Texture/Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"


#include "../Camera/Camera.h"
#include "imgui/imgui_impl_opengl3.h"


//TODO: FUCKING STATIC ya know
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

	glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
		throw std::runtime_error("Could not initialize GLEW");

	GLCall(glEnable(GL_DEPTH_TEST));

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
	Renderer renderer;

	std::vector<GLfloat> positions =
	{	//positions				//Texture coordinates
		//Front
		-0.5f, -0.5f,  0.5f,	0.0f, 0.0f,	//0				
		 0.5f, -0.5f,  0.5f,	1.0f, 0.0f, //1				
		 0.5f,  0.5f,  0.5f,	1.0f, 1.0f, //2				
		-0.5f,  0.5f,  0.5f,	0.0f, 1.0f, //3				
															
		//Right
		 0.5f, -0.5f,  0.5f,	0.0f, 0.0f, //4
		 0.5f, -0.5f, -0.5f,	1.0f, 0.0f, //5
		 0.5f,  0.5f, -0.5f,	1.0f, 1.0f, //6
		 0.5f,  0.5f,  0.5f,	0.0f, 1.0f, //7

		//Back
		 0.5f, -0.5f, -0.5f,	0.0f, 0.0f, //8
		-0.5f, -0.5f, -0.5f,	1.0f, 0.0f,	//9
		-0.5f,  0.5f, -0.5f,	1.0f, 1.0f,	//10
		 0.5f,  0.5f, -0.5f,	0.0f, 1.0f,	//11

		//Left
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,	//12
		-0.5f, -0.5f,  0.5f,	1.0f, 0.0f,	//13
		-0.5f,  0.5f,  0.5f,	1.0f, 1.0f, //14
		-0.5f,  0.5f, -0.5f,	0.0f, 1.0f,	//15

		//Bottom
		-0.5f, -0.5f,  0.5f,	0.0f, 1.0f,	//16
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,	//17
		 0.5f, -0.5f, -0.5f,	1.0f, 0.0f, //18
		 0.5f, -0.5f,  0.5f,	1.0f, 1.0f, //19

		//Top
		-0.5f,  0.5f,  0.5f,	0.0f, 0.0f, //20
		 0.5f,  0.5f,  0.5f,	1.0f, 0.0f, //21
		 0.5f,  0.5f, -0.5f,	1.0f, 1.0f, //22
		-0.5f,  0.5f, -0.5f,	0.0f, 1.0f,	//23
	};

	std::vector<GLuint> indices =
	{
		0,	1,	2,
		2,	3,	0,

		4,	5,	6,
		6,	7,	4,

		8,	9,	10,
		10,	11,	8,

		12, 13, 14,
		14, 15, 12,

		16, 17, 18,
		18, 19, 16,

		20, 21, 22,
		22, 23, 20
	};

	
	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));


	GL::VertexArray va;
	GL::VertexBuffer vb(positions.data(), sizeof(GLfloat) * positions.size());

	GL::VertexBufferLayout layout;
	layout.Push<GLfloat>(3);
	layout.Push<GLfloat>(2);

	va.AddBuffer(vb, layout);

	GL::IndexBuffer ib(indices.data(), indices.size());


	glm::mat4 model(1.0f);

	glm::mat4 view(1.0f);

	glm::mat4 proj(1.0f);
	proj = glm::perspective(glm::radians(45.0f), static_cast<float>(m_Width)/static_cast<float>(m_Height) , 0.1f, 100.0f);
	

	GL::Shader shader("res/Shaders/first.frag", "res/Shaders/first.vert");
	shader.Bind();
	shader.SetUniformMat4f("u_Projection", proj);
	shader.SetUniformMat4f("u_View", view);
	shader.SetUniformMat4f("u_Model", model);

	Texture texture("res/Textures/Texture2.png");
	texture.Bind();
	shader.SetUniform1i("u_Texture", 0);
	
	va.Bind();
	vb.Bind();
	ib.Bind();
	shader.Unbind();


	GLfloat r = 1.0f;

	

	while (!glfwWindowShouldClose(m_Window))
	{
		GLfloat CurrentFrame = glfwGetTime();
		deltaTime = CurrentFrame - lastFrame;
		lastFrame = CurrentFrame;
		
		renderer.Clear();
		
		glfwPollEvents();
	

		if (pause)
		{
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			
			ImGui::ShowDemoWindow();
		}
		else
			do_movement();

		shader.Bind();

		view = camera.GetViewMatrix();
		shader.SetUniformMat4f("u_View", view);

		//model = glm::rotate(model, glm::radians(0.5f), glm::vec3(1.0f, 1.0f, 1.0f));
		//shader.SetUniformMat4f("u_Model", model);

		renderer.Draw(va, ib, shader);

		if (pause)
		{
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
		//glfwSetWindowShouldClose(window, GL_TRUE);
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
