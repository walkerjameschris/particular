#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using Row = std::vector<std::string>;
using Data = std::vector<Row>;

Data read_file(std::string path) {
    
    Data data;
    std::ifstream file(path);
    std::string line;

    while (std::getline(file, line)) {

        std::stringstream stream(line);
        std::string value;
        Row row;

        while (std::getline(stream, value, ',')) {
            row.push_back(value);
        }

        data.push_back(row);
    }

    file.close();

    return data;
}
