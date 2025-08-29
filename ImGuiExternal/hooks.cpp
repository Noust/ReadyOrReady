#include "include.h"

// Global variable definitions
DWORD64 Actors_Hook;
DWORD64 jmpback;
Actors* Actor[255];
Actors* ActorsAddr;
BYTE* hook_target = nullptr;
PVOID veh_handle = nullptr;

void VEH_GetActors_Wrapper(PCONTEXT ctx) {
    DWORD64 rdx_val = ctx->Rdx;
    DWORD64 rbx_val = ctx->Rbx;
    
    BYTE data[16];
    memcpy(data, (void*)(rdx_val + 0x128), 16);
    
    memcpy((void*)(rbx_val + 0x80), data, 16);
    
    ActorsAddr = (Actors*)rdx_val;
    
    ctx->Rip = jmpback;
}

LONG WINAPI GetActors_VEHHandler(PEXCEPTION_POINTERS ExceptionInfo) {
    if (ExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_BREAKPOINT ||
        ExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_SINGLE_STEP) {
        
        if (ExceptionInfo->ExceptionRecord->ExceptionAddress == (PVOID)hook_target) {
            VEH_GetActors_Wrapper(ExceptionInfo->ContextRecord);
            return EXCEPTION_CONTINUE_EXECUTION;
        }
    }
    return EXCEPTION_CONTINUE_SEARCH;
}
