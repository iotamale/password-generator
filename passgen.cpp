#include <cstdlib>
#include <iostream>
#include <string>

#include "lib/csprng.cpp"

const std::string LETTERS = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
const std::string DIGITS = "0123456789";
const std::string SYMBOLS = "!@#$%^&*()_-+=<>?/[]{}|";

std::string generate_password(int length) {
    if (length < 1) throw std::invalid_argument("Password length must greater or equal to 1");

    const size_t charset_size = LETTERS.size() + DIGITS.size() + SYMBOLS.size();
    std::string charset;
    charset.reserve(charset_size);
    
    charset.append(LETTERS);
    charset.append(DIGITS);
    charset.append(SYMBOLS);

    std::string password;
    password.reserve(length);

    CSPRNG csprng = csprng_create();
    if (!csprng) {
        throw std::runtime_error("Failed to initialize CSPRNG");
    }

    bool requirements_met = false;
    while (!requirements_met) {
        password.clear();
        try {
            for (int i = 0; i < length; ++i) {
                password += charset[csprng_get_int(csprng) % charset.size()];
            }

            // Check if requirements are met
            requirements_met = 
                password.find_first_of(LETTERS) != std::string::npos &&
                password.find_first_of(DIGITS) != std::string::npos &&
                password.find_first_of(SYMBOLS) != std::string::npos;
        }
        catch (...) {
            csprng_destroy(csprng);
            throw;
        }
    }

    csprng_destroy(csprng);
    return password;
}


int main() {
    try {
        const int length = 16;
        std::string password = generate_password(length);
        std::cout << password << std::endl;
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}