#include <stdexcept>
#include "Window.h"
#include "Application.h"
#include "MD5Parser.h"

using namespace ST;
using namespace std;

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    LPSTR lpszArgument, int nCmdShow)
{
    MainWindow = new Window(TEXT("Inverse kinematics"),
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        WS_EX_ACCEPTFILES);

    Application* app = 0;
    try
    {
        app = new Application();
    }
    catch (runtime_error& ex)
    {
        log("[ERROR] %s", ex.what());
    }

    if (app)
    {
        MainWindow->SetKeyEventProcessor(app);
        return app->Run();
    }

    return 0;
}
