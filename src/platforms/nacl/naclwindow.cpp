#include "naclwindow.h"
#include "kcl_os.h"
#include "app.h"
#include "eglutils.h"

int wndcreated;

NACLGLWindow::NACLGLWindow(const char * title)
{

	lastResX = -1;
	lastResY = -1;

	m_done = 0;
	m_destroyed = 0;
	wndcreated = 0;

  m_offscreen_size_x = 1920;
  m_offscreen_size_y = 1080;

	HaveToStartResultActivity = 0;
}


NACLGLWindow::~NACLGLWindow()
{
	if(m_destroyed)
	{
		return;
	}

}


bool NACLGLWindow::Terminate()
{
  return 1;
}

bool NACLGLWindow::HandleMessage()
{
	return 1;
}


void NACLGLWindow::handle_cmd(int32_t cmd)
{
	switch (cmd) 
	{
  case 0:
    m_done = 0;
    break;
	case 1:
		m_done = 1;
		break;
	}
}


void CheckEGLErrors()
{
	//int err = eglGetError();
	//if(err != EGL_SUCCESS)
	//{
	//	INFO("eglGetError = %d", err);
	//}
}


int NACLGLWindow::GetEGLError()
{
	//return eglGetError();
}

bool NACLGLWindow::Done()
{
	return m_done;
}


void NACLGLWindow::Resize(int x, int y)
{
	extern GLB::Application *app;
  app->Resize(m_offscreen_size_x, m_offscreen_size_y);
}


int NACLGLWindow::GetOffscreenX()
{
	return m_offscreen_size_x;
}


int NACLGLWindow::GetOffscreenY()
{
  return m_offscreen_size_y;
}


bool NACLGLWindow::CreateEGL()
{
	return 1;
}


bool NACLGLWindow::Create()
{
  // Calls CreateEGL()
	return true;
}


bool NACLGLWindow::Destroy()
{
	Terminate();
}


bool NACLGLWindow::SwapBuffers()
{
  INFO("Ignoring Swapbuffer");
	return true;
}


GLWindow *NewWindow(const char* wndName)
{
	return new NACLGLWindow(wndName);
}
