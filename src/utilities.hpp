#pragma once
#include <filesystem>
#include <iostream>
#include <cmath>
    
float clamp(float x, float min, float max) {
    return std::min(max, std::max(min, x));
}

int hash(int x, int y) {
    return x * 73856093 ^ y * 19349663;
}

int raw_id(int x, float width) {
    return floor(x / width);
}

std::string validate_path(int& outcome, int argc, char* argv[]) {

    outcome = 0;

    if (argc <= 1) {
        outcome = 1;
        std::cerr << "You did not enter a path!" << std::endl;
        return "";
    }

    std::string path = argv[1];

    if (!std::filesystem::exists(path)) {
        outcome = 1;
        std::cerr << "The path does not exist!" << std::endl;
        return "";
    }

    if (std::filesystem::path(path).extension() != ".csv") {
        outcome = 1;
        std::cerr << "You did not provide a .csv!" << std::endl;
        return "";
    }

    return path;
}
