
#include "logs.h"
#include "DataStruct.h"
#pragma once


void freeMemSnap(struct SnapShot* list);
void freeMemProcess(struct ProcessDetails* list);
void freeMemDlls(struct DllNames* list);
