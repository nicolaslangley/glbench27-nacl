#
# Get pepper directory for toolchain and includes.
#
# If NACL_SDK_ROOT is not set, then assume it can be found three directories up.
#

THIS_MAKEFILE := $(abspath $(lastword $(MAKEFILE_LIST)))
NACL_SDK_ROOT ?= $(abspath $(dir $(THIS_MAKEFILE))..\..)

include $(NACL_SDK_ROOT)/tools/common.mk

# Project Build flags

WARNINGS := -Wno-long-long -Wall -Wswitch-enum -pedantic -Werror
CXXFLAGS := -pthread -std=gnu++98 $(WARNINGS)

#
# Compute tool paths
#

GETOS := python $(NACL_SDK_ROOT)\tools\getos.py
OSHELPERS = python $(NACL_SDK_ROOT)\tools\oshelpers.py
OSNAME := $(shell $(GETOS))
RM := $(OSHELPERS) rm

PNACL_TC_PATH := $(abspath $(NACL_SDK_ROOT)\toolchain\$(OSNAME)_pnacl)
PNACL_CXX := $(PNACL_TC_PATH)\bin\pnacl-clang++
PNACL_FINALIZE := $(PNACL_TC_PATH)\bin\pnacl-finalize
CXXFLAGS := -DOPENGL_ES2 -DSTATIC_LINKED_LIBRARY -DNACL -I$(NACL_SDK_ROOT)\include -I$(NACL_SDK_ROOT)\include\GLES2
LDFLAGS := -L$(NACL_SDK_ROOT)\lib\pnacl\Release -lppapi_cpp -lppapi -lppapi_gles2 -lpthread -lnacl_io

KCL_LOCAL_PATH := ../../../kcl_framework
SRC_LOCAL_PATH := ../../../src
LOCAL_PATH := .
H_LOCAL_PATH := $(SRC_LOCAL_PATH)

#### NACL Application
SOURCES := $(SRC_LOCAL_PATH)/platforms/nacl/glbench_nacl.cpp \
	$(SRC_LOCAL_PATH)/platforms/nacl/nacl_egl.cpp
	
#### INCLUDES
LOCAL_C_INCLUDES := \
	$(H_LOCAL_PATH)/platforms/nacl/include \
	$(H_LOCAL_PATH)/application \
	$(H_LOCAL_PATH)/platforms/nacl \
	$(H_LOCAL_PATH)/common \
	$(H_LOCAL_PATH)/common/opengl \
	$(H_LOCAL_PATH)/common/ogl_man \
	$(H_LOCAL_PATH)/test_module \
	$(H_LOCAL_PATH)/targets \
	$(H_LOCAL_PATH)/test_module/high_level_tests_3 \
	$(H_LOCAL_PATH)/test_module/high_level_tests_3/opengl \
	$(NACL_SDK_ROOT)/include/pnacl \
	$(H_LOCAL_PATH) \
	$(KCL_LOCAL_PATH)/kcl/src \
	$(KCL_LOCAL_PATH)/kcl_libraries/libz/src \
	$(KCL_LOCAL_PATH)/kcl_libraries/tinyxml/src \
	$(KCL_LOCAL_PATH)/kcl_libraries/libpng/src \
	$(KCL_LOCAL_PATH)/kcl_libraries/libetc1/src \


#### LIBCOMMON
SOURCES += 	$(SRC_LOCAL_PATH)/common/render_statistics.cpp \
	$(SRC_LOCAL_PATH)/common/glwindow.cpp \
	$(SRC_LOCAL_PATH)/common/misc2.cpp \
	$(SRC_LOCAL_PATH)/common/misc2_opengl.cpp \
	$(SRC_LOCAL_PATH)/common/framelogger.cpp \
	$(SRC_LOCAL_PATH)/common/batterylogger.cpp \
	$(SRC_LOCAL_PATH)/common/xml_utils.cpp \
	$(SRC_LOCAL_PATH)/common/glbshader.cpp \
	$(SRC_LOCAL_PATH)/common/opengl/offscrman_opengl.cpp \
	$(SRC_LOCAL_PATH)/common/opengl/glb_image.cpp \
	$(SRC_LOCAL_PATH)/common/ext.cpp \
	$(SRC_LOCAL_PATH)/common/opengl/notificationman_opengl.cpp \
	$(SRC_LOCAL_PATH)/common/bitmask.cpp \
	$(SRC_LOCAL_PATH)/common/font.cpp \
	$(SRC_LOCAL_PATH)/common/opengl/texture.cpp \
	$(SRC_LOCAL_PATH)/common/fbo.cpp \
	$(SRC_LOCAL_PATH)/common/opengl/test_descriptor.cpp \
	$(SRC_LOCAL_PATH)/common/glb_version.cpp \
	$(SRC_LOCAL_PATH)/common/testdescriptor_helper.cpp \
	$(SRC_LOCAL_PATH)/common/info_opengl.cpp \
	$(SRC_LOCAL_PATH)/common/info_egl.cpp \
	$(SRC_LOCAL_PATH)/common/test_error.cpp \
	$(SRC_LOCAL_PATH)/common/glb_module.cpp \
	$(SRC_LOCAL_PATH)/common/printable_result.cpp \
	$(SRC_LOCAL_PATH)/common/uploaddata.cpp \
	$(SRC_LOCAL_PATH)/common/glb_opengl_state_manager.cpp
	



