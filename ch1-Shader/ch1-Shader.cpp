#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <shader.h> 

Shader triangleShader("shader");

static const GLfloat VertexData[] =
{
	0.0f,  0.8f,
   -0.8f, -0.8f,
    0.8f, -0.8f    
};

GLuint vertex_loc, program;

void init(void)
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	triangleShader.init();
	triangleShader.attach(GL_VERTEX_SHADER, "shader.vert");
	triangleShader.attach(GL_FRAGMENT_SHADER, "shader.frag");
	triangleShader.link();
	triangleShader.use();
	program = triangleShader.program;
	vertex_loc = glGetAttribLocation(program, "position");
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(vertex_loc, 2, GL_FLOAT, GL_FALSE, 0, VertexData);

}

void render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(program);
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
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(720, 640);
	glutInitWindowPosition(300, 0);
	glutCreateWindow("ch1-Shader");
	GLenum status = glewInit();
	if (status != GLEW_OK)
		cerr << glewGetErrorString(status) << endl;
	init();
	glutReshapeFunc(reshape);
	glutDisplayFunc(render);
	glutMainLoop();
}