
#include "generateHtml.h"

#define SEPERATORnav "Dll_count"
#define SEPERATORsnapList "changeSnapTABLE"
#define SEPERATORdlllist "changeDllTABLE"
int flagNotToFree = 0;



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
	
	char* htmlTemplate = ReadAllFile("C:\\Users\\User\\source\\repos\\processProject\\processProject\\mainToCopy.html");
	
	char* newFileSpace=changeNavParemeters( htmlTemplate);
	
	char* newFileSpace2 = changeSnapTable(newFileSpace , snapList);
	
	char* newFileSpace3 = changeDllTable(newFileSpace2, snapList);

	SaveIntoFile("main.html", newFileSpace3);

	free(newFileSpace);
	free(newFileSpace2);
	free(newFileSpace3);
	if (flagNotToFree==0) {//for the case we dont have data and our tables is empty,if so we didnt allocate memory so don't need to free it
		free(htmlTemplate);
	}
	
	
}

char* changeNavParemeters(char* htmlTemplate) {

	char HTML_REPLACE_NAV[1000] = "Dll_count\" placeholder=\"Dll count\"/></li><li><input class=\"Process_count\" placeholder=\"Process count\"/>PRCCNT</li><li><input class=\"Memory_Avg\" placeholder =\"Memory Avg\"/>MEMAVG</li>";
	if(processDictionaryCnt!=0){
	sprintf(HTML_REPLACE_NAV, "Dll_count\" placeholder=\"Dll count:%u\" /></li><li><input class =\"Process_count\" placeholder=\"Process count:%u\"/></li><li><input class=\"Memory_Avg\" placeholder =\"Memory Avg:%u\"/></li>", DllDictionaryCnt, processDictionaryCnt, MemCntTotal / processDictionaryCnt);
	}
	else {
		sprintf(HTML_REPLACE_NAV, "Dll_count\" placeholder=\"Dll count:%u\" /></li><li><input class =\"Process_count\" placeholder=\"Process count:%u\"/></li><li><input class=\"Memory_Avg\" placeholder =\"Memory Avg:%u\"/></li>", 0, 0, 0 );
	}
	
	char* found = strstr(htmlTemplate, SEPERATORnav); //finding the NAV seperator

	int len = found - htmlTemplate;
	char* newFileSpace = (char*)malloc(strlen(htmlTemplate) + strlen(HTML_REPLACE_NAV));//allocate larger memory space
	if (!newFileSpace)
	{
		LogError(strerror(GetLastError()));
	}
	strncpy(newFileSpace, htmlTemplate, len);
	newFileSpace[len] = NULL;

	strcat(newFileSpace, HTML_REPLACE_NAV);
	newFileSpace[len + strlen(HTML_REPLACE_NAV)] = NULL;

	strcat(newFileSpace, found + strlen(SEPERATORnav));
	
	return newFileSpace;
}

