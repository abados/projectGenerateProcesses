#include "DataStruct.h"
#pragma warning(disable:4996)



 struct ProcessDetails* Ptail = NULL;
 struct DllNames* Dtail=NULL;
 struct DllNames* Dhead=NULL;
 struct ProcessDetails* Phead=NULL;
 struct DllNames* Dcurrent;
 struct ProcessDetails* Pcurrent;
 struct SnapShot* Shead=NULL;
 struct SnapShot* Stail=NULL;
 struct SnapShot* Scurrent;
 struct SnapShot* LastShead=NULL;
 struct SnapShot* LastStail= NULL;
 extern DllDictionary* DDhead = NULL;
 DllDictionary* DDtail = NULL;
 ProcessDictionary* PDUniqehead = NULL;
 ProcessDictionary* PDUniqetail = NULL;





struct SnapShot* addToSnapShotList(struct SnapShot* current) {
	if (current == NULL)//reseting - beacuse we Starting a new list of SnapShot 
	{
		Dhead = NULL;
		Dtail = NULL;
		return NULL;
	}

	 if (Shead == NULL) {

		current->Sprev = NULL;
		current->Snext = NULL;
		Shead = Stail = current;
	}
	else {
		current->Snext = NULL;
		Stail->Snext = current;
		current->Sprev = Stail;
		Stail = current;
	}

	 LastShead = Shead;
	 LastStail = Stail;
	return Shead;
}
struct ProcessDetails* addToProcessList(struct ProcessDetails* current) {


	if (current == NULL)//Starting a new process therefor we need a new DLL list
	{
		Phead = NULL;
		Ptail = NULL;
	}
	else if (Phead == NULL) {

		current->Pprev = NULL;
		current->Pnext = NULL;
		Phead = current;
		Ptail = current;

	}
	else {
		Ptail->Pnext = current;
		current->Pprev = Ptail;
		current->Pnext = NULL;
		Ptail = current;

	}
	return Phead;
}
struct DllNames* addToDllList(struct DllNames* current) {

	if (current == NULL)//reseting - beacuse we Starting a new list of SnapShot 
	{
		Dhead = NULL;
		Dtail = NULL;
		return NULL;
	}
	else if (Dhead == NULL) {

		Dhead = current;
		Dtail = current;
		Dhead->Dprev = NULL;
		Dtail->Dnext = NULL;

	}
	else {
		Dtail->Dnext = current;
		current->Dprev = Dtail;
		current->Dnext = NULL;
		Dtail = current;
	}
	return Dhead;
}
struct SnapShot* addToSnapShotListAfterLoad(struct SnapShot* current, struct SnapShot* lastList)
{
	Shead = lastList;
	

	if (lastList == NULL) {
		current->Sprev = NULL;
		current->Snext = NULL;
		Shead = Stail = current;
	}
	else {
		current->Snext = NULL;
		Stail->Snext = current;
		current->Sprev = Stail;
		Stail = current;
	}
	LastShead = Shead;
	LastStail = Stail;

}
void addNewDllToList(struct DllNames* oldDllList, struct DllNames* newDllList) // Take place in the merge action.
{
	struct DllNames* addDll= (struct DllNames*)malloc(sizeof(struct DllNames));
	addDll = newDllList;

	addDll->Dnext = NULL;
	oldDllList->Dnext = addDll;
	addDll->Dprev = oldDllList;
	//oldDllList = addDll;
}
void addNewProcessToList(struct ProcessDetails* listToAdd, struct ProcessDetails* processToAdd) // Take place in the merge action.
{
	struct ProcessDetails* addProcess = (struct ProcessDetails*)malloc(sizeof(struct ProcessDetails));
	addProcess = processToAdd;

	addProcess->Pnext = NULL;
	listToAdd->Pnext = addProcess;
	addProcess->Pprev = listToAdd;

}


