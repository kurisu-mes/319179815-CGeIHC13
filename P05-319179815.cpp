/*
* MAGALLY ESCAMILLA SUÁREZ
Practica 5: Optimización y Carga de Modelos
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_m.h"
#include "Camera.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

const float toRadians = 3.14159265f / 180.0f;
//float angulocola = 0.0f;
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;
Model cuerpoGoddard;
Model mandibulaGoddard;
Model piernaDGoddard;
Model piernaTGoddard;

Model chasisAuto;
Model cofreAuto;
Model llantaAuto;

Skybox skybox;

//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;


// Vertex Shader
static const char* vShader = "shaders/shader_m.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_m.frag";





void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};


	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);


}


void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}



int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	//mainWindow = Window(866, 600);
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.5f, 7.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 1.0f);

	cuerpoGoddard = Model();
	cuerpoGoddard.LoadModel("Models/Goddard/cuerpo.obj");

	mandibulaGoddard = Model();
	mandibulaGoddard.LoadModel("Models/Goddard/mandibula.obj");

	piernaDGoddard = Model();
	piernaDGoddard.LoadModel("Models/Goddard/pata_delantera.obj");

	piernaTGoddard = Model();
	piernaTGoddard.LoadModel("Models/Goddard/pata_trasera.obj");


	chasisAuto = Model();
	chasisAuto.LoadModel("Models/Auto/chasis.obj");

	cofreAuto = Model();
	cofreAuto.LoadModel("Models/Auto/cofre.obj");

	llantaAuto = Model();
	llantaAuto.LoadModel("Models/Auto/llanta.obj");


	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);


	glm::mat4 model(1.0);
	glm::mat4 modelTorso(1.0);
	glm::mat4 modelAuto(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//Se dibuja el Skybox
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformColor = shaderList[0].getColorLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		// INICIA DIBUJO DEL PISO
		color = glm::vec3(0.5f, 0.5f, 0.5f); //piso de color gris
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMesh();

		//------------*GODDARD*-------------------*
		//cuerpo
		color = glm::vec3(0.0f, 0.7f, 0.5f);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, 1.0f, 15.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cuerpoGoddard.RenderModel();
		modelTorso = model;

		//mandibula
		color = glm::vec3(0.8f, 0.2f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = glm::translate(model, glm::vec3(2.5f, 0.6f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getArticulacionMandibula()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mandibulaGoddard.RenderModel();
		model = modelTorso;

		//Pierna delantera 1
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = glm::translate(model, glm::vec3(0.9f, -0.45f, 0.7f));
		model = glm::rotate(model, glm::radians(mainWindow.getArticulacionPierna1()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		piernaDGoddard.RenderModel();
		model = modelTorso;

		//Pierna delantera 2
		color = glm::vec3(0.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = glm::translate(model, glm::vec3(0.9f, -0.45f, -0.7f));
		model = glm::rotate(model, glm::radians(mainWindow.getArticulacionPierna2()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		piernaDGoddard.RenderModel();
		model = modelTorso;

		//Pierna trasera 1
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = glm::translate(model, glm::vec3(-0.8f, -1.15f, 0.7f));
		model = glm::rotate(model, glm::radians(mainWindow.getArticulacionPierna3()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		piernaTGoddard.RenderModel();
		model = modelTorso;

		//Pierna trasera 2
		color = glm::vec3(0.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = glm::translate(model, glm::vec3(-0.8f, -1.15f, -0.7f));
		model = glm::rotate(model, glm::radians(mainWindow.getArticulacionPierna4()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		piernaTGoddard.RenderModel();
		model = modelTorso;

		//------------*AUTO*-------------------*
		//chasis
		model = glm::mat4(1.0);
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, mainWindow.getMovimientoAuto()));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		modelAuto = model;
		color = glm::vec3(0.1f, 0.2f, 0.6f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		chasisAuto.RenderModel();
		model = modelAuto;

		//cofre
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		color = glm::vec3(0.0f, 0.25f, 0.3f);
		model = glm::translate(model, glm::vec3(0.0f, 0.43f, 1.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getArticulacionCofre()), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cofreAuto.RenderModel();
		model = modelAuto;

		//llanta delantera 1
		model = glm::translate(model, glm::vec3(-1.0f, -0.3f, 1.5f));
		model = glm::rotate(model, glm::radians(mainWindow.getArticulacionllantas()), glm::vec3(1.0f, 0.0f, 0.0f));
		color = glm::vec3(0.1f, 0.1f, 0.1f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		llantaAuto.RenderModel();
		model = modelAuto;

		//llanta delantera 2
		model = glm::translate(model, glm::vec3(1.0f, -0.3f, 1.5f));
		model = glm::rotate(model, glm::radians(mainWindow.getArticulacionllantas()), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		llantaAuto.RenderModel();
		model = modelAuto;

		//llanta trasera 1
		model = glm::translate(model, glm::vec3(-1.0f, -0.3f, -1.5f));
		model = glm::rotate(model, glm::radians(mainWindow.getArticulacionllantas()), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		llantaAuto.RenderModel();
		model = modelAuto;

		//llanta trasera 2
		model = glm::translate(model, glm::vec3(1.0f, -0.3f, -1.5f));
		model = glm::rotate(model, glm::radians(mainWindow.getArticulacionllantas()), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		llantaAuto.RenderModel();
		model = modelAuto;


		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
