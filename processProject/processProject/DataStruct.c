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



struct SnapShot* addToSnapShotList(struct SnapShot* current) {

	if (current == NULL)//Starting a new snapshot therefor we
	{
		Shead = NULL;
		Stail = NULL;
	}
	else if (Shead == NULL) {

		current->Sprev = NULL;
		current->Snext = NULL;
		Shead = current;
		Stail = current;
		return;
	}
	else {
		Stail->Snext = current;
		current->Sprev = Stail;
		current->Snext = NULL;
		Stail = current;
	}


	return Shead;
}

struct DllNames* addToDllList(struct DllNames* current) {

	if (current == NULL)//Starting a new process therefor we need a new DLL list
	{
		Dhead = NULL;
		Dtail = NULL;
		return NULL;
	}
	else if (Dhead == NULL) {

		current->Dprev = NULL;
		current->Dnext = NULL;
		Dhead = current;
		Dtail = current;
	}
	else{
		Dtail->Dnext = current;
		current->Dprev = Dtail;
		current->Dnext = NULL;
		Dtail = current;
	}
	return Dhead;
}

/*void addNewDllToList(struct DllNames* oldDllList, struct DllNames* newDllList)
{
	oldDllList->Dnext = newDllList;
	newDllList->Dprev = oldDllList;
	newDllList->Dnext = NULL;
	oldDllList = newDllList;
}*/

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
		return;

	}
	else {
		Ptail->Pnext = current;
		current->Pprev = Ptail;
		current->Pnext = NULL;
		Ptail = current;

	}
	return Phead;
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

void resetSnapshot() {
	// set null to all global pointer
	// next step is to free them instead
}