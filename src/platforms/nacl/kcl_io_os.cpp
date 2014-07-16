#include "kcl_io.h"

#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <set>
#include <cassert>

#include "ppapi/cpp/completion_callback.h"
#include "ppapi/cpp/instance.h"
#include "ppapi/cpp/module.h"
#include "ppapi/cpp/file_io.h"
#include "ppapi/cpp/var.h"
#include "ppapi/cpp/var_array.h"
#include "ppapi/cpp/file_system.h"
#include "ppapi/utility/completion_callback_factory.h"
#include "ppapi/utility/threading/simple_thread.h"

pp::FileIO file;
extern PP_Instance global_instance;

//KCL::File::File(const std::string& filename, const KCL::OPENMODE& mode, const WORKING_DIRERCTORY& path)
//{
//	m_original_filename = filename;
//	m_mode = mode;
//	m_error = KCL_NO_ERROR;
//	m_working_dir = path;
//	m_buffer = 0;
//	m_file_pointer = 0;
//	m_filesize = -1;
//	m_is_filemapped = false;
//
//	if (path==KCL::RWDir)
//	{
//		m_filename = GetDataRWPath();
//	}
//	else if(path==KCL::RDir)
//	{
//		m_filename = GetDataPath();
//	}
//
//	if(m_filename.length() < 1)
//	{
//		m_error = KCL_NO_VALID_WOKRING_DIRECTORY;
//		abort();
//	}
//
//	m_filename += filename;
//}
//
//void KCL::File::open_file_()
//{
//	m_error = KCL_NO_ERROR;
//	GenerateHashName();
//
//	std::string mode;
//	if(m_mode==KCL::Read)
//	{
//		mode = "rb";
//	}
//	else if(m_mode==KCL::Write)
//	{
//		mode = "wb";
//	}
//
//	OS_PreOpen();
//	if(!m_buffer)
//	{
//		m_file_pointer = fopen(m_filename.c_str(), mode.c_str());
//
//		if (!m_file_pointer)
//		{
//			if(m_working_dir==RDir && m_mode==KCL::Read)
//			{
//				m_file_pointer = fopen(m_hash_filename.c_str(), mode.c_str());
//			}
//
//			if (!m_file_pointer)
//			{
//				m_error = KCL_NO_FILE_EXISTS;
//			}
//		}
//	}
//	GetLength();
//}
//
//int KCL::File::Read(void* data, unsigned size, unsigned count)
//{
//  if(size == 0 || count ==0)
//	{
//		return 0;
//	}
//}
//
//int KCL::File::Write(const void* data, unsigned size, unsigned count)
//{ }
//
//void KCL::File::Close()
//{ }

void KCL::File::OS_PreOpen()
{
  fprintf(stdout, "OS_PreOpen %s", m_filename.c_str());
  /*
	if(m_mode==KCL::Write || m_working_dir==KCL::RWDir)
	{
		INFO("OS_PreOpen %s", m_filename.c_str());
		return;
	}
	
	if(strstr(m_original_filename.c_str(), ".xml"))
 	{
 		FILE *fp = fopen(m_filename.c_str(), "rb");
 		if(fp)
 		{
 			fclose(fp);
 			return;
 		}
 	}

	//INFO("origfilename %s hashed filename [%s]",m_filename.c_str(), m_hash_filename.c_str() );
	FILE *f = fopen(m_hash_filename.c_str(), "rb");
	if(f)
	{
		//INFO("%s",m_hash_filename.c_str() );
		fclose(f);
		m_buffer = 0;
		return ;
	}

	if(!GLBAssetsManager)
	{
		abort();
	}
	AAsset* asset = AAssetManager_open(GLBAssetsManager, m_hash_filename_without_path.c_str(), AASSET_MODE_UNKNOWN);
	if (!asset)
	{
		//INFO("file not found in apk  [%s]",  m_original_filename.c_str() );
		return;
	}

	//INFO("start to extract = %s %s", m_original_filename.c_str(), m_hash_filename_without_path.c_str());

	m_filesize = AAsset_getLength(asset);
	m_buffer = new char[m_filesize + 1];
	m_p = m_buffer;
	m_is_filemapped = 1;
	AAsset_read(asset, m_buffer, m_filesize);
	m_buffer[m_filesize] = 0;
	AAsset_close(asset);
  */
}
