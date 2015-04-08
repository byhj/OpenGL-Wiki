#version 120

attribute vec3 position;
attribute vec3 color;

varying vec3 vColor;
uniform mat4 model_matrix;

void main(void)
{
  gl_Position = model_matrix * vec4(position, 1.0f);
  vColor = color;
}