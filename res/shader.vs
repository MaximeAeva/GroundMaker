#version 330 core
layout (location = 0) in vec3 aPos;   // la variable aPos a l'attribut de position 0
layout (location = 1) in vec3 aColor; // la variable aColor a l'attribut de position 1
  
out vec3 ourColor; // transmettre une couleur au fragment shader

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    ourColor = aColor; // affecter ourColor avec l'entrée color issue des  données vertex
}