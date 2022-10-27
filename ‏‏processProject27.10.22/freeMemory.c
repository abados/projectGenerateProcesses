#include "freeMemory.h"
extern DllDictionary* DDhead;
int counterDll = 0;
int flag1timeErase = 0;
void freeMemSnap(struct SnapShot* Slist)
{
   
    if (Slist == NULL) return;

    if (Slist->Snext == NULL) {   //we want to free the dictionary and once without taking a part in the recursion.
        freeDllDictionary(DDhead);//Free the dll dictionary.
        freeProcessDictionary();  //Free the procces dictionary.
    }

    freeMemProcess(Slist->processList);//sendin the snap's procces list to free it.


        freeMemSnap(Slist->Snext);
    free(Slist);

}

void freeMemProcess(struct ProcessDetails* Plist)
{
    if (Plist == NULL) return;

    freeMemDlls(Plist->Dlllist);//sendin the procces's dll list to free it.

        freeMemProcess(Plist->Pnext);
    free(Plist);
}

void freeMemDlls(struct DllNames* Dlist)//base recursion, only when end we will start the procces recursion and after it we will free the snapshot.
{
    
    if (Dlist == NULL) return;
    freeMemDlls(Dlist->Dnext);
    free(Dlist);
}


