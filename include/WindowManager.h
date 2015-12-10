class WindowManager {
    
    
public:
    virtual bool initWindow() = 0;
    virtual bool shouldClose() = 0;
    virtual void swapBuffers() = 0;
    virtual void checkEvents() = 0;
    virtual ~WindowManager() {};
};
