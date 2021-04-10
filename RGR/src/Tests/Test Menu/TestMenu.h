#pragma once

#include "../../Help.h"

#include "../../GL/Renderer/Renderer.h"

#include "../Test.h"


namespace Test
{
	class TestMenu : public Test
	{
	public:

		TestMenu(Test*& currentTestPointer, Window* InWnd);
		~TestMenu() override;

		void OnImGuiRender() override;
		void OnRender() override;
		void OnUpdate(GLfloat deltaTime) override;

		template<typename T>
		void RegisterTest(const std::string& name);

	private:

		GL::Renderer renderer;

		Test*& CurrentTest;
		std::vector<std::pair<std::string, std::function<Test* ()>>> m_Tests;
	};

	template <typename T>
	void TestMenu::RegisterTest(const std::string& name)
	{
		LOG_INFO("Registering the test {0}", name);
		
		m_Tests.push_back(std::make_pair(name, [this]() {return new T(this->_wnd); }));
	}
}
