#pragma once

#include "../Test.h"

#include "GL/Renderer/Renderer.h"
#include "GL/Shader/Shader.h"
#include "GL/VertexArray/VertexBufferLayout.h"
#include "GL/Texture/Texture.h"

namespace Test
{
	class TestLight : public Test
	{
	private:
		struct Material
		{
			glm::vec3	Ambient;
			glm::vec3	Diffuse;
			glm::vec3	Specular;
			GLfloat		Shininess;
		};

		struct Light
		{
			glm::vec3 Possition;

			glm::vec3 Ambient;
			glm::vec3 Diffuse;
			glm::vec3 Specular;
		};
		
		GL::Renderer renderer;
		GL::Shader LightShader;
		GL::Shader BoxShader;

		GL::Texture BoxDiffuseTexture;
		GL::Texture BoxSpecularTexture;

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

		glm::vec3 LightScale;

		Material mat;
		Light light;

	public:

		TestLight(Window* InWnd);
		~TestLight() override;


		void OnUpdate(GLfloat deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	};
}
