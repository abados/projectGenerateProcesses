
#include "generateHtml.h"

#define SEPERATORnav "Dll_count"
#define SEPERATORsnapList "changeSnapTABLE"
#define SEPERATORdlllist "changeDllTABLE"
#define SEPERATORSnapDetailsInSnapPage "ADDSNAPINFO"
#define SEPERATORdlllistInSnapPage "ADDdllLIST"
#define SEPERATORProcessesInSnapPage "ADDPROCESSINFO"


int SaveIntoFile(char* fileName, char* buff)
{
	FILE* fi = fopen(fileName, "w");
	if (!fi)
	{
		return 0;
	}

	fputs(buff, fi);


	fclose(fi);
}
char* ReadAllFile(char* fileName)
{
	FILE* f = fopen(fileName, "r");
	if (!f)
	{
		return NULL;
	}


	// Get the file size
	char* buff = (char*)malloc(1000);
	char* read;
	int fileSize = 0;
	while ((read = fgets(buff, 1000, f)))
	{
		fileSize += strlen(buff);
	}

	free(buff);
	fclose(f);
	fileSize++;
	// alloc space as file size
	buff = (char*)malloc(fileSize);

	f = fopen(fileName, "r");
	if (!f)
	{
		return NULL;
	}
	int readPosition = 0;
	char charToRead;
	while ((charToRead = fgetc(f)) != EOF)
	{
		buff[readPosition] = charToRead;
		readPosition++;
	}
	buff[readPosition] = NULL;

	fclose(f);

	return buff;
}

void handle_HOME_PAGE(struct SnapShot* snapList)
{
	
	char* htmlTemplate = ReadAllFile("C:\\Users\\User\\source\\repos\\processProject\\processProject\\snap2.html");
	
	char* newFileSpace=changeNavParemeters( htmlTemplate);
	
	newFileSpace = changeNavSnapTable(newFileSpace , snapList);
	
	newFileSpace = changeDllTable(newFileSpace);

	SaveIntoFile("newTest2.html", newFileSpace);


	free(newFileSpace);
	free(htmlTemplate);
	
}

void handle_SAMPLE_PAGES(struct SnapShot* snapList)
{
	
	char HTML_SAMPLE_PAGE[3000] = "<!DOCTYPE html>< html lang = \"en\" ><head><meta charset = \"UTF-8\" / ><meta http - equiv = \"X-UA-Compatible\" content = \"IE=edge\" / ><meta name = \"viewport\" content = \"width=device-width, initial-scale=1.0\" / ><link rel = \"stylesheet\" href = \"style/normalize\" / ><link rel = \"stylesheet\" href = \"style/style_sample.css\" / ><title>Sample Page< / title>< / head><body><header><div class = \"container\"><h1 class =\"logo\">Hay Abadi< / h1><nav><ul><li><a href = \"main.html\">Home< / a>< / li><li><a href = \"aboutMe.html\">About< / a>< / li>< / ul>< / nav>< / div>< / header><h1 class = \"SampleListH1\">Sample List< / h1><h2 class = \"SampleNumandTimeH2\">ADDSNAPINFO< / h2 ><h3 class = \"ProcessListH2\">Process list< / h3><div class = \"row\"><table class = \"tableLeft\"><tr class = \"trTableLeft1\"><th>Process Name< / th><th>Process ID< / th><th>PageFaultCount< / th><th>WorkingSetSize< / th><th>QuotaPagePoolUsage< / th><th>QuotaPeakPagePoolUsage< / th><th>PageFileUsahe< / th><th>Dll Count< / th><th>Process's Dll List</th>< / tr >ADDPROCESSINFO</table></div></body></html>";
	struct SnapShot* snapListptr = snapList;
	
	int counterPorocess = 1;
	while (snapListptr)
	{
		char HTML_REPLACE_SNAPINFO[1000] = "";
		sprintf(HTML_REPLACE_SNAPINFO, "NUMBER: %d TIME : AT %s ", snapList->snapShotNum, snapList->snapShotTime);
		char* found = strstr(HTML_SAMPLE_PAGE, SEPERATORSnapDetailsInSnapPage);
		int len = found - HTML_SAMPLE_PAGE;
		char* newFileSpace = (char*)malloc(strlen(HTML_SAMPLE_PAGE) + strlen(HTML_REPLACE_SNAPINFO));
		strncpy(newFileSpace, HTML_SAMPLE_PAGE, len);
		newFileSpace[len] = NULL;
		strcat(newFileSpace, HTML_REPLACE_SNAPINFO);
		newFileSpace[len + strlen(HTML_REPLACE_SNAPINFO)] = NULL;
		strcat(newFileSpace, found + strlen(SEPERATORSnapDetailsInSnapPage));

		struct ProcessDetails* processList = snapListptr->processList;
		while (processList)
		{
	
			generateProcessLineAndDLLTable(newFileSpace, snapListptr, processList, counterPorocess);
			counterPorocess++;
		}

	}

}

