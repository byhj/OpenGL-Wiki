#version 120

void main(void)
{
   gl_FragColor[0] = gl_FragCoord.x/640.0;
   gl_FragColor[1] = gl_FragCoord.y/480.0;
   gl_FragColor[2] = 0.5;
   // gl_FragColor = vec4(0.0f, 0.0f, 1.0f, 0.0f);
   //gl_FragColor = vec4(gl_FragCoord.x / 640, gl_FragColor.y / 480.0, 0.5, 1.0);
}