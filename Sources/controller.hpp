#pragma once
#include <Windows.h>
#include <ctime>
#include "pthread.hpp"
namespace controller
{
	enum class Mouse
	{
		LEFT,MIDDLE,RIGHT
	};
	class Clicker : public Thread
	{
	public:

		Clicker(int cps,bool enable,Mouse mouse) : cps(cps),enable(enable),mouse(mouse)
		{

		}
		void click(Mouse mouse)
		{
			switch (mouse)			
{
			case Mouse::LEFT:
				mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
				mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
			case Mouse::MIDDLE:
				mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
				mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
			case Mouse::RIGHT:
				mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
				mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);

			}
		}
		int getTime()
		{
			return clock();
		}
		void run() override
		{
			int lastTime = getTime();
			while (enable)
			{
				
				if (getTime() - lastTime > (int)(1000 / this->cps))
				{
					click(mouse);
					lastTime = getTime();
				}

			}
		}
	public:
		int cps;
		bool enable;
		Mouse mouse;
	};


}