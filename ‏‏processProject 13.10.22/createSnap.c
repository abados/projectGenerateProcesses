
#include "createSnap.h"

int processCounter = 0;

struct SnapShot* readSnapshot() {
	//malloc snapshot;
	//read processes(snapshot.processeslistHead);
	//LogEvent("create snapshot");
	struct SnapShot* currSnapShot = (struct SnapShot*)malloc(sizeof(struct SnapShot));
	currSnapShot->processList= readProcessess(currSnapShot->processList);
	currSnapShot->numberOFprocess = processCounter;
	processCounter = 0;
	return currSnapShot;
}

struct SnapShot* readProcessess() {
	/*	for (i : processes) {
			malloc process;
			read dlls(process id, dllslistHead);
			push to processeslistHea
		}*/
	//LogEvent("create processes");
	struct ProcessDetails* currProcess = NULL;
	struct ProcessDetails* ProcessList = NULL;
	DWORD aProcesses[1024], cbNeeded, cProcesses;
	HANDLE hProcess;
	PROCESS_MEMORY_COUNTERS pmc;
	size_t numConverted;//USE AT BOTH TIMES WE NEED TO PUT WCHAR_T AT CHAR[]
	// Open process in order to receive information
	unsigned int i;

	// * Receive all process ID and put in aProcesses Array
	if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
	{
		//LogError(strerror(GetLastError()));
		return;
	}

	// Calculate how many process identifiers were returned.
	cProcesses = cbNeeded / sizeof(DWORD);

	// Print the memory usage for each process
	// *Loop of all processes
	struct ProcessDetails* processList = NULL;
	//struct ProcessDetails* process;
	for (i = 0; i < cProcesses; i++)
	{
		DWORD processID = aProcesses[i];

		hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
			PROCESS_VM_READ,
			FALSE, processID);
		if (NULL == hProcess)
		{
			// Write to logError
			//LogError(strerror(GetLastError()));
			
		}
		else{
		//DWORD cbNeeded;
		TCHAR FoundProcessName[MAX_PATH];
		TCHAR wDllName[MAX_PATH];
		char regularCharArr[MAX_PATH];

		// Get Process Name
		if (!GetModuleFileNameEx(hProcess, 0, FoundProcessName, MAX_PATH))
		{
			LogError(strerror(GetLastError()));
		}

		if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc)))
		{
			currProcess = (struct ProcessDetails*)malloc(sizeof(struct ProcessDetails));
			if (!currProcess)
			{
				LogError(strerror(GetLastError()));
			}
			else {
				currProcess->processID = processID;
				wcstombs_s(&numConverted, currProcess->processName, MAX_PATH, FoundProcessName, MAX_PATH);// Convert wChar to regular char array (string)
				currProcess->pmc.PageFaultCount = pmc.PageFaultCount;
				currProcess->pmc.WorkingSetSize = pmc.WorkingSetSize;
				currProcess->pmc.QuotaPagedPoolUsage = pmc.QuotaPagedPoolUsage;
				currProcess->pmc.QuotaPeakNonPagedPoolUsage = pmc.QuotaPeakNonPagedPoolUsage;
				currProcess->pmc.PagefileUsage = pmc.PagefileUsage;
				processCounter++;
				

			}
		}
		readDlls(currProcess, hProcess);
		ProcessList = addToProcessList(currProcess);

		
		}
	}
//printf("\n\nnumber OF process to this SNAP %d\n", processCounter);
	return ProcessList;

}

struct DllNames* readDlls(struct ProcessDetails* currProcess,HANDLE hProcess) {
	// Get Dlls List
	//LogEvent("create Dlls");
	int i = 0;
	HMODULE hMods[1024];
	TCHAR wDllName[MAX_PATH];
	DWORD cbNeeded;
	size_t numConverted;//USE AT BOTH TIMES WE NEED TO PUT WCHAR_T AT CHAR[]
	if (EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded))
	{
		int numOfDll = 0;
		struct DllNames* currItemDllList = NULL;
		struct DllNames* currDllList = NULL;

		for (i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
		{
			// Get the full path to the module's file.
			currItemDllList = (struct DllNames*)malloc(sizeof(struct DllNames));
			if (!currItemDllList)
			{
				LogError(strerror(GetLastError()));
			}
			else if (GetModuleFileNameEx(hProcess, hMods[i], wDllName, MAX_PATH))
			{
				// * Get the module name and handle value- Convert wChar to regular char array (string)
				wcstombs_s(&numConverted, currItemDllList->dllName, MAX_PATH, wDllName, MAX_PATH);
				currDllList = addToDllList(currItemDllList);
				numOfDll++;
			}
		}
		if (i == 0)
			{
			currProcess->countDll = 0;
			currProcess->Dlllist = NULL;
			}
		 if (numOfDll > 0) 
			{
			currProcess->countDll = numOfDll;
			currProcess->Dlllist = currDllList;
			currDllList->Dprev = NULL;
			}

		
	}
	//where it put the list.
	if (i == 0) {
		currProcess->countDll = 0;
		currProcess->Dlllist = NULL;

	}
	CloseHandle(hProcess);
	addToDllList(NULL);
	return currProcess;
}

void checkDllToAdd(struct DllNames* oldDllList, struct DllNames* newDllList, struct ProcessDetails* toChangeDllCount) {
	//LogEvent("check Dlls to add");
	struct DllNames* oldDllListHEAD = oldDllList;
	//struct DllNames* oldlistPtrToMovie = oldDllList;
	//struct DllNames* NewlistPtrToMovie = newDllList;

	//struct DllNames* HeadPtrDllList = oldDllListHEAD; לבדוק הוזזה של פוינטר חלופי וזמני במקום פוינטר מקורי

	while (newDllList)
	{
		while (oldDllList)
		{
			if (toChangeDllCount->countDll > 0) {
				//check if we can check size and do if str.size=str2.size
			if (strcmp(newDllList->dllName, oldDllList->dllName) == 0)
				{
				break;
				}
			}
			if (oldDllList->Dnext==NULL)
			{
				addNewDllToList(oldDllList,newDllList);
				toChangeDllCount->countDll++;
				oldDllList = oldDllListHEAD;
			}
			oldDllList = oldDllList->Dnext;
		}
		newDllList = newDllList->Dnext;
		oldDllList = oldDllListHEAD;
	}

}

void combainProcessesData(struct ProcessDetails* sumProcess, struct ProcessDetails* tempProcess) {
	//LogEvent("combine processes");

	sumProcess->pmc.PageFaultCount += tempProcess->pmc.PageFaultCount;
	sumProcess->pmc.PagefileUsage += tempProcess->pmc.PagefileUsage;
	sumProcess->pmc.WorkingSetSize += tempProcess->pmc.WorkingSetSize;
	sumProcess->pmc.QuotaPagedPoolUsage += tempProcess->pmc.QuotaPagedPoolUsage;
	sumProcess->pmc.QuotaPeakPagedPoolUsage += tempProcess->pmc.QuotaPeakPagedPoolUsage;

}


