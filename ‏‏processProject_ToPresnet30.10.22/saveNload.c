#include "saveNload.h"

extern struct SnapShot* SnapshotListTotal;
t_headerFile fileHeader;
extern int snapNum;
int newSnapNup = 0;


void saving_file(char file_name[100], struct SnapShot* snapList)
{
	fileHeader.SItemsCount = snapNum-1 ;
	fileHeader.version = Version;
	FILE* f = fopen(file_name, "wb");
	if (!f)
	{
		LogError("The file did not open");
		exit(1);
	}
	fwrite(&fileHeader, sizeof(t_headerFile), 1, f);//writing the header into the file
	
	struct SnapShot* currS = snapList;
	while (currS != NULL)
	{
		fwrite(currS, sizeof(struct SnapShot), 1, f);//writing the snapshots into the file

		struct ProcessDetails* currP = currS->processList;
		while (currP != NULL) {
			fwrite(currP, sizeof(struct ProcessDetails), 1, f);//writing the process's into the file
			printf("%s\n", currP->processName);
	
			struct DllNames* currD = currP->Dlllist;

			while (currD != NULL)
			{
				fwrite(currD, sizeof(struct DllNames), 1, f);//writing the dll's into the file
				currD = currD->Dnext;
			}

			currP = currP->Pnext;
	}
		currS = currS->Snext;
	}
	fclose(f);
	
}

struct SnapShot* download_file(char file_name[100], struct SnapShot* currSnapshotlist)
{
	
	FILE* f = fopen(file_name, "rb");
	if (!f)
	{
		LogError("The file did not open");
		exit(1);
	}
	t_headerFile headeroffiel;
	int read = fread(&headeroffiel, sizeof(t_headerFile), 1, f);//reading the header into the header struct
	if (read == NULL)
	{
		LogError("The reading failed");
		return 1;
	}
	


	for (int i = 0; i < headeroffiel.SItemsCount; i++)
	{

		struct SnapShot* currS = NULL;
		currS = (struct SnapShot*)malloc(sizeof(struct SnapShot));
		read = fread(currS, sizeof(struct SnapShot), 1, f);//reading the snapshots into the SnapShot struct with all of it info

		if ((SnapshotListTotal!=NULL)&&(currS->processList == SnapshotListTotal->processList)) {
		//Mutual prevention - the case we on the same run, we saved the data into the file and didn't took any more snapshot
		//and the we load from the file - so we don't add it and also the reading from also the dll's and process's.
			return currS;
		}
		else { //otherwise we need to read the process's and the dll's seperatly
			
		struct ProcessDetails* currP = NULL;
		//printf("%s\n", currS->snapShotTime);
		addToProcessList(NULL);
		for (int j = 0; j < currS->numberOFprocess; j++)//reading the process's and dll's
		{
		
			currP = (struct ProcessDetails*)malloc(sizeof(struct ProcessDetails));
			read = fread(currP, sizeof(struct ProcessDetails), 1, f);//reafing the process's
			struct DllNames* currD = NULL;
			//printf("%s\n", currP->processName);
			addToDllList(NULL);
			for (int k = 0; k < currP->countDll;k++) {

				currD = (struct DllNames*)malloc(sizeof(struct DllNames));
				read = fread(currD, sizeof(struct DllNames), 1, f);//reading the dll's
				currP->Dlllist = addToDllList(currD);
				addToDllDictionary(currD->dllName, currP);//sending the loaded dll's to the dll's dictionary
			}

			currS->processList = addToProcessList(currP);

		}
		currS->snapShotNum = snapNum;
		
		currSnapshotlist = addToSnapShotList(currS);
		resetPandD();
		snapNum++;
		}
	}
	return currSnapshotlist;
}

	
	
