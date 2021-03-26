#pragma once
#include "../Help.h"
#include "../Window/Window.h"

namespace Test
{
	class Test
	{
	protected:
		Window* _wnd;
	public:
		Test(Window* inWnd) : _wnd(inWnd) {}
		virtual ~Test() {}

		virtual void OnUpdate(GLfloat deltaTime) {}
		virtual void OnRender() {}
		virtual void OnImGuiRender() {}
	};
}
