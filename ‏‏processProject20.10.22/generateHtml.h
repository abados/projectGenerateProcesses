#include "logs.h"
#include "DataStruct.h"

#pragma warning(disable:4996)
int SaveIntoFile(char* fileName, char* buff);

char* ReadAllFile(char* fileName);

void handle_HOME_PAGE(struct SnapShot*);

char* generateProcessLineAndDLLTable(char htmlTemplate[], struct SnapShot* snapList, struct ProcessDetails* processList, int counterPorocess);


char* changeNavParemeters( char* htmlTemplate);
char* changeNavSnapTable(char* htmlTemplate, struct SnapShot* snapList);
char* changeDllTable(char* htmlTemplate);
extern int DllDictionaryCnt;
extern int processDictionaryCnt;
extern size_t MemCntTotal;
extern DllDictionary* DDhead;

//size_t MemAVG;