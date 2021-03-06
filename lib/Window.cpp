#include "Window.h"

#include <stdexcept>

Window::Window(int windowWidth, int windowHeight,
    const std::string& windowName) :
    m_window(nullptr),
    m_width(windowWidth),
    m_height(windowHeight),
    m_name(windowName),
    m_events()
{
    initialize();
}

Window::~Window()
{
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void Window::initialize()
{
    // ================================ GLFW ============================//
    if (!glfwInit())
    {
        glfwTerminate();
        throw std::runtime_error("GLFW initialization failed!");
    }

    // Set OpenGL version
    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Core Profile = No Backwards Compatibility (?)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // Allow Forward Compatibility (?)
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    m_window = glfwCreateWindow(m_width, m_height, m_name.c_str(), nullptr, nullptr);
    if (!m_window)
    {
        glfwTerminate();
        throw std::runtime_error("GLFW window creation failed!");
    }

    // Set a special pointer stored inside GLFW window which will be used
    // in the callback functions to access EventContainer to store input
    glfwSetWindowUserPointer(m_window,&m_events);
    glfwSetKeyCallback(m_window, keyboardCallback);
    glfwSetCursorPosCallback(m_window, cursorCallback);
    glfwSetFramebufferSizeCallback(m_window, bufferResizeCallback);

    // Bind cursor to the window
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


    // ================================ GLEW ============================//
    // Make GLEW operate on a given GLFW window
    glfwMakeContextCurrent(m_window);
    // Allow modern extension features (for example?)
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        glfwDestroyWindow(m_window);
        glfwTerminate();
        throw std::runtime_error("GLEW initialization failed!");
    }

    // ================================ OpenGL ============================//
    // ???
    glEnable(GL_DEPTH_TEST);
    // ???
    glViewport(0, 0, getBufferWidth(), getBufferHeight());
    // Store image aspect ratio for other systems to use
    m_events.setAspectRatio(GLfloat(getBufferWidth())/GLfloat(getBufferHeight()));
}

void Window::pollEvents()
{
    m_events.reset();
    glfwPollEvents();
    m_events.setTime(glfwGetTime());
}

int Window::getBufferWidth() const 
{
    int bufferWidth;
    glfwGetFramebufferSize(m_window, &bufferWidth, nullptr);
    return bufferWidth;
}

int Window::getBufferHeight() const {
    int bufferHeight;
    glfwGetFramebufferSize(m_window, nullptr, &bufferHeight);
    return bufferHeight;
}

void Window::keyboardCallback(GLFWwindow* window, int key, int code,
               int action, int mode)
{
    // Use a pointer stored inside the GLFW window to get access
    // to the EventContainter of the Window class
    EventContainer* events = 
        static_cast<EventContainer *>(glfwGetWindowUserPointer(window));

    if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            events->setKeyState(key, true);
        else if (action == GLFW_RELEASE)
            events->setKeyState(key, false);
    }
}

void Window::cursorCallback(GLFWwindow* window, double xPos, double yPos)
{
    // Use a pointer stored inside the GLFW window to get access
    // to the EventContainter of the Window class
    EventContainer* events =
        static_cast<EventContainer*>(glfwGetWindowUserPointer(window));

    events->setCursorPosition(xPos, yPos);
}

void Window::bufferResizeCallback(GLFWwindow* window, int newWidth, int newHeight)
{
    // ???
    glViewport(0, 0, newWidth, newHeight);

    // Use a pointer stored inside the GLFW window to get access
    // to the EventContainter of the Window class
    EventContainer* events =
        static_cast<EventContainer*>(glfwGetWindowUserPointer(window));
    events->setAspectRatio(GLfloat(newWidth)/GLfloat(newHeight));
}