char* changeSnapTable( char* htmlTemplate, struct SnapShot* snapList) {
	char HTML_FINAL_SNAPLIST[100000] = "";
	int counter = 1;
	struct SnapShot* snapListptr = snapList;
	while (snapListptr != NULL) {
		char HTML_REPLACE_SNAPLIAT[1000] = "<tr class=\"trTableLeft2\"><td>SMAPLEID</td><td><a href=\"google.com\">SAMPLELINK</a></td><td>SAMPLEPRCCNT</td><td>SAMPLEDLLCNT< / td><td>SAMPLEMEMAVG</td></tr>";
		sprintf(HTML_REPLACE_SNAPLIAT,"<tr class=\"trTableLeft2\"><td>%d</td><td><a href=\"C:\\Users\\User\\source\\repos\\processProject\\processProject\\samplePages\\snapListNum%d.html\">SAMPL%d</a></td><td>%d</td><td>%d</td><td>%u</td></tr>", snapListptr->snapShotNum, snapListptr->snapShotNum,counter, snapListptr->numberOFprocess, snapListptr->numberOFDlls, snapListptr->avgMem);
		strcat(HTML_FINAL_SNAPLIST, HTML_REPLACE_SNAPLIAT);
		counter++;
		snapListptr = snapListptr->Snext;
	}
	
	char* found = strstr(htmlTemplate, SEPERATORsnapList);

	int len = found - htmlTemplate;
	char* newFileSpace = (char*)malloc(strlen(htmlTemplate) + strlen(HTML_FINAL_SNAPLIST));
	if (!newFileSpace)
	{
		LogError(strerror(GetLastError()));
	}
	strncpy(newFileSpace, htmlTemplate, len);
	newFileSpace[len] = NULL;

	strcat(newFileSpace, HTML_FINAL_SNAPLIST);
	newFileSpace[len + strlen(HTML_FINAL_SNAPLIST)] = NULL;

	strcat(newFileSpace, found + strlen(SEPERATORsnapList));

	
	return newFileSpace;
}

char* changeDllTable(char* htmlTemplate) {
	
	DllDictionary* ptrList=DDhead;
	char HTML_FINAL_Dlllist[1000000] = "";
	char HTML_REPLACE_Dlllist[3000] = "";
	char* FINAL_Dlllist = (char*)malloc(sizeof(struct DllNames)* DllDictionaryCnt + strlen(HTML_REPLACE_Dlllist)* 2000);
	if (!FINAL_Dlllist)
	{
		LogError(strerror(GetLastError()));
		flagNotToFree = 1;
		return htmlTemplate;
	}
	else{
	*FINAL_Dlllist = NULL;
	int counter = 1;
	while (ptrList != NULL) {
		sprintf(HTML_REPLACE_Dlllist, "<tr class=\"trTableRight2\"><td>%s</td><td><a href=\"C:\\Users\\User\\source\\repos\\processProject\\processProject\\dllPages\\DllNum%d.html\">DLL%d</a></td></tr>", ptrList->key, counter, counter);
		strcat(FINAL_Dlllist, HTML_REPLACE_Dlllist);
		counter++;
		ptrList = ptrList->next;
	}


	char* found = strstr(htmlTemplate, SEPERATORdlllist);

	int len = found - htmlTemplate;
	char* newFileSpace = (char*)malloc(strlen(htmlTemplate) + strlen(FINAL_Dlllist));

	strncpy(newFileSpace, htmlTemplate, len);
	newFileSpace[len] = NULL;

	strcat(newFileSpace, FINAL_Dlllist);
	newFileSpace[len + strlen(FINAL_Dlllist)] = NULL;

	strcat(newFileSpace, found + strlen(SEPERATORdlllist));
	
	return newFileSpace;
	}
	
}

