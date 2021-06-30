#ifndef DSQ_H
#define DSQ_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <random>
#include <cmath>
#include <iostream>

//extern unsigned int mappedIdx[6];

float* MapVertices(int size, float step, float* diamond);
unsigned int* MapIndices(int size);
float* diamondSquare(int size, int number, float smoothness, int filter);


#endif