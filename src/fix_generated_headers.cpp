#include <fstream>
#include <iostream>
#include <string>
#include <vector>

// Function to modify the header file
void modify_header(const std::string& header_path) {
    // First, efficiently check if #pragma once already exists by reading only
    // beginning of file
    {
        std::ifstream check_file(header_path);
        if (!check_file.is_open()) {
            std::cerr << "Error: Could not open file " << header_path << '\n';
            return;
        }

        // Read first 512 characters to check for #pragma once
        constexpr size_t buffer_size = 512;
        std::string buffer(buffer_size, '\0');
        check_file.read(&buffer[0], buffer_size);
        buffer.resize(check_file.gcount());

        if (buffer.find("#pragma once") != std::string::npos) {
            std::cout << "Header " << header_path << " is already fixed.\n";
            return;
        }
    }

    // Read entire file content
    std::ifstream input_file(header_path, std::ios::binary);
    if (!input_file.is_open()) {
        std::cerr << "Error: Could not open file " << header_path << '\n';
        return;
    }

    // Get file size and reserve appropriate memory
    input_file.seekg(0, std::ios::end);
    const auto file_size = input_file.tellg();
    input_file.seekg(0, std::ios::beg);

    std::string content;
    content.reserve(static_cast<size_t>(file_size) +
                    13);  // +13 for "#pragma once\n"

    // Read file efficiently
    content.assign(std::istreambuf_iterator<char>(input_file),
                   std::istreambuf_iterator<char>());
    input_file.close();

    // Add #pragma once to the beginning
    content.insert(0, "#pragma once\n");

    // Write back to file with proper error checking
    std::ofstream output_file(header_path, std::ios::binary | std::ios::trunc);
    if (!output_file.is_open()) {
        std::cerr << "Error: Could not write to file " << header_path << '\n';
        return;
    }

    output_file.write(content.data(), content.size());
    if (output_file.fail()) {
        std::cerr << "Error: Failed to write to file " << header_path << '\n';
        return;
    }

    std::cout << "Header " << header_path << " has been fixed.\n";
}

int main() {
    // Use const and uniform initialization for better performance
    const std::vector<std::string> header_files{"../includes/font_ubuntu.h",
                                                "../includes/icon_calc.h"};

    // Use range-based for loop with const auto& to avoid copies
    for (const auto& header_file : header_files) {
        modify_header(header_file);
    }

    return 0;
}