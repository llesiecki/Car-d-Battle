// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define GLFW_INCLUDE_NONE
#define GLFW_EXPOSE_NATIVE_WGL
#define _WINSOCKAPI_

constexpr char SERVER_ADDRESS[] = "localhost";
constexpr float CARD_HEIGHT = 1.178f;
constexpr float CARD_WIDTH = 0.707f;
constexpr wchar_t LGAME_NAME[] = L"Car(d)Battle";
constexpr char GAME_NAME[] = "Car(d)Battle";
constexpr unsigned int KEYS_NUM = 256;
constexpr int FONT_SIZE = 48;

#include "targetver.h"
#include <string>
#include <fstream>
#include <iostream>
#include <functional>
#include <vector>
#include <map>
#include <Windows.h>
#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"
#include "Glad/glad.h"
#include "libxl.h"
#include "GLM/glm.hpp"
#include "GLM/gtc/matrix_transform.hpp"
#include "GLM/gtc/type_ptr.hpp"
#include "GLM/gtx/quaternion.hpp"
