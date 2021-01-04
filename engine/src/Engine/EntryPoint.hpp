#pragma once

#include "Application.hpp"

extern Engine::Application *Engine::createApplication();

int main() {
    auto app = Engine::createApplication();
    app->run();
    delete app;
}