#include "cvidll.h"
#include <userint.h>
#include "license.h"

int CVICALLBACK On_PW_OK (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:

			MyDLL_License_PW_Update ();
			
			break;
	}
	return 0;
}

int CVICALLBACK On_PW_Close (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:

			MyDLL_License_PW_Close ();
			
			break;
	}
	return 0;
}

int CVICALLBACK On_Login_PW (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:

			MyDLL_License_PW_Open ();
			
			break;
	}
	return 0;
}

int CVICALLBACK On_PW_Entry (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int myKey;
	switch (event)
		{
		case EVENT_KEYPRESS:
			myKey = GetKeyPressEventVirtualKey (eventData2);
   			if (myKey==1280)//"Enter"
			MyDLL_License_PW_Entry (control);
			break;
		}
	return 0;
}

int CVICALLBACK On_PW_Entry2 (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int myKey;
	switch (event)
		{
		case EVENT_KEYPRESS:
			myKey = GetKeyPressEventVirtualKey (eventData2);
   			if (myKey==1280)//"Enter"
				On_PW_OK (panel, PAN_PW_BUT_OK, EVENT_COMMIT, 0, 0, 0);
			break;
		}
	return 0;
}

int CVICALLBACK On_Login_Username (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int myKey;
	switch (event)
		{
		case EVENT_KEYPRESS:
			myKey = GetKeyPressEventVirtualKey (eventData2);
   			if (myKey==1280)//"Enter"
				MyDLL_License_Entry_Username();
			break;
		}
	return 0;
}

