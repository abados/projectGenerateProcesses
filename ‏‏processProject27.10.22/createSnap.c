#include "createSnap.h"

int processCounter = 0;
extern size_t MemCntTotal = 0;
size_t snapMem=0;
int totalDllIn1Snap = 0;



struct SnapShot* readSnapshot() {
	//malloc snapshot;
	//read processes(snapshot.processeslistHead);
	struct SnapShot* currSnapShot = (struct SnapShot*)malloc(sizeof(struct SnapShot));
	if (!currSnapShot)
	{
		LogError(strerror(GetLastError()));
	}
	currSnapShot->processList= readProcessess(currSnapShot->processList); // starting the snapshot build.
	currSnapShot->numberOFprocess = processCounter;
	currSnapShot->numberOFDlls = totalDllIn1Snap;
	currSnapShot->avgMem = snapMem / processCounter;
	snapMem = 0;
	totalDllIn1Snap = 0;
	processCounter = 0;
	return currSnapShot;
}

struct SnapShot* readProcessess() {
	/*	for (i : processes) {
			malloc process;
			read dlls(process id, dllslistHead);
			push to processeslistHead
		}*/
	
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
			//no Error - just processes we dont have access to
			
		}
		else{

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
				MemCntTotal+= pmc.WorkingSetSize;
				snapMem+= pmc.WorkingSetSize;
				currProcess->pmc.QuotaPagedPoolUsage = pmc.QuotaPagedPoolUsage;
				currProcess->pmc.QuotaPeakNonPagedPoolUsage = pmc.QuotaPeakNonPagedPoolUsage;
				currProcess->pmc.PagefileUsage = pmc.PagefileUsage;
				currProcess->biggestMemFlag = 0; //giving default 0 to every process, later i will handle and choose the one with the biggest Mem
				processCounter++;
		
			}
		}
		readDlls(currProcess, hProcess);
		ProcessList = addToProcessList(currProcess);
		
		}
	}
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
				if (strstr(currItemDllList->dllName,".dll") || strstr(currItemDllList->dllName,".DLL"))
				{
				currDllList = addToDllList(currItemDllList);
				numOfDll++;
				totalDllIn1Snap++;
				addToDllDictionary(currItemDllList->dllName, currProcess);
				}
			
			}
		}
		if (i == 0)//If there is not Dll's in this process
			{
			currProcess->countDll = 0;
			currProcess->Dlllist = NULL;
			}
		 if (numOfDll > 0) //If there is Dll's in this process
			{
			currProcess->countDll = numOfDll;
			currProcess->Dlllist = currDllList;
			currDllList->Dprev = NULL;
			}
	}
	
	if (i == 0) {// If we didnt entered the (EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded)) if's
		currProcess->countDll = 0;
		currProcess->Dlllist = NULL;

	}
	CloseHandle(hProcess);
	addToDllList(NULL);//reset the head&tail of the struct, prepering ourself to the next procces.
	return currProcess;
}

void checkDllToAdd(struct DllNames* oldDllList, struct DllNames* newDllList, struct ProcessDetails* toChangeDllCount) {
	// funtion that take place in mergine snapshots - > it checks if there are new dll to add.

	struct DllNames* oldDllListHEAD = oldDllList;
	
	while (newDllList)
	{
		while (oldDllList)
		{
			if (toChangeDllCount->countDll > 0) {
				//check if we can check size and do if str.size=str2.size
			if (strcmp(newDllList->dllName, oldDllList->dllName) == 0)//checking if there are equal dll's names between new and origin lists.
				{
				break;//if so - nothing to do - > break;
				}
			}
			if (oldDllList->Dnext==NULL)//if we reached the end - it means that there is new dll to add.
			{
				addNewDllToList(oldDllList,newDllList);
				toChangeDllCount->countDll++;//increasing the dll count.
				oldDllList = oldDllListHEAD; //bring back the origin list to start that we wont miss nothing.
			}
			oldDllList = oldDllList->Dnext;
		}
		newDllList = newDllList->Dnext;
		oldDllList = oldDllListHEAD;
	}

}

void combainProcessesData(struct ProcessDetails* sumProcess, struct ProcessDetails* tempProcess) {
	//merging the same proccesses DATA.

	sumProcess->pmc.PageFaultCount += tempProcess->pmc.PageFaultCount;
	sumProcess->pmc.PagefileUsage += tempProcess->pmc.PagefileUsage;
	sumProcess->pmc.WorkingSetSize += tempProcess->pmc.WorkingSetSize;
	sumProcess->pmc.QuotaPagedPoolUsage += tempProcess->pmc.QuotaPagedPoolUsage;
	sumProcess->pmc.QuotaPeakPagedPoolUsage += tempProcess->pmc.QuotaPeakPagedPoolUsage;

}


