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
	std::thread::id this_id = std::this_thread::get_id();
	// if the context is already owned, just increment the number of locks
	if (last_owner == this_id)
	{
		gl_lock.lock();
		return;
	}
	// release the current OpenGL context
	glfwMakeContextCurrent(NULL);
	// wait for the OpenGL context to be released
	gl_lock.lock();
	last_owner = this_id;
	// assign the OpenGL context to the current thread
	glfwMakeContextCurrent(window);
}

void GL_Context::release()
{
	// the context can be released only by it's owner
	assert(std::this_thread::get_id() == last_owner);
	// here we are not actually release the context,
	// maybe in the future the same thread will attempt to obtain it
	// 
	// ... and also we assume it's locked
	gl_lock.unlock();
}

void GL_Context::set(GLFWwindow* window)
{
	this->window = window;
}
