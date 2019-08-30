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
	BCIEvent result = bci_event_none;
	if (state != EDK_NO_EVENT)
	{
		if (state == EDK_OK)
		{
			//unsigned int userID = 0;
			//IEE_EmoEngineEventGetUserId(expressivEvent, &userID);
			//IEE_FacialExpressionEvent_t eventType =
			//	IEE_FacialExpressionEventGetType(expressivEvent);

			IEE_Event_t eventType = IEE_EmoEngineEventGetType(eEvent);
			//printf("Got event!\n");
			IEE_EmoEngineEventGetUserId(eEvent, &userID);
			//printf("User id is %i\n", userID);
			if (eventType == IEE_UserAdded)
			{
				printf("Added emotiv user\n");
				result = bci_event_user_added;
			}
			else if (eventType == IEE_EmoStateUpdated)
			{
				IEE_EmoEngineEventGetEmoState(eEvent, eState);
				float upperFaceAmp = IS_FacialExpressionGetUpperFaceActionPower(eState);
				float lowerFaceAmp = IS_FacialExpressionGetLowerFaceActionPower(eState);
				//printf("State updated.\n");
				IEE_FacialExpressionAlgo_t upperFaceType =
					IS_FacialExpressionGetUpperFaceAction(eState);
				IEE_FacialExpressionAlgo_t lowerFaceType =
					IS_FacialExpressionGetLowerFaceAction(eState);

				if (lowerFaceAmp > 0.5)
				{
					if (lowerFaceType == FE_CLENCH)
					{
						Logger::Log("Clench! Amplitude: " + std::to_string(lowerFaceAmp));
						result = bci_event_smile;
					}
					else if (lowerFaceType == FE_SMILE)
					{
						Logger::Log("Smile! Amplitude: " + std::to_string(lowerFaceAmp));
						result = bci_event_smile;
					}
				}
			}
			else if (eventType == IEE_FacialExpressionEvent)
			{
				IEE_FacialExpressionEvent_t facial_event_type = IEE_FacialExpressionEventGetType(eEvent);
				if (facial_event_type == IEE_FacialExpressionTrainingStarted)
				{
					Logger::Log("BCI training started!\n", debug_info);
					result = bci_event_training_start;
				}
				else if (facial_event_type == IEE_FacialExpressionTrainingSucceeded)
				{
					Logger::Log("BCI training succeeded!\n", debug_info);
					result = bci_event_training_success;
				}
				else if (facial_event_type == IEE_FacialExpressionTrainingFailed)
				{
					Logger::Log("BCI training failed!\n", debug_info);
					result = bci_event_training_failed;
				}
				else if (facial_event_type == IEE_FacialExpressionTrainingCompleted)
				{
					Logger::Log("Signatures updated!\n", debug_info);
					result = bci_event_training_completed;
				}
				else if (facial_event_type == IEE_FacialExpressionTrainingDataErased)
				{
					Logger::Log("Training data erased!\n", debug_info);
					result = bci_event_training_erased;
				}
			}
		}
		else
		{
			printf("Unexpected EmoEngineBehavior!\n");
		}
	}
	else
	{
		//printf("No event!\n");
	}
	return result;
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

void BCI::TrySwitchingToTrainedSig()
{
	;
}

void BCI::EraseAllTrainingData()
{
	for (IEE_FacialExpressionAlgo_t expression : all_facial_expressions)
	{
		int exit = IEE_FacialExpressionSetTrainingAction(userID, expression);
		if (exit != EDK_OK)
		{
			Logger::Log("Error while preparing training for erasing!: " + std::to_string(expression), debug_info);
			throw("Error while preparing training for erasing!");
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

