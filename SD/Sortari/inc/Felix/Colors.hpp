#ifndef __COLORS_HPP__
#define __COLORS_HPP__

#include <string>

namespace Felix::Colors::Console
{
    const std::string red("\033[0;31m");
    const std::string green("\033[1;32m");
    const std::string yellow("\033[33m");
    const std::string cyan("\033[0;36m");
    const std::string magenta("\033[0;35m");
    const std::string reset("\033[0m");
}

namespace Felix::Colors::File
{
    const std::string red("");
    const std::string green("");
    const std::string yellow("");
    const std::string cyan("");
    const std::string magenta("");
    const std::string reset("");
}

#endif