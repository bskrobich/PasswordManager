#include <iostream>
#include <fstream>
#include <sstream>
#include "header.hpp"
#include <algorithm>
#include <ranges>
#include <set>

enum myChoice {
    DISPLAY_CONTENT = 1,
    SEARCH_PASSWORDS = 2,
    SORT_PASSWORDS = 3,
    ADD_PASSWORD = 4,
    EDIT_PASSWORD = 5,
    DELETE_PASSWORD = 6,
    ADD_CATEGORY = 7,
    DELETE_CATEGORY = 8,
    EXIT = 9
};

auto displayContent(const std::vector<PasswordData>& passwords) -> void {
    std::cout << '\n';
    for (const PasswordData &pData: passwords) {
        std::cout << "-----------------------\n";
        std::cout << "Name: " << pData.name << std::endl;
        std::cout << "Password: " << pData.password << std::endl;
        std::cout << "Category: " << pData.category << std::endl;
        if (pData.website.has_value()) {
            std::cout << "Website: " << pData.website.value() << std::endl;
        } else {
            std::cout << "Website: N/A" << std::endl;
        }
        if (pData.login.has_value()) {
            std::cout << "Username: " << pData.login.value() << std::endl;
        } else {
            std::cout << "Username: N/A" << std::endl;
        }
    }
}

auto searchPasswords(const std::vector<PasswordData>& passwords) -> void {
    std::string name, category;

    std::cout << "\n>>> Search for specific passwords by entering NAME and CATEGORY: ";
    std::cin >> name >> category;

    std::vector<PasswordData> matching;

    for (const PasswordData& passwordData : passwords) {
        if (passwordData.name == name || passwordData.category == category)
            matching.push_back(passwordData);
    }

    for (const PasswordData &pData: matching) {
        std::cout << "-----------------------\n";
        std::cout << "Name: " << pData.name << std::endl;
        std::cout << "Password: " << pData.password << std::endl;
        std::cout << "Category: " << pData.category << std::endl;
        if (pData.website.has_value()) {
            std::cout << "Website: " << pData.website.value() << std::endl;
        } else {
            std::cout << "Website: N/A" << std::endl;
        }
        if (pData.login.has_value()) {
            std::cout << "Username: " << pData.login.value() << std::endl;
        } else {
            std::cout << "Username: N/A" << std::endl;
        }
    }
    if (matching.empty())
        std::cout << "NO PASSWORDS FOUND.\n";
}


auto sortPasswords(std::vector<PasswordData>& passwords) -> void {
    std::string input1;
    std::string input2;
    auto choice1 = int();
    auto choice2 = int();

    std::cout << "\n>>> Select sorting parameter [NUMBER]\n"
    << "1. Sort by Name\n"
    << "2. Sort by Category\n";

    while ((choice1 == choice2) || (choice1 != 1 && choice1 != 2) || (choice2 != 1 && choice2 != 2)) {
        try {
            std::cout << "Enter first parameter number: ";
            std::cin >> input1;
            choice1 = std::stoi(input1);
            std::cout << "Enter second parameter number: ";
            std::cin >> input2;
            choice2 = std::stoi(input2);

            if (choice1 == choice2)
                std::cout << ">>> Parameters must be different from each other\n";
            else if ((choice1 != 1 && choice1 != 2) || (choice2 != 1 && choice2 != 2))
                std::cout << ">>> Wrong parameters.\n";
        } catch (const std::exception& e) {
            std::cout << ">>> Number [1-2] required.\n";
        }
    }

    std::ranges::sort(passwords.begin(), passwords.end(), [choice1, choice2](const PasswordData& p1, const PasswordData& p2) {
        if (choice1 == 1 && choice2 == 2) {
            if (p1.name != p2.name)
                return p1.name < p2.name;
            else
                return p1.category < p2.category;
        } else if (choice1 == 2 && choice2 == 1) {
            if (p1.category != p2.category)
                return p1.category < p2.category;
            else
                return p1.name < p2.name;
        }
    });

    displayContent(passwords);
}

auto isUppercase(const std::string& text) -> bool {
    return std::ranges::any_of(text, [](char c) {
        return std::isupper(c);
    });
}

auto isSpecialChar(const std::string& text) -> bool {
    std::string specialChar = "!@#$%^&*_+-.?";

    return std::ranges::any_of(text, [&specialChar](char c) {
        return std::ranges::find(specialChar, c) != specialChar.end();
    });
}

auto isStrong(const std::string& password) -> bool {
    return password.length() >= 8 && isUppercase(password) && isSpecialChar(password);
}

