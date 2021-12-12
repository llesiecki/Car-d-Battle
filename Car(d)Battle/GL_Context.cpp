#include "GL_Context.h"

GL_Context::GL_Context()
	:window()
{
	lock_count = 0;
}

GL_Context::~GL_Context()
{
}

void GL_Context::obtain()
{
	std::thread::id this_id = std::this_thread::get_id();
	// if the context is already owned, just increment the number of locks
	if (owner == this_id)
	{
		gl_lock.lock();
		lock_count++;
		return;
	}
	// wait for the OpenGL context to be released
	gl_lock.lock();
	lock_count++;
	owner = this_id;
	// release the current OpenGL context
	glfwMakeContextCurrent(NULL);
	// assign the OpenGL context to the current thread
	glfwMakeContextCurrent(window);
}

void GL_Context::release()
{
	// the context can be released only by it's owner
	assert(std::this_thread::get_id() == owner);
	// ... and also we assume it's locked

	lock_count--;

	if (lock_count == 0)
	{
		// release the current OpenGL context
		owner = std::thread::id();
		glfwMakeContextCurrent(NULL);
	}

	gl_lock.unlock();
}

void GL_Context::set(GLFWwindow* window)
{
	this->window = window;
}

GLFWwindow* GL_Context::get_window()
{
	assert(std::this_thread::get_id() == owner);
	return window;
}
