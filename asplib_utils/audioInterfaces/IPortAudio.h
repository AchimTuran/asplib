#pragma once

/* Copyright (C) 2014-2015 Achim Turan, Achim.Turan@o2online.de
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
 * ifnot, see <http://www.gnu.org/licenses/>.
 *
 */



#include <portaudio.h>

#include <string>
#include <vector>
#include <list>

#include "asplib_utils/os/asplib_base_os.h"
#include "asplib_utils/exceptions/asplib_StringException.h"

namespace asplib
{
// type and structure definitions
struct sPaDeviceInfo
{
  PaDeviceInfo *deviceInfo;
  PaDeviceIndex paDeviceIdx;
  std::string deviceName;

  sPaDeviceInfo(const PaDeviceInfo *DeviceInfo, PaDeviceIndex DeviceIdx) :
    deviceInfo((PaDeviceInfo*)DeviceInfo),
    paDeviceIdx(DeviceIdx)
  {
    if(!DeviceInfo)
    {
      deviceName = "UKNOWN";
    }
    else
    {
      deviceName = deviceInfo->name;
    }
  }
  sPaDeviceInfo()
  {
    deviceInfo = NULL;
    paDeviceIdx = paNoDevice;
    deviceName = "";
  }
};

typedef std::vector<sPaDeviceInfo> CPaDeviceInfoVector_t;
typedef std::vector<PaHostApiTypeId> CPaHostAPIVector_t;

#define LOG_TAG_PORTAUDIO "[PortAudio] "

class CPortAudioHandle
{
public:
  CPortAudioHandle();
  ~CPortAudioHandle();
  static bool IsInitialized();

private:
  static bool m_IsInitialized;
};

class IPortAudio
{
public: // public functions
  IPortAudio();
  IPortAudio(CPaHostAPIVector_t &UsedHostAPIs);
  virtual ~IPortAudio();
  uint32_t get_AvailableDevices(CPaDeviceInfoVector_t &Devices);

protected: // protected abstract functions
  static std::string get_PortAudioErrStr(PaError paErr);
  PaError configure_Device( uint32_t MaxInCh, uint32_t MaxOutCh,
                            uint32_t SampleFrequency,
                            PaDeviceIndex InDeviceID = paNoDevice, PaDeviceIndex OutDeviceID = paNoDevice,
                            PaSampleFormat Format = 0,
                            long InputDeviceBufferSize = -1, long OutputDeviceBufferSize = -1);
  PaError create_Device();
  PaError start_Device();
  PaError stop_Device();
  virtual int AudioCallback(const void *inputBuffer, void *outputBuffer,
                            unsigned long framesPerBuffer,
                            const PaStreamCallbackTimeInfo* timeInfo,
                            PaStreamCallbackFlags statusFlags,
                            void *userData) = 0;

  PaHostApiIndex          m_UsedOutputHostAPI;
  PaHostApiIndex          m_UsedInputHostAPI;
  CPaDeviceInfoVector_t   m_DeviceInfos;
  PaStreamParameters      m_OutputParameters;
  PaStreamParameters      m_InputParameters;
  sPaDeviceInfo           m_InputDeviceInfo;
  sPaDeviceInfo           m_OutputDeviceInfo;

  double                  m_SampleFrequency;
  unsigned long           m_InputFrameSize;
  unsigned long           m_OutputFrameSize;
  PaStream               *m_PaStream;

  CPaHostAPIVector_t      m_UsedPaHostAPIs;
  std::list<std::string>  m_DeviceFilterList;

private: // private callback functions
  static int staticAudioCallback(const void *inputBuffer, void *outputBuffer,
                                 unsigned long framesPerBuffer,
                                 const PaStreamCallbackTimeInfo* timeInfo,
                                 PaStreamCallbackFlags statusFlags,
                                 void *userData);

  PaError configure_InputDevice(long FrameSize);
  PaError configure_OutputDevice(long FrameSize);
  void CreateAvailableDevicesList();
  void ResetValues();
};
}