auto isUsed(const std::vector<PasswordData>& passwords, const std::string& password) -> bool {
    return std::ranges::any_of(passwords, [&](const PasswordData& passwordData) {
        return passwordData.password == password;
    });
}

auto passwordGenerator(const std::vector<PasswordData>& passwords, auto& length, bool& uppercase, bool& specialChar) -> std::string {
    std::string chars = "abcdefghijklmnopqrstuvwxyz0123456789";
    if (uppercase)
        chars += "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    if (specialChar)
        chars += "!@#$%^&*_+-.?";

    std::string newPassword;
    srand(time(nullptr));
    do {
        newPassword.clear();
        for (int i = 0; i < length; i++) {
            auto index = (std::rand() >> 8) % chars.length();
            newPassword += chars[index];
        }
    } while (!isStrong(newPassword) && isUsed(passwords, newPassword));

    return newPassword;
}

auto readEncryptWrite(const std::string& file) -> void {
    std::ifstream fileInput(file);
    std::stringstream buffer;
    buffer << fileInput.rdbuf();
    std::string text = buffer.str();
    std::string encrypted = encryptText(text);

    std::ofstream fileOutput(file);
    fileOutput << encrypted;
}

auto isFileEmpty(const std::string& file) -> bool {
    std::ifstream fileInput(file);
    return fileInput.peek() == std::ifstream::traits_type::eof();
}

auto availableCategory(const std::set<std::string>& categories) -> void {
    std::cout << "\nAvailable categories: ";

    std::cout << "[ ";
    for (auto& el : categories) {
        std::cout << el << ' ';
    }
    std::cout << "]\n";
}

auto addCategory(std::set<std::string>& categories) -> void {
    availableCategory(categories);
    std::cout << ">>> Add new category: ";
    std::string newCategory;
    std::cin >> newCategory;

    if (categories.find(newCategory) != categories.end()) {
        std::cout << "Category already exists.\n";
    } else {
        categories.insert(newCategory);
        std::cout << "Category successfully added.\n";
    }
}

auto addPassword(std::vector<PasswordData>& passwords, std::set<std::string>& categories) -> void {
    std::string name, password, category;
    std::string websiteIn, loginIn;
    std::optional<std::string> website, login;
    std::string in;
    auto choice = int();

    std::cout << "Enter the password name: ";
    std::cin >> name;

    std::cout << ">>> Do you want to enter your own password or generate one?\n"
              << "1. Enter the password\n"
              << "2. Generate password\n";
    while (choice != 1 && choice != 2) {
        try {
            std::cout << "Your choice: ";
            std::cin >> in;
            choice = std::stoi(in);
            if (choice != 1 && choice != 2)
                std::cout << ">>> Invalid number.\n";
        } catch (const std::exception &e) {
            std::cout << ">>> Number [1-2] required.\n";
        }

        std::string lengthStr;
        auto length = int();
        bool uppercase, specialChar;
        std::string yesNo;

        switch (choice) {
            case 1:
                std::cout << "Enter the password: ";
                std::cin >> password;

                if (!isStrong(password))
                    std::cout << ">>> The entered password is weak.\n";

                if (isUsed(passwords, password))
                    std::cout << ">>> The entered password has been already used.\n";
                break;
            case 2:
                std::cout << ">>> Enter the password parameters:\n";
                while (length < 8 || length > 25) {
                    try {
                        std::cout << "Password length: ";
                        std::cin >> lengthStr;
                        length = std::stoi(lengthStr);
                        if (length < 8 || length > 25)
                            std::cout << ">>> Length must be between 8-25.\n";
                    } catch (const std::exception &e) {
                        std::cout << ">> Number [8-25] required.\n";
                    }
                }

                std::cout << "Uppercase letters? (y/n): ";
                std::cin >> yesNo;
                while (yesNo != "y" && yesNo != "Y" && yesNo != "n" && yesNo != "N") {
                    std::cout << ">>> Please enter (y/n): ";
                    std::cin >> yesNo;
                }
                uppercase = (yesNo == "y" || yesNo == "Y");
                if (yesNo == "n" || yesNo == "N")
                    uppercase = false;

                std::cout << "Special characters? (y/n): ";
                std::cin >> yesNo;
                while (yesNo != "y" && yesNo != "Y" && yesNo != "n" && yesNo != "N") {
                    std::cout << ">>> Please enter (y/n): ";
                    std::cin >> yesNo;
                }
                if (yesNo == "y" || yesNo == "Y")
                    specialChar = true;
                else if (yesNo == "n" || yesNo == "N")
                    specialChar = false;

                password = passwordGenerator(passwords, length, uppercase, specialChar);
                std::cout << "Password successfully added.\n";
                break;
            default:
                std::cout << "OPTION NOT FOUND\n";
        }

        availableCategory(categories);
        std::cout << ">>> Enter category: ";
        std::cin >> category;
        if (categories.find(category) == categories.end()) {
            std::string input;
            std::cout << "Category does not exist. Do you want to create it? (y/n): ";
            std::cin >> input;
            while (input != "y" && input != "Y" && input != "n" && input != "N") {
                std::cout << ">>> Please enter (y/n): ";
                std::cin >> input;
            }
            if (input == "y" || input == "Y")
                categories.insert(category);
        }

        std::cout << ">>> Do you want to enter website and login? (y/n): ";
        std::cin >> yesNo;
        while (yesNo != "y" && yesNo != "Y" && yesNo != "n" && yesNo != "N") {
            std::cout << ">>> Please enter (y/n): ";
            std::cin >> yesNo;
        }
        if (yesNo == "y" || yesNo == "y") {
            std::cout << "Enter the website and login: ";
            std::cin >> websiteIn >> loginIn;
        }
        if (!websiteIn.empty() && !loginIn.empty()) {
            website = websiteIn;
            login = loginIn;
        }

        PasswordData passwordData;
        passwordData.name = name;
        passwordData.password = password;
        passwordData.category = category;
        if (website.has_value() && login.has_value()) {
            passwordData.website = website.value();
            passwordData.login = login.value();
        }

        passwords.push_back(passwordData);
    }
}

