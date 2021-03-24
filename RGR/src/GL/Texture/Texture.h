#pragma once
#include "../Help.h"

namespace GL
{
	class Texture
	{
	private:
		GLuint m_RendererID;
		std::string m_FilePath;
		GLubyte* m_LocalBuffer;

		GLint m_Width;
		GLint m_Height;
	
		//Bit's per pixel
		GLint m_BPP;

	public:

		Texture(const std::string& path);
		~Texture();

		void Bind(GLuint slot = 0) const;
		void Unbind() const;

		inline GLint GetWidth() const { return m_Width; }
		inline GLint GetHeight() const { return m_Height; }
	};
}
