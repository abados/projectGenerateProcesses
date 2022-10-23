#include "logs.h"
#include "DataStruct.h"

#pragma warning(disable:4996)
int SaveIntoFile(char* fileName, char* buff);

char* ReadAllFile(char* fileName);

void handle_HOME_PAGE(struct SnapShot*);
void handle_SAMPLE_PAGES(struct SnapShot* snapList);
void WRITE_SAMPLE_PAGES(struct SnapShot* snapList);
void WRITE_DLL_PAGES();
char* generateProcessLineAndDLLTable(char htmlTemplate[], struct SnapShot* snapList);


char* changeNavParemeters( char* htmlTemplate);
char* changeNavSnapTable(char* htmlTemplate, struct SnapShot* snapList);
char* changeDllTable(char* htmlTemplate);
extern int DllDictionaryCnt;
extern int processDictionaryCnt;
extern size_t MemCntTotal;
extern DllDictionary* DDhead;

//size_t MemAVG;