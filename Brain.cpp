#include <Brain.hpp>

unsigned int BCI::userID = 0;
const IEE_FacialExpressionAlgo_t BCI::all_facial_expressions[] = 
{
    FE_NEUTRAL,
	FE_BLINK,
	FE_WINK_LEFT,
	FE_WINK_RIGHT,
	FE_HORIEYE,
	FE_SURPRISE,
	FE_FROWN,
	FE_SMILE,
	FE_CLENCH,
	FE_LAUGH,
	FE_SMIRK_LEFT,
	FE_SMIRK_RIGHT
};

BCI::BCI(BCIMode mode)
{
	eEvent = IEE_EmoEngineEventCreate();
	eState = IEE_EmoStateCreate();
	if (mode == bci_mode_physical)
	{
		if (IEE_EngineConnect() == EDK_OK)
		{
			bci_device_in_use = bci_mode_physical;
			printf("Started physical BCI!\n");
			//Create structure to hold profile data
			EmoEngineEventHandle eProfile = IEE_ProfileEventCreate();
			//Attach base ("clean") profile to eProfile
			IEE_GetBaseProfile(eProfile);
		}
		else
		{
			printf("Unable to connect to physical BCI!\n");
			throw std::domain_error("Unable to connect to physical BCI!");
		}
	}
	else if (mode == bci_mode_virtual)
	{
		if (IEE_EngineRemoteConnect(address.c_str(), composerPort) == EDK_OK)
		{
			bci_device_in_use = bci_mode_virtual;
			printf("Started virtual BCI!\n");
			//Create structure to hold profile data
			EmoEngineEventHandle eProfile = IEE_ProfileEventCreate();
			//Attach base ("clean") profile to eProfile
			IEE_GetBaseProfile(eProfile);
		}
		else
		{
			printf("Unable to connect to virtual BCI!\n");
			throw std::domain_error("Unable to connect to virtual BCI!");
		}
	}
	else
	{
		printf("No BCI in use.\n");
	}
}

BCI::~BCI()
{
	delete ptr_cooldown_timer;
	delete ptr_doublewink_max_timer;
	delete ptr_doublewink_min_timer;
	IEE_EngineDisconnect();
	IEE_EmoStateFree(eState);
	IEE_EmoEngineEventFree(eEvent);
}

BCIMode BCI::WhatBCIIsConnected()
{
	return bci_device_in_use;
}

BCIEvent BCI::GetNextBCIEvent()
{
	state = IEE_EngineGetNextEvent(eEvent);
	//BCIEvent result = bci_event_none;
	if (state != EDK_NO_EVENT)
	{
		if (state == EDK_OK)
		{
			IEE_Event_t eventType = IEE_EmoEngineEventGetType(eEvent);
			//printf("Got event!\n");
			IEE_EmoEngineEventGetUserId(eEvent, &userID);
			//printf("User id is %i\n", userID);
			if (eventType == IEE_UserAdded)
			{
				printf("Added emotiv user %d\n", userID);
				return bci_event_user_added;
			}
			else if (eventType == IEE_EmoStateUpdated)
			{
				IEE_EmoEngineEventGetEmoState(eEvent, eState);
				//float upperFaceAmp = IS_FacialExpressionGetUpperFaceActionPower(eState);
				float lowerFaceAmp = IS_FacialExpressionGetLowerFaceActionPower(eState);
				//printf("State updated.\n");
				IEE_FacialExpressionAlgo_t upperFaceType =
					IS_FacialExpressionGetUpperFaceAction(eState);
				IEE_FacialExpressionAlgo_t lowerFaceType =
					IS_FacialExpressionGetLowerFaceAction(eState);
				//There are priorities in BCI readings handling.
				//Lowerface actions are placed on top, further actions are taken into account if there is no lowerface event.
				BCIEvent received_bci_event = bci_event_none;
				if (lowerFaceAmp > DETECTION_THRESHOLD)
				{
					received_bci_event = HandleLowerfaceExpression(lowerFaceType);
				}
				else
				{
					received_bci_event = HandleWink(eState);
				}

				if (ptr_cooldown_timer->CheckIfCountdownFinished())
				{
					ptr_cooldown_timer->ResetStartTime();
					return received_bci_event;
				}
				else
				{
					return bci_event_none;
				}
			}
			else if (eventType == IEE_FacialExpressionEvent)
			{
				IEE_FacialExpressionEvent_t facial_event_type = IEE_FacialExpressionEventGetType(eEvent);
				if (facial_event_type == IEE_FacialExpressionTrainingStarted)
				{
					Logger::Log("BCI training started!\n", debug_info);
					return bci_event_training_start;
				}
				else if (facial_event_type == IEE_FacialExpressionTrainingSucceeded)
				{
					Logger::Log("BCI training succeeded!\n", debug_info);
					return bci_event_training_success;
				}
				else if (facial_event_type == IEE_FacialExpressionTrainingFailed)
				{
					Logger::Log("BCI training failed!\n", debug_info);
					return bci_event_training_failed;
				}
				else if (facial_event_type == IEE_FacialExpressionTrainingCompleted)
				{
					Logger::Log("Signatures updated!\n", debug_info);
					return bci_event_training_completed;
				}
				else if (facial_event_type == IEE_FacialExpressionTrainingDataErased)
				{
					Logger::Log("Training data erased!\n", debug_info);
					return bci_event_training_erased;
				}
				else
				{
					//Logger::Log("Some other facial expression event.");
					return bci_event_none;
				}
			}
			else
			{
				//Logger::Log("Some other event type caught.");
				return bci_event_none;
			}
		}
		else
		{
			printf("Unexpected EmoEngineBehavior!\n");
			return bci_event_none;
		}
	}
	else
	{
		//printf("No event!\n");
		return bci_event_none;
	}
}

