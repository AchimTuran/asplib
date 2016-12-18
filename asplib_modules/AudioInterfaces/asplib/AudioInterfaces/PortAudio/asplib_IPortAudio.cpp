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



#include "AudioInterfaces/PortAudio/asplib_IPortAudio.h"

#include <iostream>
#include <string.h>
using namespace std;

#if defined(TARGET_WINDOWS)
  #include <pa_asio.h>
  #include <pa_win_wasapi.h>
  #include <pa_win_ds.h>
  #include <pa_win_wdmks.h>
  #include <pa_win_wmme.h>
  #include <pa_jack.h>
  #include <pa_win_waveformat.h>
#elif defined(TARGET_LINUX)
  #include <pa_linux_alsa.h>
  //#include <pa_jack.h>
#endif

namespace asplib
{
//----------------------------------------------
// PortAudio initializer implementations
//----------------------------------------------
bool CPortAudioHandle::m_IsInitialized = false;
CPortAudioHandle::CPortAudioHandle()
{
	if(!m_IsInitialized)
	{
		PaError paErr = Pa_Initialize();
		if(paErr != paNoError)
		{
			// throw error!
			return;
		}
	}

	m_IsInitialized = true;
}

CPortAudioHandle::~CPortAudioHandle()
{
	if(m_IsInitialized)
	{
		PaError paErr = Pa_Terminate();
		if(paErr != paNoError)
		{
			// throw error!
		}
	}
}

bool CPortAudioHandle::IsInitialized()
{
	return m_IsInitialized;
}


//----------------------------------------------
// PortAudio handle implementations
//----------------------------------------------
IPortAudio::IPortAudio()
{
  ResetValues();

  CreateAvailableDevicesList();
}

IPortAudio::IPortAudio(CPaHostAPIVector_t &UsedHostAPIs)
{
  ResetValues();

  for(uint32_t ii = 0; ii < UsedHostAPIs.size(); ii++)
  {
    m_UsedPaHostAPIs.push_back(UsedHostAPIs[ii]);
  }

  CreateAvailableDevicesList();
}

IPortAudio::~IPortAudio()
{
	stop_Device();
  DestroyValues();
}

uint32_t IPortAudio::get_AvailableDevices(CPaDeviceInfoVector_t &Devices)
{
  Devices.clear();

  for(uint32_t ii = 0; ii < m_DeviceInfos.size(); ii++)
  {
    Devices.push_back(m_DeviceInfos[ii]);
  }

  return (uint32_t)Devices.size();
}

long IPortAudio::get_InputFrameSize()
{
  return m_InputFrameSize;
}

long IPortAudio::get_OutputFrameSize()
{
  return m_OutputFrameSize;
}

uint32_t IPortAudio::get_InputChannelAmount()
{
  return m_InputParameters.channelCount;
}

uint32_t IPortAudio::get_OutputChannelAmount()
{
  return m_OutputParameters.channelCount;
}

double IPortAudio::get_InputSampleFrequency()
{
  return m_SampleFrequency;
}

double IPortAudio::get_OutputSampleFrequency()
{
  return m_SampleFrequency;
}

std::string IPortAudio::get_PortAudioErrStr(PaError paErr)
{
	return Pa_GetErrorText(paErr);
}

PaError IPortAudio::configure_Device( uint32_t MaxInCh, uint32_t MaxOutCh,
                                      uint32_t SampleFrequency,
                                      PaDeviceIndex InDeviceID, PaDeviceIndex OutDeviceID,
                                      PaSampleFormat Format,
                                      long InputDeviceBufferSize, long OutputDeviceBufferSize)
{
	if(!CPortAudioHandle::IsInitialized())
	{
		return paNotInitialized;
	}

  if(m_PaStream)
  {
    PaError paErr = stop_Device();
    if(paErr != paNoError)
    {
      return paErr;
    }

    DestroyValues();
    ResetValues();
  }

  if(MaxInCh <= 0 && MaxOutCh <= 0)
	{
		return paInvalidChannelCount;
	}
  m_InputParameters.channelCount  = MaxInCh;
  m_OutputParameters.channelCount = MaxOutCh;

	if(SampleFrequency <= 0.0)
	{
		return paInvalidSampleRate;
	}
	m_SampleFrequency = (double)SampleFrequency;

  // try to pre configure input device
  if(m_InputParameters.channelCount > 0)
  {
    // try to get selected input device
    m_InputDeviceInfo.deviceInfo = (PaDeviceInfo*)Pa_GetDeviceInfo(InDeviceID);
    if(!Pa_GetDeviceInfo(InDeviceID))
    { // try to get default device
      m_InputDeviceInfo.deviceInfo = (PaDeviceInfo*)Pa_GetDeviceInfo(Pa_GetDefaultInputDevice());
      if(!m_InputDeviceInfo.deviceInfo)
      {
        return paInvalidDevice;
      }
      else
      {
        m_InputDeviceInfo.paDeviceIdx = Pa_GetDefaultInputDevice();
      }
    }
    else
    {
      m_InputDeviceInfo.paDeviceIdx = InDeviceID;
    }

    m_InputParameters.device = m_InputDeviceInfo.paDeviceIdx;
    m_InputDeviceInfo.deviceName = m_InputDeviceInfo.deviceInfo->name;
    m_InputParameters.sampleFormat = Format;

    if(m_InputDeviceInfo.deviceInfo->maxInputChannels <= 0)
    {
      return paInvalidDevice;
    }

    if(m_InputParameters.channelCount > m_InputDeviceInfo.deviceInfo->maxInputChannels)
    {
      cout << "WARNING: requested channel amount is greater than the supported amount of the devide." << endl;
      cout << "WARNING: setting maximum input channels to: " << m_InputDeviceInfo.deviceInfo->maxInputChannels << endl;
      m_InputParameters.channelCount = m_InputDeviceInfo.deviceInfo->maxInputChannels;
    }

    PaError paErr = configure_InputDevice(InputDeviceBufferSize);
    if(paErr != paNoError)
    {
      return paErr;
    }
  }
  

  // try to pre configure input device
  if(m_OutputParameters.channelCount > 0)
  {
    // try to get selected input device
    m_OutputDeviceInfo.deviceInfo = (PaDeviceInfo*)Pa_GetDeviceInfo(OutDeviceID);
    if(!Pa_GetDeviceInfo(OutDeviceID))
    { // try to get default device
      m_OutputDeviceInfo.deviceInfo = (PaDeviceInfo*)Pa_GetDeviceInfo(Pa_GetDefaultOutputDevice());
      if (!m_OutputDeviceInfo.deviceInfo)
      {
        return paInvalidDevice;
      }
      else
      {
        m_OutputDeviceInfo.paDeviceIdx = Pa_GetDefaultOutputDevice();
      }
    }
    else
    {
      m_OutputDeviceInfo.paDeviceIdx = OutDeviceID;
    }

    m_OutputParameters.device = m_OutputDeviceInfo.paDeviceIdx;
    m_OutputDeviceInfo.deviceName = m_OutputDeviceInfo.deviceInfo->name;
    m_OutputParameters.sampleFormat = Format;
    PaError paErr = configure_OutputDevice(OutputDeviceBufferSize);
    if(paErr != paNoError)
    {
      return paErr;
    }
  }

  if((m_InputParameters.channelCount > 0 && m_OutputParameters.channelCount > 0) 
     && (m_InputParameters.device != m_OutputParameters.device))
  {
    return paBadIODeviceCombination;
  }

	// ----------------------------------------------------------------------------------------------------------------------------------
	// check if requested audio device parameters are supported
	// ----------------------------------------------------------------------------------------------------------------------------------
	PaError paErr = paNoError;
	if(m_InputParameters.channelCount > 0 && m_OutputParameters.channelCount > 0)
	{
		paErr = Pa_IsFormatSupported(&m_InputParameters, &m_OutputParameters, m_SampleFrequency);
	}
	else if(m_InputParameters.channelCount > 0)
	{
    paErr = Pa_IsFormatSupported(&m_InputParameters, NULL, m_SampleFrequency);
	}
	else if(m_OutputParameters.channelCount > 0)
	{
    paErr = Pa_IsFormatSupported(NULL, &m_OutputParameters, m_SampleFrequency);
	}
	else
	{
		return paInvalidChannelCount;
	}
	
	if(paErr != paNoError)
	{
		return paErr;
	}

	cout << "---- portaudio device configuration ----" << endl;
  if(m_InputDeviceInfo.deviceInfo)
  {
    cout << " Input Host  API: " << Pa_GetHostApiInfo(m_InputDeviceInfo.deviceInfo->hostApi)->name << " device name: " << m_InputDeviceInfo.deviceName << endl;
  }
  if(m_OutputDeviceInfo.deviceInfo)
  {
    cout << " Output Host API: " << Pa_GetHostApiInfo(m_OutputDeviceInfo.deviceInfo->hostApi)->name << " device name: " << m_OutputDeviceInfo.deviceName << endl;
  }
	cout << " maximum input  channels: " << m_InputParameters.channelCount << endl;
  cout << " maximum output channels: " << m_OutputParameters.channelCount << endl;
	cout << " used audio input  device frame size: " << m_InputFrameSize << endl << endl;
  cout << " used audio output device frame size: " << m_OutputFrameSize << endl << endl;

	return paNoError;
}

PaError IPortAudio::create_Device()
{
	PaError paErr = paNoError;
  if(m_PaStream)
	{
    paErr = Pa_StopStream(m_PaStream);
		if(paErr != paNoError)
		{
			return paErr;
		}
	}

  if(m_SampleFrequency < 0.0)
  {
    return paInvalidSampleRate;
  }

  if(m_InputFrameSize < 0)
  {
    return paInsufficientMemory;
  }

	if(m_InputParameters.channelCount > 0 && m_OutputParameters.channelCount > 0)
	{
    return Pa_OpenStream(&m_PaStream,
							           &m_InputParameters,
							           &m_OutputParameters,
                         m_SampleFrequency,
                         m_InputFrameSize,
							           paNoFlag,
							           staticAudioCallback,
							           this);
	}
	else if(m_InputParameters.channelCount > 0)
	{
    return Pa_OpenStream(&m_PaStream,
							           &m_InputParameters,
							           NULL,
                         m_SampleFrequency,
                         m_InputFrameSize,
							           paNoFlag,
							           staticAudioCallback,
							           this);
	}
	else if(m_OutputParameters.channelCount > 0)
	{
    return Pa_OpenStream(&m_PaStream,
							           NULL,
							           &m_OutputParameters,
                         m_SampleFrequency,
							           m_OutputFrameSize,
							           paNoFlag,
							           staticAudioCallback,
							           this);
	}

	return paInvalidChannelCount;
}

PaError IPortAudio::start_Device()
{
  if(!m_PaStream)
	{
		return paBadStreamPtr;
	}

  PaError paErr = Pa_StartStream(m_PaStream);
  if(paErr != paNoError)
  {
    return paErr;
  }

  // wait until the device is started
  do
  {
      paErr = Pa_IsStreamStopped(m_PaStream);
  }
  while(paErr > paNoError);

  return paErr;
}

PaError IPortAudio::stop_Device()
{
  if(!m_PaStream)
	{
		return paBadStreamPtr;
	}

  PaError paErr = Pa_AbortStream(m_PaStream);
  if(paErr != paNoError && paErr != paStreamIsStopped)
  {
    return paErr;
  }

  // wait until the device is stopped
  do
  {
    paErr = Pa_IsStreamActive(m_PaStream);
  }
  while(paErr > paNoError);

  return paErr;
}

int IPortAudio::staticAudioCallback(const void *inputBuffer, void *outputBuffer,
											              unsigned long framesPerBuffer,
											              const PaStreamCallbackTimeInfo* timeInfo,
											              PaStreamCallbackFlags statusFlags,
											              void *userData)
{
	return static_cast<IPortAudio*>(userData)->AudioCallback(	inputBuffer, outputBuffer,
																	                          framesPerBuffer,
																	                          timeInfo,
																	                          statusFlags,
																	                          userData);
}

PaError IPortAudio::configure_OutputDevice(long FrameSize)
{
#if defined(TARGET_WINDOWS)
  // configure HostAPI audio channels
#if defined(PA_USE_ASIO)
  if(m_UsedOutputHostAPI == paASIO)
  { // Configure ASIO device
    m_OutputParameters.hostApiSpecificStreamInfo = new PaAsioStreamInfo;
    memset(m_OutputParameters.hostApiSpecificStreamInfo, 0, sizeof(PaAsioStreamInfo));

    ((PaAsioStreamInfo*)m_OutputParameters.hostApiSpecificStreamInfo)->size = sizeof(PaAsioStreamInfo);
    ((PaAsioStreamInfo*)m_OutputParameters.hostApiSpecificStreamInfo)->hostApiType = paASIO;
    ((PaAsioStreamInfo*)m_OutputParameters.hostApiSpecificStreamInfo)->version = 1;
    ((PaAsioStreamInfo*)m_OutputParameters.hostApiSpecificStreamInfo)->flags = paAsioUseChannelSelectors;

    ((PaAsioStreamInfo*)m_OutputParameters.hostApiSpecificStreamInfo)->channelSelectors = new int[m_OutputParameters.channelCount];

    for(int ch = 0; ch < m_OutputParameters.channelCount; ch++)
    {
      ((PaAsioStreamInfo*)m_OutputParameters.hostApiSpecificStreamInfo)->channelSelectors[ch] = ch;
    }
  }
  else 
#endif
  if(m_UsedOutputHostAPI == paWASAPI)
  {
    if(m_OutputParameters.channelCount < m_OutputDeviceInfo.deviceInfo->maxOutputChannels)
    { // try to set WASAPI exclusive mode
      m_OutputParameters.hostApiSpecificStreamInfo = new PaWasapiStreamInfo;
      memset(m_OutputParameters.hostApiSpecificStreamInfo, 0, sizeof(PaWasapiStreamInfo));

      ((PaWasapiStreamInfo*)m_OutputParameters.hostApiSpecificStreamInfo)->hostApiType = paWASAPI;
      ((PaWasapiStreamInfo*)m_OutputParameters.hostApiSpecificStreamInfo)->version = 1;
      ((PaWasapiStreamInfo*)m_OutputParameters.hostApiSpecificStreamInfo)->size = sizeof(PaWasapiStreamInfo);
      ((PaWasapiStreamInfo*)m_OutputParameters.hostApiSpecificStreamInfo)->flags = paWinWasapiExclusive;
      PaWinWaveFormat waveFormat;
      if(PaWasapi_GetDeviceDefaultFormat(&waveFormat, sizeof(PaWinWaveFormat), m_OutputDeviceInfo.paDeviceIdx) < 0)
      {
        return paIncompatibleHostApiSpecificStreamInfo;
      }

      // TODO: check how to set channelMask and how to use waveFormat information
      //((PaWasapiStreamInfo*)m_OutputDeviceInfo.hostApiSpecificStreamInfo)->channelMask = 
    }
    else
    {
      m_OutputParameters.hostApiSpecificStreamInfo = NULL;
    }
  }
  else
  {
    m_OutputParameters.hostApiSpecificStreamInfo = NULL;
  }

  // configure device latency/bufferSize
#if defined(PA_USE_ASIO)
  if(m_UsedOutputHostAPI == paASIO)
  {
    long minBufferSizeFrames;
    long maxBufferSizeFrames;
    long preferredBufferSizeFrames;
    long granularity;
    PaError paErr = PaAsio_GetAvailableBufferSizes(m_OutputParameters.device, &minBufferSizeFrames, &maxBufferSizeFrames, &preferredBufferSizeFrames, &granularity);
    if(paErr != paNoError)
    {
      return paErr;
    }

    cout << "ASIO minimum buffer size " << minBufferSizeFrames << endl;
    cout << "ASIO maximum buffer size " << maxBufferSizeFrames << endl;
    cout << "ASIO preferred buffer size " << preferredBufferSizeFrames << endl;

    if(granularity == -1)
    {
      cout << "ASIO buffer granularity is power of 2" << endl;
    }
    else
    {
      cout << "ASIO buffer granularity: " << granularity << endl;
    }

    if(FrameSize < 0)
    {
      m_OutputFrameSize = (unsigned long)preferredBufferSizeFrames;
    }
    else if(FrameSize > maxBufferSizeFrames)
    {
      m_OutputFrameSize = (unsigned long)maxBufferSizeFrames;
    }
    else
    {
      m_OutputFrameSize = (unsigned long)FrameSize;
    }
  }
  else
  {
#endif
    if(FrameSize <= 0)
    {
      m_OutputFrameSize = 2048;
    }
    else
    {
      m_OutputFrameSize = FrameSize;
    }
#if defined(PA_USE_ASIO)
  }
#endif
#elif defined(TARGET_LINUX)
    if(FrameSize <= 0)
    {
      m_OutputFrameSize = 2048;
    }
    else
    {
      m_OutputFrameSize = FrameSize;
    }
#endif

  return paNoError;
}

PaError IPortAudio::configure_InputDevice(long FrameSize)
{
#if defined(TARGET_WINDOWS)
  // configure HostAPI audio channels
#if defined(PA_USE_ASIO)
  if(m_UsedInputHostAPI == paASIO)
  { // Configure ASIO device
    m_InputParameters.hostApiSpecificStreamInfo = new PaAsioStreamInfo;
    memset(m_InputParameters.hostApiSpecificStreamInfo, 0, sizeof(PaAsioStreamInfo));

    ((PaAsioStreamInfo*)m_InputParameters.hostApiSpecificStreamInfo)->size = sizeof(PaAsioStreamInfo);
    ((PaAsioStreamInfo*)m_InputParameters.hostApiSpecificStreamInfo)->hostApiType = paASIO;
    ((PaAsioStreamInfo*)m_InputParameters.hostApiSpecificStreamInfo)->version = 1;
    ((PaAsioStreamInfo*)m_InputParameters.hostApiSpecificStreamInfo)->flags = paAsioUseChannelSelectors;

    ((PaAsioStreamInfo*)m_InputParameters.hostApiSpecificStreamInfo)->channelSelectors = new int[m_InputParameters.channelCount];

    for(int ch = 0; ch < m_InputParameters.channelCount; ch++)
    {
      ((PaAsioStreamInfo*)m_InputParameters.hostApiSpecificStreamInfo)->channelSelectors[ch] = ch;
    }
  }
  else 
#endif
  if(m_UsedInputHostAPI == paWASAPI)
  {
    if(m_InputParameters.channelCount < m_InputDeviceInfo.deviceInfo->maxInputChannels)
    { // try to set WASAPI exclusive mode
      m_InputParameters.hostApiSpecificStreamInfo = new PaWasapiStreamInfo;
      memset(m_InputParameters.hostApiSpecificStreamInfo, 0, sizeof(PaWasapiStreamInfo));

      ((PaWasapiStreamInfo*)m_InputParameters.hostApiSpecificStreamInfo)->hostApiType = paWASAPI;
      ((PaWasapiStreamInfo*)m_InputParameters.hostApiSpecificStreamInfo)->version = 1;
      ((PaWasapiStreamInfo*)m_InputParameters.hostApiSpecificStreamInfo)->size = sizeof(PaWasapiStreamInfo);
      ((PaWasapiStreamInfo*)m_InputParameters.hostApiSpecificStreamInfo)->flags = paWinWasapiExclusive;
      PaWinWaveFormat waveFormat;
      if(PaWasapi_GetDeviceDefaultFormat(&waveFormat, sizeof(PaWinWaveFormat), m_InputDeviceInfo.paDeviceIdx) < 0)
      {
        return paIncompatibleHostApiSpecificStreamInfo;
      }

      // TODO: check how to set channelMask and how to use waveFormat information
      //((PaWasapiStreamInfo*)m_InputParameters.hostApiSpecificStreamInfo)->channelMask = 
    }
    else
    {
      m_InputParameters.hostApiSpecificStreamInfo = NULL;
    }
  }
  else
  {
    m_InputParameters.hostApiSpecificStreamInfo = NULL;
  }

  // configure device latency/bufferSize
#if defined(PA_USE_ASIO)
  if(m_UsedInputHostAPI == paASIO)
  {
    long minBufferSizeFrames;
    long maxBufferSizeFrames;
    long preferredBufferSizeFrames;
    long granularity;
    PaError paErr = PaAsio_GetAvailableBufferSizes(m_InputParameters.device, &minBufferSizeFrames, &maxBufferSizeFrames, &preferredBufferSizeFrames, &granularity);
    if(paErr != paNoError)
    {
      return paErr;
    }

    cout << "ASIO minimum buffer size " << minBufferSizeFrames << endl;
    cout << "ASIO maximum buffer size " << maxBufferSizeFrames << endl;
    cout << "ASIO preferred buffer size " << preferredBufferSizeFrames << endl;

    if(granularity == -1)
    {
      cout << "ASIO buffer granularity is power of 2" << endl;
    }
    else
    {
      cout << "ASIO buffer granularity: " << granularity << endl;
    }

    if(FrameSize < 0)
    { 
      m_InputFrameSize = (unsigned long)preferredBufferSizeFrames;
    }
    else if(FrameSize > maxBufferSizeFrames)
    {
      m_InputFrameSize = (unsigned long)maxBufferSizeFrames;
    }
    else
    {
      m_InputFrameSize = (unsigned long)FrameSize;
    }
  }
  else
  {
#endif
    if(FrameSize <= 0)
    {
      FrameSize = 2048;
    }
    else
    {
      m_InputFrameSize = FrameSize;
    }
#if defined(PA_USE_ASIO)
  }
#endif
#elif defined(TARGET_LINUX)
    if(FrameSize <= 0)
    {
      FrameSize = 2048;
    }
    else
    {
      m_InputFrameSize = FrameSize;
    }
#endif


  return paNoError;
}

void IPortAudio::CreateAvailableDevicesList()
{
  PaDeviceIndex maxDeviceIdx = Pa_GetDeviceCount();

  for(PaDeviceIndex deviceIdx = 0; deviceIdx < maxDeviceIdx; deviceIdx++)
  {
    const PaDeviceInfo *deviceInfo = Pa_GetDeviceInfo(deviceIdx);
    if(m_UsedPaHostAPIs.size() > 0  && deviceInfo)
    {
      for(uint32_t ID = 0; ID < m_UsedPaHostAPIs.size() && deviceInfo; ID++)
      { // filter host APIs
        if(m_UsedPaHostAPIs[ID] == deviceInfo->hostApi)
        {
          m_DeviceInfos.push_back(sPaDeviceInfo(deviceInfo, deviceIdx));
          deviceInfo = NULL;
        }
      }
    }
    else if(deviceInfo)
    { // only save device if pointer is valid
      m_DeviceInfos.push_back(sPaDeviceInfo(deviceInfo, deviceIdx));
    }
  }
}

void IPortAudio::ResetValues()
{
  m_SampleFrequency = -1.0;
  m_InputFrameSize  = 0;
  m_OutputFrameSize = 0;
  m_PaStream        = NULL;

  m_UsedOutputHostAPI = paInDevelopment;
  m_UsedInputHostAPI  = paInDevelopment;

  memset(&m_OutputParameters, 0, sizeof(PaStreamParameters));
  memset(&m_InputParameters,  0, sizeof(PaStreamParameters));
}

void IPortAudio::DestroyValues()
{
  if(m_OutputParameters.hostApiSpecificStreamInfo)
  {
    switch(m_UsedOutputHostAPI)
    {
#if defined(TARGET_WINDOWS)
#if defined(PA_USE_ASIO)
    case paASIO:
    if(((PaAsioStreamInfo*)m_OutputParameters.hostApiSpecificStreamInfo)->channelSelectors)
    {
      delete[]((PaAsioStreamInfo*)m_OutputParameters.hostApiSpecificStreamInfo)->channelSelectors;
      ((PaAsioStreamInfo*)m_OutputParameters.hostApiSpecificStreamInfo)->channelSelectors = NULL;
    }
    break;
#endif
#endif
    }

    delete m_OutputParameters.hostApiSpecificStreamInfo;
    m_OutputParameters.hostApiSpecificStreamInfo = NULL;
  }

  if(m_InputParameters.hostApiSpecificStreamInfo)
  {
    switch(m_UsedInputHostAPI)
    {
#if defined(TARGET_WINDOWS)
#if defined(PA_USE_ASIO)
    case paASIO:
    if(((PaAsioStreamInfo*)m_InputParameters.hostApiSpecificStreamInfo)->channelSelectors)
    {
      delete[]((PaAsioStreamInfo*)m_InputParameters.hostApiSpecificStreamInfo)->channelSelectors;
      ((PaAsioStreamInfo*)m_InputParameters.hostApiSpecificStreamInfo)->channelSelectors = NULL;
    }
    break;
#endif
#endif
    }

    delete m_InputParameters.hostApiSpecificStreamInfo;
    m_InputParameters.hostApiSpecificStreamInfo = NULL;
  }
}
}
