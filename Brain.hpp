#ifndef BRAIN_HPP
#define BRAIN_HPP
#include <stdio.h>
#include <exception>
#include "Iedk.h"
#include <IedkErrorCode.h>
#include <string>
#include <CustomDefinedTypes.hpp>
#include <Logger.hpp>

class BCI
{
    private:
		EmoEngineEventHandle eEvent = nullptr;
		EmoStateHandle eState = nullptr;
		unsigned int userID = 0;
		const unsigned short composerPort = 1726;
		int state = 0;
		std::string  address = "127.0.0.1";
		BCIMode bci_device_in_use = bci_none;
    public:
		BCI(BCIMode mode=bci_none);
		~BCI();
		BCIMode WhatBCIIsConnected();
		std::string GetNextBCIEvent();
		static int SaveUserProfile();
};


#endif // BRAIN_HPP
