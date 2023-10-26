#include "application.h"

#include <iostream>

int main(int argc, char** argv) {
    std::string directory;

    if (argc == 1) {
        directory = ".";
    } else if (argc == 2) {
        directory = argv[1];
    }

    AppSpec spec;
    spec.directory = directory;

    App app(spec);
    app.run();
}