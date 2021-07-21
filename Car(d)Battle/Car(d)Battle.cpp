#include "stdafx.h"
#include "Game.h"

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

	glViewport(0, 0, 1280, 720);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glLineWidth(1.5);

	static Game & game = Singleton<Game>();//See Note 1
	
	glfwSetFramebufferSizeCallback(window,
		[](GLFWwindow* window, int w, int h)
		{return game.set_screen_size(w, h);
		});
	glfwSetCursorPosCallback(window,
		[](GLFWwindow* window, double x, double y)
		{return game.set_cursor_pos(x, y);
		});
	glfwSwapInterval(1);//V-SYNC

	game.set_screen_size(1280, 720);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		game.draw();
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
