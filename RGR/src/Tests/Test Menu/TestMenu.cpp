#include "TestMenu.h"

#include "imgui/imgui.h"

Test::TestMenu::TestMenu(Test*& currentTestPointer)
	: CurrentTest(currentTestPointer)
{
}

Test::TestMenu::~TestMenu()
{
}

void Test::TestMenu::OnImGuiRender()
{
	for (auto& test : m_Tests)
	{
		if (ImGui::Button(test.first.c_str()))
			CurrentTest = test.second();
	}
}

void Test::TestMenu::OnRender()
{
	renderer.Clear(glm::vec4(0.4f), GL_COLOR_BUFFER_BIT);
}

void Test::TestMenu::OnUpdate(GLfloat deltaTime)
{
	
}
