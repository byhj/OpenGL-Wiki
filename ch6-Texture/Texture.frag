#version 120

varying vec2 tc;

uniform sampler2D box;

void main(void)
{
  gl_FragColor = texture2D(box, tc);
}