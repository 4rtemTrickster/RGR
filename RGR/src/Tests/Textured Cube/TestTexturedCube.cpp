#include "TestTexturedCube.h"


Test::TestTexturedCube::TestTexturedCube(Window* InWnd)
	: Test(InWnd),
	model(1.0f),
	view(1.0f),
	proj(1.0f),
	Fov(45.0f),
	shader("res/Shaders/first.frag", "res/Shaders/first.vert")
{
	this->positions = {
		//positions				//Texture coordinates
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

	this->indices = {
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
	this->layout.Push<GLfloat>(2);

	this->va.AddBuffer(vb, this->layout);

	this->ib.Init(this->indices.data(), this->indices.size());

	this->proj = glm::perspective(glm::radians(this->Fov), static_cast<float>(this->_wnd->GetWindowWidth()) / static_cast<float>(this->_wnd->GetWindowHeight()), 0.1f, 100.0f);

	this->shader.Bind();
	this->shader.SetUniformMat4f("u_Projection", proj);
	this->shader.SetUniformMat4f("u_View", view);
	this->shader.SetUniformMat4f("u_Model", model);

	this->texture.Init("res/Textures/Texture2.png");

	this->texture.Bind();
	this->shader.SetUniform1i("u_Texture", 0);
}

Test::TestTexturedCube::~TestTexturedCube()
{
}

void Test::TestTexturedCube::OnUpdate(GLfloat deltaTime)
{
	this->view = this->_wnd->m_Camera.GetViewMatrix();
	this->shader.SetUniformMat4f("u_View", this->view);

	this->_wnd->do_movement();
}

void Test::TestTexturedCube::OnRender()
{
	renderer.Clear(glm::vec4(0.12f, 0.3f, 0.8f, 1.0f), GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	renderer.Draw(va, ib, shader);
}

void Test::TestTexturedCube::OnImGuiRender()
{
	
}
