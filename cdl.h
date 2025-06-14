/**
 * @file cdl.h
 * @brief cdl86 (Compact Detours Library) - cdl.h
 *
 * Experimental Linux/Windows x86/x86_64 detours library.
 * Author: Dylan Müller
 *
 * +---------------------------------------+
 * |   .-.         .-.         .-.         |
 * |  /   \       /   \       /   \        |
 * | /     \     /     \     /     \     / |
 * |        \   /       \   /       \   /  |
 * |         "_"         "_"         "_"   |
 * |                                       |
 * |  _   _   _ _  _   _   ___   ___ _  _  |
 * | | | | | | | \| | /_\ | _ \ / __| || | |
 * | | |_| |_| | .` |/ _ \|   /_\__ \ __ | |
 * | |____\___/|_|\_/_/ \_\_|_(_)___/_||_| |
 * |                                       |
 * |                                       |
 * | Lunar RF Labs                         |
 * | https://lunar.sh                      |
 * |                                       |
 * | Research Laboratories                 |
 * | Copyright (C) 2022-2024               |
 * |                                       |
 * +---------------------------------------+
 *
 * Copyright (c) 2022 Lunar RF Labs
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright notice,
 *       this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright notice,
 *       this list of conditions and the following disclaimer in the documentation
 *       and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef CDL_H
#define CDL_H

#include <stdbool.h>
#include <stdint.h>

#define __in
#define __out
#define __in_out

/* extern "C" */
#ifdef __cplusplus
#define CDL86_EXTERN_C extern "C"
#else
#define CDL86_EXTERN_C
#endif

/**
 * Intruction probe struct
 *
 * @param size size of instruction (bytes)
 * @param bytes byte array of instruction (uint8_t*)
 */
struct cdl_ins_probe
{
    int size;
    uint8_t* bytes;
};

/**
 * JMP patch info struct
 *
 * @param active is patch active (bool)
 * @param nt_alloc number of bytes allocated to trampoline (int)
 * @param code instructions replaced by JMP patch (uint8_t*)
 * @param target pointer to function pointer (uint8_t**)
 * @param origin pointer to origin(real) target address (uint8_t*)
 * @param trampoline pointer to trampoline (uint8_t*)
 */
struct cdl_jmp_patch
{
    bool active;
    int nt_alloc;
    uint8_t* code;
    uint8_t** target;
    uint8_t* origin;
    uint8_t* trampoline;
};

/**
 * SWBP patch info struct.
 *
 * @param gid global id for SW BP (int)
 * @param active is patch active (bool)
 * @param ns_alloc number of bytes allocated to stub (int)
 * @param code instructions replaced by SWBP patch (uint8_t*)
 * @param target pointer to function pointer (uint8_t**)
 * @param stub pointer to stub (uint8_t*)
 * @param detour pointer to detour function (uint8_t*)
 * @param bp_add address of breakpoint (uint8_t*)
 */
struct cdl_swbp_patch
{
    int gid;
    bool active;
    int ns_alloc;
    uint8_t* code;
    uint8_t** target;
    uint8_t* stub;
    uint8_t* detour;
    uint8_t* bp_addr;
};

/**
 * Attach JMP patch to target funciton.
 *
 * @param target pointer to function pointer to function to hook.
 * @param detour function pointer to detour function
 */
CDL86_EXTERN_C struct cdl_jmp_patch cdl_jmp_attach(
    __in_out void** target,
    __in void* detour
);

/**
 * Attach INT3 patch to target funciton.
 *
 * @param target pointer to function pointer to function to hook.
 * @param detour function pointer to detour function
 */
CDL86_EXTERN_C struct cdl_swbp_patch cdl_swbp_attach(
    __in_out void** target,
    __in void* detour
);

/**
 * Detach JMP patch.
 *
 * @param jmp_patch pointer to cdl_jmp_patch struct.
 */
CDL86_EXTERN_C void cdl_jmp_detach(
    __in_out struct cdl_jmp_patch* jmp_patch
);

/**
 * Detach INT3 patch.
 *
 * @param swbp_patch pointer to cdl_swbp_patch struct.
 */
CDL86_EXTERN_C void cdl_swbp_detach(
   __in_out  struct cdl_swbp_patch* swbp_patch
);

/**
 * Print JMP patch debug info.
 *
 * @param jmp_patch pointer to cdl_jmp_patch struct.
 */
CDL86_EXTERN_C void cdl_jmp_dbg(
    __in struct cdl_jmp_patch* jmp_patch
);

/**
 * Print SW BP debug info.
 *
 * @param jmp_patch pointer to cdl_swbp_patch struct.
 */
CDL86_EXTERN_C void cdl_swbp_dbg(
    __in struct cdl_swbp_patch* swbp_patch
);

#endif