#### LIBHIGHLEVELTESTS
SOURCES += \
	$(KCL_LOCAL_PATH)/kcl/src/kcl_animationtrack.cpp \
	$(KCL_LOCAL_PATH)/kcl/src/kcl_actor.cpp \
	$(KCL_LOCAL_PATH)/kcl/src/kcl_animation4.cpp \
	$(KCL_LOCAL_PATH)/kcl/src/kcl_base.cpp \
	$(KCL_LOCAL_PATH)/kcl/src/kcl_camera2.cpp \
	$(KCL_LOCAL_PATH)/kcl/src/kcl_image.cpp \
	$(KCL_LOCAL_PATH)/kcl/src/kcl_lod.cpp \
	$(KCL_LOCAL_PATH)/kcl/src/kcl_material.cpp \
	$(KCL_LOCAL_PATH)/kcl/src/kcl_math3d.cpp \
	$(KCL_LOCAL_PATH)/kcl/src/kcl_mesh.cpp \
	$(KCL_LOCAL_PATH)/kcl/src/kcl_node.cpp \
	$(KCL_LOCAL_PATH)/kcl/src/kcl_room.cpp \
	$(KCL_LOCAL_PATH)/kcl/src/kcl_particlesystem.cpp \
	$(KCL_LOCAL_PATH)/kcl/src/kcl_planarmap.cpp \
	$(KCL_LOCAL_PATH)/kcl/src/kcl_scene_handler.cpp \
	$(KCL_LOCAL_PATH)/kcl/src/kcl_scene_xml.cpp \
	$(KCL_LOCAL_PATH)/kcl/src/kcl_io.cpp \
	$(SRC_LOCAL_PATH)/targets/glb27/test_interface.cpp \
	$(SRC_LOCAL_PATH)/targets/glb27/test_interface_generate.cpp \
	$(SRC_LOCAL_PATH)/test_module/high_level_tests_3/krl_scene.cpp \
	$(SRC_LOCAL_PATH)/test_module/high_level_tests_3/krl_material.cpp \
	$(SRC_LOCAL_PATH)/test_module/high_level_tests_3/glbtypes.cpp \
	$(SRC_LOCAL_PATH)/test_module/high_level_tests_3/glb_scene_utils_opengl.cpp \
	$(SRC_LOCAL_PATH)/test_module/high_level_tests_3/spacepartition.cpp \
	$(SRC_LOCAL_PATH)/test_module/high_level_tests_3/engine.cpp \
	$(SRC_LOCAL_PATH)/test_module/high_level_tests_3/opengl/cubemap_opengl.cpp \
	$(SRC_LOCAL_PATH)/test_module/high_level_tests_3/opengl/glb_material.cpp \
	$(SRC_LOCAL_PATH)/test_module/high_level_tests_3/opengl/glb_material_opengl.cpp \
	$(SRC_LOCAL_PATH)/test_module/high_level_tests_3/opengl/glb_mesh.cpp \
	$(SRC_LOCAL_PATH)/test_module/high_level_tests_3/opengl/glb_mesh_opengl.cpp \
	$(SRC_LOCAL_PATH)/test_module/high_level_tests_3/opengl/glb_scene_opengl27.cpp \
	$(SRC_LOCAL_PATH)/test_module/high_level_tests_3/opengl/shader.cpp \
	$(SRC_LOCAL_PATH)/test_module/high_level_tests_3/opengl/shadowmap_opengl.cpp \
	$(SRC_LOCAL_PATH)/test_module/high_level_tests_3/opengl/low_level_tests.cpp \
	$(SRC_LOCAL_PATH)/test_module/high_level_tests_3/opengl/low_level_tests_opengl.cpp \
	$(SRC_LOCAL_PATH)/test_module/high_level_tests_3/opengl/glb_planarmap_opengl.cpp \
	$(SRC_LOCAL_PATH)/test_module/high_level_tests_3/opengl/vbopool.cpp \
	$(SRC_LOCAL_PATH)/test_module/high_level_tests_3/opengl/glb_scene.cpp \
	$(SRC_LOCAL_PATH)/test_module/high_level_tests_3/opengl/glb_scene_opengl27_framecapture.cpp 
	
#### LIBRENDER
SOURCES += \
	$(SRC_LOCAL_PATH)/platforms/nacl/naclwindow.cpp 
	
