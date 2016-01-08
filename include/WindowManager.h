class WindowManager {
    
    
public:
    virtual bool initWindow(char *title, int width, int height) = 0;
    virtual bool shouldClose() = 0;
    virtual void swapBuffers() = 0;
    virtual void checkEvents() = 0;
    virtual ~WindowManager() {};
};
