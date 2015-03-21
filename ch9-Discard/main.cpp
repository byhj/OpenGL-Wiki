#include <GL/glew.h>
#include <GL/freeglut.h>

#include <iostream>
#include "textfile.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

GLuint program;
GLuint transformLocation;


   

void init(void)
{
  glClearColor(0.0, 0.0, 0.0, 5.0);
  glEnable(GL_BLEND | GL_DEPTH_TEST ); // GL_DEPTH_TEST
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
 //glDepthFunc(GL_LESS);
  
  GLint compile_status = GL_FALSE, link_status = GL_FALSE;
  GLuint vs = glCreateShader(GL_VERTEX_SHADER);

  const char *vs_source =  textFileRead("triangle.vert");

   glShaderSource(vs, 1, &vs_source, NULL);
   glCompileShader(vs);
   glGetShaderiv(vs, GL_COMPILE_STATUS, &compile_status);
   if (!compile_status) {
	   cerr << "Error in vertex shader" << endl;
   }
   GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);

  
     const char *fs_source = textFileRead("triangle.frag");

	 glShaderSource(fs, 1, &fs_source, NULL);
	 glCompileShader(fs);
	 glGetShaderiv(fs, GL_COMPILE_STATUS, &compile_status);
	 if (!compile_status) {
		 cerr << "fragment shader Error" << endl;
	 }
	 program = glCreateProgram();
	 glAttachShader(program, vs);
	 glAttachShader(program, fs);
	 glLinkProgram(program);
	 glGetProgramiv(program, GL_LINK_STATUS, &link_status);
	 if (!link_status) {
		 cerr << "Link shader Error" << endl;
	 }
	 transformLocation = glGetUniformLocation(program, "mvp");
	
	 
}

void render(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(program);
	glutSolidCube(1.0);
	
	glutSwapBuffers();
}

void reshape(int w, int h)
{
	glViewport(0, 0, 640, 480);
}

void free()
{
	glDeleteProgram(program);
}

void idle()
{
	float angle = glutGet(GLUT_ELAPSED_TIME) / 1000.0 * glm::radians(15.0);  // base 15бу per second
    glm::mat4 anim = \
    glm::rotate(glm::mat4(1.0f), angle*3.0f, glm::vec3(1, 0, 0)) *  // X axis
    glm::rotate(glm::mat4(1.0f), angle*2.0f, glm::vec3(0, 1, 0)) *  // Y axis
    glm::rotate(glm::mat4(1.0f), angle*4.0f, glm::vec3(0, 0, 1));   // Z axis

    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, 0.0));
    glm::mat4 view = glm::lookAt(glm::vec3(0.0, 0.0, 10.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
    glm::mat4 projection = glm::perspective(45.0f, 1.0f, 0.1f, 1000.0f);
    glm::mat4 mvp = projection * view * model ;//* anim;
	glUseProgram(program);
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(mvp));
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_ALPHA| GLUT_DOUBLE);
	glutInitWindowSize(640, 480);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Triangle");

	GLenum status = glewInit();
	if (status != GLEW_OK) {
		cerr << "Error" << glewGetErrorString(status);
		return EXIT_FAILURE;
	}
	init();
	glutDisplayFunc(render);
	glutIdleFunc(idle);
	glutMainLoop();
	free();
	return 0;
}

