#pragma once
#include <vector>
#include <map>
#include <optional>
#include <set>
#include <string>

/**
    @brief Structure representing password data.
*/
struct PasswordData {
    std::string name;
    std::string password;
    std::string category;
    std::optional<std::string> website;
    std::optional<std::string> login;
};

/**
    @brief User interface function for managing password data.

    This function provides a menu-based user interface for operating on password data.

    @param file The file path linked with the password data.
    @param passwords The vector of PasswordData objects containing the passwords.

    @return void
*/
auto userInterface(const std::string& file, std::vector<PasswordData>& passwords) -> void;

/**
    @brief Displays the content of the password list.

    This function displays the content of the password data, including the name, password, category,
    website (if available), and username (if available) for each password.

    @param passwords The vector of PasswordData objects containing the passwords.

    @return void
*/
auto displayContent(const std::vector<PasswordData>& passwords) -> void;

/**
    @brief Searches for specific passwords based on name and category.

    This function allows the user to search for specific passwords by entering a name and category.
    It iterates through the passwords list and return the data that match the criteria.
    The matching passwords are displayed, including their name, password, category, website (if available),
    and username (if available).

    @param passwords The vector of PasswordData objects containing the passwords.

    @return void
*/
auto searchPasswords(const std::vector<PasswordData>& passwords) -> void;

/**
    @brief Sorts the passwords based on selected parameters.

    This function allows the user to sort the passwords based on two selected parameters.
    The user is asked to choose the sorting parameters (name or category),
    and then passwords are sorted, then displayed.

    @param passwords The vector of PasswordData objects containing the passwords.

    @return void
*/
auto sortPasswords(std::vector<PasswordData>& passwords) -> void;

/**
    @brief Checks if a string contains uppercase letters.

    This function checks if the given string contains uppercase letters. It uses the
    std::ranges::any_of algorithm with a lambda to iterate over each character
    in the string and check if it is an uppercase letter using std::isupper.

    @param text The string to be checked.

    @return True if the string contains at least one uppercase letter, false if not.
*/
auto isUppercase(const std::string& text) -> bool;

/**
    @brief Checks if a string contains special characters.

    This function checks if the given string contains special characters. It compares each character
    in the string with a list of special characters and returns true if there is a match.

    @param text The string to be checked.

    @return True if the string contains at least one special character, false if not.
*/
auto isSpecialChar(const std::string& text) -> bool;

/**
    @brief Checks if a password meets the requirements for a strong password.

    This function checks if the given password meets the requirements for a strong password. It checks
    if the password has a minimum length of 8 characters, contains at least one uppercase letter,
    and contains at least one special character. If it does, it returns true; otherwise,
    it returns false.

    @param password The password to be checked.

    @return True if the password is strong, false otherwise.
*/
auto isStrong(const std::string& password) -> bool;

/**
    @brief Checks if a specific password is already used in the other passwords.

    This function checks if the given password is already used in the other passwords.
    It iterates through the list and compares each password with the given password.
    If there is a match, it returns true, false if not.

    @param passwords The vector of passwords to search.
    @param password The password to check.

    @return True if the password is already used, false if not.
*/
auto isUsed(const std::vector<PasswordData>& passwords, const std::string& password) -> bool;

/**
    @brief Generates a password based on criteria.

    This function generates a password based on the specified criteria: length, uppercase, and special characters.
    It uses a combination of lowercase letters, numbers, uppercase letters (if in criteria), and special characters (if in criteria).
    The generated password is checked to ensure it is "strong" and is not already used in the other passwords.
    If the generated password does not meet the criteria or is already used, a new password is generated.

    @param passwords The list of passwords.
    @param length The expected length of the password.
    @param uppercase Criterion if uppercase letters should be included.
    @param specialChar Criterion if special characters should be included.

    @return The generated password.
*/
auto passwordGenerator(const std::vector<PasswordData>& passwords, auto& length, bool& uppercase, bool& specialChar) -> std::string;

/**
    @brief Adds a new password.

    The user is asked to enter the password name and then choose to enter the password manually
    or to generate one. If the user chooses to generate a password, they can enter the
    length and other parameters for od new password. The function also ask
    the user to enter the category for the password. If the category does not
    exist, the program can create it. The user can enter the website and login for the password optionally.
    The entered password is then added to the vector of passwords.

    @param passwords The vector of passwords.
    @param categories The set of categories.

    @return void
*/
auto addPassword(std::vector<PasswordData>& passwords, std::set<std::string>& categories) -> void;

/**
    @brief Edits an existing password.

    This function let the user to edit an existing password by entering the
    password name. The function asks the user to choose from a few options,
    for example changing the password name, changing the password, changing the category,
    and changing or adding the website and login. The function works until the user
    chooses to quit. The function also can create a new category if it does not exist in the set of
    categories.

    @param passwords The vector of passwords.
    @param categories The set of categories.

    @return void
*/
auto editPassword(std::vector<PasswordData>& passwords, std::set<std::string>& categories) -> void;

