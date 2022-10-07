#include "takeSnapshot.h"
#include"logs.h"

struct SnapShot* oneSnapshot()
{
	struct SnapShot* snap = NULL;
	snap=GetProcessesInfo(snap);
	if (snap == NULL)
		LogError(strerror(GetLastError()));
	else return snap;
	
	//return snap = GetProcessesInfo(snap);
}

struct SnapShot* Snap20shot()
{
	struct SnapShot* newSnap = NULL;
	struct SnapShot* sumSnap = NULL;
	sumSnap = GetProcessesInfo(sumSnap);
	for (int i = 0; i < 19; i++) {
		printf("\nsnap %d out of 20 snaps\n", i);
		newSnap = oneSnapshot();
		sumSnap = sumSample(sumSnap, newSnap);
		LogEvent("snap sleep started");
		Sleep(1000);
		LogEvent("snap sleep ended");
	}
	if (sumSnap == NULL)
		LogError(strerror(GetLastError()));
	else return sumSnap;
}

struct SnapShot* sumSample(struct SnapShot* sumSnap, struct SnapShot* newSnap) {

	struct SnapShot* tempSS = sumSnap;
	struct SnapShot* tempNS = newSnap;
	LogEvent("ENTER to the Merge\n");
	printf("we are here with snapshot number %p and snanshot number %p", tempSS, tempNS);

	
	
}