#include "TestLight.h"

#include "imgui/imgui.h"

Test::TestLight::TestLight(Window* InWnd)
	: Test(InWnd),
	LightShader("res/Shaders/Light/Light.frag", "res/Shaders/Light/Light.vert"),
	BoxShader("res/Shaders/Box/Box.frag", "res/Shaders/Box/Box.vert"),
	BoxDiffuseTexture("res/Textures/Box Texture/Diffuse.png"),
	BoxSpecularTexture("res/Textures/Box Texture/Specular.png"),
	LightModel(1.0f),
	BoxModel(1.0f),
	view(1.0f),
	proj(1.0f),
	Fov(45.0f),
	LightScale(0.3f)
{
	positions =
	{
		//-0.5f, -0.5f,  0.5f, // 0
		// 0.5f, -0.5f,  0.5f, // 1
		// 0.5f,  0.5f,  0.5f, // 2
		//-0.5f,  0.5f,  0.5f, // 3

		// 0.5f, -0.5f, -0.5f, // 4
		// 0.5f,  0.5f, -0.5f, // 5
		//-0.5f,  0.5f, -0.5f, // 6
		//-0.5f, -0.5f, -0.5f, // 7

	// Back
	 0.5f, -0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,	0.0f, 0.0f,	// 0
	-0.5f, -0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,	1.0f, 0.0f,	// 1
	-0.5f,  0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,	1.0f, 1.0f,	// 2
	 0.5f,  0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,	0.0f, 1.0f,	// 3

	// Front
	-0.5f, -0.5f,  0.5f,	 0.0f,  0.0f,  1.0f,	0.0f, 0.0f,	 // 4
	 0.5f, -0.5f,  0.5f,	 0.0f,  0.0f,  1.0f,	1.0f, 0.0f,	 // 5
	 0.5f,  0.5f,  0.5f,	 0.0f,  0.0f,  1.0f,	1.0f, 1.0f,	 // 6
	-0.5f,  0.5f,  0.5f,	 0.0f,  0.0f,  1.0f,	0.0f, 1.0f,	 // 7
	// Left
	-0.5f, -0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,	0.0f, 0.0f,	// 8
	-0.5f, -0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,	1.0f, 0.0f,	// 9
	-0.5f,  0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,	1.0f, 1.0f,	// 10
	-0.5f,  0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,	0.0f, 1.0f,	// 11
	// Right
	 0.5f, -0.5f,  0.5f,	 1.0f,  0.0f,  0.0f,	0.0f, 0.0f,	// 12
	 0.5f, -0.5f, -0.5f,	 1.0f,  0.0f,  0.0f,	1.0f, 0.0f,	// 13
	 0.5f,  0.5f, -0.5f,	 1.0f,  0.0f,  0.0f,	1.0f, 1.0f,	// 14
	 0.5f,  0.5f,  0.5f,	 1.0f,  0.0f,  0.0f,	0.0f, 1.0f,	// 15
	// Bottom
	-0.5f, -0.5f, -0.5f,	 0.0f, -1.0f,  0.0f,	0.0f, 0.0f,	// 16
	 0.5f, -0.5f, -0.5f,	 0.0f, -1.0f,  0.0f,	1.0f, 0.0f,	// 17
	 0.5f, -0.5f,  0.5f,	 0.0f, -1.0f,  0.0f,	1.0f, 1.0f,	// 18
	-0.5f, -0.5f,  0.5f,	 0.0f, -1.0f,  0.0f,	0.0f, 1.0f,	// 19
	// Top
	-0.5f,  0.5f,  0.5f,	 0.0f,  1.0f,  0.0f,	0.0f, 0.0f,	// 20
	 0.5f,  0.5f,  0.5f,	 0.0f,  1.0f,  0.0f,	1.0f, 0.0f,	// 21
	 0.5f,  0.5f, -0.5f,	 0.0f,  1.0f,  0.0f,	1.0f, 1.0f,	// 22
	-0.5f,  0.5f, -0.5f,	 0.0f,  1.0f,  0.0f,	0.0f, 1.0f,	// 23
	};

	indices =
	{
		//0, 1, 2,
		//2, 3, 0,
		//
		//1, 4, 5,
		//5, 2, 1,

		//4, 7, 6,
		//6, 5, 4,

		//7, 0, 3,
		//3, 6, 7,

		//3, 2, 5,
		//5, 6, 3,

		//7, 4, 1,
		//1, 0, 7

		0,	1,	2,
		2,	3,	0,

		4,	5,	6,
		6,	7,	4,

		8,	9,	10,
		10,	11,	8,

		12, 13, 14,
		14, 15, 12,

		16, 17, 18,
		18, 19, 16,

		20, 21, 22,
		22, 23, 20
	};

	vb.Init(positions.data(), sizeof(GLfloat) * positions.size());

	layout.Push<GLfloat>(3);
	layout.Push<GLfloat>(3);
	layout.Push<GLfloat>(2);

	LightVa.AddBuffer(vb, layout);
	BoxVa.AddBuffer(vb, layout);

	ib.Init(indices.data(), indices.size());

	proj = glm::perspective(glm::radians(_wnd->m_Camera.Zoom), static_cast<float>(_wnd->GetWindowWidth()) / static_cast<float>(_wnd->GetWindowHeight()), 0.1f, 100.0f);

	mat.Shininess = 32.0f;

	light.Ambient = glm::vec3(0.2f);
	light.Diffuse = glm::vec3(0.5f);
	light.Specular = glm::vec3(1.0f);
	light.Possition = glm::vec3(0.0f, 1.0f, 0.0f);

	LightShader.Bind();
	LightShader.SetUniformMat4f("u_Projection", proj);
	LightShader.SetUniformMat4f("u_View", view);
	LightShader.SetUniformMat4f("u_Model", LightModel);

	BoxShader.Bind();
	BoxShader.SetUniformMat4f("u_Projection", proj);
	BoxShader.SetUniformMat4f("u_View", view);
	BoxShader.SetUniformMat4f("u_Model", BoxModel);

	BoxShader.SetUniform3f("u_Light.Ambient", light.Ambient.r, light.Ambient.g, light.Ambient.b);
	BoxShader.SetUniform3f("u_Light.Diffuse", light.Diffuse.r, light.Diffuse.g, light.Diffuse.b);
	BoxShader.SetUniform3f("u_Light.Specular", light.Specular.r, light.Specular.g, light.Specular.b);
	

	BoxShader.SetUniform3f("u_LightPos", light.Possition.x, light.Possition.y, light.Possition.z);
	BoxDiffuseTexture.Bind();
	BoxSpecularTexture.Bind(1);
	BoxShader.SetUniform1i("u_Material.Diffuse", BoxDiffuseTexture.GetSlot());
	BoxShader.SetUniform1i("u_Material.Specular", BoxSpecularTexture.GetSlot());
	BoxShader.SetUniform1f("u_Material.Shininess", mat.Shininess);
}

