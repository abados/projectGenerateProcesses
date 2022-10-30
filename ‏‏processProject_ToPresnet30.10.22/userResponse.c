#include "userRespones.h"

extern struct SnapShot* SnapshotListTotal=NULL;
struct SnapShot* SnapshotListFromFILE = NULL;
struct SnapShot* savedOldLIstHead = NULL;
//time_t t;
//struct tm* timeInfo;
int stopKey = 1;
extern int snapNum = 1;


void userRespones() {
	
	char userResponse = NULL;
	struct SnapShot* CURRSnapshotToAdd = NULL;
	struct SnapShot* CURRSnapshotToTemp = NULL;
	struct SnapShot* SUMSnapshotToTemp = NULL;
	int flagResetLists = 0; // - flag to know when we need to reset pointers and start a new list.
	while (userResponse != '9')
	{
		printf("please choose your action from the list:\n 1 - for take 1 SnapShot \n 2 - for take 20 second SnapShots \n 3 - start Snapshots until you push '4' in the keyboard. \n 5 - to genetare HTML reports \n 6 - reset collection \n 7 - save in file \n 8 - load from file \n 9 - to quite the program\n");
		userResponse = getch();
		
		switch (userResponse)
		{
			
		case '1':
				LogEvent("User choose number 1 - To build one snapShot");
				if (flagResetLists == 1) // - flag to know when we did reset and need to reset pointers and start a new list.
				{
					CURRSnapshotToAdd = NULL;
					SnapshotListTotal = NULL;
					snapNum = 1;
					resetAllHeads();
					flagResetLists = 0;
				}
				CURRSnapshotToAdd = oneSnapshot(snapNum); // making a new snapShot
				SnapshotListTotal = addToSnapShotList(CURRSnapshotToAdd);// adding the new snapShot to the list
				snapNum++; // increase the snapShot number.
				//addToProcessDictionary(SnapshotListTotal); //optionl - sending after every snapshot, or just before we generate HTML
				resetPandD(); //reseting the global heads and tails of the strcuts, before we making new lists.
			
		
			break;

		case '2':
			LogEvent("User choose number 2 - To combine 20 snapShot to 1");
			SUMSnapshotToTemp = oneSnapshot(snapNum);
			SnapshotListTotal = addToSnapShotList(SUMSnapshotToTemp);
			resetPandD();
			Sleep(1000);
			for (int i = 0; i < 19; i++)
			{
				CURRSnapshotToTemp = oneSnapshot(snapNum);
				resetPandD();
				mergeSnaps(SUMSnapshotToTemp, CURRSnapshotToTemp, snapNum);
				Sleep(1000);
			}
			snapNum++;
			//addToProcessDictionary(SnapshotListTotal);
			resetPandD();
		

			break;

		case '3': 
			LogEvent("User choose number 3 - To combine many snapShot as the user want until he want to quit");
			stopKey = 1;
			SUMSnapshotToTemp = oneSnapshot(snapNum);
			SnapshotListTotal = addToSnapShotList(SUMSnapshotToTemp);
			resetPandD();
			while (stopKey)
			{ 
				if (!kbhit())
				{
						CURRSnapshotToTemp = oneSnapshot(snapNum);
						resetPandD();
						mergeSnaps(SUMSnapshotToTemp, CURRSnapshotToTemp, snapNum);
						Sleep(1000);
				}
				else {

					char input = getch();
					if (input == '4') {
						stopKey = 0;
					}
					
				}
			}
			snapNum++;
			//addToProcessDictionary(SnapshotListTotal);
			resetPandD();
			
			break;
			

		case '5':	LogEvent("User choose number 5 - To generatr Html Pages");
					if(SnapshotListTotal){
					addToProcessDictionary(SnapshotListTotal);
					markTheBiggestProcess(SnapshotListTotal);
					sortprocessBydllCount(SnapshotListTotal);
					handle_HOME_PAGE(SnapshotListTotal);
					WRITE_SAMPLE_PAGES(SnapshotListTotal);
					WRITE_DLL_PAGES();
					}
					else {
						printf("the user have not data to present");
						handle_HOME_PAGE(SnapshotListTotal);
					}
			 
			break;
		case '6':
			LogEvent("User choose number 6 - To reset the program");
			freeMemSnap(SnapshotListTotal);
			_CrtDumpMemoryLeaks(); //checking the allocate memory status
			flagResetLists = 1;
			CURRSnapshotToAdd = NULL;
			SnapshotListTotal = NULL;
			snapNum = 1;
			resetAllHeads();
			break;
		case '7': LogEvent("User choose number 7 - To save file");
			saving_file("file1.bin", SnapshotListTotal);
			break;

		case '8':
			LogEvent("User choose number 8 - To load file");
			savedOldLIstHead = SnapshotListTotal;
			resetPandD();
			SnapshotListTotal =download_file("file1.bin", SnapshotListTotal);
			addToProcessDictionary(SnapshotListTotal);
			
			break;

		case '9': LogEvent("User choose number 9 to Exit");
			printf("Thank you for use our Great service. See you next time.\n");
			break;
			
		default:
			printf("Wrong selection. Try be more sharp\n");
			break;
		}
		
	}

}