char* changeNavParemeters(char* htmlTemplate) {

	char HTML_REPLACE_NAV[1000] = "Dll_count\" placeholder=\"Dll count\"/></li><li><input class=\"Process_count\" placeholder=\"Process count\"/>PRCCNT</li><li><input class=\"Memory_Avg\" placeholder =\"Memory Avg\"/>MEMAVG</li>";
	sprintf(HTML_REPLACE_NAV, "Dll_count\" placeholder=\"Dll count:%u\" /></li><li><input class =\"Process_count\" placeholder=\"Process count:%u\"/></li><li><input class=\"Memory_Avg\" placeholder =\"Memory Avg:%u\"/></li>", DllDictionaryCnt, processDictionaryCnt, MemCntTotal / processDictionaryCnt);

	char* found = strstr(htmlTemplate, SEPERATORnav);

	int len = found - htmlTemplate;
	char* newFileSpace = (char*)malloc(strlen(htmlTemplate) + strlen(HTML_REPLACE_NAV));

	strncpy(newFileSpace, htmlTemplate, len);
	newFileSpace[len] = NULL;

	strcat(newFileSpace, HTML_REPLACE_NAV);
	newFileSpace[len + strlen(HTML_REPLACE_NAV)] = NULL;

	strcat(newFileSpace, found + strlen(SEPERATORnav));

	return newFileSpace;
}

char* changeNavSnapTable( char* htmlTemplate, struct SnapShot* snapList) {
	char HTML_FINAL_SNAPLIST[100000] = "";
	int counter = 1;
	struct SnapShot* snapListptr = snapList;
	while (snapListptr != NULL) {
		char HTML_REPLACE_SNAPLIAT[1000] = "<tr class=\"trTableLeft2\"><td>SMAPLEID</td><td><a href=\"google.com\">SAMPLELINK</a></td><td>SAMPLEPRCCNT</td><td>SAMPLEDLLCNT< / td><td>SAMPLEMEMAVG</td></tr>";
		sprintf(HTML_REPLACE_SNAPLIAT,"<tr class=\"trTableLeft2\"><td>%d</td><td><a href=\"google.com\">SAMPL%d</a></td><td>%d</td><td>%d</td><td>%u</td></tr>", snapList->snapShotNum,counter,snapList->numberOFprocess,snapList->numberOFDlls,snapList->avgMem);
		strcat(HTML_FINAL_SNAPLIST, HTML_REPLACE_SNAPLIAT);
		counter++;
		snapListptr = snapListptr->Snext;
	}
	
	char* found = strstr(htmlTemplate, SEPERATORsnapList);

	int len = found - htmlTemplate;
	char* newFileSpace = (char*)malloc(strlen(htmlTemplate) + strlen(HTML_FINAL_SNAPLIST));

	strncpy(newFileSpace, htmlTemplate, len);
	newFileSpace[len] = NULL;

	strcat(newFileSpace, HTML_FINAL_SNAPLIST);
	newFileSpace[len + strlen(HTML_FINAL_SNAPLIST)] = NULL;

	strcat(newFileSpace, found + strlen(SEPERATORsnapList));

	
	return newFileSpace;
}

char* changeDllTable(char* htmlTemplate) {
	
	DllDictionary* ptrList=DDhead;
	char HTML_FINAL_Dlllist[300000] = "";
	int counter = 1;
	while (ptrList != NULL) {
		char HTML_REPLACE_Dlllist[1000] = "<tr class = \"trTableRight2\"><td>9-5</td><td>9-5</td></tr>";
		sprintf(HTML_REPLACE_Dlllist, "<tr class=\"trTableRight2\"><td>%s</td><td><a href=\"google.com\">DLL%d</a></td></tr>", ptrList->key, counter);
		strcat(HTML_FINAL_Dlllist, HTML_REPLACE_Dlllist);
		counter++;
		ptrList = ptrList->next;
	}


	char* found = strstr(htmlTemplate, SEPERATORdlllist);

	int len = found - htmlTemplate;
	char* newFileSpace = (char*)malloc(strlen(htmlTemplate) + strlen(HTML_FINAL_Dlllist));

	strncpy(newFileSpace, htmlTemplate, len);
	newFileSpace[len] = NULL;

	strcat(newFileSpace, HTML_FINAL_Dlllist);
	newFileSpace[len + strlen(HTML_FINAL_Dlllist)] = NULL;

	strcat(newFileSpace, found + strlen(SEPERATORdlllist));

	return newFileSpace;
	
}

