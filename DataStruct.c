#include "DataStruct.h"
#pragma warning(disable:4996)


struct ProcessDetails* Ptail = NULL;
struct DllNames* Dtail = NULL;
struct DllNames* Dhead = NULL;
struct ProcessDetails* Phead = NULL;
struct DllNames* Dcurrent;
struct ProcessDetails* Pcurrent;
struct SnapShot* Shead = NULL;
struct SnapShot* Stail = NULL;
struct SnapShot* Scurrent;
struct SnapShot* LastShead = NULL;
struct SnapShot* LastStail = NULL;
DllDictionary* DDhead = NULL;
DllDictionary* DDtail=NULL;
ProcessDictionary* PDhead = NULL;
ProcessDictionary* PDtail = NULL;



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

struct ProcessDictionary* addToProcessListDictionaryOfDll(struct ProcessDictionary* current) {


	
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

void PrintPList() {
	struct ProcessDetails* ptrPtoPrint = Phead;
	
	while (ptrPtoPrint != NULL) {
		
		printf("%s\n ", ptrPtoPrint->processName);
		printf("number of dll to this process %d\n ", ptrPtoPrint->countDll);
		//PrintDList(ptrPtoPrint->processName);
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
void PrintSList() {
	struct SnapShot* ptrStoPrint = Shead;
	while (ptrStoPrint != NULL) {
		printf("Snapshot number:%d ", ptrStoPrint->snapShotNum);
		printf("%s\n ", ptrStoPrint->snapShotTime);
		ptrStoPrint = ptrStoPrint->Snext;
	}
	printf("\n\n\n");
}

void PrintAllList() {
	printf("\nLIST OF SnapShots\n");
	PrintSList();
	printf("\nLIST OF Process\n");
	PrintPList();
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
int counter = 0;
int counterInsideFree = 0;
int counterInsideif = 0;
void addToDllDictionary(char* key, struct ProcessDetails* value)
{
	
	counter++;
	DllDictionary* subDll = (DllDictionary*)malloc(sizeof(DllDictionary));
	DllDictionary* DDcurrent = DDhead;
	strcpy(subDll->key,key);
	subDll->next = subDll->prev = NULL;
	if (!DDhead)
	{
		counterInsideif++;
		DDhead = DDtail = subDll;
		DDhead->prev = NULL;
		DDtail->prev = DDhead;
		DDhead->next = DDtail;
		DDtail->next = NULL;
		struct ProcessDictionary* processItem = (ProcessDictionary*)malloc(sizeof(ProcessDictionary));
		strcpy(processItem->key,value->processName);
		processItem->processID = value->processID;
		subDll->processList_value = addToProcessList(processItem);
	}
	else {
		while (DDcurrent)
		{
			if (strcmp(DDcurrent->key, key) == 0 )
			{
				struct ProcessDictionary* tempProcessList = DDcurrent->processList_value;
				while (tempProcessList) {
					if (value->processID == tempProcessList->processID)
					{
						counterInsideFree++;
						free(subDll);
						break;
					}
					tempProcessList = tempProcessList->next;
				}
					if(tempProcessList==NULL)
					{
						counterInsideif++;
						struct ProcessDictionary* processItem = (ProcessDictionary*)malloc(sizeof(ProcessDictionary));
						strcpy(processItem->key, value->processName);
						processItem->processID = value->processID;
						subDll->processList_value = addToProcessListDictionaryOfDll(processItem);
						break;
					}
			}
			DDcurrent = DDcurrent->next;
		}
		if (DDcurrent==NULL)
		{
			counterInsideif++;
			DDtail->next = subDll;
			subDll->prev = DDtail;
			DDtail = subDll;
			DDtail->next = NULL;
			

			struct ProcessDictionary* processItem = (ProcessDictionary*)malloc(sizeof(ProcessDictionary));
			strcpy(processItem->key, value->processName);
			processItem->processID = value->processID;
			subDll->processList_value = addToProcessList(processItem);
		}
		
	}
}
