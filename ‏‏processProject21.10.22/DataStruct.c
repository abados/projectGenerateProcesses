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
	if (current == NULL)//Starting a new list of SnapShot 
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

struct DllNames* addToDllList(struct DllNames* current) {

	if (current == NULL)//Starting a new process therefor we need a new DLL list
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
	else{
		Dtail->Dnext = current;
		current->Dprev = Dtail;
		current->Dnext = NULL;
		Dtail = current;
	}
	return Dhead;
}

void addNewDllToList(struct DllNames* oldDllList, struct DllNames* newDllList)
{
	struct DllNames* addDll= (struct DllNames*)malloc(sizeof(struct DllNames));
	addDll = newDllList;

	addDll->Dnext = NULL;
	oldDllList->Dnext = addDll;
	addDll->Dprev = oldDllList;
	//oldDllList = addDll;
}

void addNewProcessToList(struct ProcessDetails* listToAdd, struct ProcessDetails* processToAdd) {
	LogEvent("add new process to the list");

	struct ProcessDetails* addProcess = (struct ProcessDetails*)malloc(sizeof(struct ProcessDetails));
	addProcess = processToAdd;

	addProcess->Pnext = NULL;
	listToAdd->Pnext = addProcess;
	addProcess->Pprev = listToAdd;

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

struct ProcessDictionary* addToProcessListDictionaryOfDll(struct ProcessDictionary* current, struct ProcessDictionary* PDhead) {
	
	struct ProcessDictionary* PDtail= PDhead;
	
	if (PDhead){
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
	//while(ptrPtoPrint != NULL){
		ptrDtoPrint = ptrPtoPrint->Dlllist;
		while (ptrDtoPrint != NULL) {
		printf("%s\n ", ptrDtoPrint->dllName);
		ptrDtoPrint = ptrDtoPrint->Dnext;
		//}
	//ptrPtoPrint = ptrPtoPrint->Pnext;
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
/*
void PrintAllList() {
	printf("\nLIST OF SnapShots\n");
	PrintSList();
	printf("\nLIST OF Process\n");
	PrintPList();
	printf("\nLIST OF dlls\n");

}
*/
void PrintAllList2(struct SnapShot* snaplist) {
	printf("\nLIST OF SnapShots\n");
	PrintSList(snaplist);
	printf("\nLIST OF Process\n");
	PrintPList(snaplist->processList);
	printf("\nLIST OF dlls\n");
	snaplist = snaplist->Snext;

}

void resetPandD() {
	// set null to all global pointer
	// next step is to free them instead

	//Shead = NULL;
	//Stail = NULL;
	Phead = NULL;
	Ptail = NULL;
	Dhead = NULL;
	Dtail = NULL;
}

void resetAllHeads() {
	// set null to all global pointer
	// next step is to free them instead

	Shead = NULL;
	Stail = NULL;
	Phead = NULL;
	Ptail = NULL;
	Dhead = NULL;
	Dtail = NULL;
}

extern int DllDictionaryCnt = 0;
extern int processDictionaryCnt = 0;

void addToDllDictionary(char* key, struct ProcessDetails* value)
{
	ProcessDictionary* PDhead = NULL;
	ProcessDictionary* PDtail = NULL;
	DllDictionary* subDll = (DllDictionary*)malloc(sizeof(DllDictionary));
	DllDictionary* DDcurrent = DDhead;
	strcpy(subDll->key,key);
	subDll->next = subDll->prev = NULL;
	if (!DDhead)
	{
		DllDictionaryCnt++;
		DDhead = DDtail = subDll;
		DDtail->prev = DDhead;
		DDhead->next = DDtail;
		DDhead->prev = NULL;
		DDtail->next = NULL;
		struct ProcessDictionary* processItem = (ProcessDictionary*)malloc(sizeof(ProcessDictionary));
		strcpy(processItem->key,value->processName);
		processItem->processID = value->processID;
		subDll->processList_value = addToProcessListDictionaryOfDll(processItem, PDhead);;
	}
	else {
		DDcurrent = DDhead;
		while (DDcurrent && DDcurrent!= 0xdddddddddddddddd)
		{
			if (strcmp(DDcurrent->key, key) == 0 )
			{
				struct ProcessDictionary* tempProcessList = DDcurrent->processList_value;
				while (tempProcessList) {
					if (value->processID == tempProcessList->processID)
					{
						free(subDll);
						break;
					}
					tempProcessList = tempProcessList->next;
				}
					if(tempProcessList==NULL)
					{
						
						PDhead = DDcurrent->processList_value;
						struct ProcessDictionary* processItem = (ProcessDictionary*)malloc(sizeof(ProcessDictionary));
						strcpy(processItem->key, value->processName);
						processItem->processID = value->processID;
						subDll->processList_value = addToProcessListDictionaryOfDll(processItem, PDhead);
					}
					break;
			}
			DDcurrent = DDcurrent->next;
		}
		if (DDcurrent==NULL)
		{
			DllDictionaryCnt++;
			printf("%d\n", DllDictionaryCnt);
			DDtail->next = subDll;
			subDll->prev = DDtail;
			DDtail = subDll;
			DDtail->next = NULL;

			struct ProcessDictionary* processItem = (ProcessDictionary*)malloc(sizeof(ProcessDictionary));
			strcpy(processItem->key, value->processName);
			processItem->processID = value->processID;
			subDll->processList_value = addToProcessListDictionaryOfDll(processItem, PDhead);
		}
		
	}

	//headDllLIST = DDhead;
	//tailDllLIST = DDtail;
}

void addToProcessDictionary(struct SnapShot* totalList) {
	
	struct SnapShot* tempTotalList = totalList;
	struct ProcessDictionary* ptrListHead = PDUniqehead;
	struct ProcessDetails* tempProcess = totalList->processList;
	if (!totalList->Snext)
	{
		while (tempProcess)
		{
		struct ProcessDictionary* processItem = (ProcessDictionary*)malloc(sizeof(ProcessDictionary));
		strcpy(processItem->key, tempProcess->processName);
		processItem->processID = tempProcess->processID;
		PDUniqehead = addToProcessListDictionary(processItem);
		printf("process id:%d  process count:%d\n", processItem->processID, processDictionaryCnt);
		tempProcess = tempProcess->Pnext;
		processDictionaryCnt++;
		
		}
	}
	else
	{

	
	while (tempTotalList)
	{
		tempProcess = tempTotalList->processList;
		ptrListHead = PDUniqehead;
		while (tempProcess)
		{
			if(ptrListHead){
			if (ptrListHead->processID == tempProcess->processID)
			{
				tempProcess = tempProcess->Pnext;
				continue;
			}
			ptrListHead = ptrListHead->next;
			}
			else if (!ptrListHead)
			{
				while(tempProcess)
				{ 
				struct ProcessDictionary* processItem = (ProcessDictionary*)malloc(sizeof(ProcessDictionary));
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