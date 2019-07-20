#include <Logger.hpp>

DebugLevel Logger::current_debug_level = debug_info;

void Logger::Log(std::string message, DebugLevel level)
{
	if (current_debug_level == debug_none)
	{
		//Don't do a thing.
		;
	}
	else if (current_debug_level >= level)
	{
		if (level == debug_info)
		{
			std::cout << "INFO:  " + message + "\n";
		}
		else if (level == debug_full)
		{
			std::cout << "DEBUG: " + message + "\n";
		}
	}
}

void Logger::SetDebugLevel(DebugLevel my_level)
{
	printf("Debugging level set to %d.\n", my_level);
	current_debug_level = my_level;
}