void WRITE_SAMPLE_PAGES(struct SnapShot* snapList)
{
	struct SnapShot* snapListptr = snapList;
	char pagelink[120] = "";

	while(snapListptr){
		struct ProcessDetails* processListptr;
		struct DllNames* DLLListptr;
		sprintf(pagelink, "C:\\Users\\User\\source\\repos\\processProject\\processProject\\samplePages\\snapListNum%d.html", snapListptr->snapShotNum);

	FILE* fi = fopen(pagelink, "w");
	if (!fi)
	{
		return 0;
	}

	char HTML_START_PAGE[800] = "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"/><meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\"/><meta name=\"viewport\" content = \"width=device-width, initial-scale=1.0\" / ><link rel = \"stylesheet\" href =\"style/normalize\"/><link rel=\"stylesheet\" href=\"style_sample.css\"/><title>Sample Page</title></head><body><header><div class=\"container\"><h1 class=\"logo\">Hay Abadi</h1><nav><ul><li><a href=\"C:\\Users\\User\\source\\repos\\processProject\\processProject\\main.html\">Home</a></li><li><a href=\"C:\\Users\\User\\source\\repos\\processProject\\processProject\\aboutMe.html\">About</a></li></ul></nav></div></header><h1 class=\"SampleListH1\">Sample List</h1><h2 class=\"SampleNumandTimeH2\">";
	fputs(HTML_START_PAGE, fi);


	char HTML_REPLACE_SNAPINFO[300] = "";
	sprintf(HTML_REPLACE_SNAPINFO, "NUMBER: %d TIME : AT %s ", snapListptr->snapShotNum, snapListptr->snapShotTime);
	fputs(HTML_REPLACE_SNAPINFO, fi);


	char HTML_TILLPROCESS_PAGE[450] = "</h2><h3 class=\"ProcessListH2\">Process list</h3><div class=\"row\"><table class=\"tableLeft\"><tr class=\"trTableLeft1\"><th>Process Name</th><th>Process ID</th><th>Page Fault Count</th><th>Working Set Size</th><th>Quota Page Pool Usage</th><th>Quota Peak Page Pool Usage</th><th>Page File Usage</th><th>Dll Count</th><th>Process's Dll List</th></tr >";
	fputs(HTML_TILLPROCESS_PAGE, fi);

	processListptr = snapListptr->processList;
	
	unsigned long long counterflag = 0; unsigned long long counterdll = 0;
	while (processListptr != NULL) {
		char HTML_REPLACE_PROCESSINFO[1000] = "";
		if(processListptr->biggestMemFlag==0)
		{ 
		sprintf(HTML_REPLACE_PROCESSINFO, "<tr class=\"trTableLeft2\"><td>%s</td><td>%d</td><td>%u</td><td>%u</td><td>%u</td><td>%u</td><td>%u</td><td>%u</td><td><div class=\"conteinerDLLlist\"><table class=\"tableDllProcessSample\">", processListptr->processName, processListptr->processID, processListptr->pmc.PageFaultCount, processListptr->pmc.WorkingSetSize, processListptr->pmc.QuotaPagedPoolUsage, processListptr->pmc.QuotaPeakPagedPoolUsage, processListptr->pmc.PagefileUsage, processListptr->countDll);
		}
		else
		{
			sprintf(HTML_REPLACE_PROCESSINFO, "<tr class=\"trTableLeft2\"><td>%s</td><td>%d</td><td>%u</td><td>%u &#128077</td><td>%u</td><td>%u</td><td>%u</td><td>%u</td><td><div class=\"conteinerDLLlist\"><table class=\"tableDllProcessSample\">", processListptr->processName, processListptr->processID, processListptr->pmc.PageFaultCount, processListptr->pmc.WorkingSetSize, processListptr->pmc.QuotaPagedPoolUsage, processListptr->pmc.QuotaPeakPagedPoolUsage, processListptr->pmc.PagefileUsage, processListptr->countDll);
		}
		
		fputs(HTML_REPLACE_PROCESSINFO, fi);
		counterflag++;
		counterdll = 0;
		DLLListptr = processListptr->Dlllist;
	
		while (DLLListptr != NULL) {
			counterdll++;
			char HTML_REPLACE_Dlllist[1000] = "";
			sprintf(HTML_REPLACE_Dlllist, "<tr class=\"trtableDllProcessSample\"><td class=\"tdtableDllProcessSample\">%s</td></tr>", DLLListptr->dllName);
			fputs(HTML_REPLACE_Dlllist, fi);
			DLLListptr = DLLListptr->Dnext;
		}

		processListptr = processListptr->Pnext;
		fputs("</table></div></td></tr>", fi);
	}
	fputs("</table ></div></body></html>", fi);

	snapListptr = snapListptr->Snext;

	fclose(fi);

	
	}
}