void resetPandD() {
	// set null to global pointer before me make a new snanpshot
	
	Phead = NULL;
	Ptail = NULL;
	Dhead = NULL;
	Dtail = NULL;
}
void resetAllHeads() {
	// set null to all global pointer
	
	Shead = NULL;
	Stail = NULL;
	Phead = NULL;
	Ptail = NULL;
	Dhead = NULL;
	Dtail = NULL;
	DDhead = NULL;
	DDtail = NULL;
	PDUniqehead = NULL;
	PDUniqetail = NULL;

}

void sortprocessBydllCount(struct SnapShot* SnapshotListTotal)//bubble sort process's by the dll count.
{
	struct SnapShot* snaplistptr = SnapshotListTotal;
	struct ProcessDetails* prevItem, * nextItem;
	struct ProcessDetails* tempItem;
	int swapped = 1;
	while(snaplistptr)
	{ 
	struct ProcessDetails* HEAD = snaplistptr->processList;
	while (swapped)
	{
		swapped = 0;
		tempItem = HEAD;
		while (tempItem->Pnext!= NULL)
		{
			if (tempItem->countDll > tempItem->Pnext->countDll)
			{
				prevItem = tempItem->Pprev;
				nextItem = tempItem->Pnext;
				if (prevItem != NULL)
				{
					prevItem->Pnext = nextItem;
				}
				else
				{
					HEAD = nextItem;
				}
				tempItem->Pnext = nextItem->Pnext;
				if (nextItem->Pnext != NULL)
				{
					nextItem->Pnext->Pprev = tempItem;
				}
				tempItem->Pprev = nextItem;
				nextItem->Pnext = tempItem;
				nextItem->Pprev = prevItem;
				swapped = 1;
			}
			else
			{
				tempItem = tempItem->Pnext;
			}
			
		}
	}
	snaplistptr->processList = HEAD;
	snaplistptr = snaplistptr->Snext;
	swapped = 1;
	}
}
void markTheBiggestProcess(struct SnapShot* SnapshotListTotal) {
	size_t biggestNum;
	struct SnapShot* snaplistptr = SnapshotListTotal;
	struct ProcessDetails* ptrToBiggest;

	while (snaplistptr)
	{
		ptrToBiggest = NULL;
		biggestNum = 0;
		struct ProcessDetails* HEAD = snaplistptr->processList;
		while (HEAD)
		{
			if (HEAD->pmc.WorkingSetSize > biggestNum)
			{
				ptrToBiggest = HEAD;
				biggestNum = HEAD->pmc.WorkingSetSize;
			}
			HEAD = HEAD->Pnext;
		}
		ptrToBiggest->biggestMemFlag = 1;
		snaplistptr = snaplistptr->Snext;
	}
	

}

