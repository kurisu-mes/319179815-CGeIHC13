//PRACTICA 3. MAGALLY ESCAMILLA

#include <stdio.h>
#include <string.h>
#include<cmath>
#include<vector>
#include <glew.h>
#include <glfw3.h>
//glm
#include<glm.hpp>
#include<gtc\matrix_transform.hpp>
#include<gtc\type_ptr.hpp>
#include <gtc\random.hpp>
//clases para dar orden y limpieza al còdigo
#include"Mesh.h"
#include"Shader.h"
#include"Sphere.h"
#include"Window.h"
#include"Camera.h"
//tecla E: Rotar sobre el eje X
//tecla R: Rotar sobre el eje Y
//tecla T: Rotar sobre el eje Z


using std::vector;

//Dimensiones de la ventana
const float toRadians = 3.14159265f / 180.0; //grados a radianes
const float PI = 3.14159265f;
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;
Camera camera;
Window mainWindow;
vector<Mesh*> meshList;
vector<Shader>shaderList;
vector<MeshColor*> meshColorList;

//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";
Sphere sp = Sphere(1.0, 20, 20); //recibe radio, slices, stacks


// Pirámide triangular regular
void CrearPiramideTriangular()
{
	unsigned int indices_piramide_triangular[] = {
			0,1,2,
			1,3,2,
			3,0,2,
			1,0,3

	};
	GLfloat vertices_piramide_triangular[] = {
		-0.5f, -0.5f  ,0.0f,	//0
		0.5f,  -0.5f,  0.0f,	//1
		0.0f,   0.366f, -0.366f,	//2
		0.0f,  -0.5f,  -1.0f,	//3
	};
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices_piramide_triangular, indices_piramide_triangular, 12, 12);
	meshList.push_back(obj1);

}

// Pirámide triangular regular. distintos colores
void CrearPiramideColor()
{
	GLfloat vertices_piramide_color[] = {
		-0.5f, -0.5f  ,0.05f,		1.0f, 0.0f, 0.0f,	//0
		0.5f,  -0.5f,  0.05f,		1.0f, 0.0f, 0.0f,	//1
		0.0f,   0.366f, -0.316f,	1.0f, 0.0f, 0.0f,	//2

		0.55f,  -0.5f,  -0.05f,		0.0f, 1.0f, 0.0f,	//1
		0.05f,  -0.5f,  -1.05f,		0.0f, 1.0f, 0.0f,//3
		0.05f,   0.366f, -0.416f,	0.0f, 1.0f, 0.0f,	//2

		-0.05f,  -0.5f,  -1.05f,		0.0f, 0.0f, 1.0f,//3
		-0.55f, -0.5f  ,-0.05f,		0.0f, 0.0f, 1.0f,	//0
		-0.05f,   0.366f, -0.416f,	0.0f, 0.0f, 1.0f,	//2

		0.5f,  -0.55f,  -0.05f,		1.0f, 1.0f, 0.0f,	//1
		-0.5f, -0.55f  ,-0.05f,		1.0f, 1.0f, 0.0f,	//0
		0.0f,  -0.55f,  -1.05f,		1.0f, 1.0f, 0.0f,//3
	};

	MeshColor* piramide2 = new MeshColor();
	piramide2->CreateMeshColor(vertices_piramide_color, 72);
	meshColorList.push_back(piramide2);

}

void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	Shader* shader2 = new Shader();
	shader2->CreateFromFiles(vShaderColor, fShader);
	shaderList.push_back(*shader2);
}


int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();

	CrearPiramideTriangular();//índice 0 en MeshList
	CreateShaders();
	CrearPiramideColor();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.3f);

	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	GLuint uniformView = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(glm::radians(60.0f), mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	//glm::mat4 projection = glm::ortho(-1, 1, -1, 1, 1, 10);


	glm::mat4 model(1.0);//Inicializar matriz de Modelo 4x4
	glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f); //inicializar Color para enviar a variable Uniform;

	while (!mainWindow.getShouldClose())
	{

		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;
		//Recibir eventos del usuario
		glfwPollEvents();
		//Cámara
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		//Limpiar la ventana
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad
		shaderList[0].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		uniformView = shaderList[0].getViewLocation();
		uniformColor = shaderList[0].getColorLocation();

		//base negra
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.17f, -3.9f));
		model = glm::scale(model, glm::vec3(3.3f, 3.3f, 3.3f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[0]->RenderMesh();


		//caras del pyramix
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		uniformView = shaderList[1].getViewLocation();

		//esquina superior
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.62f, -4.72f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshColorList[0]->RenderMeshColor();

		//esquina izquierda
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-1.1f, -1.3f, -3.89f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshColorList[0]->RenderMeshColor();

		//esquina derecha
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(1.1f, -1.3f, -3.89f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshColorList[0]->RenderMeshColor();

		//esquina detrás
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.3f, -6.13f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshColorList[0]->RenderMeshColor();

		//costados 
		//rojo azul
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.56f, -0.35f, -4.31f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshColorList[0]->RenderMeshColor();

		//rojo verde
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.56f, -0.35f, -4.31f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshColorList[0]->RenderMeshColor();

		//azul verde
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.35f, -5.42f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshColorList[0]->RenderMeshColor();

		//rojo amarillo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.3f, -3.9f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshColorList[0]->RenderMeshColor();

		//azul amarillo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.55f, -1.3f, -5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshColorList[0]->RenderMeshColor();

		//verde amarillo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.55f, -1.3f, -5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshColorList[0]->RenderMeshColor();

		//caras de un solo color
		//caras rojas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.35f, -4.3f));
		model = glm::rotate(model, 135 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshColorList[0]->RenderMeshColor();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.55f, -1.275f, -3.9f));
		model = glm::rotate(model, 135 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshColorList[0]->RenderMeshColor();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.55f, -1.275f, -3.9f));
		model = glm::rotate(model, 135 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshColorList[0]->RenderMeshColor();

		//caras amarillas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.3f, -6.15f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshColorList[0]->RenderMeshColor();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.55f, -1.3f, -5.05f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshColorList[0]->RenderMeshColor();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.55f, -1.3f, -5.05f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshColorList[0]->RenderMeshColor();


		//caras azules
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.05f, -0.65f, -5.54f));
		model = glm::rotate(model, 157 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 46 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, 50 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshColorList[0]->RenderMeshColor();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.6f, -1.6f, -5.13f));
		model = glm::rotate(model, 157 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 46 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, 50 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshColorList[0]->RenderMeshColor();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.05f, -1.6f, -6.23f));
		model = glm::rotate(model, 157 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 46 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, 50 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshColorList[0]->RenderMeshColor();

		//caras verdes
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.05f, -0.65f, -5.54f));
		model = glm::rotate(model, 157 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, -46 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -50 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshColorList[0]->RenderMeshColor();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.6f, -1.6f, -5.13f));
		model = glm::rotate(model, 157 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, -46 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -50 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshColorList[0]->RenderMeshColor();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.05f, -1.6f, -6.23f));
		model = glm::rotate(model, 157 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, -46 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -50 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshColorList[0]->RenderMeshColor();

		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}



//model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
//model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));  //al presionar la tecla Y se rota sobre el eje y
//model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));

//model = glm::scale(model, glm::vec3(1.2f, 1.0f, 1.2f));
//model = glm::rotate(model, 105 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));


