#include "Help.h"
#include "TestGame.h"

#include "InGame objects/Mesh/Mesh.h"
#include "InGame objects/World/World.h"
#include "InGame objects/World/Chunk/Chunk.h"

Test::TestGame::TestGame(Window* InWnd)
	: Test(InWnd),
	model(1.0f),
	view(1.0f),
	proj(1.0f),
	shader("res/Shaders/TexturedBox/TexturedBox.frag", "res/Shaders/TexturedBox/TexturedBox.vert")
{
	_wnd->m_Camera.Position = glm::vec3(-1.0f);

	World world;

	const GLdouble start_time = glfwGetTime();
	mh = world.GenerateMesh();
	LOG_INFO("Chunk's data generated in {0} seconds", glfwGetTime() - start_time);
		

	this->proj = glm::perspective(glm::radians(this->_wnd->m_Camera.Zoom), static_cast<float>(this->_wnd->GetWindowWidth()) / static_cast<float>(this->_wnd->GetWindowHeight()), 0.1f, 100.0f);

	this->shader.Bind();
	this->shader.SetUniformMat4f("u_Projection", proj);
	this->shader.SetUniformMat4f("u_View", view);
	this->shader.SetUniformMat4f("u_Model", model);

	this->texture.Init("res/Textures/Texture Atlas/TextureAtlas.png");

	this->texture.Bind();
	this->shader.SetUniform1i("u_Texture", 0);
}

Test::TestGame::~TestGame()
{
}

void Test::TestGame::OnUpdate(GLfloat deltaTime)
{
	this->view = this->_wnd->m_Camera.GetViewMatrix();
	this->shader.SetUniformMat4f("u_View", this->view);
	this->proj = glm::perspective(glm::radians(this->_wnd->m_Camera.Zoom), static_cast<float>(this->_wnd->GetWindowWidth()) / static_cast<float>(this->_wnd->GetWindowHeight()), 0.1f, 1000.0f);
	this->shader.SetUniformMat4f("u_Projection", proj);

	this->_wnd->do_movement();
}

void Test::TestGame::OnRender()
{
	renderer.Clear(glm::vec4(0.12f, 0.3f, 0.8f, 1.0f), GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for	(auto var : mh)
	renderer.Draw(var->vao, var->ibo, shader);
}

void Test::TestGame::OnImGuiRender()
{
	
}
