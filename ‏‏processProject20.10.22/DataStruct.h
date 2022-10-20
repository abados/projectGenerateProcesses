#pragma once
//#ifndef _dataStructHeader
//#define _dataStructHeader

#include "logs.h"
#pragma warning(disable:4996)


struct DllNames {
	char dllName[MAX_PATH];
	struct DllNames* Dnext;
	struct DllNames* Dprev;
};

struct ProcessDetails
{
	PROCESS_MEMORY_COUNTERS pmc;
	int processID;
	char processName[MAX_PATH];
	struct DllNames* Dlllist;
	int countDll;
	struct ProcessDetails* Pnext;
	struct ProcessDetails* Pprev;
};

struct SnapShot
{
	struct ProcessDetails* processList;
	int numberOFprocess;
	int numberOFDlls;
	unsigned long avgMem;
	char snapShotTime[50];
	int snapShotNum;
	struct SnapShot* Snext;
	struct SnapShot* Sprev;
};

typedef struct headerFile{
	int version;
	int serialNum;
	int SItemsCount;
	char reserve[100];

} t_headerFile;

typedef struct DllDictionary {
	char key[MAX_PATH];
	struct ProcessDictionary* processList_value;
	struct DllDictionary* next;
	struct DllDictionary* prev;
}DllDictionary;

typedef struct ProcessDictionary {
	char key[MAX_PATH];
	int processID;
	struct ProcessDictionary* next;
	struct ProcessDictionary* prev;
}ProcessDictionary;

//DllDictionary* headDllLIST = NULL;
//DllDictionary* tailDllLIST = NULL;

void addToDllDictionary(char* key, struct ProcessDetails* value);

struct ProcessDictionary* addToProcessListDictionaryOfDll(struct ProcessDictionary* current);

struct SnapShot* addToSnapShotList(struct SnapShot*);

struct DllNames* addToDllList(struct DllNames*);

void addNewDllToList(struct DllNames*, struct DllNames*);

struct ProcessDetails* addToProcessList(struct ProcessDetails*);

void addNewProcessToList(struct ProcessDetails* listToAdd, struct ProcessDetails* processToAdd);

void addNewDllToList(struct DllNames* oldDllList, struct DllNames* newDllList);

void addToProcessDictionary(struct SnapShot*);

struct ProcessDictionary* addToProcessListDictionary(struct ProcessDictionary* current);

void PrintSList();
void PrintPList();
void PrintDList();
void PrintAllList();

void resetPandD();
void resetAllHeads();

//#endif // !_dataStructHeader