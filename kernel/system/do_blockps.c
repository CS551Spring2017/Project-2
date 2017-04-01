#include "kernel/system.h"
#include <minix/endpoint.h>
#include "kernel/proc.h"

int do_blockps(struct proc *caller, message *m_ptr){
	int p;
    isokendpt(m_ptr->m1_i1, &p);
    
    if(m_ptr->m1_i2)
        RTS_SET(proc_addr(p), RTS_BLOCKPS);
    else
        RTS_UNSET(proc_addr(p), RTS_BLOCKPS);
}