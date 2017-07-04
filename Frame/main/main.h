#ifndef RWE_FRAME_MAIN_MAIN
#define RWE_FRAME_MAIN_MAIN

#define _CRT_SECURE_NO_WARNINGS
#define GLM_FORCE_RADIANS
#define FREEGLUT_STATIC

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <glfw/glew.h>
#include <gl/freeglut.h>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#ifndef GL_Lib
#ifdef _DEBUG
#define GL_Lib(name) name "d.lib"
#else
#define GL_Lib(name) name ".lib"
#endif
#endif
#pragma comment(lib, GL_Lib("freeglut_static"))
#pragma comment(lib,"glew32.lib")

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

#define PI 3.14159265359

template <typename T>
T *toArray(vector<T> *v) {
	return &((*v)[0]);
}
template <typename T>
T clamp(T low, T high, T input) {
	if (input < low)return low;
	if (input > high)return high;
	return input;
}

struct Mouse {
	glm::vec2 pre, tmp;
	int state[3];
};

#endif