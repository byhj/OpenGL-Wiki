#version 120

attribute vec4 position;
attribute vec3 normal;
uniform mat4 mvp_matrix;
uniform mat4 model_matrix;

varying vec3 vNormal;
varying vec4 vPosition;

void main(void)
{
   vNormal = normal * mat3(model_matrix);
   vPosition = position * mat4(model_matrix);
   gl_Position = mvp_matrix * position;

}