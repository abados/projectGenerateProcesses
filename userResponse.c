#include "userRespones.h"

extern struct SnapShot* SnapshotListTotal=NULL;
struct SnapShot* SnapshotListFromFILE = NULL;
struct SnapShot* savedOldLIstHead = NULL;
time_t t;
struct tm* timeInfo;
int stopKey = 1;
extern int snapNum = 1;

void userRespones() {
	
	char userResponse = NULL;
	struct SnapShot* CURRSnapshotToAdd = NULL;
	struct SnapShot* CURRSnapshotToTemp = NULL;
	struct SnapShot* SUMSnapshotToTemp = NULL;
	int flagFirstSnap = 0;
	int flagResetLists = 0;
	int flagLoadList = 0;
	int userChose=0;
	while (userResponse != '9')
	{
		printf("please choose your action from the list:\n 1 - for take 1 SnapShot \n 2 - for take 20 second SnapShots \n 3 - start Snapshots until you push '4' in the keyboard. \n 5 - to genetare HTML reports \n 6 - reset collection \n 7 - save in file \n 8 - load from file \n 9 - to quite the program\n");
		userResponse = getch();
		
		switch (userResponse)
		{
		case '1':
			
				if (flagResetLists == 1)
				{
					CURRSnapshotToAdd = NULL;
					SnapshotListTotal = NULL;
					snapNum = 1;
					resetAllHeads();
					flagResetLists = 0;
				}
				CURRSnapshotToAdd = oneSnapshot(snapNum);
				SnapshotListTotal = addToSnapShotList(CURRSnapshotToAdd);
				LogEvent("snap is completed and added to the list\n");
				snapNum++, PrintAllList();
				resetPandD();
				flagFirstSnap = 1;
				//_CrtDumpMemoryLeaks(); checking the allocate memory status
		
			break;

		case '2':
			SUMSnapshotToTemp = oneSnapshot(snapNum);
			SnapshotListTotal = addToSnapShotList(SUMSnapshotToTemp);
			resetPandD();
			Sleep(1000);
			for (int i = 2; i < 21; i++)
			{
				CURRSnapshotToTemp = oneSnapshot(snapNum);
				resetPandD();
				mergeSnaps(SUMSnapshotToTemp, CURRSnapshotToTemp, snapNum);
				printf("snap num is%d", i);
				LogEvent("specific num completed");
				Sleep(1000);
			}
			LogEvent("snap is completed and added to the list\n");
			snapNum++;
			PrintAllList();
			resetPandD();
			flagFirstSnap = 1;

			break;

		case '3': 
			stopKey = 1;
			while (stopKey)
			{ 
				SUMSnapshotToTemp = oneSnapshot(snapNum);
				SnapshotListTotal = addToSnapShotList(SUMSnapshotToTemp);
				resetPandD();
				if (!kbhit())
				{
						CURRSnapshotToTemp = oneSnapshot(snapNum);
						resetPandD();
						mergeSnaps(SUMSnapshotToTemp, CURRSnapshotToTemp, snapNum);
						//printf("snap num is%d", i);
						LogEvent("specific num completed");
						Sleep(1000);
				}
				else {

					char input = getch();
					if (input == '4') {
						stopKey = 0;
					}
					
				}
			}
			LogEvent("snap is completed and added to the list\n");
			snapNum++;
			PrintAllList();
			resetPandD();
			flagFirstSnap = 1;
			break;

		case '5'://HTML
			 
			break;
		case '6':
			freeMemSnap(SnapshotListTotal);
			_CrtDumpMemoryLeaks(); //checking the allocate memory status
			flagResetLists = 1;
			CURRSnapshotToAdd = NULL;
			SnapshotListTotal = NULL;
			snapNum = 1;
			resetAllHeads();
			break;
		case '7': saving_file("file1.bin", SnapshotListTotal);
			break;

		case '8':
			flagLoadList = 1;
			savedOldLIstHead = SnapshotListTotal;
			resetPandD();
			SnapshotListTotal =download_file("file1.bin", SnapshotListTotal);
	
			
			break;

		case '9': printf("Thank you for use our Great service. See you next time. Drink only water\n");
			break;
			
		default:
			printf("Wrong selection. Try be more sharp\n");
			break;
		}
		
	}

}