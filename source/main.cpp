#include <cstdlib>
#include <exception>
#include <iostream>

#include "core/shell.hpp"

int main() {
    try {
        trm::core::Shell shell;
        return shell.run();
    }
    catch (const std::exception& e) {
        std::cerr << "\nError: " << e.what() << '\n';
        return EXIT_FAILURE;
    }
    catch (...) {
        std::cerr << "\nAn unexpected error occurred.\n";
        return EXIT_FAILURE;
    }
}