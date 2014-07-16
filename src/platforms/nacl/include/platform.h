#ifndef PLATFORM_H
#define PLATFORM_H

#define GLB_API

#include "glbtypes.h"

#include <kcl_base.h>

#if defined OPENGL_ES2
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#elif defined OPENGL_ES3
# include <EGL/egl.h>
# include <gl3.h>
#endif

#define GL_UNSIGNED_INT_24_OES                     0x6005   // TEMP

/* OES_depth_texture */	// from <gl2atiext.h>
#   define GL_UNSIGNED_INT_24_OES                     0x6005   // TEMP

extern unsigned int WINWIDTH;
extern unsigned int WINHEIGHT;

extern unsigned int OFFSCREEN_WIDTH;
extern unsigned int OFFSCREEN_HEIGHT;

void DiscardDepthAttachment();
void DiscardColorAttachment();

extern "C" void ExtractFromAssets(char* in, char* out);

#ifndef glRenderbufferStorageMultisample 
#define glRenderbufferStorageMultisample 
#endif

#ifndef glBlitFramebuffer
#define glBlitFramebuffer 
#endif

#ifndef GL_DRAW_FRAMEBUFFER_BINDING 
#define GL_DRAW_FRAMEBUFFER_BINDING 0x8CA6
#endif

#ifndef GL_READ_FRAMEBUFFER
#define GL_READ_FRAMEBUFFER 0x8CA8
#endif

#ifndef GL_DRAW_FRAMEBUFFER 
#define GL_DRAW_FRAMEBUFFER 0x8CA9
#endif

#ifndef GL_READ_FRAMEBUFFER_BINDING
#define GL_READ_FRAMEBUFFER_BINDING 0x8CAA
#endif

#ifndef GL_BGR 
#define GL_BGR 0x80E0
#endif

#ifndef GL_BGRA_EXT
#define GL_BGRA_EXT                                             0x80E1
#endif

#endif
