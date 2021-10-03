#include "GL_Context.h"

GL_Context::GL_Context()
	:window()
{

}

GL_Context::~GL_Context()
{
}

void GL_Context::obtain()
{
	// release the current OpenGL context
	glfwMakeContextCurrent(NULL);
	// wait for the OpenGL context to be released
	gl_lock.lock();
	locker_id = std::this_thread::get_id();
	// assign the OpenGL context to the current thread
	glfwMakeContextCurrent(window);
}

void GL_Context::release()
{
	assert(std::this_thread::get_id() == locker_id);
	// release the current OpenGL context
	glfwMakeContextCurrent(NULL);
	gl_lock.unlock();
}

void GL_Context::set(GLFWwindow* window)
{
	this->window = window;
}