extern int DllDictionaryCnt = 0;
extern int processDictionaryCnt = 0;
void addToDllDictionary(char* key, struct ProcessDetails* value)
{//adding dll to it dictionary
	ProcessDictionary* PDhead = NULL;
	ProcessDictionary* PDtail = NULL;
	DllDictionary* subDll = (DllDictionary*)malloc(sizeof(DllDictionary));
	if (!subDll)
	{
		LogError(strerror(GetLastError()));
	}
	DllDictionary* DDcurrent = DDhead;
	strcpy(subDll->key, key);
	subDll->next = subDll->prev = NULL;
	if (!DDhead)	//first item in the dll list so we adding it and its process
	{
		DllDictionaryCnt = 0;
		DllDictionaryCnt++;
		DDhead = DDtail = subDll;
		DDtail->prev = DDhead;
		DDhead->next = DDtail;
		DDhead->prev = NULL;
		DDtail->next = NULL;
		struct ProcessDictionary* processItem = (ProcessDictionary*)malloc(sizeof(ProcessDictionary));
		if (!processItem)
		{
			LogError(strerror(GetLastError()));
		}
		strcpy(processItem->key, value->processName);
		processItem->processID = value->processID;
		subDll->processList_value = addToProcessListDictionaryOfDll(processItem, PDhead);;
	}
	else {//its isn't the first dll in the dictionary/
		DDcurrent = DDhead;
		while (DDcurrent && DDcurrent != 0xdddddddddddddddd)//checikng if this dll is already in the 
		{
			if (strcmp(DDcurrent->key, key) == 0) //case that we have this dll allready and we need to check if to add the procces to the list
			{
				struct ProcessDictionary* tempProcessList = DDcurrent->processList_value;
				while (tempProcessList) {
					if (value->processID == tempProcessList->processID) //checking the procces, if we found it so nothing to do except free()
					{
						free(subDll);
						break;
					}
					tempProcessList = tempProcessList->next;
				}
				if (tempProcessList == NULL) //if we reach the end -> we didnt find the procces and we need to add it.
				{

					PDhead = DDcurrent->processList_value;
					struct ProcessDictionary* processItem = (ProcessDictionary*)malloc(sizeof(ProcessDictionary));
					if (!processItem)
					{
						LogError(strerror(GetLastError()));
					}
					strcpy(processItem->key, value->processName);
					processItem->processID = value->processID;
					subDll->processList_value = addToProcessListDictionaryOfDll(processItem, PDhead);
				}
				break;
			}
			DDcurrent = DDcurrent->next;
		}

		if (DDcurrent == NULL) //if we didnt find the dll so we need to had it and its procces
		{
			DllDictionaryCnt++;
			//printf("%d\n", DllDictionaryCnt);
			DDtail->next = subDll;
			subDll->prev = DDtail;
			DDtail = subDll;
			DDtail->next = NULL;

			struct ProcessDictionary* processItem = (ProcessDictionary*)malloc(sizeof(ProcessDictionary));
			if (!processItem)
			{
				LogError(strerror(GetLastError()));
			}
			strcpy(processItem->key, value->processName);
			processItem->processID = value->processID;
			subDll->processList_value = addToProcessListDictionaryOfDll(processItem, PDhead);
		}
	}

}
void addToProcessDictionary(struct SnapShot* totalList) {

	struct SnapShot* tempTotalList = totalList;
	struct ProcessDictionary* ptrListHead = PDUniqehead;
	struct ProcessDetails* tempProcess = totalList->processList;
	if (!totalList->Snext)//if this the first snapshot/have just 1 snapshot : we adding all its process's to the dictionary
	{
		while (tempProcess)
		{
			struct ProcessDictionary* processItem = (ProcessDictionary*)malloc(sizeof(ProcessDictionary));
			if (!processItem)
			{
				LogError(strerror(GetLastError()));
			}
			strcpy(processItem->key, tempProcess->processName);
			processItem->processID = tempProcess->processID;
			PDUniqehead = addToProcessListDictionary(processItem);
			printf("process id:%d  process count:%d\n", processItem->processID, processDictionaryCnt);
			tempProcess = tempProcess->Pnext;
			processDictionaryCnt++;

		}
	}
	else //we have a few snapshot to check regard to process's dictionary
	{
		while (tempTotalList) //run all the snanshot we have 
		{
			tempProcess = tempTotalList->processList;
			ptrListHead = PDUniqehead;
			while (tempProcess)
			{
				if (ptrListHead) {
					if (ptrListHead->processID == tempProcess->processID)//if this process exsit in the dictionary already
					{
						tempProcess = tempProcess->Pnext;
						continue;
					}
					ptrListHead = ptrListHead->next;
				}
				else if (!ptrListHead)//if we arrived to the end of the dictionary list -  this process don't exsit in the dictionary and we need to add it.

				{
					while (tempProcess)//if we have new process's they will come in the end of the list (checked few time), so we adding all of them and saving to run again on the start of the function.
					{
						struct ProcessDictionary* processItem = (ProcessDictionary*)malloc(sizeof(ProcessDictionary));
						if (!processItem)
						{
							LogError(strerror(GetLastError()));
						}
						strcpy(processItem->key, tempProcess->processName);
						processItem->processID = tempProcess->processID;
						PDUniqehead = addToProcessListDictionary(processItem);
						printf("process id:%d  process count:%d\n", processItem->processID, processDictionaryCnt);
						processDictionaryCnt++;
						tempProcess = tempProcess->Pnext;
					}

				}

			}
			tempTotalList = tempTotalList->Snext;
		}
	}

}
struct ProcessDictionary* addToProcessListDictionary(struct ProcessDictionary* current) {

