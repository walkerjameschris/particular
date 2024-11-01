#pragma once

#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using FileRow = std::vector<float>;
using FileData = std::vector<FileRow>;

struct Reader {

    bool first_load = true;

    bool has_spec = true;
    bool has_motion = true;
    bool has_softbody = true;

    std::string spec_path;
    std::string motion_path;
    std::string softbody_path;

    FileData spec_data;
    FileData motion_data;
    FileData softbody_data;

    static bool check_path(std::string path, std::string file) {

        if (path == "--pass") {
            return false;
        }

        if (std::filesystem::is_regular_file(path)) {
            return true;
        }

        std::cout << file + " path is not a regular file!\n";
        std::cout << "You can skip simulation components with '--pass'\n";
        std::exit(1);
    }

    void validate(int argc, char* argv[]) {

        if (argc < 4) {
            std::cout << "You must provide '--pass' or a path!\n";
            std::cout << "The commands follow this order:\n";
            std::cout << "./particular <specification> <motion> <softbody>\n";
            std::cout << "Tip: ./particular ../spec/fluid.csv --pass --pass\n";
            std::exit(1);
        }

        spec_path = argv[1];
        motion_path = argv[2];
        softbody_path = argv[3];
        
        has_spec = check_path(spec_path, "Specification");
        has_motion = check_path(motion_path, "Motion");
        has_softbody = check_path(softbody_path, "Softbody");
    }

    static float str_to_num(std::string x, std::string file_name, int index) {

        try {
            return std::stof(x);
        } catch (...) {
            std::cout << file_name + " file is malformed on ";
            std::cout << "line " + std::to_string(index) + "\n";
            std::cout << "Cannot convert value to number!\n";
            std::exit(1);
        }
    }

    FileData read_file(std::string path, std::string file_name) {
        
        FileData data;

        std::ifstream file(path);
        std::string line;
        int index = 0;

        while (std::getline(file, line)) {

            std::stringstream stream(line);
            FileRow row;
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
        int size,
        int index,
        FileRow& row,
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

    FileData read_spec() {

        if (!has_spec || !first_load) {
            return spec_data;
        }

        std::string file_name = "Specification";
        spec_data = read_file(spec_path, file_name);
        int index = 1;

        for (FileRow& row : spec_data) {
            check_row(4, index, row, "x, y, linked-index, fixed", file_name);
            index += 1;
        }

        return spec_data;
    }

    FileData read_motion() {

        if (!has_motion || !first_load) {
            return motion_data;
        }

        std::string file_name = "Motion";
        motion_data = read_file(motion_path, file_name);
        int index = 1;

        for (FileRow& row : motion_data) {
            check_row(3, index, row, "path-id, x, y", file_name);
            index += 1;
        }

        return motion_data;
    }

    FileData read_softbody() {

        if (!has_softbody || !first_load) {
            return softbody_data;
        }

        std::string file_name = "Softbody";
        softbody_data = read_file(softbody_path, file_name);
        int index = 1;

        for (FileRow& row : softbody_data) {
            check_row(3, index, row, "shape-id, x, y", file_name);
            index += 1;
        }

        return softbody_data;
    }
};

