#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <shader.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define BUFFER_OFFSET(offset) ((GLvoid *)(offset + NULL))

Shader TransformaitonShader("Transformaiton");

static const glm::vec3 VertexData[] =
{
    glm::vec3(0.0,  0.8, 0.0),  glm::vec3(1.0, 1.0, 0.0),
    glm::vec3(-0.8, -0.8, 0.0), glm::vec3(0.0, 0.0, 1.0),
    glm::vec3(0.8, -0.8, 0.0),  glm::vec3(1.0, 0.0, 0.0)
};

GLuint program, vbo;
GLuint vertex_loc, color_loc;
GLuint fade_loc, model_matrix_loc;

void init_buffer()
{
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData), &VertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void init_shader()
{	
	TransformaitonShader.init();
	TransformaitonShader.attach(GL_VERTEX_SHADER, "Transformation.vert");
	TransformaitonShader.attach(GL_FRAGMENT_SHADER, "Transformation.frag");
	TransformaitonShader.link();
	TransformaitonShader.use();
	program = TransformaitonShader.program;

	vertex_loc = glGetAttribLocation(program, "position");
	color_loc = glGetAttribLocation(program, "color");
	fade_loc = glGetUniformLocation(program, "fade");
	model_matrix_loc = glGetUniformLocation(program, "model_matrix");
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
	float move = sinf(glutGet(GLUT_ELAPSED_TIME) / 1000.0 * (2*3.14) / 5); // -1<->+1 every 5 seconds
    float angle = glutGet(GLUT_ELAPSED_TIME) / 1000.0 * 45;  // 45бу per second
    glm::vec3 axis_z(0.0f, 0.0f, 1.0f);
	static glm::mat4 model_matrix ;
	model_matrix = glm::translate(glm::mat4(1.0), glm::vec3(move, 0.0, 0.0))
                                  * glm::rotate(glm::mat4(1.0f), angle, axis_z);
	glUniformMatrix4fv(model_matrix_loc, 1, GL_FALSE, &model_matrix[0][0]);
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
	glutCreateWindow("ch4-Transformation");
	GLenum status = glewInit();
	if (status != GLEW_OK)
		cerr << glewGetErrorString(status) << endl;
	init();
	glutReshapeFunc(reshape);
	glutDisplayFunc(render);
	glutMainLoop();
}