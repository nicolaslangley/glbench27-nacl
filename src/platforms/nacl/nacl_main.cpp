#define READ_BUFFER_SIZE 32768

#include "app.h"
#include "kcl_os.h"
#include "naclwindow.h"

#include <errno.h>
#include <string.h>

#include "test_descriptor.h"

#include "ppapi/cpp/input_event.h"
#include "ppapi/cpp/point.h"
#include "ppapi/cpp/completion_callback.h"
#include "ppapi/cpp/graphics_3d.h"
#include "ppapi/cpp/instance.h"
#include "ppapi/cpp/module.h"
#include "ppapi/cpp/var.h"
#include "ppapi/cpp/core.h"
#include "ppapi/cpp/var_array.h"
#include "ppapi/cpp/file_io.h"
#include "ppapi/cpp/file_system.h"
#include "ppapi/lib/gl/gles2/gl2ext_ppapi.h"
#include "ppapi/utility/completion_callback_factory.h"
#include "ppapi/utility/threading/simple_thread.h"
#include "ppapi/cpp/url_loader.h"
#include "ppapi/cpp/url_request_info.h"

#include "nacl_io/nacl_io.h"
#include "sys/mount.h"

// Global variables for use in other classes
extern pp::Graphics3D egllib_context;
PP_Instance g_instance;
GLB::Application* app = 0;

// Global GL functions for debugging
GLenum GetGLError() {
  GLenum error = glGetError();
  return error;
}

const GLubyte* GetGLString() {
  const GLubyte* vendor = glGetString(GL_VENDOR);
  return vendor;
}

// GLBench Instance class
class GLBenchInstance : public pp::Instance {
public:
  // Constructor for instance
  explicit GLBenchInstance(PP_Instance instance)
    : pp::Instance(instance),
      glb_app_thread(this),
      m_callback_factory(this),
      m_width(0),
      m_height(0),
      m_onscreen_test(false) {}

  // Destructor that destroys GLB application thread
  virtual ~GLBenchInstance() { glb_app_thread.Join(); }

  // Init function that is called when the Instance is created
  virtual bool Init(uint32_t argc, const char* argn[], const char* argv[]) {
    PostMessage(pp::Var("Init has been called for instance"));
    // Set global instance for posting messages to JavaScript
    g_instance = this->pp_instance();

    // Initialize NaCL I/O library
    nacl_io_init_ppapi(this->pp_instance(), pp::Module::Get()->get_browser_interface());

    // Create GLB Application thread
    glb_app_thread.Start();
    
    // Unmount default file system and mount persistent HTML5 filesystem
    umount("/");
    int mount_result = mount("","/persistent","html5fs",0,"type=PERSISTENT,expected_size=1048576");
    PostMessage(pp::Var("The mount result is: (0 for success, -1 for failure)"));
    PostMessage(pp::Var(mount_result));

    return true;
  }

  // Handle message sent from JavaScript
  virtual void HandleMessage(const pp::Var& var_message) {
    // Ignore the message if it is not a string.
    if (!var_message.is_string())
      return;
    // Convert message to a string
    PostMessage("Message received");
    PostMessage(var_message);
    std::string message_str = var_message.AsString();

    // If the message received is 'Start', then setup the GLB application on application thread
    if (message_str == "Start") 
    {
      glb_app_thread.message_loop().PostWork(m_callback_factory.NewCallback(&GLBenchInstance::SetupGLBApplication)); 
      return;
    }
    // If the message is 'NextTest' tell window that next test command has been started
    else if (message_str == "NextTest")
    {
      PostMessage("Moving to next test");
      // This crashes somewhere down the line
      m_wnd->handle_cmd(1);
    }
    // Set onscreen or offscreen value for trex test
    else if (message_str == "trex_on")
    {
      m_onscreen_test = true;
    }
    else if (message_str == "trex_off")
    {
      m_onscreen_test = false;
    }

  }

  // Function called when view information for instance is changed
  virtual void DidChangeView(const pp::View& view) {
    PostMessage(pp::Var("DidChangeView has been called for this instance"));
    // Init rect to fill view
    m_width = view.GetRect().width();
    m_height = view.GetRect().height();
  }

private:

