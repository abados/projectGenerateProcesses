#include "createSnap.h"
#pragma once

struct SnapShot* oneSnapshot();
struct SnapShot* Snap20shot();
struct SnapShot* sumSample(struct SnapShot* sumSnap, struct SnapShot* newSnap);
void mergeSnaps(struct SnapShot* sumSnaps, struct SnapShot* newSnap, struct ProcessDetails* toChangeDllCount);
