#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <shader.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <objmesh.h>

using namespace std;

GLuint program, vbo, ibo;
GLuint vertex_loc, mvp_matrix_loc;
Shader loadShader("load Model");
objMesh mesh;

void init_shader()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glClearDepth(1.0f);
	loadShader.init();
	loadShader.attach(GL_VERTEX_SHADER, "LoadModel.vert");
	loadShader.attach(GL_FRAGMENT_SHADER, "LoadModel.frag");
	loadShader.link();
	loadShader.use();
	program = loadShader.program;
	mesh.load_obj("suzanne.txt");
	mesh.init(program);
	mvp_matrix_loc = glGetUniformLocation(program, "mvp_matrix");

}
void init()
{
	init_shader();
}

void render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	static glm::mat4 model_matrix = glm::translate(glm::mat4(1.0), glm::vec3(0.0, 0.0, -5.0));
	static glm::mat4 view_matrix = glm::lookAt(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, -5.0),
		                                       glm::vec3(0.0, 1.0, 0.0));
	static glm::mat4 proj_matrix = glm::perspective(45.0f, 720.f / 640.0f, 1.0f, 1000.0f);
	static glm::mat4 mvp_matrix = proj_matrix * view_matrix * model_matrix;
	glUniformMatrix4fv(mvp_matrix_loc, 1, GL_FALSE, &mvp_matrix[0][0]);
	mesh.render();	
	glutSwapBuffers();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA | GLUT_DEPTH);
	glutInitWindowPosition(300, 0);
	glutInitWindowSize(720, 620);
	glutCreateWindow("loadModel");
	glewInit();
	init();
	glutDisplayFunc(render);
	glutMainLoop();
}