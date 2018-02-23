#ifndef PROC_H
#define PROC_H

#ifdef __ASSEMBLER__

#define SAVE_CONTEXT \
	push {r0, r14};\
	mov r0, lr;\
	bl __saveContext1;\
	pop {r0, r14};\
	push {r14};\
	bl __saveContext2;\
	pop {r14};

#else

#include <types.h>
#include <mmu.h>

typedef void (*EntryFunctionT)(void);

#define PROCESS_COUNT_MAX 128

enum ProcessState {
	UNUSED,
	CREATED,
	SLEEPING,
	READY,
	RUNNING,
	TERMINATED
};

struct MemBlock;

enum ContextItem {
	CPSR, RESTART_ADDR,
	R0, R1, R2, R3, R4, R5, R6, R7, R8, R9, R10, R11, R12,
	SP, LR
};

typedef struct {
	enum ProcessState state;
	int pid;
	EntryFunctionT entry;
	PageDirEntryT *vm;
	uint32_t heapSize;
	char *userStack;
	char *kernelStack;
	int context[17];

	int waitPid;
	int childReturnValue;

	/*for malloc*/
	struct MemBlock* mHead;
	struct MemBlock* mTail;
} ProcessT;

/* public symbols */
extern ProcessT *_currentProcess;
extern ProcessT _processTable[PROCESS_COUNT_MAX];

extern void procInit();
extern ProcessT *procCreate(void);
bool procLoad(ProcessT *proc, char **procImage, int pageCount);
void procStart(ProcessT *proc);
void procFree(ProcessT *proc);
bool procExpandMemory(ProcessT *proc, int pageCount);
void procShrinkMemory(ProcessT *proc, int pageCount);

#endif

#endif
