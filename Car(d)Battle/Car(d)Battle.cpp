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
    glfwSwapInterval(1);//V-SYNC

    while (!glfwWindowShouldClose(window))
    {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    //Commented out due to refactor with GLFW
    /*
	glutInit(&argc, argv);
	glutInitWindowSize(1280, 720);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutCreateWindow(GAME_NAME);
    static Game game;//See Note 1
    game.load();

    unsigned int opp_num;
    do
    {
        std::cout << "Number of opponents for the next round: ";
        //std::cin >> opp_num;
        opp_num = 3;
    } 		while (opp_num < 1 && opp_num > 3);
    game.start(opp_num + 1);

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glLineWidth(1.5);

    static void (*render_timer)(int) = [](int place_holder){//See Note 1
        game.draw();
        glutTimerFunc(17, render_timer, 0); };

	glutTimerFunc(17, render_timer, 0);
	glutDisplayFunc([]() {return game.draw(); });
    glutReshapeFunc([](int w, int h) {return game.set_screen_size(w, h); });
    glutPassiveMotionFunc([](int x, int y) {return game.set_cursor_pos(x, y); });
	glutMainLoop();
    */
	return 0;
}

/// Note 1:
/// Must be static as it must be used in lambdas without capture lists.
/// Capture lists cannot be used in these lambdas, because they are passed as C-style function pointers.
