// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

constexpr char SERVER_ADDRESS[] = "localhost";
constexpr float CARD_HEIGHT = 1.178f;
constexpr float CARD_WIDTH = 0.707f;

#include <string>
#include <fstream>
#include <iostream>
#include "utilities\Bitmap.h"
#include <vector>
#include <GL/glut.h>
#include <libxl.h>
#include "Card.h"
#include "Cards.h"
#include "WorkBook.h"
