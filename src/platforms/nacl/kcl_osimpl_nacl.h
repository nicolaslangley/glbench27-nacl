#include "kcl_os.h"

namespace KCL
{

	struct OSImpl : public OS
	{
		void* LoadModuleLibrary(const char* libraryName);
		void ReleaseLibrary(void* module);

		OSImpl(const char *device_name);
		void Sleep (uint32 millisec);
		double GetTimeMilliSec();

		const char* GetDeviceName();
		void* GetGlobalFBO();
		void SetGlobalFBO( void* fbo);
		void ResetInactivity();
		void Log( const char *a, ...);

		const char* GetSoftwareVersionString();
		void SetSoftwareVersionString(const char* newvalue);

		double GetBatteryLevel();
		void SetBatteryLevel(double newvalue);


		double GetDisplayBrightness();
		void SetDisplayBrightness(double newvalue);

		void SetBatteryCharging(bool newvalue);
		bool IsBatteryCharging();

		uint32 GetFreeMemoryMBytes();
		bool LoadingCallback( float p);

		unsigned int m_globalFBO;
		char m_device_name[2048];

		char m_software_version[256];
		bool m_battery_charging;
		double m_display_brightness;
		double m_battery_level;
		int m_low_memory_flag;
		//		std::map<std::string, void*> m_loaded_libraries;
	};

}