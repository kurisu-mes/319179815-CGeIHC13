#include "Window.h"

Window::Window()
{
	width = 800;
	height = 600;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
	estadoCofre = 0;
	estadoMandibula = 0;
	estadoPierna1 = 0;
	estadoPierna2 = 0;
	estadoPierna3 = 0;
	estadoPierna4 = 0;

	articulacionCofre = 0.0f;
	articulacionllantas = 0.0f;
	movimientoAuto = 0.0f;
	articulacionMandibula = 0.0f;
	ArticulacionPierna1 = 0.0f;
	ArticulacionPierna2 = 0.0f;
	ArticulacionPierna3 = 0.0f;
	ArticulacionPierna4 = 0.0f;

	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
int Window::Initialise()
{
	//Inicializaci�n de GLFW
	if (!glfwInit())
	{
		printf("Fall� inicializar GLFW");
		glfwTerminate();
		return 1;
	}
	//Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//CREAR VENTANA
	mainWindow = glfwCreateWindow(width, height, "Practica 05: Importacion de modelos", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tama�o de Buffer
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//MANEJAR TECLADO y MOUSE
	createCallbacks();


	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Fallo inicializacion de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST); //HABILITAR BUFFER DE PROFUNDIDAD
	// Asignar valores de la ventana y coordenadas

	//Asignar Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);
	//Callback para detectar que se est� usando la ventana
	glfwSetWindowUserPointer(mainWindow, this);
}

void Window::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, ManejaTeclado);
	glfwSetCursorPosCallback(mainWindow, ManejaMouse);
}

GLfloat Window::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat Window::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}

