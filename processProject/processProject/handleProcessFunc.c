#include "handleProcessFunc.h"
#pragma warning(disable:4996)


//struct SnapShot* currSnapShot = NULL; //initilize
struct ProcessDetails* currProcess = NULL;
struct DllNames* currDllList = NULL;

struct ProcessDetails* ProcessMemoryInfo(DWORD processID)
{

	HANDLE hProcess;
	PROCESS_MEMORY_COUNTERS pmc;
	size_t numConverted;//USE AT BOTH TIMES WE NEED TO PUT WCHAR_T AT CHAR[]
	// Open process in order to receive information
	
	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
		PROCESS_VM_READ,
		FALSE, processID);
	if (NULL == hProcess)
	{
		// Write to logError
		//LogError(strerror(GetLastError()));
		return NULL;
	}

	currDllList=addToDllList(NULL); // for the case we have a new process and we start a new list of dll
	HMODULE hMods[1024];
	DWORD cbNeeded;
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
		else{
		currProcess->processID = processID;
		wcstombs_s(&numConverted, currProcess->processName, MAX_PATH, FoundProcessName, MAX_PATH);// Convert wChar to regular char array (string)
		currProcess->pmc.PageFaultCount = pmc.PageFaultCount;
		currProcess->pmc.WorkingSetSize = pmc.WorkingSetSize;
		currProcess->pmc.QuotaPagedPoolUsage = pmc.QuotaPagedPoolUsage;
		currProcess->pmc.QuotaPeakNonPagedPoolUsage = pmc.QuotaPeakNonPagedPoolUsage;
		currProcess->pmc.PagefileUsage = pmc.PagefileUsage;
		
		}
	}




	// Get Dlls List
	
	int i = 0;
	if (EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded))
	{
		long numOfDll=0;
		struct DllNames* currItemDllList = NULL;
		
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
				currDllList=addToDllList(currItemDllList);
				numOfDll++;
			}
		}
		if (i == 0) {
			currProcess->countDll = 0;
			currProcess->Dlllist = NULL;
			
		}else{
		currProcess->countDll = numOfDll;
		currProcess->Dlllist = currDllList;
		}
	}
	//where it put the list.
	
	CloseHandle(hProcess);
	return currProcess;
}

struct SnapShot* GetProcessesInfo(struct SnapShot* snap)
{
	// Get Processes

	struct SnapShot* currSnapShot = (struct SnapShot*)malloc(sizeof(struct SnapShot));
	//currSnapShot->snapShotNum = SnapNum;
	DWORD aProcesses[1024], cbNeeded, cProcesses;
	unsigned int i;

	// * Receive all process ID and put in aProcesses Array
	if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
	{
		//LogError(strerror(GetLastError()));
		return 1;
	}

	// Calculate how many process identifiers were returned.
	cProcesses = cbNeeded / sizeof(DWORD);

	// Print the memory usage for each process
	// *Loop of all processes
	struct ProcessDetails* processList = NULL;
	struct ProcessDetails* process;
	for (i = 0; i < cProcesses; i++)
	{
		process = ProcessMemoryInfo(aProcesses[i]);
		if (!process)
		{
			processList = addToProcessList(currProcess);
		}
	}

	//struct SnapShot* snapshotList = addToSnapShotList(currSnapShot);
	currSnapShot->processList = processList;
	free(currSnapShot);

}
/*
read snapshot() {
	malloc snapshot;
	read processes(snapshot.processeslistHead);
}

read processess(snapshot.processeslistHea) {
	for (i : processes) {
		malloc process;
		read dlls(process id, dllslistHead);
		push to processeslistHea
	}
}

read dlls(process id, dll list head) {
	for (i : dlls) {
		malloc dll;
		read dll;
		push to dll list
	}
}
*/