#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <shader.h> 
#include <glm/glm.hpp>

#define BUFFER_OFFSET(offset) ((GLvoid *)(offset + NULL))

Shader InformationShader("Information");

static const glm::vec3 VertexData[] =
{
    glm::vec3(0.0,  0.8, 0.0),  glm::vec3(1.0, 1.0, 0.0),
    glm::vec3(-0.8, -0.8, 0.0), glm::vec3(0.0, 0.0, 1.0),
    glm::vec3(0.8, -0.8, 0.0),  glm::vec3(1.0, 0.0, 0.0)
};

GLuint program, vbo;
GLuint vertex_loc, color_loc;
GLuint fade_loc;

void init_buffer()
{
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData), &VertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void init_shader()
{	
	InformationShader.init();
	InformationShader.attach(GL_VERTEX_SHADER, "Information.vert");
	InformationShader.attach(GL_FRAGMENT_SHADER, "Information.frag");
	InformationShader.link();
	InformationShader.use();
	program = InformationShader.program;

	vertex_loc = glGetAttribLocation(program, "position");
	color_loc = glGetAttribLocation(program, "color");
	fade_loc = glGetUniformLocation(program, "fade");
}

void init(void)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	init_buffer();
	init_shader();
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(vertex_loc, 3, GL_FLOAT, GL_FALSE, 2*sizeof(glm::vec3), 0);
	glVertexAttribPointer(color_loc, 3, GL_FLOAT, GL_FALSE, 2*sizeof(glm::vec3), BUFFER_OFFSET(sizeof(glm::vec3) + NULL));
	glEnableVertexAttribArray(vertex_loc);
	glEnableVertexAttribArray(color_loc);
}

void render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(program);
	glUniform1f(fade_loc, 0.5);
	glDrawArrays(GL_TRIANGLES, 0, 3);
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
	glutCreateWindow("ch3-Information");
	GLenum status = glewInit();
	if (status != GLEW_OK)
		cerr << glewGetErrorString(status) << endl;
	init();
	glutReshapeFunc(reshape);
	glutDisplayFunc(render);
	glutMainLoop();
}