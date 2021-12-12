#pragma once

#include "stdafx.h"
#include <thread>
#include <mutex>
#include <assert.h>
#include "Singleton.h"

class GL_Context
{
	unsigned int lock_count;
	std::recursive_mutex gl_lock;
	std::thread::id owner;
	GLFWwindow* window;
	GL_Context();
public:
	friend GL_Context& Singleton<GL_Context>();
	~GL_Context();
	void obtain();
	void release();
	void set(GLFWwindow*);
	GLFWwindow* get_window();
};
