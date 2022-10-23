#include "logs.h"


void log(char* message)
{
	time_t t;
	time(&t);
	struct tm* timeInfo;
	timeInfo = localtime(&t);
	char strTime[1000];
	sprintf(strTime, " Date: %d/%d/%d Time: %d:%d:%d \n", timeInfo->tm_mday, timeInfo->tm_mon + 1, timeInfo->tm_year + 1900,  timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec);


	FILE* f = fopen("myProg.log", "a");

	strcat(strTime, message);
	printf("%s ", strTime);
	while (fputs(strTime, f) != NULL)
	{

	}

	fclose(f);

}

void LogError(char* message) {

	char s[10000] = "";
	sprintf(s, "%s %s", "Error -", message);
	log(s);

}

void LogEvent(char* message) {
	char s[10000] = "";
	sprintf(s, "%s %s", "EVENT -", message);
	log(s);

}