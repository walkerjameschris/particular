#pragma once
#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using Data = std::vector<std::vector<float>>;

struct Reader {

    bool has_spec = true;
    bool has_motion = true;
    bool has_softbody = true;

    std::string spec_path;
    std::string motion_path;
    std::string softbody_path;

    void check_path(std::string path, std::string file, bool& indicator) {

        if (path != "--pass") {
            if (std::filesystem::exists(path)) {
                indicator = true;
            } else {
                std::cout << file + " path does not exist!\n";
                std::cout << "Tip: you can skip with '--pass'\n";
                std::exit(1);
            }
        }
    }

    void validate(int argc, char* argv[]) {

        if (argc < 4) {
            std::cout << "You must provide '--pass' or a path!\n";
            std::cout << "The commands follow this order:\n";
            std::cout << "Specification, Motion, Softbody\n\n";
            std::cout << "For example: ./particular <a path> --pass --pass\n";
            std::exit(1);
        }

        spec_path = argv[1];
        motion_path = argv[2];
        softbody_path = argv[3];
        
        check_path(spec_path, "Specification", has_spec);
        check_path(motion_path, "Motion", has_motion);
        check_path(softbody_path, "Softbody", has_softbody);
    }

    float str_to_num(std::string x, std::string file_name, int index) {

        try {
            return std::stof(x);
        } catch (...) {
            std::cout << file_name + " file is malformed on ";
            std::cout << "line " + std::to_string(index) + "\n";
            std::cout << "Cannot convert value to number!\n";
            std::exit(1);
        }
    }

    Data read_file(std::string path, std::string file_name) {
        
        Data data;

        std::ifstream file(path);
        std::string line;
        int index = 0;

        while (std::getline(file, line)) {

            std::stringstream stream(line);
            std::vector<float> row;
            std::string value;

            while (std::getline(stream, value, ',')) {
                row.push_back(str_to_num(value, file_name, index));
            }

            data.push_back(row);
            index += 1;
        }

        file.close();

        return data;
    }

    void check_row(
        std::vector<float> row,
        int size,
        int index,
        std::string layout,
        std::string file_name
    ) {

        if (row.size() != size) {
            std::cout << file_name + " file is malformed on line ";
            std::cout << std::to_string(index) + "!\n";
            std::cout << "It must have the following layout:\n";
            std::cout << layout + "\n";
            std::exit(1);
        }
    }

    Data read_spec() {

        Data data;

        if (!has_spec) {
            return data;
        }

        std::string file_name = "Specification";
        data = read_file(spec_path, file_name);

        for (int i = 0; i < data.size(); i++) {
            check_row(data[i], 4, i, "x, y, linked, fixed", file_name);
        }

        return data;
    }

    Data read_motion() {

        Data data;

        if (!has_motion) {
            return data;
        }

        std::string file_name = "Motion";
        data = read_file(motion_path, file_name);

        for (int i = 0; i < data.size(); i++) {
            check_row(data[i], 3, i, "path-id, x, y", file_name);
        }

        return data;
    }

    Data read_softbody() {

        Data data;

        if (!has_softbody) {
            return data;
        }

        std::string file_name = "Softbody";
        data = read_file(softbody_path, file_name);

        for (int i = 0; i < data.size(); i++) {
            check_row(data[i], 3, i, "shape-id, x, y", file_name);
        }

        return data;
    }
};

