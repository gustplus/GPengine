#ifndef _GLRENDER_H_
#define _GLRENDER_H_
#include "gprenderer.h"
#include <windows.h>

namespace GPEngine3D{

    class GLRenderer :
        public Renderer
    {
    public:
        GLRenderer(PixelFormat pFormat, char depthBits, char stencilBits, bool doubleBuffer, int width, int height);
        ~GLRenderer(void);

        bool initContent(HWND window);

        bool releaseContent();

        void setClearColor(const ColorRGBA &color) override;

        void clearBuffer() override;

        void swapBuffer() override;

        void resizeViewPort(int width, int height) override;

    private:
        HWND _hWindow;
        HDC _hDeviceContext;
        HGLRC _hWindowRC;
    };
}
#endif
