/*
 * Copyright (C) 2014 Achim Turan, Achim.Turan@o2online.de
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



#include <string>
using namespace std;

#include "utils/computeDeviceInfo/cpuInfo.h"
#include "BiQuads/apslib_BiQuadFactory.h"

// Matlab include files
#include <mex.h>

#include "asplib_MatlabDll.h"
using namespace asplib;

// buffers
single *g_Inbuffers = NULL;
single *g_Outbuffers = NULL;
single **g_Channels = NULL;

// signal processing parameters
unsigned int g_MaxChannels = 0;     // max in-/output channels
unsigned int g_MaxFrameSize = 0;    // max in-/output framelength
single g_SampleFrequency = 0.0f;    // sample frequency

// global BiQuad variables
ASPLIB_BIQUAD_HANDLE *g_BiQuadHandle = NULL;

// matlab <--> C/C++ control variables
bool g_InitSuccess = false;

// BiQuad helper function prototypes
void destroy_BiQuads();


// ---------------------------------------- BiQuad helper functions ----------------------------------------
void destroy_BiQuads()
{
    ASPLIB_ERR err = CBiQuadFactory::destroy_BiQuads(&g_BiQuadHandle);
    if(err == ASPLIB_ERR_NO_ERROR)
    {
        mexPrintf("%ssucessful destroyed BiQuads\n", ASPLIB_LOGGIN_TAG);
    }
    else
    {
        string errStr = string(ASPLIB_LOGGIN_TAG) + string("Error! Some error occured by destroying BiQuads!\n");
        mexErrMsgTxt(errStr.c_str());
    }
}

// ---------------------------------------- BiQuad functions ----------------------------------------
DLL_EXPORT RET_ERR create_BiQuad(uint32 BiQuadQuantity)
{
    g_BiQuadHandle = CBiQuadFactory::get_BiQuads(BiQuadQuantity, g_SampleFrequency, ASPLIB_OPT_NATIVE);

    if(!g_BiQuadHandle)
    {
        return ERR_FATAL_ERROR;
    }

    return ERR_NO_ERROR;
}

DLL_EXPORT RET_ERR process_BiQuads(single *Data, uint32 MaxFrames)
{
    if(Data == NULL || !MaxFrames)
    {
        string errStr = string(ASPLIB_LOGGIN_TAG) + string("Error! Input pointers are NULL!\n");
        mexErrMsgTxt(errStr.c_str());
        return ERR_INVALID_INPUT;
    }

    if(!g_BiQuadHandle && !g_InitSuccess)
    {
        string errStr = string(ASPLIB_LOGGIN_TAG) + string("Error! asplib_MatlabDll was not initialized!\n");
        mexErrMsgTxt(errStr.c_str());
        return ERR_FATAL_ERROR;
    }

    if(!g_BiQuadHandle)
    {
        string errStr = string(ASPLIB_LOGGIN_TAG) + string("Error! No BiQuads created! Call create_BiQuads first, before calling process_BiQuads!\n");
        mexErrMsgTxt(errStr.c_str());
        return ERR_FATAL_ERROR;
    }

    // copy Data to internal buffer
    memcpy(g_Inbuffers, Data, sizeof(single)*g_MaxChannels*g_MaxFrameSize);

    // process samples
    CBiQuadFactory::calc_BiQuadSamples(g_BiQuadHandle, g_Inbuffers, g_Inbuffers, g_MaxFrameSize);
    //memcpy(g_Outbuffers, g_Inbuffers, sizeof(single)*g_MaxChannels*g_MaxFrameSize);

    // copy from internal output to Data
    memcpy(g_Outbuffers, Data, sizeof(single)*g_MaxChannels*g_MaxFrameSize);

    return ERR_NO_ERROR;
}

DLL_EXPORT RET_ERR set_BiQuadGain(uint32 BiQuadIdx, single Gain)
{
    ASPLIB_CONST_Q_PEAKING_PARAM constQPeakingParam;
    constQPeakingParam.Gain = Gain;

    if(CBiQuadFactory::set_constQPeakingParams(g_BiQuadHandle, Gain, BiQuadIdx) != ASPLIB_ERR_NO_ERROR)
    {
        // ToDo: throw some error!
        return ERR_INVALID_INPUT;
    }

    return ERR_NO_ERROR;
}

DLL_EXPORT RET_ERR set_BiQuadGains(single Gain)
{
    ASPLIB_CONST_Q_PEAKING_PARAM constQPeakingParam;
    constQPeakingParam.Gain = Gain;

    if(CBiQuadFactory::set_constQPeakingParams(g_BiQuadHandle, Gain) != ASPLIB_ERR_NO_ERROR)
    {
        // ToDo: throw some error!
        return ERR_INVALID_INPUT;
    }

    return ERR_NO_ERROR;
}

DLL_EXPORT RET_ERR init_asplib(single SampleFrequency, uint32 MaxChannels, uint32 MaxFrameSize)
{
    if (MaxChannels == 0 || MaxFrameSize == 0 || SampleFrequency <= 0.0f)
    {
        string errStr = string(ASPLIB_LOGGIN_TAG) + string("Error! Invalid input! MaxFrames == 0 or MaxChannels == 0 or MaxFrameSize == 0 or ProcessingData_fA <= 0.0f\n");
        mexErrMsgTxt(errStr.c_str());
        return ERR_INVALID_INPUT;
    }

    mexPrintf("%sloading asplib_MatlabDll\n", ASPLIB_LOGGIN_TAG);
    //mexPrintf("%screated asplib_BiQuads\n", ASPLIB_LOGGIN_TAG);
    g_MaxChannels = MaxChannels;
    g_MaxFrameSize = MaxFrameSize;
    g_SampleFrequency = SampleFrequency;

    // create internal buffers
    g_Inbuffers = new single[g_MaxChannels*g_MaxFrameSize];
    g_Outbuffers = new single[g_MaxChannels*g_MaxFrameSize];
    g_Channels = new single*[g_MaxChannels];
    if (g_Inbuffers == NULL || g_Outbuffers == NULL || g_Channels == NULL)
    {
        string errStr = string(ASPLIB_LOGGIN_TAG) + string("Error! Could not create internal! Not enough free memory?\n");
        mexErrMsgTxt(errStr.c_str());
        delete [] g_Inbuffers;
        delete [] g_Outbuffers;
        delete [] g_Channels;
        return ERR_FATAL_ERROR;
    }

    for (unsigned int ii = 0; ii < g_MaxChannels; ii++)
    {
        g_Channels[ii] = &g_Outbuffers[ii*g_MaxFrameSize];
    }

    g_InitSuccess = true;

    return ERR_NO_ERROR;
}

DLL_EXPORT void destroy()
{
    mexPrintf("%sunloading asplib_MatlabDll\n", ASPLIB_LOGGIN_TAG);

    if(g_Channels)
    {
        delete [] g_Channels;
        g_Channels = NULL;
    }

    if(g_Inbuffers)
    {
        delete [] g_Inbuffers;
        g_Inbuffers = NULL;
    }

    if(g_Outbuffers)
    {
        delete [] g_Outbuffers;
        g_Outbuffers = NULL;
    }

    g_SampleFrequency = 0.0f;
    g_Channels = 0;
    g_MaxChannels = 0;
    g_InitSuccess = false;

    destroy_BiQuads();
}
