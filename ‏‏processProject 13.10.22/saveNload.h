#include "logs.h"
#include "DataStruct.h"
#define Version 1
#pragma once

void saving_file(char file_name[100], struct SnapShot* snapList);
struct SnapShot* download_file(char file_name[100]);