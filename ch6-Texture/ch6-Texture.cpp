#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <shader.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "texture.h"

#define BUFFER_OFFSET(offset) ((GLvoid *)(offset + NULL))

Shader TextureShader("Transformaiton");

const static GLfloat VertexData[] = {
    // front
    -1.0, -1.0,  1.0,
     1.0, -1.0,  1.0,
     1.0,  1.0,  1.0,
    -1.0,  1.0,  1.0,
    // top
    -1.0,  1.0,  1.0,
     1.0,  1.0,  1.0,
     1.0,  1.0, -1.0,
    -1.0,  1.0, -1.0,
    // back
     1.0, -1.0, -1.0,
    -1.0, -1.0, -1.0,
    -1.0,  1.0, -1.0,
     1.0,  1.0, -1.0,
    // bottom
    -1.0, -1.0, -1.0,
     1.0, -1.0, -1.0,
     1.0, -1.0,  1.0,
    -1.0, -1.0,  1.0,
    // left
    -1.0, -1.0, -1.0,
    -1.0, -1.0,  1.0,
    -1.0,  1.0,  1.0,
    -1.0,  1.0, -1.0,
    // right
     1.0, -1.0,  1.0,
     1.0, -1.0, -1.0,
     1.0,  1.0, -1.0,
     1.0,  1.0,  1.0,
  };

const static GLushort ElementData[] = {
    // front
    0,  1,  2,
    2,  3,  0,
    // top
    4,  5,  6,
    6,  7,  4,
    // back
    8,  9, 10,
    10, 11,  8,
    // bottom
    12, 13, 14,
    14, 15, 12,
    // left
    16, 17, 18,
    18, 19, 16,
    // right
    20, 21, 22,
    22, 23, 20,
  };
GLfloat TexCoordData[2*4*6] = {
    // front
    0.0, 0.0,
    1.0, 0.0,
    1.0, 1.0,
    0.0, 1.0,
  };
GLuint program, vbo, ibo, tex, tbo;
GLuint vertex_loc,  tc_loc;
GLuint box_loc, mvp_matrix_loc;

void init_buffer()
{
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData), VertexData, GL_STATIC_DRAW);
	glGenBuffers(1, &tex);
    glBindBuffer(GL_TEXTURE_2D, tex);
	loadTexture("box.jpg");
	 for (int i = 1; i < 6; i++)
		 memcpy(&TexCoordData[i*4*2], &TexCoordData[0], 2*4*sizeof(GLfloat));
	glGenBuffers(1, &tbo);
    glBindBuffer(GL_ARRAY_BUFFER, tbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TexCoordData), TexCoordData, GL_STATIC_DRAW);

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ElementData), &ElementData, GL_STATIC_DRAW);

}

void init_shader()
{	
	TextureShader.init();
	TextureShader.attach(GL_VERTEX_SHADER, "Texture.vert");
	TextureShader.attach(GL_FRAGMENT_SHADER, "Texture.frag");
	TextureShader.link();
	TextureShader.use();
	program = TextureShader.program;

	vertex_loc = glGetAttribLocation(program, "position");
	tc_loc = glGetAttribLocation(program, "texcoord");
	box_loc = glGetUniformLocation(program, "box");
	glUniform1f(box_loc, 0);
	mvp_matrix_loc = glGetUniformLocation(program, "mvp_matrix");
}

void init(void)
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClearDepth(1.0f);

	init_buffer();
	init_shader();
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(vertex_loc, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, tbo);
	glVertexAttribPointer(tc_loc, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vertex_loc);
	glEnableVertexAttribArray(tc_loc);
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
	glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, tex);
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
	glutCreateWindow("ch5-Texture");
	GLenum status = glewInit();
	if (status != GLEW_OK)
		cerr << glewGetErrorString(status) << endl;
	init();
	glutReshapeFunc(reshape);
	glutDisplayFunc(render);
	glutMainLoop();
}