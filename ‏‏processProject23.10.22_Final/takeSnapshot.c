#include "createSnap.h"
#include"logs.h"


time_t t;
struct tm* timeInfo;


struct SnapShot* oneSnapshot(int snapNum)
{
	char strSnapTime[50];
	time(&t);
	timeInfo = localtime(&t);
	//LogEvent("Working on Snapshot");
	struct SnapShot* snapToPut = readSnapshot();
	snapToPut->snapShotNum = snapNum;
	sprintf(strSnapTime, "Date:%d/%d/%d Time: %d:%d:%d \n", timeInfo->tm_mday, timeInfo->tm_mon + 1, timeInfo->tm_year + 1900, timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec);
	strcpy(snapToPut->snapShotTime, strSnapTime);

	return snapToPut;

}

void mergeSnaps(struct SnapShot* oldSnap, struct SnapShot* newSnap, int snapNum) {
	struct SnapShot* oldSnapHEAD= oldSnap->processList;
	struct SnapShot* newSnapHEAD = newSnap->processList;
	
	oldSnap->snapShotNum = snapNum;

	while (newSnap->processList)
	{
		while (oldSnap->processList)
		{ 
		if (strcmp(oldSnap->processList->processName, newSnap->processList->processName) == 0 && (oldSnap->processList->processID == newSnap->processList->processID))
		{

			if(newSnap->processList->countDll!=0){
			checkDllToAdd(oldSnap->processList->Dlllist, newSnap->processList->Dlllist, oldSnap->processList);
			combainProcessesData(oldSnap->processList, newSnap->processList);
			oldSnap->processList = oldSnapHEAD;
			break;
			}
		}
		if (oldSnap->processList->Pnext == NULL) {
			addNewProcessToList(oldSnap->processList, newSnap->processList);
			oldSnap->processList = oldSnap->processList->Pnext;
		}
			oldSnap->processList = oldSnap->processList->Pnext;
			
		}
		newSnap->processList = newSnap->processList->Pnext;
		oldSnap->processList = oldSnapHEAD;
	}
	oldSnap->processList = oldSnapHEAD;

}
