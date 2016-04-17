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

#include "Biquad/apslib_BiquadFactory.h"

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

// global Biquad variables
ASPLIB_BIQUAD_HANDLE *g_BiquadHandle = NULL;

// matlab <--> C/C++ control variables
bool g_InitSuccess = false;

// Biquad helper function prototypes
void destroy_Biquads();


// ---------------------------------------- Biquad helper functions ----------------------------------------
void destroy_Biquads()
{
    ASPLIB_ERR err = CBiquadFactory::destroy_Biquads(&g_BiquadHandle);
    if(err == ASPLIB_ERR_NO_ERROR)
    {
        mexPrintf("%ssucessful destroyed Biquads\n", ASPLIB_LOGGING_TAG);
    }
    else
    {
        string errStr = string(ASPLIB_LOGGING_TAG) + string("Error! Some error occured by destroying Biquads!\n");
        mexErrMsgTxt(errStr.c_str());
    }
}

// ---------------------------------------- Biquad functions ----------------------------------------
DLL_EXPORT RET_ERR create_Biquad(uint32 BiquadQuantity)
{
    if(g_BiquadHandle)
    {
        const uint maxOldFreqBands = CBiquadFactory::get_maxBiquads(g_BiquadHandle);
        CBiquadFactory::destroy_Biquads(&g_BiquadHandle);

        mexPrintf("%sdeleted old Biquad Filter with %i frequency bands\n", ASPLIB_LOGGING_TAG, maxOldFreqBands);
    }
    // ToDo: add support for several optimizations
    g_BiquadHandle = CBiquadFactory::get_Biquads(BiquadQuantity, g_SampleFrequency, ASPLIB_OPT_NATIVE);

    if(!g_BiquadHandle)
    {
        // Evaluate get_Biquads function error
        string errStr = string(ASPLIB_LOGGING_TAG) + string("Error! Input pointers are NULL!\n");
        mexErrMsgTxt(errStr.c_str());
        return ERR_FATAL_ERROR;
    }

    mexPrintf("%ssuccessful created Biquad Filter with %i frequency bands\n", ASPLIB_LOGGING_TAG, BiquadQuantity);

    return ERR_NO_ERROR;
}

DLL_EXPORT RET_ERR process_Biquads(single *Data, uint32 MaxFrames)
{
    if(Data == NULL || !MaxFrames)
    {
        string errStr = string(ASPLIB_LOGGING_TAG) + string("Error! Input pointers are NULL!\n");
        mexErrMsgTxt(errStr.c_str());
        return ERR_INVALID_INPUT;
    }

    if(!g_BiquadHandle && !g_InitSuccess)
    {
        string errStr = string(ASPLIB_LOGGING_TAG) + string("Error! asplib_MatlabDll was not initialized!\n");
        mexErrMsgTxt(errStr.c_str());
        return ERR_FATAL_ERROR;
    }

    if(!g_BiquadHandle)
    {
        string errStr = string(ASPLIB_LOGGING_TAG) + string("Error! No Biquads created! Call create_Biquads first, before calling process_Biquads!\n");
        mexErrMsgTxt(errStr.c_str());
        return ERR_FATAL_ERROR;
    }

    //// copy Data to internal buffer
    //memcpy(g_Inbuffers, Data, sizeof(single)*g_MaxChannels*g_MaxFrameSize);

    // process samples
    CBiquadFactory::calc_BiquadSamples(g_BiquadHandle, Data, Data, MaxFrames);
    //CBiquadFactory::calc_BiquadSamples(g_BiquadHandle, g_Inbuffers, g_Outbuffers, g_MaxFrameSize);
    //memcpy(g_Outbuffers, g_Inbuffers, sizeof(single)*g_MaxChannels*g_MaxFrameSize);

    //// copy from internal output to Data
    //memcpy(g_Outbuffers, Data, sizeof(single)*g_MaxChannels*g_MaxFrameSize);

    return ERR_NO_ERROR;
}

DLL_EXPORT RET_ERR set_BiquadGain(uint32 BiquadIdx, single Gain)
{
    ASPLIB_CONST_Q_PEAKING_PARAM constQPeakingParam;
    constQPeakingParam.Gain = Gain;

    if(CBiquadFactory::set_constQPeakingParams(g_BiquadHandle, Gain, BiquadIdx) != ASPLIB_ERR_NO_ERROR)
    {
        // ToDo: throw some error!
        return ERR_INVALID_INPUT;
    }

    return ERR_NO_ERROR;
}

DLL_EXPORT RET_ERR set_BiquadGains(single Gain)
{
    ASPLIB_CONST_Q_PEAKING_PARAM constQPeakingParam;
    constQPeakingParam.Gain = Gain;

    if(CBiquadFactory::set_constQPeakingParams(g_BiquadHandle, Gain) != ASPLIB_ERR_NO_ERROR)
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
        string errStr = string(ASPLIB_LOGGING_TAG) + string("Error! Invalid input! MaxFrames == 0 or MaxChannels == 0 or MaxFrameSize == 0 or ProcessingData_fA <= 0.0f\n");
        mexErrMsgTxt(errStr.c_str());
        return ERR_INVALID_INPUT;
    }

    mexPrintf("%sinitialize asplib_MatlabDll\n", ASPLIB_LOGGING_TAG);
    //mexPrintf("%screated asplib_Biquads\n", ASPLIB_LOGGING_TAG);
    g_MaxChannels = MaxChannels;
    g_MaxFrameSize = MaxFrameSize;
    g_SampleFrequency = SampleFrequency;

    // create internal buffers
    g_Inbuffers = new single[g_MaxChannels*g_MaxFrameSize];
    g_Outbuffers = new single[g_MaxChannels*g_MaxFrameSize];
    g_Channels = new single*[g_MaxChannels];
    if (g_Inbuffers == NULL || g_Outbuffers == NULL || g_Channels == NULL)
    {
        string errStr = string(ASPLIB_LOGGING_TAG) + string("Error! Could not create internal! Not enough free memory?\n");
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
    mexPrintf("%sdestroy asplib_MatlabDll\n", ASPLIB_LOGGING_TAG);

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

    destroy_Biquads();
}
