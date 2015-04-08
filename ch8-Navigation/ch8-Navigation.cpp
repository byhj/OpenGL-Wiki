#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <shader.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include <objmesh.h>


using namespace std;

GLuint program, vbo, ibo;
GLuint vertex_loc, mvp_matrix_loc;
Shader navigatoinShader("Navigation");
objMesh mesh;

static glm::vec3 target(0.0f, 0.0f, 0.0f);
static glm::vec3 up(0.0f, 1.0f, 0.0f);
static float camX = 0.0, camY = 0.0, camZ = 5.0;
int startX, startY, tracking = 0;
float alpha = 0.0f, beta = 0.0f;
float r = 5.0f;

void processKeys(unsigned char key, int xx, int yy) 
{
	switch(key) {

		case 27:
			glutLeaveMainLoop();
			break;
		case 'z': r -= 0.1f; break;
		case 'x': r += 0.1f; break;	
		case 'm': glEnable(GL_MULTISAMPLE); break;
		case 'n': glDisable(GL_MULTISAMPLE); break;
	}
	camX = r * sin(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
	camZ = r * cos(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
	camY = r *   						     sin(beta * 3.14f / 180.0f);

//  uncomment this if not using an idle func
//	glutPostRedisplay();
}

void processMouseButtons(int button, int state, int xx, int yy)
{
	// start tracking the mouse
	if (state == GLUT_DOWN)  {
		startX = xx; //获取当前坐标
		startY = yy;
		if (button == GLUT_LEFT_BUTTON)
			tracking = 1;
		else if (button == GLUT_RIGHT_BUTTON)
			tracking = 2;
	}

	//stop tracking the mouse
	else if (state == GLUT_UP) {
		if (tracking == 1) {
			alpha += (startX - xx);
			beta += (yy - startY);
		}
		else if (tracking == 2) {
			r += (yy - startY) * 0.01f;
		}
		tracking = 0;
	}
}

void processMouseMotion(int xx, int yy)
{
	int deltaX, deltaY;
	float alphaAux, betaAux;
	float rAux;

	deltaX = startX - xx;
	deltaY = yy - startY;

	// left mouse button: move camera
	if (tracking == 1) {
		alphaAux = alpha + deltaX;
		betaAux = beta + deltaY;
		if (betaAux > 85.0f)
			betaAux = 85.0f;
		else if (betaAux < -85.0f)
			betaAux = -85.0f;
		rAux = r;

		camX = rAux * cos(betaAux * 3.14f / 180.0f) * sin(alphaAux * 3.14f / 180.0f);
		camZ = rAux * cos(betaAux * 3.14f / 180.0f) * cos(alphaAux * 3.14f / 180.0f);
		camY = rAux * sin(betaAux * 3.14f / 180.0f);
	}
	// right mouse button: zoom
	else if (tracking == 2) {

		alphaAux = alpha;
		betaAux = beta;
		rAux = r + (deltaY * 0.01f);

		camX = rAux * cos(betaAux * 3.14f / 180.0f) * sin(alphaAux * 3.14f / 180.0f);
		camZ = rAux * cos(betaAux * 3.14f / 180.0f) * cos(alphaAux * 3.14f / 180.0f);
		camY = rAux * sin(betaAux * 3.14f / 180.0f);
	}
}

void mouseWheel(int wheel, int direction, int x, int y) {

	r += direction * 0.1f;
	camX = r * sin(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
	camZ = r * cos(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
	camY = r * sin(beta * 3.14f / 180.0f);
}


void init_shader()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glClearDepth(1.0f);
	navigatoinShader.init();
	navigatoinShader.attach(GL_VERTEX_SHADER, "navigation.vert");
	navigatoinShader.attach(GL_FRAGMENT_SHADER, "navigation.frag");
	navigatoinShader.link();
	navigatoinShader.use();
	program = navigatoinShader.program;
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
	glm::mat4 model_matrix = glm::translate(glm::mat4(1.0), glm::vec3(0.0, 0.0, 0.0));
	glm::mat4 view_matrix = glm::lookAt(glm::vec3(camX, camY, camZ), target, up);
	glm::mat4 proj_matrix = glm::perspective(45.0f, 720.f / 640.0f, 1.0f, 1000.0f);
	glm::mat4 mvp_matrix = proj_matrix * view_matrix * model_matrix;
	glUniformMatrix4fv(mvp_matrix_loc, 1, GL_FALSE, &mvp_matrix[0][0]);
	mesh.render();	
	glutSwapBuffers();
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA | GLUT_DEPTH);
	glutInitWindowPosition(300, 0);
	glutInitWindowSize(720, 620);
	glutCreateWindow("navigatoin");
	glewInit();
	init();
	glutKeyboardFunc(processKeys);
	glutMouseFunc(processMouseButtons);
	glutMotionFunc(processMouseMotion);
	glutMouseWheelFunc (mouseWheel);
	glutDisplayFunc(render);
	glutMainLoop();
}