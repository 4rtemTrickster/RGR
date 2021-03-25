#pragma once
#include "../Help.h"

namespace Test
{
	class Test
	{
	public:
		Test() {}
		virtual ~Test() {}

		virtual void OnUpdate(GLfloat deltaTime) {}
		virtual void OnRender() {}
		virtual void OnImGuiRender() {}
	};
}
