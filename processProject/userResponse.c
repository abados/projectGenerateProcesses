#include "userRespones.h"

struct SnapShot* SnapshotListTotal=NULL;

time_t t;
struct tm* timeInfo;
int stopKey = 1;

void userRespones() {
	int snapNum = 1;
	char userResponse = NULL;
	struct SnapShot* CURRSnapshotToAdd = NULL;
	struct SnapShot* CURRSnapshotToTemp = NULL;
	struct SnapShot* SUMSnapshotToTemp = NULL;
	int flagFirstSnap = 0;
	while (userResponse != '8')
	{
		printf("please choose your action from the list:\n 1 - for take 1 SnapShot \n 2 - for take 20 second SnapShots \n 3 - start Snapshots until you push the keyboard. \n 4 - to genetare HTML reports \n 5 - reset collection \n 6 - save in file \n 7 - load from file \n 8 - to quite the program\n");
		userResponse = getch();
		
		switch (userResponse)
		{
		case '1':
			CURRSnapshotToAdd = oneSnapshot(snapNum);
			SnapshotListTotal = addToSnapShotList(CURRSnapshotToAdd);
			LogEvent("snap is completed and added to the list\n");
			snapNum++, PrintAllList();
			resetSnapshot();
			flagFirstSnap = 1;
			break;
			
			break;

		case '2':
			SUMSnapshotToTemp = oneSnapshot(snapNum);
			SnapshotListTotal = addToSnapShotList(SUMSnapshotToTemp);
			resetSnapshot();
			Sleep(1000);
			for (int i = 2; i < 21; i++)
			{
				CURRSnapshotToTemp = oneSnapshot(snapNum);
				resetSnapshot();
				mergeSnaps(SUMSnapshotToTemp, CURRSnapshotToTemp, snapNum);
				printf("snap num is%d", i);
				LogEvent("specific num completed");
				Sleep(1000);
			}
			LogEvent("snap is completed and added to the list\n");
			snapNum++;
			PrintAllList();
			resetSnapshot();
			flagFirstSnap = 1;
			
			
			/*time(&t);
			timeInfo = localtime(&t);
			CURRSnapshot = NULL;
			CURRSnapshot = Snap20shot();
			CURRSnapshot->snapShotNum = snapnum;
			sprintf(strSnapTime, " Date: %d/%d/%d Time: %d:%d:%d \n", timeInfo->tm_mday, timeInfo->tm_mon + 1, timeInfo->tm_year + 1900, timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec);
			strcpy(CURRSnapshot->snapShotTime, strSnapTime);
			SnapshotList = addToSnapShotList(CURRSnapshot);
			LogEvent("20 snaps is completed and added to the list\n");
			snapnum++;*/
			break;

		case '3': 
			stopKey = 1;
			while (stopKey)
			{ 
				SUMSnapshotToTemp = oneSnapshot(snapNum);
				SnapshotListTotal = addToSnapShotList(SUMSnapshotToTemp);
				resetSnapshot();

				if (!kbhit())
				{
						CURRSnapshotToTemp = oneSnapshot(snapNum);
						resetSnapshot();
						mergeSnaps(SUMSnapshotToTemp, CURRSnapshotToTemp, snapNum);
						//printf("snap num is%d", i);
						LogEvent("specific num completed");
						Sleep(1000);

				}
				else {
					stopKey = 0;
				}
			}
			
			LogEvent("snap is completed and added to the list\n");
			snapNum++;
			PrintAllList();
			resetSnapshot();
			flagFirstSnap = 1;
			
			
			break;

		case '4': 
			break;
		case '5':
			break;
		case '6':
			break;
		case '7':
			break;

		case '8': printf("Thank you for use our Great service. See you next time. Drink only water\n");
			break;
			
		default:
			printf("Wrong selection. Try be more sharp\n");
			break;
		}
		
	}

}