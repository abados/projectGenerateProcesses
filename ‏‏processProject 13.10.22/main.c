
#include "takeSnapshot.h"
#include "userRespones.h"

#pragma warning(disable:4996)

int main()
{
    LogEvent("Program Start");
    userRespones();
    LogEvent("Program End");
    
    return 0;
}

/*
void recursive_close_snapshot func (list ptr) {
     if list ptr not null --> recursive_close_snapshot (list ptr next);
     else return;
     recursive_close_process (this snapshot process);
     free this process;
}

1 , 2 , 3 , null

recursive_close_snapshot(1)
---> recursive_close_snapshot(2)
--- ---> recursive_close_snapshot(3)
--- --- ---> recursive_close_snapshot(null)
--- ---> close processes of 3
---> close processes of 2
close processes of 1

void recursive_close_dlls(dll_list_ptr) {
    if dll_list_ptr is null --> return;
    recursive_close_dlls(dll_list_ptr next);
    free dll_list_ptr next;
}

*/