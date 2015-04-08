#include <gl/glew.h>
#include <gl/freeglut.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>

using namespace std;

const GLchar *readShader(const char *fileName)
{
	FILE *inFile = fopen(fileName, "rb");

	if (!inFile)
	{
		fprintf(stderr, "%s: %sn", "Failed to open file", fileName);
		return NULL;
	}

	fseek(inFile, 0, SEEK_END);
	int len = ftell(inFile);
	fseek(inFile, 0, SEEK_SET);

	GLchar *source = new GLchar[len + 1];

	fread(source, 1, len, inFile);

	fclose(inFile);
	source[len];

	return const_cast<const GLchar *>(source);

}

void init()
{
	GLfloat vertices[][2] = 
	{
		0.0f, 0.5f,
		0.5f, -0.5f,
		-0.5f, -0.5f
	};
	
	GLuint vbo, vao;

	//glGenVertexArrays(1, &vao);
	//glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	GLuint vshader = glCreateShader(GL_VERTEX_SHADER);
	const GLchar *vertexSource = readShader("shader.vert");
	glShaderSource(vshader, 1, &vertexSource, NULL);
	glCompileShader(vshader);

	GLint status;
	glGetShaderiv(vshader, GL_COMPILE_STATUS, &status);
	if (status == GL_TRUE)
	{
		printf("vertex YESn");
	}
	

	GLuint fshader = glCreateShader(GL_FRAGMENT_SHADER);
	const GLchar *fragmentSource = readShader("shader.frag");
	glShaderSource(fshader, 1, &fragmentSource, NULL);
	glCompileShader(fshader);
	glGetShaderiv(fshader, GL_COMPILE_STATUS, &status);
	if (status == GL_TRUE)
	{
		printf("fragment YESn");
	}


	GLuint program = glCreateProgram();
	glAttachShader(program, vshader);
	glAttachShader(program, fshader);

	glLinkProgram(program);
	glUseProgram(program);

	GLuint posAttrib = glGetAttribLocation(program, "position");
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(posAttrib);
}

void draw()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glFlush();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(600, 600);

	glutCreateWindow("The first programe");

	if (glewInit())
	{
		fprintf(stderr, "%sn", "Failed to init glew");
		exit(-1);
	}

	init();

	glutDisplayFunc(draw);

	glutMainLoop();
	return 0;
}