auto editPassword(std::vector<PasswordData> &passwords, std::set<std::string>& categories) -> void {
     std::string passwordName;
     std::cout << "\nEnter password name to edit: ";
     std::cin >> passwordName;
     std::string choiceStr;

     for (auto &data: passwords) {
         if (data.name == passwordName) {
             do {
                 std::cout << ">>> Editing password: " + data.name << '\n';
                 std::cout << "1. Change password name\n"
                 << "2. Change password\n"
                 << "3. Change category\n"
                 << "4. Change/add website and login\n"
                 << "5. QUIT\n";

                 std::cout << "Select option: ";
                 std::cin >> choiceStr;
                 auto choice = std::stoi(choiceStr);
                 while (choice < 1 || choice > 5) {
                     try {
                         std::cout << ">>> Number [1-5] required: ";
                         std::cin >> choiceStr;
                         choice = std::stoi(choiceStr);
                     } catch (const std::exception& e) {
                         std::cout << ">>> Invalid argument. Try again: ";
                         std::cin >> choiceStr;
                         choice = std::stoi(choiceStr);
                     }
                 }
                 std::string newWebsite, newLogin;
                 std::string newPass;

                 switch (choice) {
                     case 1 :
                         std::cout << "Enter new password name: ";
                         std::cin >> data.name;
                         std::cout << "Password name updated.\n";
                         break;
                     case 2:
                         std::cout << "Enter new password: ";
                         std::cin >> data.password;
                         std::cout << "Password updated.\n";
                         break;
                     case 3:
                         availableCategory(categories);
                         std::cout << ">>> Enter new category: ";
                         std::cin >> data.category;
                         if (categories.find(data.category) == categories.end()) {
                             std::string input;
                             std::cout << "Category does not exist. Do you want to create it? (y/n): ";
                             std::cin >> input;
                             while (input != "y" && input != "Y" && input != "n" && input != "N") {
                                 std::cout << ">>> Please enter (y/n): ";
                                 std::cin >> input;
                             }
                             if (input == "y" || input == "Y")
                                 categories.insert(data.category);
                         }
                         break;
                     case 4:
                         std::cout << "Enter new website and new login: ";
                         std::cin >> newWebsite >> newLogin;
                         data.website = newWebsite;
                         data.login = newLogin;
                         std::cout << "Website and login updated";
                         break;
                     case 5:
                         return;
                     default:
                         std::cout << "COMMAND NOT FOUND\n";
                    }
             } while (true);
         }
     }
     std::cout << "PASSWORD NOT FOUND\n";
}

