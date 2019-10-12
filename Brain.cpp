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
				if (lowerFaceAmp > DETECTION_THRESHOLD)
				{
					if (ptr_cooldown_timer->CheckIfCountdownFinished())
					{
						ptr_cooldown_timer->ResetStartTime();
						return HandleLowerfaceExpression(lowerFaceType);
					}
					else
					{
						Logger::Log("Cooldown not passed yet! Remaining: " + std::to_string(ptr_cooldown_timer->HowManyMilisecondsLeftTillEnd()) + " ms\n");
						return bci_event_none;
					}
				}
				else if (ptr_cooldown_timer->CheckIfCountdownFinished())
				{
					return HandleWink(eState);
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
		Logger::Log("Wink candidate!\n");
		subsequent_wink_detections_recorded++;
		if (subsequent_wink_detections_recorded >= SUBSEQUENT_FACIAL_EXPRESSION_DETECTIONS_NEEDED)
		{
			subsequent_wink_detections_recorded = 0;
			return bci_event_wink;
		}
		else
		{
			return bci_event_none;
		}
	}
	else
	{
		subsequent_wink_detections_recorded = 0;
		return bci_event_none;
	}
}

BCIEvent BCI::HandleLowerfaceExpression(IEE_FacialExpressionAlgo_t my_expression)
{
	if (my_expression == FE_CLENCH)
	{
		if (last_detected_lowerface_expression == FE_CLENCH)
		{
			subsequent_lowerface_detections_recorded++;
			if (subsequent_lowerface_detections_recorded >= SUBSEQUENT_FACIAL_EXPRESSION_DETECTIONS_NEEDED)
			{
				Logger::Log("Clench!");
				return bci_event_clench;
			}
			else
			{
				return bci_event_none;
			}
		}
		else
		{
			last_detected_lowerface_expression = FE_CLENCH;
			subsequent_lowerface_detections_recorded = 0;
			return bci_event_none;
		}
	}
	else if (my_expression == FE_SMILE)
	{
		if (last_detected_lowerface_expression == FE_SMILE)
		{
			subsequent_lowerface_detections_recorded++;
			if (subsequent_lowerface_detections_recorded >= SUBSEQUENT_FACIAL_EXPRESSION_DETECTIONS_NEEDED)
			{
				subsequent_lowerface_detections_recorded = 0;
				return bci_event_smile;
			}
			else
			{
				return bci_event_none;
			}
		}
		else
		{
			last_detected_lowerface_expression = FE_SMILE;
			subsequent_lowerface_detections_recorded = 0;
			return bci_event_none;
		}
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

