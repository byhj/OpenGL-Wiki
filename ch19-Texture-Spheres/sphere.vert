attribute vec3 position;
varying vec4 texCoords;
uniform mat4 mvp_matrix;

void main(void) {
    gl_Position = mvp_matrix * vec4(position, 1.0);
    texCoords = vec4(position, 1.0);
}
