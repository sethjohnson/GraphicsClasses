#include "WindowManager.h"

struct GLFWwindow;

class WindowManagerGLFW : public WindowManager {
    GLFWwindow* window;
    
public:
    virtual bool initWindow();
    virtual bool shouldClose();
    virtual void swapBuffers();
    virtual void checkEvents();
    virtual ~WindowManagerGLFW();

};
