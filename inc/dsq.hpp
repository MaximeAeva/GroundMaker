#ifndef DSQ_H
#define DSQ_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <cstdlib>
#include <cmath>
#include <iostream>

//extern float mappedVert[24];

//extern unsigned int mappedIdx[6];

float* MapVertices(int size, float step, float* diamond);
unsigned int* MapIndices(int size);
float* diamondSquare(int size, float smoothness, int filter);


#endif