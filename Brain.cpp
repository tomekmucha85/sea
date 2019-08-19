#include <Brain.hpp>

unsigned int BCI::userID = 0;

BCI::BCI(BCIMode mode)
{
	eEvent = IEE_EmoEngineEventCreate();
	eState = IEE_EmoStateCreate();
	if (mode == bci_physical)
	{
		if (IEE_EngineConnect() == EDK_OK)
		{
			bci_device_in_use = bci_physical;
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
	else if (mode == bci_virtual)
	{
		if (IEE_EngineRemoteConnect(address.c_str(), composerPort) == EDK_OK)
		{
			bci_device_in_use = bci_virtual;
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

std::string BCI::GetNextBCIEvent()
{
	state = IEE_EngineGetNextEvent(eEvent);
	std::string result = "";
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
						result = "clench";
					}
					else if (lowerFaceType == FE_SMILE)
					{
						Logger::Log("Smile! Amplitude: " + std::to_string(lowerFaceAmp));
						result = "smile";
					}
				}
			}
			else if (eventType == IEE_FacialExpressionEvent)
			{
				IEE_FacialExpressionEvent_t facial_event_type = IEE_FacialExpressionEventGetType(eEvent);
				if (facial_event_type == IEE_FacialExpressionTrainingStarted)
				{
					Logger::Log("BCI training started!\n", debug_info);
				}
				else if (facial_event_type == IEE_FacialExpressionTrainingSucceeded)
				{
					Logger::Log("BCI training succeeded!\n", debug_info);
				}
				else if (facial_event_type == IEE_FacialExpressionTrainingFailed)
				{
					Logger::Log("BCI training failed!\n", debug_info);
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

void BCI::ResetTrainingData()
{
	;
}

/*
EmoEngineEventHandle eEvent = IEE_EmoEngineEventCreate();
EmoStateHandle eState = IEE_EmoStateCreate();

unsigned int userID = 0;
//const unsigned short composerPort = 1726;
int state = 0;
std::string  address = "127.0.0.1";

bool is_connected_to_emo = false;
*/


/*if (IEE_EngineConnect() == EDK_OK)
{
	is_connected_to_emo = true;
	printf("Successfully connected to Emotiv device!\n");
}*/
/*else if (IEE_EngineRemoteConnect(address.c_str(), composerPort) == EDK_OK)
{
	is_connected_to_emo = true;
	printf("Connected to EmoComposer!\n");
}
else
{
	std::string errMsg = "Cannot connect neither to Emotiv device nor to EmoComposer on [" +
		address + "]";
	printf("Cannot connect to EmoComposer!\n");
	//throw std::runtime_error(errMsg.c_str());
	printf("WARNING!\nDID NOT CONNECT TO EMO ENGINE.\n WARNING!\n");
}*/



//IEE_FacialExpressionGetTrainedSignatureAvailable()


//WITHIN MAIN LOOP

/*
if (is_connected_to_emo)
{
	//printf("Connected to emo.\n");
	state = IEE_EngineGetNextEvent(eEvent);
	if (state != EDK_NO_EVENT)
	{
		if (state == EDK_OK)
		{
			IEE_Event_t eventType = IEE_EmoEngineEventGetType(eEvent);
			//printf("Got event!\n");
			//IEE_EmoEngineEventGetUserId(eEvent, &userID);
			if (eventType == IEE_EmoStateUpdated)
			{
				IEE_EmoEngineEventGetEmoState(eEvent, eState);
				float upperFaceAmp = IS_FacialExpressionGetUpperFaceActionPower(eState);
				float lowerFaceAmp = IS_FacialExpressionGetLowerFaceActionPower(eState);
				//printf("State updated.\n");
				IEE_FacialExpressionAlgo_t upperFaceType =
					IS_FacialExpressionGetUpperFaceAction(eState);
				IEE_FacialExpressionAlgo_t lowerFaceType =
					IS_FacialExpressionGetLowerFaceAction(eState);

				if (lowerFaceAmp > 0)
				{
					if (lowerFaceType == FE_CLENCH)
					{
						printf("Clench!\n");
					}
					else if (lowerFaceType == FE_SMILE)
					{
						printf("Smile\n");
					}
				}

			}
			//
			else if (eventType == IEE_UserAdded || eventType == IEE_UserRemoved)
			{
				printf("Got user related event.\n");
			}
			else if (eventType == IEE_InternalStateChanged)
			{
				printf("Internal state change.\n");
			}
			else if (eventType == IEE_ProfileEvent)
			{
				printf("Profile event.\n");
			}
			else if (eventType == IEE_MentalCommandEvent)
			{
				printf("Mental command.\n");
			}
			else if (eventType == IEE_FacialExpressionEvent)
			{
				printf("Facial expression event.\n");
			}
			else
			{
				printf("Some other event.\n");
			}
		}
		else
		{
			printf("Unexpected EmoEngineBehavior!\n");
		}

		//state = IEE_EngineGetNextEvent(eEvent);
	}
	else
	{
		//printf("No event!\n");
	}
}
*/

//OTHER VARIANT

/*if (is_connected_to_emo)
{
	// New event needs to be handled
	if (state == EDK_OK)
	{
		IEE_Event_t eventType = IEE_EmoEngineEventGetType(eEvent);
		IEE_EmoEngineEventGetUserId(eEvent, &userID);
		// Log the EmoState if it has been updated
		if (eventType == IEE_EmoStateUpdated)
		{
			IEE_EmoEngineEventGetEmoState(eEvent, eState);
			float timestamp = IS_GetTimeFromStart(eState);
			//printf("Got update at %f!\n", timestamp);
			if (IS_FacialExpressionIsLeftWink(eState) == 1)
			{
				printf("Wink!\n");
				if (cooldown == 0)
				{
					printf("Wink!\n");
					Creature::ptr_current_main_charater->CastSpell(spell_vortex);
					cooldown = 200;
				}
			}
			else if (IS_FacialExpressionIsLookingLeft(eState) == 1)
			{
				printf("Looking left.\n");
			}
			else if (IS_FacialExpressionIsEyesOpen(eState) == 0)
			{
				printf("Eyes closed.\n");
			}
		}
	}
	else if (state != EDK_NO_EVENT)
	{
		std::cout << "Internal error in Emotiv Engine!" << std::endl;
		break;
	}
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





//Quit Emotiv subsystem
/*if (is_connected_to_emo)
{
	IEE_EngineDisconnect();
	IEE_EmoStateFree(eState);
	IEE_EmoEngineEventFree(eEvent);
}*/