

#include "logs.h"
#include "DataStruct.h"
#pragma once
#pragma warning(disable:4996)
struct SnapShot* readSnapshot();
struct SnapShot* readProcessess();
struct DllNames* readDlls(struct ProcessDetails* snap, HANDLE hProcess);
void checkDllToAdd(struct DllNames* oldDllList, struct DllNames* newDllList, int* oldCountDll);
void combainProcessesData(struct ProcessDetails* sumProcess, struct ProcessDetails* tempProcess);