#ifndef LOGGER_HPP
#define LOGGER_HPP
#include <string>
#include <stdio.h>
#include <iostream>
#include <CustomDefinedTypes.hpp>

class Logger
{
    private:
		static DebugLevel current_debug_level;
    public:
		//Logger();
		static void Log(std::string message, DebugLevel level=debug_info);
		static void SetDebugLevel(DebugLevel my_level);
};

#endif
