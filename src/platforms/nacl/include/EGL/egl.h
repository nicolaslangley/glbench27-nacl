
// EGL function prototypes - to be overwritten using NaCl
unsigned int eglGetConfigAttrib(void * display, void * config, unsigned int attribute, int * value);
unsigned int eglGetConfigAttrib(void * display, void * config, unsigned int attribute, unsigned int * value);
unsigned int eglGetConfigs(void * display, void ** configs, unsigned int config_size, unsigned int * num_config);
char const * eglQueryString(void* display, unsigned int name);