BCIEvent BCI::HandleUpperfaceExpression(IEE_FacialExpressionAlgo_t my_expression)
{
	return bci_event_none;
}

BCIEvent BCI::HandleWink(EmoStateHandle my_state)
{
	if (IS_FacialExpressionIsLeftWink(my_state) == 1 ||
		IS_FacialExpressionIsRightWink(my_state) == 1)
	{
		if (subsequent_wink_detections_recorded == 0)
		{
			ptr_doublewink_max_timer->ResetStartTime();
			ptr_doublewink_min_timer->ResetStartTime();
			subsequent_wink_detections_recorded++;
			return bci_event_none;
		}
		else
		{
			if (ptr_doublewink_max_timer->CheckIfCountdownFinished() == false)
			{
				if (ptr_doublewink_min_timer->CheckIfCountdownFinished() == true)
				{
					printf("Wink!\n");
					subsequent_wink_detections_recorded = 0;
					return bci_event_wink;
				}
				else
				{
					printf("Winking too fast.\n");
					return bci_event_none;
				}
			}
			else
			{
				printf("Winking too slow.\n");
				//Second event, which came too late becomes first event. We're still waiting for second to come.
				subsequent_wink_detections_recorded = 1;
				ptr_doublewink_max_timer->ResetStartTime();
				ptr_doublewink_min_timer->ResetStartTime();
				return bci_event_none;
			}
		}
	}
	else
	{
		return bci_event_none;
	}
}

BCIEvent BCI::HandleSpecificLowerfaceEvent(BCIEvent my_event)
{
	//Assigning value to pointer
	unsigned int* ptr_subsequent_lowerface_detections_recorded = nullptr;
	if (my_event == bci_event_smile)
	{
		printf("Evaluating smile.\n");
		ptr_subsequent_lowerface_detections_recorded = &subsequent_smile_detections_recorded;
	}
	else if (my_event == bci_event_clench)
	{
		printf("Evaluating clench.\n");
		ptr_subsequent_lowerface_detections_recorded = &subsequent_clench_detections_recorded;
	}
	else
	{
		Logger::Log("Unexpected lowerface event!");
		return bci_event_none;
	}
	//Processing lowerface event
	if (*ptr_subsequent_lowerface_detections_recorded == 0)
	{
		ptr_lowerface_max_timer->ResetStartTime();
		ptr_lowerface_min_timer->ResetStartTime();
		//Dirty trick with parentheses here!
		(*ptr_subsequent_lowerface_detections_recorded)++;
		printf("Timers reset for lowerface event.\n");
		return bci_event_none;
	}
	else
	{
		printf("There was a detection recorded before.\n");
		if (ptr_lowerface_max_timer->CheckIfCountdownFinished() == false)
		{
			printf("Time window still open.\n");
			if (ptr_lowerface_min_timer->CheckIfCountdownFinished() == true)
			{
				*ptr_subsequent_lowerface_detections_recorded = 0;
				printf("Lowerface event just in time!\n");
				return my_event;
			}
			else
			{
				printf("Lowerface event coming too fast.\n");
				return bci_event_none;
			}
		}
		else
		{
			printf("Lowerface event coming too slow.\n");
			//Second event, which came too late becomes first event. We're still waiting for second to come.
			*ptr_subsequent_lowerface_detections_recorded = 1;
			ptr_lowerface_max_timer->ResetStartTime();
			ptr_lowerface_min_timer->ResetStartTime();
			return bci_event_none;
		}
	}
}

BCIEvent BCI::HandleLowerfaceExpression(IEE_FacialExpressionAlgo_t my_expression)
{
	if (my_expression == FE_CLENCH)
	{
		printf("Will handle clench. Subsequent detections so far: %d.\n", subsequent_clench_detections_recorded);
		return HandleSpecificLowerfaceEvent(bci_event_clench);
	}
	else if (my_expression == FE_SMILE)
	{
		printf("Will handle smile. Subsequent detections so far: %d.\n", subsequent_smile_detections_recorded);
		return HandleSpecificLowerfaceEvent(bci_event_smile);
	}
	else
	{
		return bci_event_none;
	}
}

