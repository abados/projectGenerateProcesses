#include "createSnap.h"
#pragma once

struct SnapShot* oneSnapshot();
struct SnapShot* Snap20shot();
void mergeSnaps(struct SnapShot* sumSnaps, struct SnapShot* newSnap, struct ProcessDetails* toChangeDllCount);