void Window::ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode)
{

	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	//cofre
	if (key == GLFW_KEY_E)
	{
		//estado 0: Cofre abre hasta alcanzar limite
		if (theWindow->estadoCofre == 0 && theWindow->articulacionCofre > -45.0f) {
			theWindow->articulacionCofre -= 5.0f;
		}

		//limite 1: estado 1
		if (theWindow->estadoCofre == 0 && theWindow->articulacionCofre == -45.0f) {
			theWindow->estadoCofre = 1;
		}

		//estado 1: Cofre cierra hasta alcanzar limite
		if (theWindow->estadoCofre == 1 && theWindow->articulacionCofre < 0.0f) {
			theWindow->articulacionCofre += 5.0f;
		}

		//limite 2: estado 0
		if (theWindow->estadoCofre == 1 && theWindow->articulacionCofre == 0.0f) {
			theWindow->estadoCofre = 0;
		}
	}
	
	//movimiento auto delante
	if (key == GLFW_KEY_R)
	{
		theWindow->movimientoAuto += 0.5;
		theWindow->articulacionllantas += 5.0;
	}
	
	//movimiento auto detras
	if (key == GLFW_KEY_T)
	{
		theWindow->movimientoAuto -= 0.5;
		theWindow->articulacionllantas -= 5.0;
	}

	//mandibula
	if (key == GLFW_KEY_F) {
		//estado 0: Mandibula abre hasta alcanzar limite
		if (theWindow->estadoMandibula == 0 && theWindow->articulacionMandibula > -45.0f) {
			theWindow->articulacionMandibula -= 5.0f;
		}

		//limite 1: estado 1
		if (theWindow->estadoMandibula == 0 && theWindow->articulacionMandibula == -45.0f) {
			theWindow->estadoMandibula = 1;
		}

		//estado 1: Mandibula cierra hasta alcanzar limite
		if (theWindow->estadoMandibula == 1 && theWindow->articulacionMandibula < 0.0f) {
			theWindow->articulacionMandibula += 5.0f;
		}

		//limite 2: estado 0
		if (theWindow->estadoMandibula == 1 && theWindow->articulacionMandibula == 0.0f) {
			theWindow->estadoMandibula = 0;
		}
	}

	//pierna d 1
	if (key == GLFW_KEY_G) {
		//estado 0: pierna abre hasta alcanzar limite
		if (theWindow->estadoPierna1 == 0 && theWindow->ArticulacionPierna1 < 45.0f) {
			theWindow->ArticulacionPierna1 += 5.0f;
		}

		//limite 1: estado 1
		if (theWindow->estadoPierna1 == 0 && theWindow->ArticulacionPierna1 == 45.0f) {
			theWindow->estadoPierna1 = 1;
		}

		//estado 1: pierna cierra hasta alcanzar limite
		if (theWindow->estadoPierna1 == 1 && theWindow->ArticulacionPierna1 > -45.0f) {
			theWindow->ArticulacionPierna1 -= 5.0f;
		}

		//limite 2: estado 0
		if (theWindow->estadoPierna1 == 1 && theWindow->ArticulacionPierna1 == -45.0f) {
			theWindow->estadoPierna1 = 0;
		}
	}

	//pierna d2
	if (key == GLFW_KEY_H) {
		//estado 0: pierna abre hasta alcanzar limite
		if (theWindow->estadoPierna2 == 0 && theWindow->ArticulacionPierna2 < 45.0f) {
			theWindow->ArticulacionPierna2 += 5.0f;
		}

		//limite 1: estado 1
		if (theWindow->estadoPierna2 == 0 && theWindow->ArticulacionPierna2 == 45.0f) {
			theWindow->estadoPierna2 = 1;
		}

		//estado 1: pierna cierra hasta alcanzar limite
		if (theWindow->estadoPierna2 == 1 && theWindow->ArticulacionPierna2 > -45.0f) {
			theWindow->ArticulacionPierna2 -= 5.0f;
		}

		//limite 2: estado 0
		if (theWindow->estadoPierna2 == 1 && theWindow->ArticulacionPierna2 == -45.0f) {
			theWindow->estadoPierna2 = 0;
		}

	}

	//pierna t1
	if (key == GLFW_KEY_J) {
		//estado 0: pierna abre hasta alcanzar limite
		if (theWindow->estadoPierna3 == 0 && theWindow->ArticulacionPierna3 < 45.0f) {
			theWindow->ArticulacionPierna3 += 5.0f;
		}

		//limite 1: estado 1
		if (theWindow->estadoPierna3 == 0 && theWindow->ArticulacionPierna3 == 45.0f) {
			theWindow->estadoPierna3 = 1;
		}

		//estado 1: pierna cierra hasta alcanzar limite
		if (theWindow->estadoPierna3 == 1 && theWindow->ArticulacionPierna3 > -45.0f) {
			theWindow->ArticulacionPierna3 -= 5.0f;
		}

		//limite 2: estado 0
		if (theWindow->estadoPierna3 == 1 && theWindow->ArticulacionPierna3 == -45.0f) {
			theWindow->estadoPierna3 = 0;
		}
	}

	//pierna t2
	if (key == GLFW_KEY_K) {
		//estado 0: pierna abre hasta alcanzar limite
		if (theWindow->estadoPierna4 == 0 && theWindow->ArticulacionPierna4 < 45.0f) {
			theWindow->ArticulacionPierna4 += 5.0f;
		}

		//limite 1: estado 1
		if (theWindow->estadoPierna4 == 0 && theWindow->ArticulacionPierna4 == 45.0f) {
			theWindow->estadoPierna4 = 1;
		}

		//estado 1: pierna cierra hasta alcanzar limite
		if (theWindow->estadoPierna4 == 1 && theWindow->ArticulacionPierna4 > -45.0f) {
			theWindow->ArticulacionPierna4 -= 5.0f;
		}

		//limite 2: estado 0
		if (theWindow->estadoPierna4 == 1 && theWindow->ArticulacionPierna4 == -45.0f) {
			theWindow->estadoPierna4 = 0;
		}
	}

	/*
	if (key == GLFW_KEY_L) {
		if (theWindow->articulacion6 > 45)
			theWindow->articulacion6 += 5.0;
	}
	*/
	

	if (key == GLFW_KEY_D && action == GLFW_PRESS)
	{
		const char* key_name = glfwGetKeyName(GLFW_KEY_D, 0);
		//printf("se presiono la tecla: %s\n",key_name);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
			//printf("se presiono la tecla %d'\n", key);
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
			//printf("se solto la tecla %d'\n", key);
		}
	}
}

void Window::ManejaMouse(GLFWwindow* window, double xPos, double yPos)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;
}


Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();

}
