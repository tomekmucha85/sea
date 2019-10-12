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
#include <TimerCountdown.hpp>

class BCI
{
    private:
		EmoEngineEventHandle eEvent = nullptr;
		EmoStateHandle eState = nullptr;
		static unsigned int userID;
		const unsigned short composerPort = 1726;
		int state = 0;
		std::string  address = "127.0.0.1";
		BCIMode bci_device_in_use = bci_mode_none;
		static const IEE_FacialExpressionAlgo_t all_facial_expressions[];
		const float DETECTION_THRESHOLD = 0.5;
		unsigned int subsequent_wink_detections_recorded = 0;
		unsigned int subsequent_smile_detections_recorded = 0;
		unsigned int subsequent_clench_detections_recorded = 0;
		static const unsigned int COOLDOWN_INTERVAL_MILISECONDS = 400;
		static const unsigned int DOUBLEWINK_TIME_WINDOW = 600;
		static const unsigned int DOUBLEWINK_MIN_INTERVAL = 100;
		static const unsigned int LOWERFACE_TIME_WINDOW = 600;
		static const unsigned int LOWERFACE_MIN_INTERVAL = 200;
		TimerCountdown* ptr_cooldown_timer = new TimerCountdown(COOLDOWN_INTERVAL_MILISECONDS);
		TimerCountdown* ptr_doublewink_max_timer = new TimerCountdown(DOUBLEWINK_TIME_WINDOW);
		TimerCountdown* ptr_doublewink_min_timer = new TimerCountdown(DOUBLEWINK_MIN_INTERVAL);
		TimerCountdown* ptr_lowerface_max_timer = new TimerCountdown(LOWERFACE_TIME_WINDOW);
		TimerCountdown* ptr_lowerface_min_timer = new TimerCountdown(LOWERFACE_MIN_INTERVAL);

    public:
		BCI(BCIMode mode=bci_mode_none);
		~BCI();
		BCIMode WhatBCIIsConnected();
		BCIEvent GetNextBCIEvent();
		BCIEvent HandleLowerfaceExpression(IEE_FacialExpressionAlgo_t my_expression);
		BCIEvent HandleUpperfaceExpression(IEE_FacialExpressionAlgo_t my_expression);
		BCIEvent HandleWink(EmoStateHandle my_state);
		BCIEvent HandleSpecificLowerfaceEvent(BCIEvent my_event);
		static int SaveUserProfile();
		static void TrainSmile();
		static void TrainNeutral();
		static void TrainClench();
		static void AcceptTraining();
		static void RejectTraining();
		static void EraseAllTrainingData();
		static bool TrySwitchingToTrainedSig();
		static bool TrySwitchingToUntrainedSig();
};


#endif // BRAIN_HPP
