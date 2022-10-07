#include "userRespones.h"
#include "takeSnapshot.h"
#include <time.h>
struct SnapShot* SnapshotList=NULL;
struct SnapShot* CURRSnapshot=NULL;
time_t t;
struct tm* timeInfo;


void userRespones() {
	int snapnum = 1;
	char userResponse = NULL;
	char strSnapTime[50];
	
	while (userResponse != '8')
	{
		printf("please choose your action from the list:\n 1 - for take 1 SnapShot \n 2 - for take 20 second SnapShots \n 3 - to choose how long you want to take SnapShot \n 4 - to genetare HTML reports \n 5 - reset collection \n 6 - save in file \n 7 - load from file \n 8 - to quite the program");
		userResponse = getch();

		switch (userResponse)
		{
		case '1': 
			time(&t);
			timeInfo = localtime(&t);
			CURRSnapshot = NULL;
			CURRSnapshot = oneSnapshot();
			CURRSnapshot->snapShotNum = snapnum;
			sprintf(strSnapTime, "Date:%d/%d/%d Time: %d:%d:%d \n", timeInfo->tm_mday, timeInfo->tm_mon + 1, timeInfo->tm_year + 1900, timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec);
			strcpy(CURRSnapshot->snapShotTime,strSnapTime);
			SnapshotList = addToSnapShotList(CURRSnapshot);
			LogEvent("snap is completed and added to the list\n");
			snapnum++, PrintAllList();
			//resetSnapshot();
			break;
			struct snapshot snapshow_case_1;
			snapshow_case_1.snapShotNum = snapnum;
			break;

		case '2':time(&t);
			timeInfo = localtime(&t);
			CURRSnapshot = NULL;
			CURRSnapshot = Snap20shot();
			CURRSnapshot->snapShotNum = snapnum;
			sprintf(strSnapTime, " Date: %d/%d/%d Time: %d:%d:%d \n", timeInfo->tm_mday, timeInfo->tm_mon + 1, timeInfo->tm_year + 1900, timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec);
			strcpy(CURRSnapshot->snapShotTime, strSnapTime);
			SnapshotList = addToSnapShotList(CURRSnapshot);
			LogEvent("20 snaps is completed and added to the list\n");
			snapnum++;
			break;

		case '3':
			break;

		case ':': 
			break;

		case '8': printf("Thank you for use our Great service. See you next time. Drink only water\n");
			break;
			
		default:
			printf("Wrong selection. Try be more sharp\n");
			break;
		}
		
	}

}