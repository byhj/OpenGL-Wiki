#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <shader.h>

using namespace std;

GLuint program;
GLuint mvp_matrix_loc, position_loc;
Shader blendShader("blend & cullface");
static const GLfloat VertexData[] = {
	0.0f, -1.0f, 2.0f,
	1.73205081f, -1.0f, -1.0f,
   -1.73205081f, -1.0f, -1.0f,
    0.0f, 2.0f, 0.0f
};
static const GLushort ElementData[] = 
{
	0, 1, 2,
	0, 2, 3,
	0, 3, 1,
	1, 3, 2
};

void init_shader()
{
	blendShader.init();
	blendShader.attach(GL_VERTEX_SHADER, "blend.vert");
	blendShader.attach(GL_FRAGMENT_SHADER, "blend.frag");
	blendShader.link();
	blendShader.use();
	program = blendShader.program;
	GLuint ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ElementData), ElementData, GL_STATIC_DRAW);
	mvp_matrix_loc = glGetUniformLocation(program, "mvp_matrix");
	position_loc = glGetAttribLocation(program, "position");
	glEnableVertexAttribArray(position_loc);
	glVertexAttribPointer(position_loc, 3, GL_FLOAT, GL_FALSE, 0, VertexData);
}
void init(void)
{
    init_shader();
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST); // GL_DEPTH_TEST
	glDepthFunc(GL_LEQUAL);

	glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glPolygonMode( GL_FRONT, GL_FILL);
    glPolygonMode( GL_BACK, GL_LINE);
	
}

void render(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(program);
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_SHORT, 0);
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
	float angle = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    glm::mat4 anim = glm::rotate(glm::mat4(1.0f), angle*30.0f, glm::vec3(0, 1, 0)) ;  // Y axis

    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, 0.0));
    glm::mat4 view = glm::lookAt(glm::vec3(0.0, 0.0, 10.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
    glm::mat4 projection = glm::perspective(45.0f, 1.0f, 0.1f, 1000.0f);
    glm::mat4 mvp = projection * view * model* anim;
	glUseProgram(program);
	glUniformMatrix4fv(mvp_matrix_loc, 1, GL_FALSE, glm::value_ptr(mvp));
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(720, 640);
	glutInitWindowPosition(300, 100);
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

