#pragma once
#include "GL/Shader/Shader.h"
#include "GL/Texture/Texture.h"
#include "InGame objects/Mesh/Mesh.h"

class LoadingScreen
{
private:
    GL::VertexBuffer vb;

    GL::VertexBufferLayout layout;
public:

    GL::VertexArray va;
    GL::IndexBuffer ib;
    GL::Shader _shader;
    GL::Texture _texture;
    
    LoadingScreen();

};