  // Entry setup function for Graphics context and GLB Application
  void SetupGLBApplication(int32_t) {
    PostMessage("Setting up GLB application");
    if (m_context.is_null()) 
    {
      PostMessage(pp::Var("Context is null so we are creating one"));
      // Create context using InitGL function and return if it fails
      if (!InitGL(m_width, m_height)) 
      {
        PostMessage(pp::Var("InitGL failed"));
        // Creating a new GL context failed.
        return;
      }
      // Set up the GLB application to be used
      // Create NACL GL Window - this is a wrapper for GLWindow
      m_wnd = 0;
	    m_wnd = dynamic_cast<NACLGLWindow*>(NewWindow("NACL Edition"));
	    m_wnd->Create();
      PostMessage(pp::Var("NACLGLWindow has been created"));
      // Initialize application
      const char* datadir_nacl = "/persistent/data/";
      m_app = new GLB::Application(datadir_nacl, "NACL App");
      // Determine what the current test to be run is
      char* current_test;
      if (m_onscreen_test) current_test = "2700100";
      else current_test = "2700101";
      // Set application parameters and get count of arguments
      char* app_argv[] = {"", "-select_test", current_test};
      int app_argc = sizeof(app_argv)/sizeof(app_argv[0]);
      // Parse the application parameters and initialize the application
      ParseAppParameters(m_app, app_argc, app_argv);
      // Set global application reference
	    app = m_app;
      PostMessage(pp::Var("GLB Application has been created"));
	    m_app->Init();
      PostMessage(pp::Var("GLB Application has been initialized"));

      // Set initial value for Setup Tests
      m_prev_td = 0;
	    m_td = 0;
      score = 0;
	    fps = 0;
	    test_id = 0;

      // Enter main loop
      SetupTests();
    } 
    else   
    {
      // Resize the buffers to the new size of the module.
      int32_t result = m_context.ResizeBuffers(m_width, m_height);
      if (result < 0) 
      {
        fprintf(stderr, "unable to resize buffers to %d x %d!\n", m_width, m_height);
        return;
      }
    }
    // Update glViewport with new height and width 
    glViewport(0, 0, m_width, m_height);
  }


  // Initialization functions for OpenGL ES 2.0
  // Sets up Context for drawing and makes it available globally
  bool InitGL(int32_t new_width, int32_t new_height) {
    // Attempt to initialize OpenGL using PPAPI
    if (!glInitializePPAPI(pp::Module::Get()->get_browser_interface())) 
    {
      PostMessage("Unable to initialize GL PPAPI!");
      return false;
    }

    // Define attributes for graphics context
    const int32_t attrib_list[] = {
      PP_GRAPHICS3DATTRIB_DEPTH_SIZE, 24,
      PP_GRAPHICS3DATTRIB_WIDTH, new_width,
      PP_GRAPHICS3DATTRIB_HEIGHT, new_height,
      PP_GRAPHICS3DATTRIB_NONE
    };

    // Create new context with given attribute list
    m_context = pp::Graphics3D(this, attrib_list);\
    // Set global context to be current context for egllibs
    egllib_context = m_context;
    // Attempt to bind 3d context
    if (!BindGraphics(m_context)) 
    {
      PostMessage("Unable to bind 3d context!");
      m_context = pp::Graphics3D();
      glSetCurrentContextPPAPI(0);
      return false;
    }

    glSetCurrentContextPPAPI(m_context.pp_resource());    

    return true;
  }

  // Initialize the current test and render the running screen
  void InitTest(int32_t) {
    PostMessage(pp::Var("Initializing current test"));
    if (m_app->InitCurrentTest()) 
    {
      m_app->RenderRunningScreen();
      // Call SwapBuffers with RunTest as the callback function
      m_context.SwapBuffers(m_callback_factory.NewCallback(&GLBenchInstance::RunTest));
    } 
    else 
    {
      // Last argument is the ActiveConfigID - set it to 1 since we only have one configuration
      m_app->FinishCurrentTest(score, fps, error, 1);
      // Call SetupTests which will move to next test in list
      SetupTests();
    }
  }

  // Finish the current test
  void EndTest() {
    PostMessage(pp::Var("Finishing current test"));
    // Last argument should be ActiveConfigID - set it to 1
    m_app->FinishCurrentTest(score, fps, error, 1);
    // Set previous test descriptor to be current test descriptor
    m_prev_td = m_td;
    // Call SetupTests which will move to next test in list
    SetupTests();
  }

