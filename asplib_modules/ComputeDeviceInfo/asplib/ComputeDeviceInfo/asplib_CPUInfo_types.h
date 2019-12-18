#pragma once

/*
 * Copyright (C) 2014 Achim Turan, mail@achim-turan.de
 * https://github.com/AchimTuran/asplib
 *
 * This file is part of asplib (Achim's Signal Processing LIBrary)
 *
 * asplib (Achim's Signal Processing LIBrary) is free software:
 * you can redistribute it and/or modify it under the terms of the
 * GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * asplib (Achim's Signal Processing LIBrary) is distributed
 * in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with asplib (Achim's Signal Processing LIBrary).
 * If not, see <http://www.gnu.org/licenses/>.
 *
 */



#if defined(TARGET_WINDOWS)
    typedef unsigned __int32 uint32_t;
#elif defined(TARGET_LINUX)
    typedef unsigned int uint32_t;
    typedef int __int32;
#endif

typedef __int32 CPUINFO_ERROR;

#define CPUINFO_ERR_NO_ERR              0x00000000
#define CPUINFO_ERR_INVALID_INPUT       0x80100001
#define CPUINFO_ERR_DYNAMIC_MEMORY      0x80100002
#define CPUINFO_ERR_CANT_GET_CPU_NAME   0x80100003
#define CPUINFO_ERR_NOT_SOPPORTED       0x80100004


// CPUINFO helpers
#define CPUINFO_EAX 0
#define CPUINFO_EBX 1
#define CPUINFO_ECX 2
#define CPUINFO_EDX 3

// Defines to help with calls to CPUINFO
#define CPUINFO_FEATURE_BITS                                0x00000001
#define CPUINFO_GET_HIGHEST_EXTENDED_FUNCTION_SUPPORTED     0x80000000
#define CPUINFO_EXTENDED_FEATURE_BITS                       0x80000001
#define CPUINFO_PROCESSOR_BRAND_STRING_2                    0x80000002
#define CPUINFO_PROCESSOR_BRAND_STRING_3                    0x80000003
#define CPUINFO_PROCESSOR_BRAND_STRING_4                    0x80000004


typedef enum CPUINFO_modelIDs
{
    Earlier_Intel_486 = 1,
    Later_Intel_486_Pentium,
    Pentium_Pro_II_Celeron,
    Pentium_III,
    Pentium_4,
    Xeon,
    Pentium_M,
    Pentium_4_Hyper_Threading,
    Pentium_D_8xx,
    Pentium_D_9xx,
    Core_Duo,
    Core_2_Duo,
    Xeon_3000_5100_5200_5300_5400,
    Core_2_Duo_8000,
    Xeon_5200_5400,
    Atom,
    Core_i7,
    Unkown
};


// CPUINFO feature flags
#define CPUINFO_FEATURE_FLAG_3DNOWEXT       (1 << 0)
#define CPUINFO_FEATURE_FLAG_3DNOW          (1 << 1)

#define CPUINFO_FEATURE_FLAG_MMX            (1 << 2)
#define CPUINFO_FEATURE_FLAG_MMXEXT         (1 << 3)
#define CPUINFO_FEATURE_FLAG_SSE            (1 << 4)
#define CPUINFO_FEATURE_FLAG_SSE2           (1 << 5)
#define CPUINFO_FEATURE_FLAG_SSE3           (1 << 6)
#define CPUINFO_FEATURE_FLAG_SSSE3          (1 << 7)
#define CPUINFO_FEATURE_FLAG_SSE41          (1 << 8)
#define CPUINFO_FEATURE_FLAG_SSE42          (1 << 9)
#define CPUINFO_FEATURE_FLAG_SSE4a          (1 << 10)
#define CPUINFO_FEATURE_FLAG_AVX            (1 << 11)
#define CPUINFO_FEATURE_FLAG_AVX2           (1 << 12)
#define CPUINFO_FEATURE_FLAG_FMA3           (1 << 13)
#define CPUINFO_FEATURE_FLAG_FMA4           (1 << 14)
// 15...25 reserved for future feature flags
#define CPUINFO_FEATURE_FLAG_ALTIVEC        (1 << 26)
#define CPUINFO_FEATURE_FLAG_ARM_VFP        (1 << 27)
#define CPUINFO_FEATURE_FLAG_ARM_NEON       (1 << 28)
// 29...31 reserved for future feature flags

// CPUINFO feature flags bitmasks
#define CPUINFO_BITMASK_00000001_EDX_MMX        (1 << 23)
#define CPUINFO_BITMASK_00000001_EDX_SSE        (1 << 25)
#define CPUINFO_BITMASK_00000001_EDX_SSE2       (1 << 26)
#define CPUINFO_BITMASK_00000001_ECX_SSE3       (1 << 0)
#define CPUINFO_BITMASK_00000001_ECX_SSSE3      (1 << 9)
#define CPUINFO_BITMASK_00000001_ECX_FMA3       (1 << 12)
#define CPUINFO_BITMASK_00000001_ECX_SSE41      (1 << 19)
#define CPUINFO_BITMASK_00000001_ECX_SSE42      (1 << 20)
#define CPUINFO_BITMASK_00000001_ECX_AVX        (1 << 28)
#define CPUINFO_BITMASK_00000001_EBX_AVX2       (1 << 5)

// EAX=80000001h: Extended Processor Info and Feature Bits
#define CPUINFO_BITMASK_80000001_EDX_MMXEXT     (1 << 22)
#define CPUINFO_BITMASK_80000001_EDX_MMX        (1 << 23)
#define CPUINFO_BITMASK_80000001_EDX_3DNOWEXT   (1 << 30)
#define CPUINFO_BITMASK_80000001_EDX_3DNOW      (1 << 31)
#define CPUINFO_BITMASK_80000001_ECX_FMA4       (1 << 16)
#define CPUINFO_BITMASK_80000001_ECX_SSE4a      (1 << 6)

// CPUINFO vendor IDs
typedef enum CPUINFO_vendorIDs
{
    AMD_K5 = 0,
    AMD,
    Centaur_VIA,
    Cyrix,
    Intel,
    Transmeta_CPU,
    Transmeta_x86,
    National_Semiconductor,
    NexGen,
    Rise,
    SiS,
    UMC,
    VIA,
    Vortex,
    KVM,
    Microsoft_Hyper_V__Windows_Virtual_PC,
    VMware,
    Xen_HVM
};
