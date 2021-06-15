// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define GLFW_INCLUDE_NONE

#include "targetver.h"

constexpr char SERVER_ADDRESS[] = "localhost";
constexpr float CARD_HEIGHT = 1.178f;
constexpr float CARD_WIDTH = 0.707f;
constexpr wchar_t LGAME_NAME[] = L"Car(d)Battle";
constexpr char GAME_NAME[] = "Car(d)Battle";

#include <string>
#include <fstream>
#include <iostream>
#include <functional>
#include <vector>
#include "utilities\Bitmap.h"
//#include <GL/glut.h>
#include "GLFW/glfw3.h"
#include "Glad/glad.h"
#include <libxl.h>
#include "utilities/Vec3.h"
