#version 120  // OpenGL 2.1

attribute vec3 position;
uniform mat4 mvp_matrix; 
varying vec3 vPosition;

void main() 
{                        
      gl_Position = mvp_matrix * vec4(position, 1.0);
	  vPosition = position; 
}