	if (PDUniqehead == NULL) {

		PDUniqehead = current;
		PDUniqetail = current;
		PDUniqehead->prev = NULL;
		PDUniqetail->next = NULL;

	}
	else {
		PDUniqetail->next = current;
		current->prev = PDUniqetail;
		PDUniqetail = current;
		PDUniqetail->next = NULL;

	}
	return PDUniqehead;
}
struct ProcessDictionary* addToProcessListDictionaryOfDll(struct ProcessDictionary* current, struct ProcessDictionary* PDhead)
{// adding process's to specific dll in the dictionary
	struct ProcessDictionary* PDtail = PDhead;

	if (PDhead) {
		while (PDtail->next)
		{
			PDtail = PDtail->next;
		}
	}
	if (PDhead == NULL) {

		PDhead = current;
		PDtail = current;
		PDhead->prev = NULL;
		PDtail->next = NULL;

	}
	else {
		PDtail->next = current;
		current->prev = PDtail;
		PDtail = current;
		PDtail->next = NULL;

	}
	return PDhead;
}

int counterdll = 0;
int counterfree = 0;
void freeDllDictionary(DllDictionary* DDhead) {
	
	if (!DDhead) return;
	
	freeMemProcessictionary(DDhead->processList_value);
	counterdll++;
	freeDllDictionary(DDhead->next);

	free(DDhead);
	counterfree++;
	DllDictionaryCnt--;
}
void freeMemProcessictionary(struct ProcessDictionary* Plist)
{
	if (Plist == NULL) return;
	freeMemProcessictionary(Plist->next);
	free(Plist);
}
void freeProcessDictionary() {
	
	while (PDUniqetail)
	{

		
		PDUniqetail = PDUniqetail->prev;
		if (PDUniqetail->prev == NULL)
		{
			free(PDUniqetail);
			PDUniqetail = NULL;
			processDictionaryCnt= processDictionaryCnt-2;
		}
		else {
			free(PDUniqetail->next);
			processDictionaryCnt--;
		}
	}

}



void PrintPList(struct ProcessDetails* processList) {
	struct ProcessDetails* ptrPtoPrint = processList;

	while (ptrPtoPrint != NULL) {

		printf("%s\n ", ptrPtoPrint->processName);
		printf("number of dll to this process %d\n ", ptrPtoPrint->countDll);
		PrintDList(ptrPtoPrint);

		ptrPtoPrint = ptrPtoPrint->Pnext;
	}
	printf("\n\n\n");
}
void PrintDList(struct ProcessDetails* process) {
	struct ProcessDetails* ptrPtoPrint = process;
	struct DllNames* ptrDtoPrint = ptrPtoPrint->Dlllist;
	ptrDtoPrint = ptrPtoPrint->Dlllist;
	while (ptrDtoPrint != NULL) {
		printf("%s\n ", ptrDtoPrint->dllName);
		ptrDtoPrint = ptrDtoPrint->Dnext;
	}
	printf("\n\n\n");
}
void PrintSList(struct SnapShot* snaplist) {
	struct SnapShot* ptrStoPrint = snaplist;
	while (ptrStoPrint != NULL) {
		printf("Snapshot number:%d ", ptrStoPrint->snapShotNum);
		printf("%s\n ", ptrStoPrint->snapShotTime);
		ptrStoPrint = ptrStoPrint->Snext;
	}
	printf("\n\n\n");
}
void PrintAllList2(struct SnapShot* snaplist) {
	printf("\nLIST OF SnapShots\n");
	PrintSList(snaplist);
	printf("\nLIST OF Process\n");
	PrintPList(snaplist->processList);
	printf("\nLIST OF dlls\n");
	snaplist = snaplist->Snext;

}
/*
void PrintAllList() {
	printf("\nLIST OF SnapShots\n");
	PrintSList();
	printf("\nLIST OF Process\n");
	PrintPList();
	printf("\nLIST OF dlls\n");

}
*/


