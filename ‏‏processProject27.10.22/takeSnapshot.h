#include "createSnap.h"
#pragma once

struct SnapShot* oneSnapshot();
void mergeSnaps(struct SnapShot* sumSnaps, struct SnapShot* newSnap, struct ProcessDetails* toChangeDllCount);
