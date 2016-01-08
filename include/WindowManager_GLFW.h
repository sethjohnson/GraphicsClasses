#include "WindowManager.h"

struct GLFWwindow;

class WindowManagerGLFW : public WindowManager {
    GLFWwindow* window;
    
public:
    virtual bool initWindow(char *title, int width, int height);
    virtual bool shouldClose();
    virtual void swapBuffers();
    virtual void checkEvents();
    virtual ~WindowManagerGLFW();

};
