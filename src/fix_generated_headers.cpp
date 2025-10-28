#include <fstream>
#include <iostream>
#include <string>
#include <vector>

// Function to check if a file exists
bool file_exists(const std::string& file_path) {
    std::ifstream file(file_path);
    return file.good();
}

// Function to modify the header file
void modify_header(const std::string& header_path) {
    if (!file_exists(header_path)) {
        std::cerr << "Error: Header file not found at " << header_path << '\n';
        return;
    }

    std::ifstream input_file(header_path);
    if (!input_file.is_open()) {
        std::cerr << "Error: Could not open file " << header_path << '\n';
        return;
    }

    std::string content;
    input_file.seekg(0, std::ios::end);
    content.reserve(input_file.tellg());
    input_file.seekg(0, std::ios::beg);
    content.assign((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>());
    input_file.close();

    // Check if the header is already fixed
    if (content.find("#pragma once") != std::string::npos) {
        std::cout << "Header " << header_path << " is already fixed." << '\n';
        return;
    }

    // Add #pragma once to the beginning of the file
    content.insert(0, "#pragma once\n");

    std::ofstream output_file(header_path);
    if (!output_file.is_open()) {
        std::cerr << "Error: Could not write to file " << header_path << '\n';
        return;
    }

    output_file << content;
    output_file.close();

    std::cout << "Header " << header_path << " has been fixed." << '\n';
}

int main() {
    // Paths to the generated header files (relative to the build directory)
    std::vector<std::string> header_files{"../includes/font_ubuntu.h", "../includes/icon_calc.h"};

    for (const std::string& header_file : header_files) {
        modify_header(header_file);
    }

    return 0;
}