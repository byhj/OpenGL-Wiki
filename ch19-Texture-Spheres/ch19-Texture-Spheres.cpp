#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <shader.h> 
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SOIL/SOIL.h>

#define BUFFER_OFFSET(offset) ((GLvoid *)(offset + NULL))

Shader TextureShader("Transformaiton");

GLuint program, sphere_vbo;
GLuint vertex_loc,  tc_loc;
GLuint sphere_loc, mvp_matrix_loc;
GLuint textureID;
const float M_PI = 3.141598;

GLuint sphere(float radius, int slices, int stacks) {
  GLuint vbo;
  const int n = 2 * (slices + 1) * stacks;
  int i = 0;
  vector<glm::vec3> points(n);
  
  for (float theta = -M_PI / 2; theta < M_PI / 2 - 0.0001; theta += M_PI / stacks) {
    for (float phi = -M_PI; phi <= M_PI + 0.0001; phi += 2 * M_PI / slices) {
      points[i++] = glm::vec3(cos(theta) * sin(phi), -sin(theta), cos(theta) * cos(phi));
      points[i++] = glm::vec3(cos(theta + M_PI / stacks) * sin(phi), -sin(theta + M_PI / stacks), cos(theta + M_PI / stacks) * cos(phi));
    }
  }
  
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof points, &points[0], GL_STATIC_DRAW);
 
  return vbo;
}

void init_buffer()
{
	sphere_vbo = sphere(1, 30, 30);
}

void init_shader()
{	
	TextureShader.init();
	TextureShader.attach(GL_VERTEX_SHADER, "sphere.vert");
	TextureShader.attach(GL_FRAGMENT_SHADER, "sphere.frag");
	TextureShader.link();
	TextureShader.use();
	program = TextureShader.program;

	vertex_loc = glGetAttribLocation(program, "position");
	sphere_loc = glGetUniformLocation(program, "sphere");
	glUniform1f(sphere_loc, 0);
	mvp_matrix_loc = glGetUniformLocation(program, "mvp_matrix");
}

void init_texture()
{
	glActiveTexture(GL_TEXTURE0);
	textureID = SOIL_load_OGL_texture(     
	            "Earthmap720x360_grid.jpg",
                SOIL_LOAD_AUTO,
                SOIL_CREATE_NEW_ID,
                SOIL_FLAG_INVERT_Y | SOIL_FLAG_TEXTURE_REPEATS
               );
}

void init(void)
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearDepth(1.0f);

	init_buffer();
	init_shader();
	glBindBuffer(GL_ARRAY_BUFFER, sphere_vbo);
	glVertexAttribPointer(vertex_loc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vertex_loc);
}

void render()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(1.0, 1.0, 1.0, 1.0);
    float angle = glutGet(GLUT_ELAPSED_TIME) / 1000.0 * 30;  // 30бу per second
  glm::mat4 anim = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0, 1, 0));


  glm::mat4 fix_orientation = glm::rotate(glm::mat4(1.0f), glm::radians(-90.f), glm::vec3(1, 0, 0));
  glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, -3.0));
  glm::mat4 view = glm::lookAt(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, -3.0), glm::vec3(0.0, 1.0, 0.0));
  glm::mat4 projection = glm::perspective(45.0f, 1.0f*720.0f/640.0f, 0.1f, 1000.0f);
  glUseProgram(program);
  glm::mat4 model_matrix = model;
  glm::mat4 mvp_matrix = projection * view * model_matrix;
  glUniformMatrix4fv(mvp_matrix_loc, 1, GL_FALSE, &mvp_matrix[0][0]);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, textureID);
  glUniform1i(sphere_loc, 0);
  glBindBuffer(GL_ARRAY_BUFFER, sphere_vbo);
  /*
   glCullFace(GL_FRONT);
   glDrawArrays(GL_TRIANGLE_STRIP, 0, 2 * 31 * 30);

   glCullFace(GL_BACK);
   glDrawArrays(GL_TRIANGLE_STRIP, 0, 2 * 31 * 30);
   &*/
   glutSolidSphere(1.0, 30, 30);
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