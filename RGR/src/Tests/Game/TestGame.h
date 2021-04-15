#pragma once
#include "../Test.h"


#include "GL/Texture/Texture.h"
#include "GL/Renderer/Renderer.h"
#include "GL/VertexArray/VertexBufferLayout.h"



#include "../Vendor/imgui/imgui.h"

class Mesh;

namespace Test
{
	class TestGame : public Test
	{
	private:
		GL::Renderer renderer;
		GL::Shader shader;
		GL::Texture texture;

		std::vector<GLfloat> positions;
		std::vector<GLuint> indices;
		
		GL::VertexArray va;
		GL::VertexBuffer vb;
		GL::IndexBuffer ib;
		GL::VertexBufferLayout layout;

		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 proj;

		std::vector<Mesh*> mh;

	public:
		TestGame(Window* InWnd);
		~TestGame() override;


		void OnUpdate(GLfloat deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	};
	
}
