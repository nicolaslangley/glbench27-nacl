#include <EGL/egl.h>
#include "ppapi/lib/gl/gles2/gl2ext_ppapi.h"
#include "ppapi\cpp\graphics_3d.h"
#include "ppapi\c\ppb_graphics_3d.h"

// Global Graphics3D context to be set to current context by main instance
pp::Graphics3D egllib_context;

// Override EGL functions using NaCl Graphics3D API
// Handle both case where value is unsigned or signed
unsigned int eglGetConfigAttrib(void * display, void * config, unsigned int attribute, unsigned int * value) {
  return eglGetConfigAttrib( display, config, attribute, (int*)value);
}

unsigned int eglGetConfigAttrib(void * display, void * config, unsigned int attribute, int * value) {
  int32_t attrib_list[] = {attribute, 0};
  PP_Resource context = glGetCurrentContextPPAPI();
  int32_t return_value = egllib_context.GetAttribs(attrib_list);
  return (int) return_value;
}

// Does this need to be implemented or can we return dummy information?
unsigned int eglGetConfigs(void * display, void ** configs, unsigned int config_size, unsigned int * num_config) {
  return (int) 0;
}

char const * eglQueryString(	void* display, unsigned int name) {
  return "";
}