#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <shader.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define BUFFER_OFFSET(offset) ((GLvoid *)(offset + NULL))

Shader CubeShader("Transformaiton");

const static GLfloat VertexData[] = {
    // front
    -1.0, -1.0,  1.0,
     1.0, -1.0,  1.0,
     1.0,  1.0,  1.0,
    -1.0,  1.0,  1.0,
    // back
    -1.0, -1.0, -1.0,
     1.0, -1.0, -1.0,
     1.0,  1.0, -1.0,
    -1.0,  1.0, -1.0,
  };
static const GLfloat ColorData[] = {
    // front colors
    1.0, 0.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 0.0, 1.0,
    1.0, 1.0, 1.0,
    // back colors
    1.0, 0.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 0.0, 1.0,
    1.0, 1.0, 1.0,
};
const static GLushort ElementData[] = {
    // front
    0, 1, 2,
    2, 3, 0,
    // top
    3, 2, 6,
    6, 7, 3,
    // back
    7, 6, 5,
    5, 4, 7,
    // bottom
    4, 5, 1,
    1, 0, 4,
    // left
    4, 0, 3,
    3, 7, 4,
    // right
    1, 5, 6,
    6, 2, 1,
  };

GLuint program, vbo, ibo;
GLuint vertex_loc, color_loc;
GLuint fade_loc, mvp_matrix_loc;

void init_buffer()
{
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData) + sizeof(ColorData), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, NULL, sizeof(VertexData), VertexData);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(VertexData), sizeof(ColorData), ColorData);
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ElementData), &ElementData, GL_STATIC_DRAW);

}

void init_shader()
{	
	CubeShader.init();
	CubeShader.attach(GL_VERTEX_SHADER, "Cube.vert");
	CubeShader.attach(GL_FRAGMENT_SHADER, "Cube.frag");
	CubeShader.link();
	CubeShader.use();
	program = CubeShader.program;

	vertex_loc = glGetAttribLocation(program, "position");
	color_loc = glGetAttribLocation(program, "color");
	fade_loc = glGetUniformLocation(program, "fade");
	mvp_matrix_loc = glGetUniformLocation(program, "mvp_matrix");
}

void init(void)
{
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClearDepth(1.0f);

	init_buffer();
	init_shader();
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(vertex_loc, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glVertexAttribPointer(color_loc, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 
		                  BUFFER_OFFSET(sizeof(VertexData) + NULL));
	glEnableVertexAttribArray(vertex_loc);
	glEnableVertexAttribArray(color_loc);
}

void render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    float angle = glutGet(GLUT_ELAPSED_TIME) / 1000.0 * 45;  // 45бу per second
    glm::vec3 axis_y(0.0, 1.0, 0.0);
    glm::mat4 anim = glm::rotate(glm::mat4(1.0f), angle, axis_y);
	glm::mat4 proj_matrix = glm::perspective(45.0f, 720.0f/620.0f, 1.0f, 100.0f);
	glm::mat4 view_matrix = glm::lookAt(glm::vec3(0.0, 2.0, 0.0), 
		                    glm::vec3(0.0, 0.0, -4.0), glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -4.0f));
	glm::mat4 mvp_matrix = proj_matrix * view_matrix * model_matrix * anim;
	glUniformMatrix4fv(mvp_matrix_loc, 1, GL_FALSE, &mvp_matrix[0][0]);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(program);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
	glutSwapBuffers();
	glutPostRedisplay();
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA);
	glutInitWindowSize(720, 640);
	glutInitWindowPosition(300, 0);
	glutCreateWindow("ch5-Cube");
	GLenum status = glewInit();
	if (status != GLEW_OK)
		cerr << glewGetErrorString(status) << endl;
	init();
	glutReshapeFunc(reshape);
	glutDisplayFunc(render);
	glutMainLoop();
}