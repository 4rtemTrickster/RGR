#include "TestLight.h"

#include "imgui/imgui.h"

Test::TestLight::TestLight(Window* InWnd)
	: Test(InWnd),
	LightShader("res/Shaders/Light/Light.frag", "res/Shaders/Light/Light.vert"),
	BoxShader("res/Shaders/Box/Box.frag", "res/Shaders/Box/Box.vert"),
	LightModel(1.0f),
	BoxModel(1.0f),
	view(1.0f),
	proj(1.0f),
	Fov(45.0f),
	LightPos(0.0f, 1.0f, 0.0f),
	LightScale(0.3f)
{
	this->positions =
	{
		//-0.5f, -0.5f,  0.5f, // 0
		// 0.5f, -0.5f,  0.5f, // 1
		// 0.5f,  0.5f,  0.5f, // 2
		//-0.5f,  0.5f,  0.5f, // 3

		// 0.5f, -0.5f, -0.5f, // 4
		// 0.5f,  0.5f, -0.5f, // 5
		//-0.5f,  0.5f, -0.5f, // 6
		//-0.5f, -0.5f, -0.5f, // 7

	// Back
	-0.5f, -0.5f, -0.5f,	 0.0f,  0.0f, -1.0f, // 0
	 0.5f, -0.5f, -0.5f,	 0.0f,  0.0f, -1.0f, // 1
	 0.5f,  0.5f, -0.5f,	 0.0f,  0.0f, -1.0f, // 2
	-0.5f,  0.5f, -0.5f,	 0.0f,  0.0f, -1.0f, // 3
	
	// Front
	-0.5f, -0.5f,  0.5f,	 0.0f,  0.0f,  1.0f, // 4
	 0.5f, -0.5f,  0.5f,	 0.0f,  0.0f,  1.0f, // 5
	 0.5f,  0.5f,  0.5f,	 0.0f,  0.0f,  1.0f, // 6
	-0.5f,  0.5f,  0.5f,	 0.0f,  0.0f,  1.0f, // 7
	// Left
	-0.5f,  0.5f,  0.5f,	-1.0f,  0.0f,  0.0f, // 8
	-0.5f,  0.5f, -0.5f,	-1.0f,  0.0f,  0.0f, // 9
	-0.5f, -0.5f, -0.5f,	-1.0f,  0.0f,  0.0f, // 10
	-0.5f, -0.5f,  0.5f,	-1.0f,  0.0f,  0.0f, // 11
	// Right
	 0.5f,  0.5f,  0.5f,	 1.0f,  0.0f,  0.0f, // 12
	 0.5f,  0.5f, -0.5f,	 1.0f,  0.0f,  0.0f, // 13
	 0.5f, -0.5f, -0.5f,	 1.0f,  0.0f,  0.0f, // 14
	 0.5f, -0.5f,  0.5f,	 1.0f,  0.0f,  0.0f, // 15
	// Bottom
	-0.5f, -0.5f, -0.5f,	 0.0f, -1.0f,  0.0f, // 16
	 0.5f, -0.5f, -0.5f,	 0.0f, -1.0f,  0.0f, // 17
	 0.5f, -0.5f,  0.5f,	 0.0f, -1.0f,  0.0f, // 18
	-0.5f, -0.5f,  0.5f,	 0.0f, -1.0f,  0.0f, // 19
	// Top
	-0.5f,  0.5f, -0.5f,	 0.0f,  1.0f,  0.0f, // 20
	 0.5f,  0.5f, -0.5f,	 0.0f,  1.0f,  0.0f, // 21
	 0.5f,  0.5f,  0.5f,	 0.0f,  1.0f,  0.0f, // 22
	-0.5f,  0.5f,  0.5f,	 0.0f,  1.0f,  0.0f, // 23
	};

	this->indices =
	{
		//0, 1, 2,
		//2, 3, 0,
		//
		//1, 4, 5,
		//5, 2, 1,

		//4, 7, 6,
		//6, 5, 4,

		//7, 0, 3,
		//3, 6, 7,

		//3, 2, 5,
		//5, 6, 3,

		//7, 4, 1,
		//1, 0, 7

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
	
	this->vb.Init(this->positions.data(), sizeof(GLfloat) * this->positions.size());
	
	this->layout.Push<GLfloat>(3);
	this->layout.Push<GLfloat>(3);
	
	this->LightVa.AddBuffer(this->vb, this->layout);
	this->BoxVa.AddBuffer(this->vb, this->layout);

	this->ib.Init(this->indices.data(), this->indices.size());

	this->proj = glm::perspective(glm::radians(this->_wnd->m_Camera.Zoom), static_cast<float>(this->_wnd->GetWindowWidth()) / static_cast<float>(this->_wnd->GetWindowHeight()), 0.1f, 100.0f);
	
	
	this->LightShader.Bind();
	this->LightShader.SetUniformMat4f("u_Projection", proj);
	this->LightShader.SetUniformMat4f("u_View", view);
	this->LightShader.SetUniformMat4f("u_Model", LightModel);

	this->BoxShader.Bind();
	this->BoxShader.SetUniformMat4f("u_Projection", proj);
	this->BoxShader.SetUniformMat4f("u_View", view);
	this->BoxShader.SetUniformMat4f("u_Model", BoxModel);
	
	this->BoxShader.SetUniform3f("u_LightPos", LightPos.x, LightPos.y, LightPos.z);
	this->BoxShader.SetUniform3f("u_ObjectColor", 1.0f, 0.5f, 0.31f);
	this->BoxShader.SetUniform3f("u_LightColor", 1.0f, 1.0f, 1.0f);
}

Test::TestLight::~TestLight()
{
}

void Test::TestLight::OnUpdate(GLfloat deltaTime)
{
	this->view = this->_wnd->m_Camera.GetViewMatrix();
	
	this->BoxShader.SetUniformMat4f("u_View", this->view);
	//this->BoxShader.SetUniform3f("u_ViewPos", this->_wnd->m_Camera.Position.x, this->_wnd->m_Camera.Position.y, this->_wnd->m_Camera.Position.z);
	this->BoxShader.SetUniform3f("u_LightPos", LightPos.x, LightPos.y, LightPos.z);
	
	this->LightShader.Bind();
	this->LightShader.SetUniformMat4f("u_View", this->view);
	this->LightModel = glm::translate(glm::mat4(1.0f), this->LightPos);
	this->LightModel = glm::scale(LightModel, this->LightScale);
	this->LightShader.SetUniformMat4f("u_Model", LightModel);
}

void Test::TestLight::OnRender()
{
	renderer.Clear(glm::vec4(0.0f), GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	renderer.Draw(LightVa, ib, LightShader);
	renderer.Draw(BoxVa, ib, BoxShader);

}

void Test::TestLight::OnImGuiRender()
{
	ImGui::SliderFloat3("Move the light", &LightPos.x, -5.0f, 5.0f);
	ImGui::SliderFloat("Scale", &LightScale.x, 0.001f, 2.f);

	LightScale.y = LightScale.z = LightScale.x;
}
