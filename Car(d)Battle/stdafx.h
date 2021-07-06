// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define GLFW_INCLUDE_NONE

constexpr char SERVER_ADDRESS[] = "localhost";
constexpr float CARD_HEIGHT = 1.178f;
constexpr float CARD_WIDTH = 0.707f;
constexpr wchar_t LGAME_NAME[] = L"Car(d)Battle";
constexpr char GAME_NAME[] = "Car(d)Battle";

#include "targetver.h"
#include <string>
#include <fstream>
#include <iostream>
#include <functional>
#include <vector>
#include "utilities\Bitmap.h"
#include "GLFW/glfw3.h"
#include "Glad/glad.h"
#include "libxl.h"
#include "GLM/glm.hpp"
#include "GLM/gtc/matrix_transform.hpp"
#include "GLM/gtc/type_ptr.hpp"
