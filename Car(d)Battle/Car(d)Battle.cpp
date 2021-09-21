#include "stdafx.h"
#include "Game.h"
#include "UI.h"

int main(int argc, char* argv[])
{
	//GLFW test
	if(glfwInit() != GLFW_TRUE)
		exit(1);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(1280, 720, GAME_NAME, NULL, NULL);
	if (window == NULL)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return 2;
	}

	glfwMakeContextCurrent(window);
	gladLoadGL();

	glClearColor(0.53f, 0.8f, 0.98f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	static UI& ui = Singleton<UI>();//See Note 1
	
	glfwSetFramebufferSizeCallback(window,
		[](GLFWwindow* window, int w, int h)
		{return ui.set_screen_size(w, h);
		});
	glfwSetCursorPosCallback(window,
		[](GLFWwindow* window, double x, double y)
		{return ui.set_cursor_pos(
			static_cast<float>(x),
			static_cast<float>(y));
		});

	glfwSwapInterval(1);//V-SYNC

	ui.set_screen_size(1280, 720);
	ui.start();
	
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		ui.render();
		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

/// Note 1:
/// Must be static as it must be used in lambdas without capture lists.
/// Capture lists cannot be used in these lambdas,
/// because they are passed as C-style function pointers.
