#include "freeMemory.h"
extern DllDictionary* DDhead;
int counterDll = 0;
int flag1timeErase = 0;
void freeMemSnap(struct SnapShot* Slist)
{
   
    if (Slist == NULL) return;

    if (Slist->Snext == NULL) {
        freeDllDictionary(DDhead);
        freeProcessDictionary();
    }

    freeMemProcess(Slist->processList);


        freeMemSnap(Slist->Snext);
    free(Slist);

   

}

void freeMemProcess(struct ProcessDetails* Plist)
{
    if (Plist == NULL) return;

    freeMemDlls(Plist->Dlllist);

        freeMemProcess(Plist->Pnext);
    free(Plist);
}

void freeMemDlls(struct DllNames* Dlist)
{
    counterDll++;
    if (Dlist == NULL) return;
    freeMemDlls(Dlist->Dnext);
    free(Dlist);
}


