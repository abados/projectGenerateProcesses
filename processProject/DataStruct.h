#include <stdio.h>
#include <conio.h>
#include <stdlib.h> 
#include <string.h>
#include<Windows.h>
#include<psapi.h>
#include "logs.h"
#pragma once
#pragma warning(disable:4996)

struct DllNames {
	char dllName[MAX_PATH];
	struct DllNames* Dnext;
	struct DllNames* Dprev;
};

struct ProcessDetails
{
	char processName[MAX_PATH];
	int processID;
	int countDll;
	PROCESS_MEMORY_COUNTERS pmc;
	struct DllNames* Dlllist;
	struct ProcessDetails* Pnext;
	struct ProcessDetails* Pprev;
};

struct SnapShot
{
	int snapShotNum;
	char snapShotTime[1000];
	int numberOFprocess;
	struct ProcessDetails* processList;
	struct SnapShot* Snext;
	struct SnapShot* Sprev;
};


struct SnapShot* addToSnapShotList(struct SnapShot*);

struct DllNames* addToDllList(struct DllNames*);
void addNewDllToList(struct DllNames*, struct DllNames*);

struct ProcessDetails* addToProcessList(struct ProcessDetails*);

void addNewProcessToList(struct ProcessDetails* listToAdd, struct ProcessDetails* processToAdd);

void addNewDllToList(struct DllNames* oldDllList, struct DllNames* newDllList);

void PrintSList();
void PrintPList();
void PrintDList();
void PrintAllList();


