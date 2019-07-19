#ifndef BRAIN_HPP
#define BRAIN_HPP
#include <stdio.h>
#include <exception>
#include "Iedk.h"
#include <IedkErrorCode.h>
#include <string>

class BCI
{
    private:
		EmoEngineEventHandle eEvent = nullptr;
		EmoStateHandle eState = nullptr;
		unsigned int userID = 0;
		const unsigned short composerPort = 1726;
		int state = 0;
		std::string  address = "127.0.0.1";
		bool is_physical_BCI_device_connected = false;
    public:
		BCI(bool real_device_in_use=true);
		~BCI();
		bool IsPhysicalBCIDeviceConnected();
		std::string GetNextBCIEvent();
};


#endif // BRAIN_HPP
