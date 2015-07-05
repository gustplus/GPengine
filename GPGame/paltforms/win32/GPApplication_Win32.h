#ifndef _WINAPPLICATION_H_
#define _WINAPPLICATION_H_

#include <windows.h> 
#include "GPVector.h"
#include "resource.h"
#include "glfunc.h"
#include "GPScreen.h"
#include "GPApplicationProtocol.h"
#include "GPGLViewImp_Win32.h"

#include <time.h>

namespace GPEngine3D{
	class Application : public ApplicationProtocol
	{
	public:
		static Application *getInstance();

		Application();
		~Application(void);

		int run();
        
        void setTimeInterval(float interval) override;

	protected:
		Screen *runScreen;

		clock_t _startTime;
		float _deltaTime;

		static Application *application;
	};
}
#endif

#include <windows.h>

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
					   _In_opt_ HINSTANCE hPrevInstance,
					   _In_ LPTSTR    lpCmdLine,
					   _In_ int       nCmdShow);