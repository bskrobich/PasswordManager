#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include "header.hpp"

namespace fs = std::filesystem;

auto selectFile() -> std::string {

    std::string folderPath = "/Users/bskrobich/CLionProjects/ProjektPJC/";

    std::cout << ">>> Select an available path [NUMBER] or press [0] for entering an absolute path:\n";

    auto i = int();

    for (const auto &entry: fs::directory_iterator(folderPath)) {
        if (entry.path().filename() != "CMakeLists.txt") {
            if (entry.is_regular_file() && entry.path().extension() == ".txt") {
                ++i;
                std::cout << i << ". " << entry.path() << std::endl;
            }
        }
    }

    auto selectedFile = fs::path();

    while (selectedFile.empty()) {
        std::string input;
        auto num = int();
        auto choice = int();

        std::cout << "Your choice: ";
        std::cin >> input;
        try {
            choice = std::stoi(input);
            if (choice > 0 && choice <= i) {
                for (const auto &entry: fs::directory_iterator(folderPath)) {
                    if (entry.path().filename() != "CMakeLists.txt") {
                        if (entry.is_regular_file() && entry.path().extension() == ".txt") {
                            ++num;
                            if (num == choice) {
                                selectedFile = entry.path();
                                break;
                            }
                        }
                    }
                }
            } else if (choice == 0) {
                std::cout << "Enter absolute path: ";
                std::cin >> selectedFile;
                while (!fs::exists(selectedFile)) {
                    std::cout << "File does not exists. Try again: ";
                    std::cin >> selectedFile;
                }
            } else {
                std::cout << ">>> THERE IS NO FILE OF GIVEN NUMBER.\n";
            }
        } catch (const std::exception &e) {
            std::cout << ">>> INVALID ARGUMENT (NUMBER REQUIRED).\n";
        }
    }

    std::cout << "\n>>> Selected file: " << selectedFile << std::endl;
    return selectedFile;
}

auto splitString(const std::string &input) -> std::vector<PasswordData> {

    std::vector<PasswordData> result;
    PasswordData passwordData;

    std::istringstream iss(input);
    std::string line;

    while(std::getline(iss, line)) {
        std::istringstream lineStream(line);

        std::string name, password, category, website, login;
        lineStream >> name >> password >> category;

        passwordData.name = name;
        passwordData.password = password;
        passwordData.category = category;

        if (lineStream >> website >> login) {
            passwordData.website = website;
            passwordData.login = login;
        } else {
            passwordData.website = std::nullopt;
            passwordData.login = std::nullopt;
        }
        result.push_back(passwordData);
    }
    return result;
}

auto fileModify(const std::string& file, const std::string& data) -> void {
    std::ofstream output(file);
    output << data;
    output.close();
}

auto makeTimestamp(const std::string& file) -> void {
    std::ifstream input(file);

    std::string data;
    std::string line;
    auto found = false;

    while(std::getline(input, line)) {
        if (line.substr(0, 12) == "[TIMESTAMP] ") {
            std::chrono::system_clock::time_point currentTime = std::chrono::system_clock::now();
            std::time_t currentTime_t = std::chrono::system_clock::to_time_t(currentTime);

            std::stringstream ss;
            ss << std::put_time(std::localtime(&currentTime_t), "%Y-%m-%d %H:%M:%S");
            std::string timestamp = ss.str();
            data += "[TIMESTAMP] " + timestamp;
            found = true;
        } else
            data += line + '\n';
    } input.close();

    if (!found) {
        std::chrono::system_clock::time_point currentTime = std::chrono::system_clock::now();
        std::time_t currentTime_t = std::chrono::system_clock::to_time_t(currentTime);

        std::stringstream ss;
        ss << std::put_time(std::localtime(&currentTime_t), "%Y-%m-%d %H:%M:%S");
        std::string timestamp = ss.str();
        data += "[TIMESTAMP] " + timestamp;
    }
    fileModify(file, data);
}

auto fileRead() -> void {
    std::vector<PasswordData> passwords;
    std::string file = selectFile();

    if (!isFileEmpty(file)) {
        std::ifstream stream(file);
        std::string data;
        std::string line;

        while(std::getline(stream, line)){
            if (line.substr(0, 12) != "[TIMESTAMP] ")
                data += line + '\n';
        }
        stream.close();

        std::string decrypted = decryptText(data);
        passwords = splitString(decrypted);

        makeTimestamp(file);
    }

    userInterface(file, passwords);
}