auto deletePassword(std::vector<PasswordData> &passwords) -> void {
    std::string passName;
    std::string yesNo;
    auto count = int();

    std::vector<std::string> toDelete;

    do {
        std::cout << ">>> Enter a name of password to delete: ";
        std::cin >> passName;
        for (auto &e: passwords) {
            if (e.name == passName) {
                toDelete.push_back(passName);
                count++;
            }
        }
        std::cout << "Delete another password? (y/n): ";
        std::cin >> yesNo;
        while (yesNo != "y" && yesNo != "Y" && yesNo != "n" && yesNo != "N") {
            std::cout << ">>> Please enter (y/n): ";
            std::cin >> yesNo;
        }
    } while (yesNo != "n" && yesNo != "N");

    if (toDelete.empty()) {
        std::cout << "Passwords name(s) not found\n";
        return;
    }

    std::string choice;
    std::cout << "Do you confirm deleting " << count << " password(s)? (y/n): ";
    std::cin >> choice;
    while (choice != "y" && choice != "Y" && choice != "n" && choice != "N") {
        std::cout << ">>> Please enter (y/n): ";
        std::cin >> choice;
    }

    if (choice == "y" || choice == "Y") {
        for (const auto &e: toDelete) {
            passwords.erase(std::remove_if(passwords.begin(), passwords.end(), [&](const PasswordData &p) {
                return p.name == e;
            }), passwords.end());
        }
        std::cout << "Removed " << count << " element(s)\n";
    } else
        std::cout << "Operation cancelled.\n";
}

auto deleteCategory(std::vector<PasswordData> &passwords, std::set<std::string> &categories) -> void {
    std::vector<std::string> toDelete;
    std::string category;

    std::cout << "\nEnter a category to delete: ";
    std::cin >> category;

    if (categories.find(category) == categories.end()) {
        std::cout << ">>> CATEGORY NOT FOUND.\n";
    } else {
        categories.erase(category);

        passwords.erase(std::remove_if(passwords.begin(), passwords.end(), [&](const PasswordData &p) {
            return p.category == category;
        }), passwords.end());
        std::cout << "Category successfully deleted.\n";
    }
}

auto secondLine(const std::string &file) -> std::string {
    std::ifstream inputStr(file);
    std::string second;

    std::getline(inputStr, second);
    std::getline(inputStr, second);
    return second;
}

auto passwordsSave(const std::vector<PasswordData> &passwords, const std::string &file) -> void {
    std::string s = secondLine(file);

    std::ofstream fileOutput(file, std::ios::trunc);

    if (!passwords.empty()) {
        for (const auto &password: passwords) {
            fileOutput << password.name << " " << password.password << " " << password.category;
            if (password.website.has_value() && password.login.has_value()) {
                fileOutput << " " << password.website.value() << " " << password.login.value() << '\n';
            } else
                fileOutput << '\n';
        }
        fileOutput.close();
        std::cout << "\n>>> Passwords saved to file.\n";
        std::cout << "Encrypting file...\n";
        readEncryptWrite(file);

        std::ofstream output(file, std::ios::app);
        output << "\n" << s;
        output.close();
    }

}

auto userInterface(const std::string &file, std::vector<PasswordData> &passwords) -> void {
    std::set<std::string> categories;
    for (auto &el: passwords) {
        categories.insert(el.category);
    }

    std::string input;
    auto choice = int();
    do {
        do {
            try {
                std::cout << "\n>>> Choose an option:\n"
                             "1. DISPLAY_CONTENT\n"
                             "2. SEARCH_PASSWORDS\n"
                             "3. SORT_PASSWORDS\n"
                             "4. ADD_PASSWORD\n"
                             "5. EDIT_PASSWORD\n"
                             "6. DELETE_PASSWORD\n"
                             "7. ADD_CATEGORY\n"
                             "8. DELETE_CATEGORY\n"
                             "9. EXIT\n";


                std::cout << "Your choice: ";
                std::cin >> input;

                choice = std::stoi(input);

                switch (choice) {
                    case DISPLAY_CONTENT:
                        displayContent(passwords);
                        break;
                    case SEARCH_PASSWORDS:
                        searchPasswords(passwords);
                        break;
                    case SORT_PASSWORDS:
                        sortPasswords(passwords);
                        break;
                    case ADD_PASSWORD:
                        addPassword(passwords, categories);
                        break;
                    case EDIT_PASSWORD:
                        editPassword(passwords, categories);
                        break;
                    case DELETE_PASSWORD:
                        deletePassword(passwords);
                        break;
                    case ADD_CATEGORY:
                        addCategory(categories);
                        break;
                    case DELETE_CATEGORY:
                        deleteCategory(passwords, categories);
                        break;
                    case EXIT:
                        passwordsSave(passwords, file);
                        return;
                    default:
                        std::cout << ">>> COMMAND NOT FOUND.\n";
                }
            } catch (const std::exception &e) {
                std::cout << ">>> INVALID ARGUMENT (NUMBER REQUIRED).\n";
            }
        } while (choice >= 1 && choice < 9);
    } while (true);
}