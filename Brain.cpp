#include <Brain.hpp>

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
	return result;
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