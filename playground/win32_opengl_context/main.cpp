#include <windows.h>

#include <GL/GL.h>

#pragma comment(lib, "opengl32.lib")

template <typename Type>
void IgnoreUnused(Type const& /*unused*/)
{
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int WinMain(__in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in_opt LPSTR lpCmdLine, __in int nShowCmd)
{
    IgnoreUnused(nShowCmd);
    IgnoreUnused(lpCmdLine);
    IgnoreUnused(hPrevInstance);

    MSG msg          = {0};
    WNDCLASS wc      = {0};
    wc.lpfnWndProc   = WndProc;
    wc.hInstance     = hInstance;
    wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);
    wc.lpszClassName = L"oglversionchecksample";
    wc.style         = CS_OWNDC;
    if (!RegisterClass(&wc)) return 1;
    CreateWindowW(wc.lpszClassName, L"openglversioncheck", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0, 0, 640, 480, 0, 0,
                  hInstance, 0);

    while (GetMessage(&msg, NULL, 0, 0) > 0) DispatchMessage(&msg);

    return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_CREATE:
        {
            PIXELFORMATDESCRIPTOR pfd = {
                // clang-format off
			sizeof(PIXELFORMATDESCRIPTOR),
			1,
			PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    //Flags
			PFD_TYPE_RGBA,        // The kind of framebuffer. RGBA or palette.
			32,                   // Colordepth of the framebuffer.
			0, 0, 0, 0, 0, 0,
			0,
			0,
			0,
			0, 0, 0, 0,
			24,                   // Number of bits for the depthbuffer
			8,                    // Number of bits for the stencilbuffer
			0,                    // Number of Aux buffers in the framebuffer.
			PFD_MAIN_PLANE,
			0,
			0, 0, 0
                // clang-format on
            };

            HDC ourWindowHandleToDeviceContext = GetDC(hWnd);

            int letWindowsChooseThisPixelFormat;
            letWindowsChooseThisPixelFormat = ChoosePixelFormat(ourWindowHandleToDeviceContext, &pfd);
            SetPixelFormat(ourWindowHandleToDeviceContext, letWindowsChooseThisPixelFormat, &pfd);

            HGLRC ourOpenGLRenderingContext = wglCreateContext(ourWindowHandleToDeviceContext);
            wglMakeCurrent(ourWindowHandleToDeviceContext, ourOpenGLRenderingContext);

            MessageBoxA(0, (char*)glGetString(GL_VERSION), "OPENGL VERSION", 0);

            wglDeleteContext(ourOpenGLRenderingContext);
            PostQuitMessage(0);
        }
        break;
        default: return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}