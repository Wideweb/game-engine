#pragma once

#include "Application.hpp"

extern Engine::Application *Engine::createApplication(ApplicationSettings);

int main(int argc, char *argv[]) {
    Engine::ApplicationSettings settings;

    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "--edit") {
            settings.edit = true;
        }

        if (std::string(argv[i]) == "--hdr") {
            settings.hdr = true;
        }

        if (std::string(argv[i]) == "--antialiasing") {
            settings.antialiasing = true;
        }
    }

    auto app = Engine::createApplication(settings);
    app->run();
    delete app;
}