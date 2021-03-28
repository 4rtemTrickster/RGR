#pragma once
#include "../Help.h"

namespace GL
{
	class Texture
	{
	private:
		GLuint m_RendererID;
		GLuint m_Slot;
		std::string m_FilePath;
		GLubyte* m_LocalBuffer;

		GLint m_Width;
		GLint m_Height;
	
		//Bit's per pixel
		GLint m_BPP;


		bool InitializationFlag;
	public:

		Texture();
		Texture(const std::string& path);
		~Texture();

		void Bind(GLuint slot = 0);
		void Unbind();

		inline bool IsInit() const { return InitializationFlag; }
		void Init(const std::string& path);

		inline GLint GetWidth() const { return m_Width; }
		inline GLint GetHeight() const { return m_Height; }

		inline GLuint GetSlot() const { return m_Slot; }
	};
}
