#include "pthread.h"
#include "lib.h"
/*
 * pthread lib here
 * 用户态多线程写在这
 */
 
ThreadTable tcb[MAX_TCB_NUM];
int current;

void pthread_initial(void){
    int i;
    for (i = 0; i < MAX_TCB_NUM; i++) {
        tcb[i].state = STATE_DEAD;
        tcb[i].joinid = -1;
    }
    tcb[0].state = STATE_RUNNING;
    tcb[0].pthid = 0;
    current = 0; // main thread
    return;
}

int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void *), void *arg){
    
    int i = 0;
    for (i = 0; i < MAX_TCB_NUM; i++)
    {
        if (tcb[i].state == STATE_DEAD)
            break;
    }
    if (i == MAX_TCB_NUM) // can't find a empty thread
    {
        return -1;
    }
    if (attr == NULL) // use default thread attribution
    {
        tcb[current].state = STATE_RUNNABLE;

        *thread = i;

        asm volatile("pushal");
        asm volatile("movl %%esp, %0": "=m"(tcb[current].stackTop)); // store current esp

        // don't care this part
        // just something making gcc behavior stable
        struct Context *sf = (struct Context *)tcb[current].stackTop;
        sf->eax = 0;
        tcb[current].cont.eip = *(uint32_t *)(sf->ebp + 4);

        current = i; // change current thread to i
        tcb[i].pthid = i;
        tcb[i].state = STATE_RUNNING;
        tcb[i].pthArg = (uint32_t)arg; // store function arg
        tcb[i].retPoint = (uint32_t)(void *)pthread_exit; // return to pthread_exit
        tcb[i].cont.eip = (uint32_t)(void *)start_routine;

        tcb[i].stack[MAX_STACK_SIZE - 1] = tcb[i].pthArg; // use stack[MAX_STACK_SIZE - 1] as the stack top
        tcb[i].stack[MAX_STACK_SIZE - 2] = tcb[i].retPoint; // return address stores at the bottom of pthread Argument

        tcb[i].stackTop = (uint32_t)&tcb[i].cont - 8; // now the esp address should be &cont -8
        asm volatile("movl %0, %%esp"::"m"(tcb[i].stackTop));
        asm volatile("jmpl *%0"::"m"(tcb[i].cont.eip));
    }
    return 0;
}

void pthread_exit(void *retval){
    tcb[current].state = STATE_DEAD;
    int join = tcb[current].joinid;
    tcb[join].state = STATE_RUNNABLE;

    int i;
    for (i = (current + 1) % MAX_TCB_NUM; ; i = (i + 1)%MAX_TCB_NUM)
    {
        if (tcb[i].state == STATE_RUNNABLE)
            break;
    }
    tcb[i].state = STATE_RUNNING;

    // make nonsense: just make sure that gcc push all regs(ebx, esi, edi) when calling this fuction
    // make sure that compilation of 4 pthread functions are consistent
    struct Context *sf = (struct Context *)tcb[current].stackTop;
    tcb[current].cont.eip = *(uint32_t *)(sf->ebp + 4); // don't care this instruction

    current = i;
    asm volatile("movl %0, %%esp":: "m"(tcb[i].stackTop));
    asm volatile("popal");
    asm volatile("movl %0, %%eax":: "i"(0));
    return;
}

int pthread_join(pthread_t thread, void **retval){
    
    if (tcb[thread].state == STATE_DEAD)
    {
        return -1;
    }
    tcb[current].state = STATE_BLOCKED;
    int join = thread;
    tcb[join].joinid = current;

    int i;
    for (i = (current + 1) % MAX_TCB_NUM; ; i = (i + 1)%MAX_TCB_NUM)
    {
        if (tcb[i].state == STATE_RUNNABLE)
            break;
    }

    asm volatile("pushal");
    asm volatile("movl %%esp, %0": "=m"(tcb[current].stackTop));

    // don't care this part
    struct Context *sf = (struct Context *)tcb[current].stackTop;
    sf->eax = 0;
    tcb[current].cont.eip = *(uint32_t *)(sf->ebp + 4);

    current = i;
    tcb[i].state = STATE_RUNNING;
    
    asm volatile("movl %0, %%esp":: "m"(tcb[i].stackTop));
    asm volatile("popal");
    return 0;
}

int pthread_yield(void){

    int i;
    for (i = (current + 1) % MAX_TCB_NUM; ; i = (i + 1)%MAX_TCB_NUM)
    {
        if (tcb[i].state == STATE_RUNNABLE)
            break;
    }
    if (i == current)
    {
        return -1;
    }
    tcb[current].state = STATE_RUNNABLE;

    asm volatile("pushal");
    asm volatile("movl %%esp, %0": "=m"(tcb[current].stackTop));

    // don't care this part
    struct Context *sf = (struct Context *)tcb[current].stackTop;
    sf->eax = 0;
    tcb[current].cont.eip = *(uint32_t *)(sf->ebp + 4);
    
    current = i;

    tcb[i].state = STATE_RUNNING;
    
    asm volatile("movl %0, %%esp":: "m"(tcb[i].stackTop));
    asm volatile("popal");
    return 0;
}
