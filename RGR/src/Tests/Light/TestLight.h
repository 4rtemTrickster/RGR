#pragma once

#include "../Test.h"

#include "../../GL/Renderer/Renderer.h"
#include "../../GL/Shader/Shader.h"
#include "../GL/VertexArray/VertexBufferLayout.h"

namespace Test
{
	class TestLight : public Test
	{
	private:
		GL::Renderer renderer;
		GL::Shader LightShader;
		GL::Shader BoxShader;

		std::vector<GLfloat> positions;
		std::vector<GLuint> indices;

		GL::VertexArray LightVa;
		GL::VertexArray BoxVa;
		GL::VertexBuffer vb;
		GL::IndexBuffer ib;
		GL::VertexBufferLayout layout;

		glm::mat4 LightModel;
		glm::mat4 BoxModel;
		glm::mat4 view;
		glm::mat4 proj;
		GLfloat Fov;

		glm::vec3 LightPos;
		glm::vec3 LightScale;

	public:

		TestLight(Window* InWnd);
		~TestLight() override;


		void OnUpdate(GLfloat deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	};
}
