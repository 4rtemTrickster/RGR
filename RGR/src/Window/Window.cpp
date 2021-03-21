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

Window::Window(const std::string& title, int width, int height)
	: m_Width(width), m_Height(height)
{
	mWindow = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

	if (!mWindow)
		throw std::runtime_error("Could not initialize window");

	SetContextCurrent();

	glewExperimental = true;
	if (glewInit() != GLEW_OK)
		throw std::runtime_error("Could not initialize GLEW");

	glEnable(GL_DEPTH_TEST);
}

Window::~Window()
{
	glfwDestroyWindow(mWindow);
}

void Window::SetContextCurrent() const
{
	glfwMakeContextCurrent(mWindow);
}

void Window::loop()
{
	Renderer renderer;

	GLfloat positions[] =
	{	//positions				//Texture coordinates
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f, //0
		 0.5f, -0.5f, -0.5f,	1.0f, 0.0f, //1
		 0.5f,  0.5f, -0.5f,	1.0f, 1.0f, //2
		-0.5f,  0.5f, -0.5f,	0.0f, 1.0f, //3

		-0.5f, -0.5f,  0.5f,	0.0f, 0.0f, //4
		 0.5f, -0.5f,  0.5f,	1.0f, 0.0f, //5
		 0.5f,  0.5f,  0.5f,	1.0f, 1.0f, //6
		-0.5f,  0.5f,  0.5f,	0.0f, 1.0f, //7

		-0.5f,  0.5f,  0.5f,	1.0f, 0.0f, //8
		-0.5f,  0.5f, -0.5f,	1.0f, 1.0f, //9
		-0.5f, -0.5f, -0.5f,	0.0f, 1.0f, //10
		-0.5f, -0.5f,  0.5f,	0.0f, 0.0f, //11

		 0.5f,  0.5f,  0.5f,	1.0f, 0.0f, //12
		 0.5f,  0.5f, -0.5f,	1.0f, 1.0f, //13
		 0.5f, -0.5f, -0.5f,	0.0f, 1.0f, //14
		 0.5f, -0.5f,  0.5f,	0.0f, 0.0f, //15

		-0.5f, -0.5f, -0.5f,	0.0f, 1.0f, //16
		 0.5f, -0.5f, -0.5f,	1.0f, 1.0f, //17
		 0.5f, -0.5f,  0.5f,	1.0f, 0.0f, //18
		-0.5f, -0.5f,  0.5f,	0.0f, 0.0f, //19

		-0.5f,  0.5f, -0.5f,	0.0f, 1.0f, //20
		 0.5f,  0.5f, -0.5f,	1.0f, 1.0f, //21
		 0.5f,  0.5f,  0.5f,	1.0f, 0.0f, //22
		-0.5f,  0.5f,  0.5f,	0.0f, 0.0f  //23
	};

	std::vector<GLuint> indices =
	{
		0,	1,	2,
		2,	3,	0,

		4,	5,	6,
		6,	7,	4,

		8,	9,	10,
		10,	11,	8,

		12,	13, 14,
		14, 15, 12,

		16,	17,	18,
		18, 19, 16,

		20, 21, 22,
		22, 23, 20
	};

	
	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));


	GL::VertexArray va;
	GL::VertexBuffer vb(positions, sizeof(positions));

	GL::VertexBufferLayout layout;
	layout.Push<GLfloat>(3);
	layout.Push<GLfloat>(2);

	va.AddBuffer(vb, layout);

	GL::IndexBuffer ib(indices.data(), indices.size());


	glm::mat4 model(1.0f);
	model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	//model = glm::scale(model, glm::vec3(2.0f));

	glm::mat4 view(1.0f);;
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

	glm::mat4 proj(1.0f);;
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


	GLfloat r = 0.f;
	GLfloat increment = 0.05f;

	

	while (!glfwWindowShouldClose(mWindow))
	{
		renderer.Clear();

		shader.Bind();
		//shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

		renderer.Draw(va, ib, shader);

		//if (r > 1.0f)
		//	increment = -0.05f;
		//else if (r < 0.05f)
		//	increment = 0.05f;

		//r += increment;


		glfwSwapBuffers(mWindow);
		glfwPollEvents();
	}
}