Test::TestLight::~TestLight()
{
}

void Test::TestLight::OnUpdate(GLfloat deltaTime)
{
	view = _wnd->m_Camera.GetViewMatrix();
	this->proj = glm::perspective(glm::radians(this->_wnd->m_Camera.Zoom), static_cast<float>(this->_wnd->GetWindowWidth()) / static_cast<float>(this->_wnd->GetWindowHeight()), 0.1f, 1000.0f);
	
	BoxShader.SetUniformMat4f("u_View", view);
	BoxShader.SetUniformMat4f("u_Projection", proj);
	//BoxShader.SetUniform3f("u_ViewPos", this->_wnd->m_Camera.Position.x, this->_wnd->m_Camera.Position.y, this->_wnd->m_Camera.Position.z);


	LightShader.Bind();
	LightShader.SetUniformMat4f("u_View", view);
	LightShader.SetUniformMat4f("u_Projection", proj);
	
	LightModel = glm::translate(glm::mat4(1.0f), light.Possition);
	LightModel = glm::scale(LightModel, LightScale);
	LightShader.SetUniformMat4f("u_Model", LightModel);
	
}

void Test::TestLight::OnRender()
{
	renderer.Clear(glm::vec4(0.3f), GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	renderer.Draw(LightVa, ib, LightShader);
	renderer.Draw(BoxVa, ib, BoxShader);

}

void Test::TestLight::OnImGuiRender()
{
	ImGui::Text("Light options:");
	
	if (ImGui::SliderFloat3("Move the light", &light.Possition.x, -5.0f, 5.0f))
		BoxShader.SetUniform3f("u_LightPos", light.Possition.x, light.Possition.y, light.Possition.z);

	if (ImGui::SliderFloat("Scale", &LightScale.x, 0.001f, 2.f))
		LightScale.y = LightScale.z = LightScale.x;

	ImGui::Text("Box material options:");
	
	if (ImGui::SliderFloat("Shininess", &mat.Shininess, 0.0f, 256.0f))
		BoxShader.SetUniform1f("u_Material.Shininess", mat.Shininess);

}