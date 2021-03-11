#pragma once
#include "../../Help.h"

struct ShaderProgramSource;

namespace GL
{
	class Shader
	{
	private:
		GLuint m_RendererID;

		std::string m_PathF;
		std::string m_PathV;
		
		// Caching for uniforms
		std::unordered_map <std::string, GLint> m_UniformLocationCache;
		
	public:

		Shader(const std::string& FragmentPath, const std::string& VertexPath);
		~Shader();

		void Bind() const;
		void Unbind() const;

		//Set uniforms
		void SetUniform4f(const std::string& name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);


	private:
		std::string ParseShader(const std::string& path);
		GLuint CompileShader(GLuint type, const std::string& source);
		GLuint CreateShader(const std::string& VertexShader, const std::string& FragmentShader);
		GLuint GetUniformLocation(const std::string& name);
	};
}

