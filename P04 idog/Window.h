#pragma once
#include<stdio.h>
#include<glew.h>
#include<glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	int Initialise();
	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }
	bool getShouldClose() {
		return  glfwWindowShouldClose(mainWindow);}
	bool* getsKeys() { return keys; }
	GLfloat getXChange();
	GLfloat getYChange();
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }
	GLfloat getrotay() { return rotay; }
	GLfloat getrotax() { return rotax; }
	GLfloat getrotaz() { return rotaz; }
	GLfloat getarticulacionCuello() { return articulacionCuello; }
	GLfloat getarticulacionOreja1() { return articulacionOreja1; }
	GLfloat getarticulacionOreja2() { return articulacionOreja2; }
	GLfloat getarticulacionPierna1() { return articulacionPierna1; }
	GLfloat getarticulacionPierna2() { return articulacionPierna2; }
	GLfloat getarticulacionPierna3() { return articulacionPierna3; }
	GLfloat getarticulacionPierna4() { return articulacionPierna4; }
	GLfloat getarticulacionPata1() { return articulacionPata1; }
	GLfloat getarticulacionPata2() { return articulacionPata2; }
	GLfloat getarticulacionPata3() { return articulacionPata3; }
	GLfloat getarticulacionPata4() { return articulacionPata4; }
	GLfloat getarticulacionCola() { return articulacionCola; }

	~Window();
private: 
	GLFWwindow *mainWindow;
	GLint width, height;
	GLfloat rotax, rotay, rotaz, articulacionCuello, articulacionOreja1, articulacionOreja2, articulacionPierna1,
		articulacionPierna2, articulacionPierna3, articulacionPierna4, articulacionPata1, articulacionPata2,
		articulacionPata3, articulacionPata4, articulacionCola;
	bool keys[1024];
	GLint bufferWidth, bufferHeight;
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	bool mouseFirstMoved;
	void createCallbacks();
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);
};

