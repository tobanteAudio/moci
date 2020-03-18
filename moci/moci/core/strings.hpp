#pragma once

#include <cassert>
#include <cctype>
#include <cstdio>
#include <cstdlib>

#include <algorithm>
#include <iostream>
#include <locale>
#include <optional>
#include <sstream>
#include <vector>

namespace moci
{

/**
 * @brief Collection of string helper functions.
 */
class Strings
{

public:
    /**
     * @brief Deleted constructor. Static class.
     */
    Strings() = delete;

    /**
     * @brief Remove all leading spaces on the given string in place.
     */
    static inline auto LeftTrim(std::string& s) -> void
    {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) { return std::isspace(ch) == 0; }));
    }

    /**
     * @brief Remove all leading spaces on the given string copied.
     */
    static inline auto LeftTrimCopy(std::string s) -> std::string
    {
        LeftTrim(s);
        return s;
    }

    /**
     * @brief Remove all trailing spaces on the given string in place
     */
    static inline auto RightTrim(std::string& s) -> void
    {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) { return std::isspace(ch) == 0; }).base(), s.end());
    }

    /**
     * @brief Remove all trailing spaces on the given string copied.
     */
    static inline auto RightTrimCopy(std::string s) -> std::string
    {
        RightTrim(s);
        return s;
    }

    /**
     * @brief Remove all leading & trailing spaces on the given string in place.
     */
    static inline auto Trim(std::string& s) -> void
    {
        LeftTrim(s);
        RightTrim(s);
    }

    /**
     * @brief Remove all leading & trailing spaces on the given string copied.
     */
    static inline auto TrimCopy(std::string s) -> std::string
    {
        Trim(s);
        return s;
    }

    /**
     * @brief Returns a vector of string splits. Split by the delimiter.
     */
    static inline auto Split(std::string const& s, char delimiter) -> std::vector<std::string>
    {
        std::vector<std::string> tokens;
        std::string token;
        std::istringstream tokenStream(s);
        while (std::getline(tokenStream, token, delimiter))
        {
            tokens.push_back(token);
        }
        return tokens;
    }

    /**
     * @brief Convertes a string to int. If it fails an empty optional will be returned. If yout input a string with
     * numbers at the beginning followed by characters, it will giv you the number at the beginning. Even if there are
     * no spaces between digits & characters.
     *
     * Strings::ToInt("-143abc") = -143
     */
    static inline std::optional<int> ToInt(std::string const& input)
    {
        try
        {
            int output = std::stoi(input);
            return output;
        }
        catch (...)
        {
            return std::nullopt;
        }
    }

    /**
     * @brief Same as ToInt. First converted to a plain int, then static_cast into the given integer type.
     */
    template<typename IntType>
    static std::optional<IntType> ToInteger(std::string const& input)
    {
        try
        {
            int output = std::stoi(input);
            return static_cast<IntType>(output);
        }
        catch (...)
        {
            return std::nullopt;
        }
    }
};

}  // namespace moci