#### LIBKCL
SOURCES += \
	$(KCL_LOCAL_PATH)/kcl_libraries/libetc1/src/etc1.cpp \
	$(KCL_LOCAL_PATH)/kcl_libraries/tinyxml/src/tinystr.cpp \
	$(KCL_LOCAL_PATH)/kcl_libraries/tinyxml/src/tinyxml.cpp \
	$(KCL_LOCAL_PATH)/kcl_libraries/tinyxml/src/tinyxmlerror.cpp \
	$(KCL_LOCAL_PATH)/kcl_libraries/tinyxml/src/tinyxmlparser.cpp \
	$(KCL_LOCAL_PATH)/kcl_libraries/libz/src/adler32.c \
	$(KCL_LOCAL_PATH)/kcl_libraries/libz/src/compress.c \
	$(KCL_LOCAL_PATH)/kcl_libraries/libz/src/crc32.c \
	$(KCL_LOCAL_PATH)/kcl_libraries/libz/src/deflate.c \
	$(KCL_LOCAL_PATH)/kcl_libraries/libz/src/inffast.c \
	$(KCL_LOCAL_PATH)/kcl_libraries/libz/src/inflate.c \
	$(KCL_LOCAL_PATH)/kcl_libraries/libz/src/inftrees.c \
	$(KCL_LOCAL_PATH)/kcl_libraries/libz/src/trees.c \
	$(KCL_LOCAL_PATH)/kcl_libraries/libz/src/uncompr.c \
	$(KCL_LOCAL_PATH)/kcl_libraries/libz/src/zutil.c \
	$(KCL_LOCAL_PATH)/kcl_libraries/libz/src/gzwrite.c \
	$(KCL_LOCAL_PATH)/kcl_libraries/libz/src/gzlib.c \
	$(KCL_LOCAL_PATH)/kcl_libraries/libpng/src/png.c \
	$(KCL_LOCAL_PATH)/kcl_libraries/libpng/src/pngerror.c \
	$(KCL_LOCAL_PATH)/kcl_libraries/libpng/src/pngget.c \
	$(KCL_LOCAL_PATH)/kcl_libraries/libpng/src/pngmem.c \
	$(KCL_LOCAL_PATH)/kcl_libraries/libpng/src/pngread.c \
	$(KCL_LOCAL_PATH)/kcl_libraries/libpng/src/pngrio.c \
	$(KCL_LOCAL_PATH)/kcl_libraries/libpng/src/pngrtran.c \
	$(KCL_LOCAL_PATH)/kcl_libraries/libpng/src/pngrutil.c \
	$(KCL_LOCAL_PATH)/kcl_libraries/libpng/src/pngset.c \
	$(KCL_LOCAL_PATH)/kcl_libraries/libpng/src/pngtrans.c \
	$(KCL_LOCAL_PATH)/kcl_libraries/libpng/src/pngwio.c \
	$(KCL_LOCAL_PATH)/kcl_libraries/libpng/src/pngwutil.c \
	$(KCL_LOCAL_PATH)/kcl_libraries/libpng/src/pngwtran.c \
	$(KCL_LOCAL_PATH)/kcl_libraries/libpng/src/pngwrite.c
	
#### LIBGLBENCHMARK
SOURCES += \
	$(SRC_LOCAL_PATH)/platforms/nacl/kcl_os.cpp \
	$(SRC_LOCAL_PATH)/platforms/nacl/kcl_io_os.cpp \
	$(SRC_LOCAL_PATH)/application/app.cpp \
	$(SRC_LOCAL_PATH)/application/app_opengl.cpp \
	$(SRC_LOCAL_PATH)/application/app_result.cpp 
	
# Prepend -I to includes	
LOCAL_C_INCLUDES_I := $(foreach d, $(LOCAL_C_INCLUDES), -I$d)

#
# Disable DOS PATH warning when using Cygwin based tools Windows
#

CYGWIN ?= nodosfilewarning
export CYGWIN


# Declare the ALL target first, to make the 'all' target the default build

all: glb_nacl.pexe

clean:
	$(RM) glb_nacl.pexe glb_nacl.bc

glb_nacl.bc: $(SOURCES)
	$(PNACL_CXX) -o $@ $(SOURCES) $(CXXFLAGS) -O2 $(LOCAL_C_INCLUDES_I) $(LDFLAGS)

glb_nacl.pexe: glb_nacl.bc
	$(PNACL_FINALIZE) -o $@ $<


#
# Makefile target to run the SDK's simple HTTP server and serve this test.
# Added --no-dir-check flag so can be run from anywhere
#
HTTPD_PY := python $(NACL_SDK_ROOT)\tools\httpd.py --no-dir-check

.PHONY: serve
serve: all
	$(HTTPD_PY) -C $(CURDIR)