  // Exit the application and close the NaCl module
  void ExitTests() {
    // Save results if the app is corporate
    if (m_app->IsCorporate())
    {
      PostMessage(pp::Var("App is corporate and so results are being saved"));
      m_app->ShowPrintableResults();
		  //m_app->SaveResultsExcelXML();
      PostMessage(pp::Var("Results have been saved"));
    }
    // Destroy the NaClWindow and exit module
    PostMessage(pp::Var("Destroying window"));
    m_wnd->Destroy();
    delete m_wnd;
    PostMessage(pp::Var("ExitModule"));
  }

  // Run the current test - handles rendering and animation
  void RunTest(int32_t) {
    PostMessage(pp::Var("Running current test"));
    m_wnd->HandleMessage();
    // If the window is done (i.e. test is over) end the test
    if (m_wnd->Done())
    {
      EndTest();
      return;
    }

    // Clear buffers if we need to render
    if (m_app->IsSwapbufferNeeded())
    {
      m_wnd->ClearBuffers();
    }
    
    // Animate the application - end the test if animation fails
    bool res = m_app->Animate();
    if (!res)
    {
      EndTest();
      return;
    }

    // Render the application and swap buffers if needed
    m_app->Render();
    if (m_app->IsSwapbufferNeeded())
    {
      // SwapBuffers call to Chrome Main Thread with MainLoop passed as callback
      m_context.SwapBuffers(m_callback_factory.NewCallback(&GLBenchInstance::RunTest));
      return;
    }
    // If we did not render continue to loop RunTest
    RunTest(0);
  }

  void SetupTests() {
    PostMessage(pp::Var("Setup tests has been called and is running"));
    // Initialize properties
	  std::string error;

    // Select the enxt test and exit tests if there are no more
    PostMessage(pp::Var("Preparing to select next test"));
    m_app->SelectNextTest(m_td);
    if (!m_td)
    {
      PostMessage(pp::Var("No test exists and now exiting"));
      ExitTests();
      return;
    } 
    else 
    {
      m_wnd->handle_cmd(0);
    }

    // If window is done, finish the current test and move to the next one
    if (m_wnd->Done())
    {
      PostMessage(pp::Var("Finishing current test"));
      m_app->FinishCurrentTest(score, fps, error, 1);
      SetupTests();
    }

    // If previous test doesn't exits or is not equal to current test then report error and reset
    if (!m_prev_td || (*m_prev_td != *m_td))
    {
      m_app->ClearNotificationManager();
      m_wnd->Destroy();
      if (m_wnd->Create() == false)
      {
        INFO("ERROR: not supported resolution. EGL Error code = %X", m_wnd->GetEGLError());
        std::string name;
        int errcode = 0;
        if (m_td->m_fsaa > 0)
        {
          errcode = GLB_TESTERROR_NOFSAA;
        }
        else if (m_td->m_depth_bpp > 16)
        {
          errcode = GLB_TESTERROR_Z24_NOT_SUPPORTED;
        }
        else
        {
          errcode = m_wnd->GetEGLError();
        }
        m_app->ReportError(errcode, name);
        INFO("EGL error report : %s", name.c_str());
        SetupTests();
      }
      m_app->Reset();
    }

    PostMessage(pp::Var("Preparing to load test with following ID:"));
    PostMessage(pp::Var(m_td->m_id));
    // Render loading screen and swapbuffers with InitTest as the callback function
    // Loading screen crashes so disable it
    //m_app->RenderLoadingScreen();
    PostMessage(pp::Var("Calling swap buffers with InitTest as callback"));
    m_context.SwapBuffers(m_callback_factory.NewCallback(&GLBenchInstance::InitTest));

  }

  // Private instance variables
  // NaCl variables
  pp::CompletionCallbackFactory<GLBenchInstance> m_callback_factory;
  pp::Graphics3D m_context;
  pp::InputEvent m_input;
  pp::SimpleThread glb_app_thread;
  int32_t m_height;
  int32_t m_width;
  
  // GLB application variables
  bool m_onscreen_test;
  Application *m_app;
  NACLGLWindow *m_wnd;

  // Variables for SetupTests()
  const TestDescriptor *m_prev_td;
  const TestDescriptor *m_td;
  std::string error;
  float score;
  float fps;
  int test_id;

};

// Module class that extends pp::Module
class GLBenchModule : public pp::Module {
public:
  GLBenchModule() : pp::Module() {}
  virtual ~GLBenchModule() {}

  virtual pp::Instance* CreateInstance(PP_Instance instance) {
    return new GLBenchInstance(instance);
  }

};

namespace pp {
  // Factory method for creating module
  Module* CreateModule() {
    return new GLBenchModule();
  }
} // Namespace pp