#include "saveNload.h"
extern struct SnapShot* SnapshotListTotal;
t_headerFile fileHeader;
extern int snapNum;
void saving_file(char file_name[100], struct SnapShot* snapList)
{
	fileHeader.SItemsCount = snapNum-1 ;
	fileHeader.version = Version;
	FILE* f = fopen(file_name, "wb");
	if (!f)
	{
		//logerror
		printf("The file did not open");
		exit(1);
	}
	fwrite(&fileHeader, sizeof(t_headerFile), 1, f);
	
	struct SnapShot* currS = snapList;
	while (currS != NULL)
	{
		fwrite(currS, sizeof(struct SnapShot), 1, f);
		//fwrite(&currS->snapShotNum, sizeof(int), 1, f);
		//fwrite(currS->snapShotTime, sizeof(currS->snapShotTime), 1, f);
		//fwrite(&currS->numberOFprocess, sizeof(int), 1, f);

		struct ProcessDetails* currP = currS->processList;
		while (currP != NULL) {
			fwrite(currP, sizeof(struct ProcessDetails), 1, f);
			printf("%s\n", currP->processName);
			//fwrite(currP->processName, sizeof(MAX_PATH), 1, f);
			//fwrite(&currP->processID, sizeof(int), 1, f);
			//fwrite(&currP->countDll, sizeof(int), 1, f);
			//fwrite(&currP->pmc.PageFaultCount, sizeof(DWORD), 1, f);
			//fwrite(&currP->pmc.WorkingSetSize, sizeof(SIZE_T), 1, f);
			//fwrite(&currP->pmc.QuotaPagedPoolUsage, sizeof(SIZE_T), 1, f);
			//fwrite(&currP->pmc.QuotaPeakNonPagedPoolUsage, sizeof(SIZE_T), 1, f);
			//fwrite(&currP->pmc.PagefileUsage, sizeof(SIZE_T), 1, f);

			struct DllNames* currD = currP->Dlllist;

			while (currD != NULL)
			{
				fwrite(currD, sizeof(struct DllNames), 1, f);
				//fwrite(currD->dllName, sizeof(MAX_PATH), 1, f);
				//fwrite(&currD->Dnext, sizeof(struct DllNames*), 1, f);
				//fwrite(&currD->Dprev, sizeof(struct DllNames*), 1, f);
				currD = currD->Dnext;
			}

			//fwrite(&currP->Pnext, sizeof(struct ProcessDetails*), 1, f);
			//fwrite(&currP->Pprev, sizeof(struct ProcessDetails*), 1, f);
			currP = currP->Pnext;

	}
		//fwrite(&currS->Snext, sizeof(struct SnapShot*), 1, f);
		//fwrite(&currS->Sprev, sizeof(struct SnapShot*), 1, f);
		currS = currS->Snext;

	}
	fclose(f);
	
}

struct SnapShot* download_file(char file_name[100])
{
	
	struct SnapShot* currSnapshotlist = NULL;
	//addToSnapShotList(currSnapshotlist);
	int flagFromAnotherRun = 0;
	FILE* f = fopen(file_name, "rb");
	if (!f)
	{
		printf("The file did not open");
		exit(1);
	}
	t_headerFile headeroffiel;
	int read = fread(&headeroffiel, sizeof(t_headerFile), 1, f);
	if (read == NULL)
	{
		return 1;
	}
	printf("%d  %d", headeroffiel.SItemsCount, headeroffiel.version);


	for (int i = 0; i < headeroffiel.SItemsCount; i++)
	{
		//fileHeader.SItemsCount
		struct SnapShot* currS = NULL;
		//struct ProcessDetails* currProcesslist = NULL;
		
		currS = (struct SnapShot*)malloc(sizeof(struct SnapShot));
		read = fread(currS, sizeof(struct SnapShot), 1, f);

		if ((SnapshotListTotal!=NULL)&&(currS->processList == SnapshotListTotal->processList)) {//????? ?????
			
			return currS;

		}
		else {
			
		struct ProcessDetails* currP = NULL;
		printf("%s\n", currS->snapShotTime);
		for (int j = 0; j < currS->numberOFprocess; j++)
		{
		
			currP = (struct ProcessDetails*)malloc(sizeof(struct ProcessDetails));
			read = fread(currP, sizeof(struct ProcessDetails), 1, f);
			struct DllNames* currD = NULL;
			//printf("%s\n", currP->processName);

			for (int k = 0; k < currP->countDll;k++) {

				currD = (struct DllNames*)malloc(sizeof(struct DllNames));
				read = fread(currD, sizeof(struct DllNames), 1, f);
				currP->Dlllist = addToDllList(currD);
			}

			currS->processList = addToProcessList(currP);

		}
		
		currSnapshotlist = addToSnapShotList(currS);
		}
	}
	return currSnapshotlist;
}

	
	
