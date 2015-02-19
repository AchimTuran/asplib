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

// Matlab include files
#include <mex.h>

#include "asplib_MatlabDll.h"

// buffers
single *g_Inbuffers = NULL;
single *g_Outbuffers = NULL;
single **g_Channels = NULL;

// signal processing parameters
unsigned int g_MaxChannels = 0;     // max in-/output channels
unsigned int g_MaxFrameSize = 0;    // max in-/output framelength
single g_fA = 0.0f;                 // sample frequency

// matlab <--> C/C++ control variables
bool g_InitSuccess = false;

// helper function prototypes
void destroy_BiQuads();


// ---------------------------------------- helper functions ----------------------------------------
void destroy_BiQuads()
{
}

// ---------------------------------------- BiQuad functions ----------------------------------------
DLL_EXPORT RET_ERR create_BiQuads()
{

    return ERR_NO_ERROR;
}

DLL_EXPORT RET_ERR process_BiQuads(single *Data)
{
    if(Data == NULL)
    {
        mexErrMsgTxt("Error! Input pointers are NULL!\n");
        return ERR_INVALID_INPUT;
    }

    // copy Data to internal buffer
    memcpy( g_Inbuffers, Data, sizeof(single)*g_MaxChannels*g_MaxFrameSize);

    // process samples
    memcpy(g_Outbuffers, g_Inbuffers, sizeof(single)*g_MaxChannels*g_MaxFrameSize);

    // copy from internal output to Data
    memcpy( Data, g_Outbuffers, sizeof(single)*g_MaxChannels*g_MaxFrameSize);

    return ERR_NO_ERROR;
}

DLL_EXPORT RET_ERR init(single SampleFrequency, uint32 MaxChannels, uint32 MaxFrameSize)
{
    if (MaxChannels == 0 || MaxFrameSize == 0 || SampleFrequency <= 0.0f)
    {
        mexErrMsgTxt("Error! Invalid input! MaxFrames == 0 or MaxChannels == 0 or MaxFrameSize == 0 or ProcessingData_fA <= 0.0f\n");
        return ERR_INVALID_INPUT;
    }

    mexPrintf("-=created asplib_BiQuads=-\n");
    g_MaxChannels = MaxChannels;
    g_MaxFrameSize = MaxFrameSize;
    g_fA = SampleFrequency;

    // create internal buffers
    g_Inbuffers = new single[g_MaxChannels*g_MaxFrameSize];
    g_Outbuffers = new single[g_MaxChannels*g_MaxFrameSize];
    g_Channels = new single*[g_MaxChannels];
    if (g_Inbuffers == NULL || g_Outbuffers == NULL || g_Channels == NULL)
    {
        mexErrMsgTxt("Error! Could not create internal! Not enough free memory?\n");
        delete[] g_Inbuffers;
        delete[] g_Outbuffers;
        delete[] g_Channels;
        return ERR_FATAL_ERROR;
    }

    for (unsigned int ii = 0; ii < g_MaxChannels; ii++)
    {
        g_Channels[ii] = &g_Outbuffers[ii*g_MaxFrameSize];
    }

    g_InitSuccess = true;
}

DLL_EXPORT void destroy()
{
    mexPrintf("-=unloading asplib_MatlabDll=-\n");

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

    destroy_BiQuads();
}
