#include "Help.h"
#include "LoadingScreen.h"

LoadingScreen::LoadingScreen()
    : _shader("res/Shaders/Loading Screen/LoadingScreen.frag", "res/Shaders/Loading Screen/LoadingScreen.vert"),
      _texture("res/Textures/Loading Screen.png")
{
    std::vector<GLfloat> Positions =
    {
        -1.0f, -1.0f, 0.0f,   0.0f, 0.0f,
         1.0f, -1.0f, 0.0f,   1.0f, 0.0f,
         1.0f,  1.0f, 0.0f,   1.0f, 1.0f,
        -1.0f,  1.0f, 0.0f,   0.0f, 1.0f
    };

    std::vector<GLuint> Indices =
    {
        0,1,2,
        2,3,0
    };

    vb.Init(Positions.data(), sizeof(GLfloat) * Positions.size());
    
    layout.Push<GLfloat>(3);
    layout.Push<GLfloat>(2);

    va.AddBuffer(vb, layout);

    ib.Init(Indices.data(), Indices.size());

    _shader.Bind();
    _texture.Bind();
    _shader.SetUniform1i("u_Texture", _texture.GetSlot());
}
