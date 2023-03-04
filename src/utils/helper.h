#ifndef HELPER_H
#define HELPER_H

#include <memory>
#include <string>
#include <stdexcept>
#include <set>
#include <sstream>
#include <iterator>

class Helper
{
public:
    /// @brief Provides a convenient string formatting for debug printing.
    /// @tparam ...Args Argument template to provide injection of several data types such as integer, floats, strings...
    /// @param format The string to inject arguments into.
    /// @param ...args The arguments to inject into the string.
    /// @return The final formatted string.
    template<typename ... Args>
    static std::string string_format( const std::string& format, Args ... args )    // Provided from code snippet licensed under CC0 1.0
    {
        int size_s = std::snprintf( nullptr, 0, format.c_str(), args ... ) + 1; // Extra space for '\0'
        if( size_s <= 0 ){ throw std::runtime_error( "Error during formatting." ); }
        auto size = static_cast<size_t>( size_s );
        std::unique_ptr<char[]> buf( new char[ size ] );
        std::snprintf( buf.get(), size, format.c_str(), args ... );
        return std::string( buf.get(), buf.get() + size - 1 ); // We don't want the '\0' inside
    }

    /// @brief Converts a given set to a string delimited with commas.
    /// @param s Set to convert.
    /// @return The final string.
    static std::string set_to_string(const std::set<std::string> s)
    {
        if (s.empty()) {return "";}
        std::ostringstream stream;
        std::copy(s.begin(), s.end(), std::ostream_iterator<std::string>(stream, ", "));
        std::string result = stream.str();
        result.pop_back(); // Delete ending comma
        result.pop_back();
        return result;
    }
};

#endif // HELPER_H
