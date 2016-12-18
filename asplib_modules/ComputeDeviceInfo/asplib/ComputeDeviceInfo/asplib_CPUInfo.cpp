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
    #include <intrin.h>
#elif defined(TARGET_LINUX)
    #include <cpuid.h>
    #include <malloc.h>
#else
    #error "Unimplemented cpuInfo platform!"
#endif

#include <iostream>
#include <string.h>
#include <memory.h>

#include "asplib_CPUInfo.h"

const char* CPUINFO_vendorIDTable[] =
{
    /*0*/ "AMDisbetter!", "AMD",
    /*2*/ "AuthenticAMD", "AMD",
    /*4*/ "CentaurHauls", "Centaur",
    /*6*/ "CyrixInstead", "Cyrix",
    /*8*/ "GenuineIntel", "Intel",
    /*10*/ "TransmetaCPU", "Transmeta",
    /*12*/ "GenuineTMx86", "Transmeta",
    /*14*/ "Geode by NSC", "National Semiconductor",
    /*16*/ "NexGenDriven", "NexGen",
    /*18*/ "RiseRiseRise", "Rise",
    /*20*/ "SiS SiS SiS ", "SiS",
    /*22*/ "UMC UMC UMC ", "UMC",
    /*24*/ "VIA VIA VIA ", "VIA",
    /*26*/ "Vortex86 SoC", "Vortex",
    /*28*/ "KVMKVMKVMKVM", "KVM",
    /*30*/ "Microsoft Hv", "Microsoft Hyper - V or Windows Virtual PC",
    /*32*/ "VMwareVMware", "VMware",
    /*34*/ "XenVMMXenVMM", "Xen HVM",
    /*36*/ "Uknown", "Unknown"
};

CPUINFO_ERROR CPUINFO_getVendorName(char **VendorStr)
{
    if (!VendorStr)
    {
        return CPUINFO_ERR_INVALID_INPUT;
    }
    
#if defined(TARGET_WINDOWS)
    int tempCPUInfo[4]; // saved in this order EAX, EBX, ECD and EDX
    __cpuid(tempCPUInfo, 0);
#elif defined(TARGET_LINUX)
    unsigned int tempCPUInfo[4]; // saved in this order EAX, EBX, ECD and EDX
    __get_cpuid(0, &tempCPUInfo[CPUINFO_EAX], &tempCPUInfo[CPUINFO_EBX],
                &tempCPUInfo[CPUINFO_ECX], &tempCPUInfo[CPUINFO_EDX]);
#endif

    // get vendor id
    char vendorID[96];
    memset(vendorID, 0, sizeof(vendorID));
    memcpy(vendorID, &tempCPUInfo[1], sizeof(int));
    memcpy(vendorID +4, &tempCPUInfo[3], sizeof(int));
    memcpy(vendorID +8, &tempCPUInfo[2], sizeof(int));

    const uint32_t maxSizeTable = (sizeof(CPUINFO_vendorIDTable) / sizeof(char*) -1)/ 2;
    int strFound = 0;
    for (uint32_t ii=0; ii < maxSizeTable && !strFound; ii++)
    {
        if (strcmp(vendorID, &(*CPUINFO_vendorIDTable[ii*2])) == 0)
        {
            size_t strSize = strlen(&(*CPUINFO_vendorIDTable[ii*2 + 1])) + 1;
            *VendorStr = (char*)malloc(sizeof(char)*strSize);
            if (!*VendorStr)
            {
	            return CPUINFO_ERR_DYNAMIC_MEMORY;
            }
            strcpy(*VendorStr, &(*CPUINFO_vendorIDTable[ii*2 + 1]));
			
            strFound = 1;
        }
    }

    if(!strFound)
    {
        size_t strSize = strlen(&(*CPUINFO_vendorIDTable[maxSizeTable*2 + 1])) + 1;
        *VendorStr = (char*)malloc(sizeof(char)*strSize);
        if (!*VendorStr)
        {
            return CPUINFO_ERR_DYNAMIC_MEMORY;
        }
        strcpy(*VendorStr, &(*CPUINFO_vendorIDTable[maxSizeTable*2 + 1]));
    }

    return CPUINFO_ERR_NO_ERR;
}

