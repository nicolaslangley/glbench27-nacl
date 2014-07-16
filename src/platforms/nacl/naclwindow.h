#ifndef NACLGLWINDOW_H
#define NACLGLWINDOW_H

#include "glwindow.h"
#include "platform.h"
#include "kcl_os.h"
#include "info_egl.h"


class NACLGLWindow : public GLWindow
{
public:
	NACLGLWindow(const char * title);
	~NACLGLWindow();
	bool Destroy();
	bool SwapBuffers();
	bool HandleMessage();
	bool Done();
  // Handle() is an abstract function in GLWindow
  void* Handle() { return 0; }

	bool Terminate();

	int GetEGLError();

	void Resize(int x, int y);
	GLBEgl* Egl()
	{
		return &m_egl;
	}

	void handle_cmd(int32_t cmd);
//	int32_t handle_input(AInputEvent* event);

	//static void handle_cmd_s(struct android_app* app, int32_t cmd);
//	static int32_t handle_input_s(struct android_app* app, AInputEvent* event);

	int lastResX;
	int lastResY;
	bool HaveToStartResultActivity;
	void SetBrightness(float newvalue);
	void RegisterBatteryReceiver();
	void UnregisterBatteryReceiver();
	void LoadOriginalBrightness();

	int GetOffscreenX();
	int GetOffscreenY();

	bool Create();
protected:
private:
	bool CreateEGL();

	bool m_destroyed;	
  
	EGLDisplay m_display;
	EGLSurface m_surface;
	EGLContext m_context;
	GLBEgl m_egl;
	bool m_done;
  int m_offscreen_size_x;
  int m_offscreen_size_y;
};

#endif