char* generateProcessLineAndDLLTable(char htmlTemplate[], struct SnapShot* snapList, struct ProcessDetails* processList, int counterPorocess)
{

	struct DllNames* DLLList = processList->Dlllist;
	char HTML_FINAL_Dlllist[300000] = "";
	char HTML_FINAL_PROCESSINFO[700000] = "";
	struct ProcessDetails* processListptr = processList;

	while (processListptr != NULL) {
		char HTML_REPLACE_PROCESSINFO[1000] = "";
		sprintf(HTML_REPLACE_PROCESSINFO, "<tr class=\"trTableLeft2\"><td>%s</td><td>%d</td><td>%u</td><td>%u</td><td>%u</td><td>%u</td><td>%u</td><td>%u</td><td><div class=\"conteinerDLLlist\"><table class=\"tableDllProcessSample\">ADDdllLIST</table></div></td></tr>", processListptr->processName, processListptr->processID, processListptr->pmc.PageFaultCount, processListptr->pmc.WorkingSetSize, processListptr->pmc.QuotaPagedPoolUsage, processListptr->pmc.QuotaPeakPagedPoolUsage, processListptr->pmc.PagefileUsage, processListptr->countDll);
		strcat(HTML_FINAL_PROCESSINFO, HTML_REPLACE_PROCESSINFO);
		processListptr = processListptr->Pnext;
	}



	//Replace the process info
	//char HTML_REPLACE_PROCESSINFO[2000] = "";
	//sprintf(HTML_REPLACE_PROCESSINFO,"<tr class=\"trTableLeft2\"><td>%s</td><td>%d</td><td>%u</td><td>%u</td><td>%u</td><td>%u</td><td>%u</td><td>%u</td><td><div class=\"conteinerDLLlist\"><table class=\"tableDllProcessSample\">ADDdllLIST</table></div></td></tr>", processList->processName, processList->processID, processList->pmc.PageFaultCount, processList->pmc.WorkingSetSize, processList->pmc.QuotaPagedPoolUsage, processList->pmc.QuotaPeakPagedPoolUsage, processList->pmc.PagefileUsage, processList->countDll);
	char* found = strstr(htmlTemplate, SEPERATORProcessesInSnapPage);
	int len = found - htmlTemplate;
	char* newProcessSpace = (char*)malloc(strlen(htmlTemplate) + strlen(HTML_FINAL_PROCESSINFO));
	strncpy(newProcessSpace, htmlTemplate, len);
	newProcessSpace[len] = NULL;
	strcat(newProcessSpace, HTML_FINAL_PROCESSINFO);
	newProcessSpace[len + strlen(HTML_FINAL_PROCESSINFO)] = NULL;

	strcat(newProcessSpace, found + strlen(SEPERATORProcessesInSnapPage));
	

	//Replace the dll list info
	while (DLLList != NULL) {
		char HTML_REPLACE_Dlllist[1000] = "<tr class=\"trtableDllProcessSample\"><td class=\"tdtableDllProcessSample\"></td></tr>";
		sprintf(HTML_REPLACE_Dlllist, "<tr class=\"trtableDllProcessSample\"><td class=\"tdtableDllProcessSample\">%s</td></tr>", DLLList->dllName);
		strcat(HTML_FINAL_Dlllist, HTML_REPLACE_Dlllist);
		DLLList = DLLList->Dnext;
	}

	 found = strstr(newProcessSpace, SEPERATORdlllistInSnapPage);

	 len = found - newProcessSpace;
	 char* newPageReady = (char*)malloc(strlen(htmlTemplate) + strlen(HTML_FINAL_Dlllist));

	strncpy(newPageReady, newProcessSpace, len);
	newPageReady[len] = NULL;

	strcat(newPageReady, HTML_FINAL_Dlllist);
	newPageReady[len + strlen(HTML_FINAL_Dlllist)] = NULL;

	strcat(newPageReady, found + strlen(SEPERATORdlllistInSnapPage));

	return newPageReady;



}