int BCI::SaveUserProfile()
{
	std::string path = "C:\\Users\\tmucha\\ppp.bci";
	int result = IEE_SaveUserProfile(0, path.c_str());
	printf("Result of profile saving is %d\n", result);
	return result;
}

//#TODO - usun¹æ duplikacje kodu
void BCI::TrainSmile()
{
	Logger::Log("Will attempt to start smile facial expression training for user: " + std::to_string(userID), 
		debug_info);
	int exit = (IEE_FacialExpressionSetTrainingAction(userID, FE_SMILE));
	if (exit != EDK_OK)
	{
		Logger::Log("Error while preparing smile training!", debug_info);
		throw("Error while preparing smile training!");
	}
	exit = IEE_FacialExpressionSetTrainingControl(userID, FE_START);
	if (exit != EDK_OK)
	{
		Logger::Log("Error while starting smile training!", debug_info);
		throw("Error while starting smile training!");
	}
}

void BCI::TrainNeutral()
{
	Logger::Log("Will attempt to start neutral facial expression training for user: " + std::to_string(userID),
		debug_info);
	int exit = (IEE_FacialExpressionSetTrainingAction(userID, FE_NEUTRAL));
	if (exit != EDK_OK)
	{
		Logger::Log("Error while preparing neutral training!", debug_info);
		throw("Error while preparing neutral training!");
	}
	exit = IEE_FacialExpressionSetTrainingControl(userID, FE_START);
	if (exit != EDK_OK)
	{
		Logger::Log("Error while starting neutral training!", debug_info);
		throw("Error while starting neutral training!");
	}
}

void BCI::TrainClench()
{
	Logger::Log("Will attempt to start clench facial expression training for user: " + std::to_string(userID),
		debug_info);
	int exit = (IEE_FacialExpressionSetTrainingAction(userID, FE_CLENCH));
	if (exit != EDK_OK)
	{
		Logger::Log("Error while preparing clench training!", debug_info);
		throw("Error while preparing clench training!");
	}
	exit = IEE_FacialExpressionSetTrainingControl(userID, FE_START);
	if (exit != EDK_OK)
	{
		Logger::Log("Error while starting clench training!", debug_info);
		throw("Error while starting clench training!");
	}
}

void BCI::AcceptTraining()
{
	int exit = (IEE_FacialExpressionSetTrainingControl(userID, FE_ACCEPT));
	if (exit != EDK_OK)
	{
		Logger::Log("Error while accepting training!", debug_info);
		throw("Error while accepting training!");
	}
}

void BCI::RejectTraining()
{
	int exit = (IEE_FacialExpressionSetTrainingControl(userID, FE_REJECT));
	if (exit != EDK_OK)
	{
		Logger::Log("Error while rejecting training!", debug_info);
		throw("Error while rejecting training!");
	}
}

bool BCI::TrySwitchingToTrainedSig()
{
	int result = IEE_FacialExpressionSetSignatureType(userID, FE_SIG_TRAINED);
	for (IEE_FacialExpressionAlgo_t expression : all_facial_expressions)
	{

	}
	if (result == EDK_FE_NO_SIG_AVAILABLE)
	{
		Logger::Log("No trained signature for user " + std::to_string(userID));
		return false;
	}
	else if (result != EDK_OK)
	{
		Logger::Log("Something went wrong when setting trained signature for user " + std::to_string(userID));
		return false;
	}
	else
	{
		Logger::Log("Trained signature successfully set for user " + std::to_string(userID));
		return true;
	}
}

void BCI::EraseAllTrainingData()
{
	for (IEE_FacialExpressionAlgo_t expression : all_facial_expressions)
	{
		int exit = IEE_FacialExpressionSetTrainingAction(userID, expression);
		if (exit != EDK_OK)
		{
			Logger::Log("Error while preparing training for erasing!: " + std::to_string(expression), debug_info);
			//throw("Error while preparing training for erasing!");
		}
		exit = IEE_FacialExpressionSetTrainingControl(userID, FE_ERASE);
		if (exit != EDK_OK)
		{
			Logger::Log("Error while erasing training!: " + std::to_string(expression), debug_info);
			throw("Error while erasing training!");
		}
	}
}


/*
	double theta = 0;
	double alpha = 0;
	double low_beta = 0;
	double high_beta = 0;
	double gamma = 0;

	if (ptr_emotiv_bands_check->CheckIfIntervalPassed())
	{
		IEE_GetAverageBandPowers(userID, IED_AF3, &theta, &alpha, &low_beta, &high_beta, &gamma);

		printf("Current band values: THETA: %f\n ALPHA: %f\n, LOW BETA: %f\n, HIGH_BETA: %f\n, GAMMA: %f\n.",
			theta, alpha, low_beta, high_beta, gamma);

	}
}*/

