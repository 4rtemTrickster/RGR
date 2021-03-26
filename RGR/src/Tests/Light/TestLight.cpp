#include "TestLight.h"

Test::TestLight::TestLight(Window* InWnd)
	: Test(InWnd),
	BoxShader("res/Shaders/Box/Box.frag", "res/Shaders/Box/Box.vert"),
	LightShader("res/Shaders/Light/Light.frag", "res/Shaders/Light/Light.vert"),
	LightModel(1.0f),
	BoxModel(1.0f),
	view(1.0f),
	proj(1.0f),
	Fov(45.0f)
{
	this->positions =
	{
		-0.5f, -0.5f,  0.5f, // 0
		 0.5f, -0.5f,  0.5f, // 1
		 0.5f,  0.5f,  0.5f, // 2
		-0.5f,  0.5f,  0.5f, // 3

		 0.5f, -0.5f, -0.5f, // 4
		 0.5f,  0.5f, -0.5f, // 5
		-0.5f,  0.5f, -0.5f, // 6
		-0.5f, -0.5f, -0.5f, // 7
	};

	this->indices =
	{
		0, 1, 2,
		2, 3, 0,
		
		1, 4, 5,
		5, 2, 1,

		4, 7, 6,
		6, 5, 4,

		7, 0, 3,
		3, 6, 7,

		3, 2, 5,
		5, 6, 3,

		7, 4, 1,
		1, 0, 7
	};

	this->vb.Init(this->positions.data(), sizeof(GLfloat) * this->positions.size());
	
	this->layout.Push<GLfloat>(3);
	
	this->LightVa.AddBuffer(this->vb, this->layout);
	this->BoxVa.AddBuffer(this->vb, layout);

	this->ib.Init(this->indices.data(), this->indices.size());

	this->proj = glm::perspective(glm::radians(this->Fov), static_cast<float>(this->_wnd->GetWindowWidth()) / static_cast<float>(this->_wnd->GetWindowHeight()), 0.1f, 100.0f);

	LightModel = glm::scale(LightModel, glm::vec3(0.3f));
	LightModel = glm::translate(LightModel, glm::vec3(10.0f,0.0f, -10.0f));
	
	this->LightShader.Bind();
	this->LightShader.SetUniformMat4f("u_Projection", proj);
	this->LightShader.SetUniformMat4f("u_View", view);
	this->LightShader.SetUniformMat4f("u_Model", LightModel);

	this->BoxShader.Bind();
	this->BoxShader.SetUniformMat4f("u_Projection", proj);
	this->BoxShader.SetUniformMat4f("u_View", view);
	this->BoxShader.SetUniformMat4f("u_Model", BoxModel);

	this->BoxShader.SetUniform3f("u_ObjectColor", 1.0f, 0.5f, 0.31f);
	this->BoxShader.SetUniform3f("u_LightColor", 1.0f, 0.5f, 0.31f);
}

Test::TestLight::~TestLight()
{
}

void Test::TestLight::OnUpdate(GLfloat deltaTime)
{
	this->view = this->_wnd->m_Camera.GetViewMatrix();
	this->BoxShader.SetUniformMat4f("u_View", this->view);
	this->LightShader.Bind();
	this->LightShader.SetUniformMat4f("u_View", this->view);

	this->_wnd->do_movement();
}

void Test::TestLight::OnRender()
{
	renderer.Clear(glm::vec4(0.0f), GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	renderer.Draw(LightVa, ib, LightShader);
	renderer.Draw(BoxVa, ib, BoxShader);
}

void Test::TestLight::OnImGuiRender()
{
}
