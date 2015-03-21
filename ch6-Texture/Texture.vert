#version 120

attribute vec3 position;
attribute vec2 texcoord;

varying vec3 vColor;
varying vec2 tc;

uniform mat4 mvp_matrix;

void main(void)
{
  gl_Position = mvp_matrix * vec4(position, 1.0f);
  tc = texcoord;
}