CPUINFO_ERROR CPUINFO_getCPUName(char **CPUName)
{
    if (!CPUName)
    {
        return CPUINFO_ERR_INVALID_INPUT;
    }

#if defined(TARGET_WINDOWS)
    int tempCPUInfo[4]; // saved in this order EAX, EBX, ECD and EDX
    __cpuid(tempCPUInfo, CPUINFO_GET_HIGHEST_EXTENDED_FUNCTION_SUPPORTED);
#elif defined(TARGET_LINUX)
    unsigned int tempCPUInfo[4]; // saved in this order EAX, EBX, ECD and EDX
    __get_cpuid(CPUINFO_GET_HIGHEST_EXTENDED_FUNCTION_SUPPORTED, &tempCPUInfo[CPUINFO_EAX], &tempCPUInfo[CPUINFO_EBX],
        &tempCPUInfo[CPUINFO_ECX], &tempCPUInfo[CPUINFO_EDX]);
#endif

    if (tempCPUInfo[0] < CPUINFO_PROCESSOR_BRAND_STRING_4)
    {
        const char *unsupportedFeatureStr = "Unknown";
        *CPUName = (char*)malloc(sizeof(char)*(strlen(unsupportedFeatureStr)+1));
        if (!CPUName)
        {
            return CPUINFO_ERR_DYNAMIC_MEMORY;
        }

        strcpy(*CPUName, unsupportedFeatureStr);
        return CPUINFO_ERR_CANT_GET_CPU_NAME;
    }

    // get cpu brand
    *CPUName = (char*)malloc(sizeof(char)*48);
    if (!*CPUName)
    {
        return CPUINFO_ERR_DYNAMIC_MEMORY;
    }

    memset(*CPUName, 0, sizeof(char)*48);
    for (int ii = 0; ii < 3; ii++)
    {
#if defined(TARGET_WINDOWS)
        __cpuid(tempCPUInfo, ii + CPUINFO_PROCESSOR_BRAND_STRING_2);
#elif defined(TARGET_LINUX)
        __get_cpuid(ii + CPUINFO_PROCESSOR_BRAND_STRING_2, &tempCPUInfo[CPUINFO_EAX], &tempCPUInfo[CPUINFO_EBX],
                    &tempCPUInfo[CPUINFO_ECX], &tempCPUInfo[CPUINFO_EDX]);
#endif
        memcpy(*CPUName + ii*16, &tempCPUInfo, sizeof(tempCPUInfo));
    }

    return CPUINFO_ERR_NO_ERR;
}