/**
    @brief Deletes password.

    This function lets the user to delete one or more password by entering
    the password name(s). The function repeats the process until the user chooses not to
    delete any more passwords. After confirming the operation, the function removes the password(s)
    from the vector. If no matching passwords are found, a message is displayed.

    @param passwords The vector of passwords;

    @return void
*/
auto deletePassword(std::vector<PasswordData> &passwords) -> void;

/**
    @brief Prints the available categories.

    This function prints the set of available categories.

    @param categories The set of categories.

    @return void
*/
auto availableCategory(const std::set<std::string>& categories) -> void;

/**
    @brief Adds a new category to the set of categories.

    This function allows to add a new category to the set of categories.
    It displays the available categories and then asks the user to enter
    the new category. If the new category already exists in the set, a message is displayed.
    Otherwise, the new category is added to the set and a success message is displayed.

    @param categories The set of categories.

    @return void
*/
auto addCategory(std::set<std::string>& categories) -> void;

/**
    @brief Deletes a category and attributed passwords

    This function lets the user to delete a category and all the password
    attributed to that category. The user is asked to enter the name of the category
    to delete. If the category is found, it is removed from the set of categories, and
    all the password from that category are deleted too.
    If the category is not found, a message is displayed.

    @param passwords The vector of passwords;
    @param categories The set of categories.

    @return void
*/
auto deleteCategory(std::vector<PasswordData>& passwords, std::set<std::string>& categories) -> void;

/**
    @brief Saves the passwords to a file.

    This function saves the modified/added passwords to a file. It overwrites the existing file
    with updated data. Then the function encrypts the file using the readEncryptWrite()
    function and appends the second line [TIMESTAMP] from the original file to the end of the encrypted file.

    @param passwords The vector of PasswordData objects containing the passwords to be saved.
    @param file The path to the file where the passwords will be saved.

    @return void
*/
auto passwordsSave(const std::vector<PasswordData>& passwords, const std::string& file) -> void;

/**
    @brief Checks if a file is empty.

    This function checks if the specified file is empty by checking if the input stream
    reaches the end-of-file.

    @param file The path to the file to be checked.

    @return true if the file is empty, false if not.
*/
auto isFileEmpty(const std::string& file) -> bool;

/**
    @brief Selects a file from the available options or allows to enter an absolute path.

    This function displays a list of files from project folder and asks the user to
    select a file by entering the number. Optionally, the user can enter an absolute
    path to a file. The selected file path is returned.

    @return The selected file path as a string.
*/
auto selectFile() -> std::string;

/**
    @brief Splits a string into a vector of PasswordData objects.

    This function takes a string as input, each line represents a PasswordData object,
    separated by white space . The string is split into individual lines, and each
    line is further divided to the fields for creating a PasswordData object.
    The new PasswordData objects are stored in a vector and returned.

    @param input The input string to split.

    @return A vector of PasswordData objects.
*/
auto splitString(const std::string &input) -> std::vector<PasswordData>;

/**
    @brief Reads password data from a file and displays the user interface.

    This function reads passwords from a selected file, decrypts, and insert them
    to the vector of PasswordData objects. Then it calls the userInterface function
    to display the user interface.

    @return void
*/
auto fileRead() -> void;

/**
    @brief Reads the content of a file, encrypts it, and writes the encrypted text to the file.

    This function reads the content of the file, encrypts the text, and writes
    the encrypted text to the same file, overwriting its previous content.

    @param file The path to the file to be read, encrypted, and written.

    @return void
*/
auto readEncryptWrite(const std::string& file) -> void;

/**
    @brief Encrypts the given text using a password.

    This function encrypts the text using a password. The password is used for XOR encryption,
    where each character of the text is applied a XOR operation with the corresponding character from the password.

    @param text The text to be encrypted.

    @return The encrypted text.
*/
auto encryptText(const std::string& text) -> std::string;

/**
    @brief Decrypts the text using a password.

    This function decrypts the text by converting the hexadecimal characters to unicode,
    then apply a XOR operation with the password characters to get the decrypted text.

    @param text The text to be decrypted.

    @return The decrypted text.
*/
auto decryptText(const std::string& text) -> std::string;

/**
    @brief Adds or modifies a timestamp in the file.

    This function reads the content of the file line by line. If a line starts with "[TIMESTAMP] ",
    it replaces it with the modified timestamp. If no such line is found, it appends a new line with
    the current timestamp at the end of the file.

    @param file The path to the file.

    @return void
*/
auto makeTimestamp(const std::string& file) -> void;

/**
    @brief Get the second line from a file.

    This function opens the file, reads the first two lines, and returns the second line as a string.

    @param file The path to the file.

    @return The second line from the file.
*/
auto secondLine(const std::string &file) -> std::string;

/**
    @brief Modifies the content of the file with the new content.

    This function opens the file in output mode and replaces its content with the new content

    @param file The path to the file.
    @param data The new content to be written to the file.

    @return void
*/
auto fileModify(const std::string& file, const std::string& data) -> void;


