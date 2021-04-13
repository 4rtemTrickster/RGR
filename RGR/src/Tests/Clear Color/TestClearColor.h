#pragma once
#include "../Test.h"

#include "Help.h"



namespace Test
{
	class TestClearColor : public Test
	{
	public:

		TestClearColor(Window* InWnd);
		~TestClearColor() override;

		void OnUpdate(GLfloat deltaTime) override;

		void OnRender() override;
		void OnImGuiRender() override;

	private:

		std::array<GLfloat, 4> m_ClearColor;
	};
}
