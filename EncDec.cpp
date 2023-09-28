#include <iostream>
#include <filesystem>
#include <string>
#include "header.hpp"

auto encryptText(const std::string& text) -> std::string {
    std::string password;
    std::cout << "Enter the file password: ";
    std::cin >> password;

    std::string encrypted;
    auto passwordItr = 0;
    std::stringstream s;

    for (char i : text) {
        auto temp = i ^ password[passwordItr];
        s << std::hex << std::setfill('0') << std::setw(2) << (int)(temp);
        passwordItr++;
        if (passwordItr >= password.length()) {
            passwordItr = 0;
        }
    }
    s >> encrypted;
    return encrypted;
}

auto decryptText(const std::string& text) -> std::string {

    std::string password;

    std::cout << "Enter the file password: ";
    std::cin >> password;

    std::string hexToUni;

    for (auto i = 0; i < text.length() - 1; i += 2) {
        std::string output = text.substr(i, 2);
        long decimal = std::strtol(output.c_str(), nullptr, 16);
        hexToUni += (char) (decimal);
    }

    std::string decrypted;
    int passwordItr = 0;
    for (char i: hexToUni) {
        auto temp = i ^ password[passwordItr];
        decrypted += (char) (temp);
        passwordItr++;
        if (passwordItr >= password.length()) {
            passwordItr = 0;
        }
    }

    return decrypted;
}
