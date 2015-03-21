#version 120

varying vec3 vColor;
uniform float fade;

void main(void)
{
  gl_FragColor = vec4(vColor, 1.0f);

}