CPUINFO_ERROR CPUINFO_getCPUFeatureFlags(uint32_t *CPUFeatureFlags)
{
    if (!CPUFeatureFlags)
    {
        return CPUINFO_ERR_INVALID_INPUT;
    }

    *CPUFeatureFlags = 0;

#if defined(TARGET_WINDOWS)
    int tempCPUInfo[4]; // saved in this order EAX, EBX, ECD and EDX
    __cpuid(tempCPUInfo, 0);
    int maxFuntionID = tempCPUInfo[0];
#elif defined(TARGET_LINUX)
    unsigned int tempCPUInfo[4]; // saved in this order EAX, EBX, ECD and EDX
    __get_cpuid(0, &tempCPUInfo[CPUINFO_EAX], &tempCPUInfo[CPUINFO_EBX], 
                &tempCPUInfo[CPUINFO_ECX], &tempCPUInfo[CPUINFO_EDX]);
    unsigned int maxFuntionID = tempCPUInfo[0];
#endif

    if (maxFuntionID < CPUINFO_FEATURE_BITS)
    {
        return CPUINFO_ERR_NOT_SOPPORTED;
    }

#if defined(TARGET_WINDOWS)
    __cpuid(tempCPUInfo, CPUINFO_FEATURE_BITS);
#elif defined(TARGET_LINUX)
    __get_cpuid(CPUINFO_FEATURE_BITS, &tempCPUInfo[CPUINFO_EAX], &tempCPUInfo[CPUINFO_EBX],
                &tempCPUInfo[CPUINFO_ECX], &tempCPUInfo[CPUINFO_EDX]);
#endif
	
    if (tempCPUInfo[CPUINFO_EDX] & CPUINFO_BITMASK_00000001_EDX_MMX)
    {
        *CPUFeatureFlags |= CPUINFO_FEATURE_FLAG_MMX;
    }

    if (tempCPUInfo[CPUINFO_EDX] & CPUINFO_BITMASK_00000001_EDX_SSE)
    {
        *CPUFeatureFlags |= CPUINFO_FEATURE_FLAG_SSE;
    }
	
    if (tempCPUInfo[CPUINFO_EDX] & CPUINFO_BITMASK_00000001_EDX_SSE2)
    {
        *CPUFeatureFlags |= CPUINFO_FEATURE_FLAG_SSE2;
    }

    if (tempCPUInfo[CPUINFO_ECX] & CPUINFO_BITMASK_00000001_ECX_SSE3)
    {
        *CPUFeatureFlags |= CPUINFO_FEATURE_FLAG_SSE3;
    }

    if (tempCPUInfo[CPUINFO_ECX] & CPUINFO_BITMASK_00000001_ECX_SSSE3)
    {
        *CPUFeatureFlags |= CPUINFO_FEATURE_FLAG_SSSE3;
    }

    if (tempCPUInfo[CPUINFO_ECX] & CPUINFO_BITMASK_00000001_ECX_FMA3)
    {
        *CPUFeatureFlags |= CPUINFO_FEATURE_FLAG_FMA3;
    }

    if (tempCPUInfo[CPUINFO_ECX] & CPUINFO_BITMASK_00000001_ECX_SSE41)
    {
    *CPUFeatureFlags |= CPUINFO_FEATURE_FLAG_SSE41;
    }
	
    if (tempCPUInfo[CPUINFO_ECX] & CPUINFO_BITMASK_00000001_ECX_SSE42)
    {
        *CPUFeatureFlags |= CPUINFO_FEATURE_FLAG_SSE42;
    }

    if (tempCPUInfo[CPUINFO_ECX] & CPUINFO_BITMASK_00000001_ECX_AVX)
    {
        *CPUFeatureFlags |= CPUINFO_FEATURE_FLAG_AVX;
    }

    if (tempCPUInfo[CPUINFO_EBX] & CPUINFO_BITMASK_00000001_EBX_AVX2)
    {
        *CPUFeatureFlags |= CPUINFO_FEATURE_FLAG_AVX2;
    }
	
	
    // EAX=80000001h: Extended Processor Info and Feature Bits
#if defined(TARGET_WINDOWS)
    __cpuid(tempCPUInfo, CPUINFO_GET_HIGHEST_EXTENDED_FUNCTION_SUPPORTED);
#elif defined(TARGET_LINUX)
    __get_cpuid(CPUINFO_GET_HIGHEST_EXTENDED_FUNCTION_SUPPORTED, &tempCPUInfo[CPUINFO_EAX], &tempCPUInfo[CPUINFO_EBX],
                &tempCPUInfo[CPUINFO_ECX], &tempCPUInfo[CPUINFO_EDX]);
#endif

    maxFuntionID = tempCPUInfo[0];
    if (maxFuntionID >= CPUINFO_EXTENDED_FEATURE_BITS)
    {
        
#if defined(TARGET_WINDOWS)
        __cpuid(tempCPUInfo, CPUINFO_EXTENDED_FEATURE_BITS);
#elif defined(TARGET_LINUX)
        __get_cpuid(CPUINFO_EXTENDED_FEATURE_BITS, &tempCPUInfo[CPUINFO_EAX], &tempCPUInfo[CPUINFO_EBX],
                    &tempCPUInfo[CPUINFO_ECX], &tempCPUInfo[CPUINFO_EDX]);
#endif

        if (tempCPUInfo[CPUINFO_EDX] & CPUINFO_BITMASK_80000001_EDX_MMXEXT)
        {
	        *CPUFeatureFlags |= CPUINFO_FEATURE_FLAG_MMXEXT;
        }

        if (tempCPUInfo[CPUINFO_EDX] & CPUINFO_BITMASK_80000001_EDX_MMX)
        {
	        *CPUFeatureFlags |= CPUINFO_FEATURE_FLAG_MMX;
        }
	
        if (tempCPUInfo[CPUINFO_EDX] & CPUINFO_BITMASK_80000001_EDX_3DNOWEXT)
        {
	        *CPUFeatureFlags |= CPUINFO_FEATURE_FLAG_3DNOWEXT;
        }

        if (tempCPUInfo[CPUINFO_EDX] & CPUINFO_BITMASK_80000001_EDX_3DNOW)
        {
	        *CPUFeatureFlags |= CPUINFO_FEATURE_FLAG_3DNOW;
        }
	
        if (tempCPUInfo[CPUINFO_ECX] & CPUINFO_BITMASK_80000001_ECX_FMA4)
        {
	        *CPUFeatureFlags |= CPUINFO_FEATURE_FLAG_FMA4;
        }

        if (tempCPUInfo[CPUINFO_ECX] & CPUINFO_BITMASK_80000001_ECX_SSE4a)
        {
	        *CPUFeatureFlags |= CPUINFO_FEATURE_FLAG_SSE4a;
        }
    }

    return CPUINFO_ERR_NO_ERR;
}