void WRITE_DLL_PAGES() {

	struct DllDictionary* dllDictionaryPtr = DDhead;
	char pagelink[120] = "";
	int counterForHtmlUrl = 1;
	while (dllDictionaryPtr) {
		struct ProcessDictionary* processListptr;
		sprintf(pagelink, "C:\\Users\\User\\source\\repos\\processProject\\processProject\\dllPages\\DllNum%d.html", counterForHtmlUrl);
		counterForHtmlUrl++;
		FILE* fi = fopen(pagelink, "w");
		if (!fi)
		{
			return 0;
		}

		char HTML_START_PAGE[800] = "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"/><meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\"/><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"/><link rel=\"stylesheet\" href=\"style/normalize\"/><link rel=\"stylesheet\" href=\"style_Dll.css\"/><title>Processes use Dll</title></head><body><header><div class=\"container\"><h1 class=\"logo\">Hay Abadi</h1><nav><ul><li><a href=\"C:\\Users\\User\\source\\repos\\processProject\\processProject\\main.html\">Home</a></li><li><a href=\"C:\\Users\\User\\source\\repos\\processProject\\processProject\\aboutMe.html\">About</a></li></ul></nav></div></header><h1 class=\"processNameH1\">";
		fputs(HTML_START_PAGE, fi);


		char HTML_REPLACE_DLLINFO[300] = "";
		sprintf(HTML_REPLACE_DLLINFO, "DLL Name: %s ", dllDictionaryPtr->key);
		fputs(HTML_REPLACE_DLLINFO, fi);


		char HTML_TILLPROCESS_INFO[450] = "</h1><div class=\"row\"><table class =\"tableLeft\"><tr class=\"trTableLeft1\"><th>Process Name</th><th>Process ID</th></tr>";
		fputs(HTML_TILLPROCESS_INFO, fi);

		processListptr = dllDictionaryPtr->processList_value;

		unsigned long long counterflag = 0; unsigned long long counterdll = 0;
		while (processListptr != NULL) {
			char HTML_REPLACE_PROCESSINFO[1000] = "";
			sprintf(HTML_REPLACE_PROCESSINFO, "<tr class=\"trTableLeft2\"><td>%s</td><td>%d</td></tr>", processListptr->key, processListptr->processID);
			fputs(HTML_REPLACE_PROCESSINFO, fi);
			counterflag++;
			counterdll = 0;
			processListptr = processListptr->next;

		}
		fputs("</table ></div></body></html>", fi);

		dllDictionaryPtr = dllDictionaryPtr->next;

		fclose(fi);


	}

}
/*
void handle_SAMPLE_PAGES(struct SnapShot* snapList)
{

	char HTML_SAMPLE_PAGE[3000] = "<!DOCTYPE html>< html lang = \"en\" ><head><meta charset = \"UTF-8\" / ><meta http - equiv = \"X-UA-Compatible\" content = \"IE=edge\" / ><meta name = \"viewport\" content = \"width=device-width, initial-scale=1.0\" / ><link rel = \"stylesheet\" href = \"style/normalize\" / ><link rel = \"stylesheet\" href = \"style/style_sample.css\" / ><title>Sample Page< / title>< / head><body><header><div class = \"container\"><h1 class =\"logo\">Hay Abadi< / h1><nav><ul><li><a href = \"main.html\">Home< / a>< / li><li><a href = \"aboutMe.html\">About< / a>< / li>< / ul>< / nav>< / div>< / header><h1 class = \"SampleListH1\">Sample List< / h1><h2 class = \"SampleNumandTimeH2\">ADDSNAPINFO< / h2 ><h3 class = \"ProcessListH2\">Process list< / h3><div class = \"row\"><table class = \"tableLeft\"><tr class = \"trTableLeft1\"><th>Process Name< / th><th>Process ID< / th><th>PageFaultCount< / th><th>WorkingSetSize< / th><th>QuotaPagePoolUsage< / th><th>QuotaPeakPagePoolUsage< / th><th>PageFileUsahe< / th><th>Dll Count< / th><th>Process's Dll List</th>< / tr >ADDPROCESSINFO</table></div></body></html>";
	struct SnapShot* snapListptr = snapList;


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


			 generateProcessLineAndDLLTable(newFileSpace, snapListptr);

	}

}
*/
/*
char* generateProcessLineAndDLLTable(char htmlTemplate[], struct SnapShot* snapList)
{

	struct DllNames* DLLList = snapList->processList->Dlllist;

	//char HTML_FINAL_PROCESSINFO[10000];
	char HTML_TEMP_PROCESSINFO[1000] ="< tr class = \"trTableLeft2\"><td>%s</td><td>%d</td><td>%u</td><td>%u</td><td>%u</td><td>%u</td><td>%u</td><td>%u</td><td><div class=\"conteinerDLLlist\"><table class=\"tableDllProcessSample\">ADDdllLIST</table></div></td></tr>";
	char* FINAL_PROCESSINFO = (char*)malloc(sizeof(struct ProcessDetails)*snapList->numberOFprocess+ strlen(HTML_TEMP_PROCESSINFO)* snapList->numberOFprocess);
	*FINAL_PROCESSINFO = NULL;
	char* pageRaws = (char*)malloc(strlen(HTML_TEMP_PROCESSINFO) * snapList->numberOFprocess);
	*pageRaws = NULL;
	struct ProcessDetails* processListptr = snapList->processList;

	char HTML_FINAL_Dlllist[50000] = "";
	while (processListptr != NULL) {
		char HTML_REPLACE_PROCESSINFO[1000] = "";
		sprintf(HTML_REPLACE_PROCESSINFO, "<tr class=\"trTableLeft2\"><td>%s</td><td>%d</td><td>%u</td><td>%u</td><td>%u</td><td>%u</td><td>%u</td><td>%u</td><td><div class=\"conteinerDLLlist\"><table class=\"tableDllProcessSample\">ADDdllLIST</table></div></td></tr>", processListptr->processName, processListptr->processID, processListptr->pmc.PageFaultCount, processListptr->pmc.WorkingSetSize, processListptr->pmc.QuotaPagedPoolUsage, processListptr->pmc.QuotaPeakPagedPoolUsage, processListptr->pmc.PagefileUsage, processListptr->countDll);
		strcat(FINAL_PROCESSINFO, HTML_REPLACE_PROCESSINFO);
		struct DllNames* DLLListptr = processListptr->Dlllist;


		for (int i = 0; i < 20000; i++) {
			HTML_FINAL_Dlllist[i] = NULL;
		}

		while (DLLListptr != NULL) {
			char HTML_REPLACE_Dlllist[1000] = "";
			sprintf(HTML_REPLACE_Dlllist, "<tr class=\"trtableDllProcessSample\"><td class=\"tdtableDllProcessSample\">%s</td></tr>", DLLListptr->dllName);
			strcat(HTML_FINAL_Dlllist, HTML_REPLACE_Dlllist);
			DLLListptr = DLLListptr->Dnext;
		}
		//HTML_FINAL_Dlllist[strlen(HTML_FINAL_Dlllist)] = NULL;
		char* found = strstr(FINAL_PROCESSINFO, SEPERATORdlllistInSnapPage);

		int len = found - FINAL_PROCESSINFO;
		char* newPageReady = (char*)malloc(strlen(FINAL_PROCESSINFO) + strlen(HTML_FINAL_Dlllist));

		strncpy(newPageReady, FINAL_PROCESSINFO, len);
		newPageReady[len] = NULL;

		strcat(newPageReady, HTML_FINAL_Dlllist);
		newPageReady[len + strlen(HTML_FINAL_Dlllist)] = NULL;

		strcat(newPageReady, found + strlen(SEPERATORdlllistInSnapPage));

		strcat(pageRaws,newPageReady);

		processListptr = processListptr->Pnext;
	}

}
*/


