#include "Help.h"
#include "Shader.h"


GL::Shader::Shader()
	: InitializationFlag(false)
{
}

GL::Shader::Shader(const std::string& FragmentPath, const std::string& VertexPath)
	: m_PathF(FragmentPath), m_PathV(VertexPath), m_RendererID(0)
{
	std::string VertexSource = ParseShader(m_PathV);
	std::string FragmentSource = ParseShader(m_PathF);

	m_RendererID = CreateShader(VertexSource, FragmentSource);

	InitializationFlag = true;
}

GL::Shader::~Shader()
{
	GLCall(glDeleteProgram(m_RendererID));
}

void GL::Shader::Init(const std::string& FragmentPath, const std::string& VertexPath)
{
	this->m_PathF = FragmentPath;
	this->m_PathV = VertexPath;
	
	std::string VertexSource = ParseShader(m_PathV);
	std::string FragmentSource = ParseShader(m_PathF);

	m_RendererID = CreateShader(VertexSource, FragmentSource);

	InitializationFlag = true;
}

void GL::Shader::Bind() const
{
	GLCall(glUseProgram(m_RendererID));
}

void GL::Shader::Unbind() const
{
	GLCall(glUseProgram(0));
}

void GL::Shader::SetUniform1i(const std::string& name, GLint value)
{
	GLCall(glUniform1i(GetUniformLocation(name), value));
}

void GL::Shader::SetUniform1f(const std::string& name, GLfloat value)
{
	GLCall(glUniform1f(GetUniformLocation(name), value));
}

void GL::Shader::SetUniform4f(const std::string& name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
{
	GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void GL::Shader::SetUniform3f(const std::string& name, GLfloat v0, GLfloat v1, GLfloat v2)
{
	GLCall(glUniform3f(GetUniformLocation(name), v0, v1, v2));
}

void GL::Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
	GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]))
}

std::string GL::Shader::ParseShader(const std::string& path)
{
	std::ifstream fs(path);
	std::string line;

	if (!fs.is_open())
		throw std::runtime_error("Can not open shader file! Path:" + path);
	
	LOG_TRACE("Shader file is open. Path:" + path);

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
		LOG_ERROR("Failed to compile {0} shader \n {1}", (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment"), message);
		glDeleteShader(id);
		return 0;
	}
	else
		LOG_TRACE("{0} shader is compiled", (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment"));

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

GLint GL::Shader::GetUniformLocation(const std::string& name)
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		return m_UniformLocationCache[name];

	GLCall(GLint location = glGetUniformLocation(m_RendererID, name.c_str()));

	if (location == -1)
		LOG_WARN("uniform: {0} doesn't exist!", name);

	m_UniformLocationCache[name] = location;

	return location;
}
