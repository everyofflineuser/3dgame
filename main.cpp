#include "engine/engine.h"

int main() {
    Engine::WindowCreateInfo info;

    info.title = "valve";
    info.resizable = false;

    Engine::Window::create(info);

#if GAME_DEBUG
    std::ostringstream ossResolution;
    ossResolution << "main: Resolution: " << Engine::Window::getWidth() << "x" << TT::Window::getHeight();
    std::string formattedResolution = ossResolution.str();

    Logger::Log(Logger::INFO, formattedResolution);
#endif

    while (Engine::Window::isRunning())
    {
        Engine::Window::pollEvents();

        Engine::Window::swapBuffers();
    }

    Engine::Window::close();

    return 0;
}