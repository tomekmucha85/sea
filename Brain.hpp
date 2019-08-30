#ifndef BRAIN_HPP
#define BRAIN_HPP
#include <stdio.h>
#include <exception>
#include "Iedk.h"
#include <IedkErrorCode.h>
#include <string>
#include <vector>
#include <CustomDefinedTypes.hpp>
#include <Logger.hpp>

class BCI
{
    private:
		EmoEngineEventHandle eEvent = nullptr;
		EmoStateHandle eState = nullptr;
		//#TODO - czy na pewno ma byæ static?
		static unsigned int userID;
		const unsigned short composerPort = 1726;
		int state = 0;
		std::string  address = "127.0.0.1";
		BCIMode bci_device_in_use = bci_mode_none;
		static const IEE_FacialExpressionAlgo_t all_facial_expressions[];

    public:
		BCI(BCIMode mode=bci_mode_none);
		~BCI();
		BCIMode WhatBCIIsConnected();
		BCIEvent GetNextBCIEvent();
		static int SaveUserProfile();
		static void TrainSmile();
		static void TrainNeutral();
		static void TrainClench();
		static void AcceptTraining();
		static void RejectTraining();
		static void EraseAllTrainingData();
		static void TrySwitchingToTrainedSig();
};


#endif // BRAIN_HPP
