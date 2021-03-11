#include "Shader.h"


GL::Shader::Shader(const std::string& FragmentPath, const std::string& VertexPath)
	: m_PathF(FragmentPath), m_PathV(VertexPath), m_RendererID(0)
{
	std::string VertexSource = ParseShader(m_PathV);
	std::string FragmentSource = ParseShader(m_PathF);

	m_RendererID = CreateShader(VertexSource, FragmentSource);
}

GL::Shader::~Shader()
{
	GLCall(glDeleteProgram(m_RendererID));
}

void GL::Shader::Bind() const
{
	GLCall(glUseProgram(m_RendererID));
}

void GL::Shader::Unbind() const
{
	GLCall(glUseProgram(0));
}

void GL::Shader::SetUniform4f(const std::string& name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
{
	GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

std::string GL::Shader::ParseShader(const std::string& path)
{
	std::ifstream fs(path);
	std::string line;

	if (!fs.is_open())
		throw std::runtime_error("ERROR:: Can not open shader file!");

	//while (std::getline(fs, line))
	//	line += "\n";
	//
	//fs.close();


	std::stringstream ss;
	ss << fs.rdbuf();
	return ss.str();
}


GLuint GL::Shader::CompileShader(GLuint type, const std::string& source)
{

	GLCall(GLuint id = glCreateShader(type));
	const char* src = source.c_str();
	GLCall(glShaderSource(id, 1, &src, nullptr));
	GLCall(glCompileShader(id));

	GLint result;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));

	if (GL_FALSE == result)
	{
		int length;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		char* message = (char*)alloca(length * sizeof(char));
		GLCall(glGetShaderInfoLog(id, length, &length, message));
		std::cout << "ERROR::Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}

GLuint GL::Shader::CreateShader(const std::string& VertexShader, const std::string& FragmentShader)
{
	GLCall(GLuint program = glCreateProgram())
	GLuint vs = CompileShader(GL_VERTEX_SHADER, VertexShader);
	GLuint fs = CompileShader(GL_FRAGMENT_SHADER, FragmentShader);

	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));
	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));

	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));

	return program;
}

GLuint GL::Shader::GetUniformLocation(const std::string& name)
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		return m_UniformLocationCache[name];

	GLCall(GLint location = glGetUniformLocation(m_RendererID, name.c_str()));

	if (location == -1)
		std::cout << "WARNING: uniform'" << name << "' doesn't exist!" << std::endl;

	m_UniformLocationCache[name] = location;

	return location;
}
