#include "kcl_os.h"
#include "kcl_io.h"
#include "fbo.h"
#include "platform.h"
#include "zlib.h"
#include "glb_module.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#include "kcl_osimpl_nacl.h"

#include <cstdarg>
#include "ppapi/cpp/instance.h"
#include "ppapi/cpp/var.h"
extern PP_Instance g_instance;

using namespace GLB;
using namespace KCL;

#define SLEEP_MICROTIMER
//#define NV_TIMER

KCL::OS *KCL::g_os = 0;
extern "C" long GetFreeMemory();

namespace KCL
{
	void* OSImpl::LoadModuleLibrary(const char* libraryName)
	{
		assert(0);
		return 0;
	}


	void OSImpl::ReleaseLibrary(void* module)
	{
		assert(0);
	}


	const char* OSImpl::GetSoftwareVersionString()
	{
		return m_software_version;
	}


	void OSImpl::SetSoftwareVersionString(const char* newvalue)
	{
		strncpy(m_software_version, newvalue, sizeof(m_software_version)-1);
		m_software_version[sizeof(m_software_version)-1] = 0;
	}


	bool OSImpl::LoadingCallback( float p)
	{
		return m_low_memory_flag == 0;
	}


	uint32 OSImpl::GetFreeMemoryMBytes()
	{
		//return GetFreeMemory();
    return 5000;
	}


	OSImpl::OSImpl(const char *device_name)
	{
		m_globalFBO = 0;
		memset(m_device_name, 0, 2048);
		memset(m_software_version, 0, sizeof(m_software_version));

		strcpy( m_device_name, device_name);
		m_low_memory_flag = 0;
	}

	const char* OSImpl::GetDeviceName()
	{
		return m_device_name;
	}


	void OSImpl::ResetInactivity()
	{
	}


	void OSImpl::Sleep (uint32 millisec)
	{
#ifdef SLEEP_MICROTIMER
		//usleep(millisec * 1000);
#else
		struct timespec timeLeft;
		timeLeft.tv_sec = 0;
		timeLeft.tv_nsec = millisec * 1000000;
		do
		{

		} while ( nanosleep(&timeLeft, &timeLeft) == -1);
#endif
	}


	double OSImpl::GetTimeMilliSec()
	{
#ifdef NV_TIMER
		if(eglGetSystemTimeNV)
		{
			khronos_uint64_t egltime;
			khronos_uint64_t egltimequot;
			khronos_uint64_t egltimerem;

			egltime = eglGetSystemTimeNV();

			egltimequot = egltime / eglGetSystemTimeFrequencyNV();
			egltimerem = egltime - (eglGetSystemTimeFrequencyNV() * egltimequot);
			egltimequot *= 1000;
			egltimerem *= 1000;
			egltimerem /= eglGetSystemTimeFrequencyNV();
			egltimequot += egltimerem;
			return (long) egltimequot;
		}
#else
		struct timespec time_now;
		clock_gettime(CLOCK_MONOTONIC, &time_now);
		return (time_now.tv_sec * 1000LL) + time_now.tv_nsec / 1000000LL;
#endif
	}


	void* OSImpl::GetGlobalFBO()
	{
		return (void*)&m_globalFBO;
	}

	void OSImpl::SetGlobalFBO(void* fbo)
	{
		m_globalFBO = *(KCL::uint32*)fbo;
		FBO::InvalidateLastBound();
	}


	void OSImpl::Log( const char *fmt, ...)
	{
#ifndef DISTRIBUTION

    pp::Instance cur_instance = pp::Instance(g_instance);

    char buffer[256];
    va_list args;
    va_start (args, fmt);
    vsnprintf (buffer,256,fmt, args);
    pp::Var message = pp::Var(buffer);
    cur_instance.PostMessage(message); 
    va_end (args);
#endif
	}


	OS* OS::CreateI (const char *device_name)
	{
		OSImpl *osi = new OSImpl( device_name);		

		return osi;
	}

	void OS::DestroyI( OS** os)
	{
		delete *os;
	}

	double OSImpl::GetDisplayBrightness()
	{
		return m_display_brightness;
	}


	void OSImpl::SetDisplayBrightness(double newvalue)
	{
		m_display_brightness = newvalue;
	}


	void OSImpl::SetBatteryLevel(double newvalue)
	{
		m_battery_level = newvalue;
	}


	double OSImpl::GetBatteryLevel()
	{
		return m_battery_level;
	}


	void OSImpl::SetBatteryCharging(bool newvalue)
	{
		m_battery_charging = newvalue;
	}


	bool OSImpl::IsBatteryCharging()
	{
		return m_battery_charging;
	}
}
