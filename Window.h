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
		return  glfwWindowShouldClose(mainWindow);
	}
	bool* getsKeys() { return keys; }
	GLfloat getXChange();
	GLfloat getYChange();
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }
	GLfloat getArticulacionCofre() { return articulacionCofre; }
	GLfloat getArticulacionllantas() { return articulacionllantas; }
	GLfloat getMovimientoAuto() { return movimientoAuto; }
	GLfloat getArticulacionMandibula() { return articulacionMandibula; }
	GLfloat getArticulacionPierna1() { return ArticulacionPierna1; }
	GLfloat getArticulacionPierna2() { return ArticulacionPierna2; }
	GLfloat getArticulacionPierna3() { return ArticulacionPierna3; }
	GLfloat getArticulacionPierna4() { return ArticulacionPierna4; }

	~Window();
private:
	GLFWwindow* mainWindow;
	GLint width, height;
	int estadoCofre, estadoMandibula, estadoPierna1, estadoPierna2, estadoPierna3, estadoPierna4;
	GLfloat articulacionCofre, articulacionllantas, movimientoAuto, articulacionMandibula, ArticulacionPierna1, ArticulacionPierna2, ArticulacionPierna3, ArticulacionPierna4;
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

