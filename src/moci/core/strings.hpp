#pragma once

#include <cassert>
#include <cctype>
#include <cstdio>
#include <cstdlib>

#include "moci/core/vector.hpp"
#include <algorithm>
#include <iostream>
#include <locale>
#include <optional>
#include <sstream>

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
    static inline auto leftTrim(std::string& str) -> void
    {
        str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](int ch) { return std::isspace(ch) == 0; }));
    }

    /**
     * @brief Remove all leading spaces on the given string copied.
     */
    static inline auto leftTrimCopy(std::string str) -> std::string
    {
        leftTrim(str);
        return str;
    }

    /**
     * @brief Remove all trailing spaces on the given string in place
     */
    static inline auto rightTrim(std::string& str) -> void
    {
        str.erase(
            std::find_if(str.rbegin(), str.rend(), [](int character) { return std::isspace(character) == 0; }).base(),
            str.end());
    }

    /**
     * @brief Remove all trailing spaces on the given string copied.
     */
    static inline auto rightTrimCopy(std::string str) -> std::string
    {
        rightTrim(str);
        return str;
    }

    /**
     * @brief Remove all leading & trailing spaces on the given string in place.
     */
    static inline auto trim(std::string& str) -> void
    {
        leftTrim(str);
        rightTrim(str);
    }

    /**
     * @brief Remove all leading & trailing spaces on the given string copied.
     */
    static inline auto trimCopy(std::string str) -> std::string
    {
        trim(str);
        return str;
    }

    /**
     * @brief Returns a vector of string splits. Split by the delimiter.
     */
    static inline auto split(std::string const& str, char delimiter) -> Vector<std::string>
    {
        Vector<std::string> tokens;
        std::string token;
        std::istringstream tokenStream(str);
        while (std::getline(tokenStream, token, delimiter)) { tokens.push_back(token); }
        return tokens;
    }

    /**
     * @brief Convertes a string to int. If it fails an empty optional will be returned. If yout input a string with
     * numbers at the beginning followed by characters, it will giv you the number at the beginning. Even if there are
     * no spaces between digits & characters.
     *
     * Strings::ToInt("-143abc") = -143
     */
    static inline auto toInt(std::string const& input) -> std::optional<int>
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
    static auto toInteger(std::string const& input) -> std::optional<IntType>
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
