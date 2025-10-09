/* MAGALLY ESCAMILLA SUÁREZ.	Práctica 6: Texturizado
*/
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
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture dadoTexture;
Texture logofiTexture;

Texture d8Texture;
Texture llantaTexture;
Texture chasisTexture;
Texture cofreTexture;

Model chasisAuto;
Model cofreAuto;
Model llantaAuto;
Model dado_M;

Skybox skybox;

//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;


// Vertex Shader
static const char* vShader = "shaders/shader_texture.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_texture.frag";




//cálculo del promedio de las normales para sombreado de Phong
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}



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

	unsigned int vegetacionIndices[] = {
		0, 1, 2,
		0, 2, 3,
		4,5,6,
		4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,
	};
	calcAverageNormals(indices, 12, vertices, 32, 8, 5);



	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

}


void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

void CrearDado()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,

		// back
		8, 9, 10,
		10, 11, 8,

		// left
		12, 13, 14,
		14, 15, 12,
		// bottom
		16, 17, 18,
		18, 19, 16,
		// top
		20, 21, 22,
		22, 23, 20,

		// right
		4, 5, 6,
		6, 7, 4,

	};
	//Ejercicio 1: reemplazar con sus dados de 6 caras texturizados, agregar normales
// average normals
	GLfloat cubo_vertices[] = {
		// front
		//x		y		z		S		T			NX		NY		NZ
		-0.5f, -0.5f,  0.5f,	0.485f, 0.356f,		0.0f,	0.0f,	-1.0f,	//0
		0.5f, -0.5f,  0.5f,		0.716f, 0.356f, 	0.0f,	0.0f,	-1.0f,	//1
		0.5f,  0.5f,  0.5f,		0.716f, 0.642f, 	0.0f,	0.0f,	-1.0f,	//2
		-0.5f,  0.5f,  0.5f,	0.485f, 0.642f, 	0.0f,	0.0f,	-1.0f,	//3
		
		// right
		//x		y		z		S		T
		0.5f, -0.5f,  0.5f,	    0.722f, 0.356f,		-1.0f,	0.0f,	0.0f, //4
		0.5f, -0.5f,  -0.5f,	0.953f,	0.356f,		-1.0f,	0.0f,	0.0f, //5
		0.5f,  0.5f,  -0.5f,	0.953f,	0.642f,		-1.0f,	0.0f,	0.0f, //6
		0.5f,  0.5f,  0.5f,	    0.722f,	0.642f,		-1.0f,	0.0f,	0.0f, //7

		// back
		-0.5f, -0.5f, -0.5f,	0.242f, 0.356f,		0.0f,	0.0f,	1.0f, //8
		0.5f, -0.5f, -0.5f,		0.011f,	0.356f,		0.0f,	0.0f,	1.0f, //9
		0.5f,  0.5f, -0.5f,		0.011f,	0.642f,		0.0f,	0.0f,	1.0f, //10
		-0.5f,  0.5f, -0.5f,	0.242f,	0.642f,		0.0f,	0.0f,	1.0f, //11

		// left
		//x		y		z		S		T
		-0.5f, -0.5f,  -0.5f,	0.248f, 0.356f,		1.0f,	0.0f,	0.0f, //12
		-0.5f, -0.5f,  0.5f,	0.479f,	0.356f,		1.0f,	0.0f,	0.0f, //13
		-0.5f,  0.5f,  0.5f,	0.479f,	0.642f,		1.0f,	0.0f,	0.0f, //14
		-0.5f,  0.5f,  -0.5f,	0.248f,	0.642f,		1.0f,	0.0f,	0.0f, //15

		//down
		//x		y		z		S		T
		-0.5f, -0.5f,  0.5f,	0.716f, 0.059f,		0.0f,	1.0f,	0.0f, //16
		0.5f,  -0.5f,  0.5f,	0.485f,	0.059f,		0.0f,	1.0f,	0.0f, //17
		 0.5f,  -0.5f,  -0.5f,	0.485f,	0.349f,		0.0f,	1.0f,	0.0f, //18
		-0.5f, -0.5f,  -0.5f,	0.716f,	0.349f,		0.0f,	1.0f,	0.0f, //19

		//up
		 //x		y		z		S		T
		 -0.5f, 0.5f,  0.5f,	0.716f, 0.652f,		0.0f,	-1.0f,	0.0f, //20
		 0.5f,  0.5f,  0.5f,	0.485f,	0.652f,		0.0f,	-1.0f,	0.0f, //21
		  0.5f, 0.5f,  -0.5f,	0.485f,	0.938f,		0.0f,	-1.0f,	0.0f, //22
		 -0.5f, 0.5f,  -0.5f,	0.716f,	0.938f,		0.0f,	-1.0f,	0.0f, //23

	};

	Mesh* dado = new Mesh();
	dado->CreateMesh(cubo_vertices, cubo_indices, 192, 36);
	meshList.push_back(dado);

}


// d8
void CrearDadoPiramide()
{
	unsigned int indices_dado_piramide[] = {
		// front up
		0, 1, 2,
		// right up
		3, 4, 5,
		//left up
		6, 7, 8,
		// back up
		9, 10, 11,
		// front down
		12, 13, 14,
		// right down
		15, 16, 17,
		//left down
		18, 19, 20,
		// back down
		21, 22, 23
	};
	GLfloat vertices_dado_piramide[] = {
		//front up : 4
		//x		y		z		 S		T			NX		NY		NZ
		 0.0f,  0.43f,  0.0f,	0.500f, 0.741f, 	0.0f,	1.0f,	-1.0f,	//0
		-0.5f, -0.5f,  0.5f,	0.265f, 0.504f, 	0.0f,	1.0f,	-1.0f,	//1
		 0.5f, -0.5f,  0.5f,	0.735f, 0.504f,		0.0f,	1.0f,	-1.0f,	//2

		// right up: 2
		//x		y		z		 S		T			NX		NY		NZ
		0.5f, -0.5f, -0.5f,		0.985f, 0.744f, 	1.0f,	1.0f,	0.0f,	//3
		0.0f,  0.43f, 0.0f,		0.510f, 0.744f, 	1.0f,	1.0f,	0.0f,	//0
		0.5f, -0.5f,  0.5f,		0.750f, 0.507f,		1.0f,	1.0f,	0.0f,	//2

		// left up: 6
		//x		y		z		 S		T			NX		NY		NZ
		0.0f,  0.43f,  0.0f,	0.490f, 0.744f, 	-1.0f,	1.0f,	0.0f,	//0
		-0.5f, -0.5f,  0.5f,	0.250f, 0.507f, 	-1.0f,	1.0f,	0.0f,	//1
		-0.5f, -0.5f, -0.5f,	0.015f, 0.744f, 	-1.0f,	1.0f,	0.0f,	//4

		//back up : 8
		//x		y		z		 S		T			NX		NY		NZ
		 0.5f, -0.5f, -0.5f,	0.250f, 0.985f, 	0.0f,	1.0f,	1.0f,	//3
		-0.5f, -0.5f, -0.5f,	0.015f, 0.753f,		0.0f,	1.0f,	1.0f,	//4
		 0.0f,  0.43f,  0.0f,	0.490f, 0.753f, 	0.0f,	1.0f,	1.0f,	//0

		 //front down : 1
		//x		y		z		 S		T			NX		NY		NZ
		 0.0f,  -1.43f, 0.0f,	0.500f, 0.258f, 	0.0f,	-1.0f,	-1.0f,	//5
		-0.5f, -0.5f,   0.5f,	0.265f, 0.495f,  	0.0f,	-1.0f,	-1.0f,	//1
		 0.5f, -0.5f,   0.5f,	0.735f, 0.495f, 	0.0f,	-1.0f,	-1.0f,	//2

		  // right down: 3 
		 //x		y		z		 S		T			NX		NY		NZ
		 0.0f,  -1.43f, 0.0f,	0.510f, 0.255f, 	1.0f,	-1.0f,	0.0f,	//5
		 0.5f, -0.5f,   0.5f,	0.750f, 0.492f,		1.0f,	-1.0f,	0.0f,	//2
		 0.5f, -0.5f,  -0.5f,	0.985f, 0.255f, 	1.0f,	-1.0f,	0.0f,	//3

		 // left down: 7 
		 //x		y		z		 S		T			NX		NY		NZ
		-0.5f, -0.5f,   0.5f,	0.250f, 0.492f, 	-1.0f,	-1.0f,	0.0f,	//1
		-0.5f, -0.5f,  -0.5f,	0.015f, 0.255f,		-1.0f,	-1.0f,	0.0f,	//4
		 0.0f,  -1.43f, 0.0f,	0.490f, 0.255f, 	-1.0f,	-1.0f,	0.0f,	//5

		 //back down : 5
		 //x		y		z		 S		T			NX		NY		NZ
		  0.5f, -0.5f,  -0.5f,	0.250f, 0.008f, 	0.0f,	-1.0f,	1.0f,	//3
		 -0.5f, -0.5f,  -0.5f,	0.015f, 0.249f,		0.0f,	-1.0f,	1.0f,	//4
		  0.0f,  -1.43f, 0.0f,	0.490f, 0.249f, 	0.0f,	-1.0f,	1.0f,	//5

	};
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices_dado_piramide, indices_dado_piramide, 192, 24);
	meshList.push_back(obj1);

}

int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CrearDado();
	CreateShaders();
	CrearDadoPiramide();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	dadoTexture = Texture("Textures/dado_animales.tga");
	dadoTexture.LoadTextureA();
	logofiTexture = Texture("Textures/escudo_fi_color.tga");
	logofiTexture.LoadTextureA();

	d8Texture = Texture("Textures/d8_textura.tga");
	d8Texture.LoadTextureA();

	llantaTexture = Texture("Textures/textura_llanta.tga");
	llantaTexture.LoadTextureA();
	chasisTexture = Texture("Textures/textura_auto.tga");
	chasisTexture.LoadTextureA();
	cofreTexture = Texture("Textures/textura_cofre.tga");
	cofreTexture.LoadTextureA();



	chasisAuto  = Model();
	chasisAuto.LoadModel("Models/Auto/chasis.obj");
	cofreAuto = Model();
	cofreAuto.LoadModel("Models/Auto/cofre.obj");
	llantaAuto = Model();
	llantaAuto.LoadModel("Models/Auto/llanta.obj");

	dado_M = Model();
	dado_M.LoadModel("Models/cuboAnimales.obj");


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
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformColor = shaderList[0].getColorLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		color = glm::vec3(1.0f, 1.0f, 1.0f);//color blanco, multiplica a la información de color de la textura

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		meshList[2]->RenderMesh();


		//Ejercicio 1: Dado de Opengl
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-1.5f, 4.5f, -2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dadoTexture.UseTexture();
		meshList[4]->RenderMesh();

		//Ejercicio 2:Dado importado
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-7.0f, 3.0f, -2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dado_M.RenderModel();

		//Practica
		//Ejercicio 1: Crear un dado de 8 caras y texturizarlo por medio de código
		//dado 8 caras
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(1.5f, 4.5f, -2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		d8Texture.UseTexture();
		meshList[5]->RenderMesh();

		
		/*Ejercicio 2: Importar el modelo de su coche con sus 4 llantas acomodadas
		y tener texturizadas las 4 llantas (diferenciar caucho y rin)  
		*/
		//------------*AUTO*-------------------*
		//chasis
		model = glm::mat4(1.0);
		//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		model = glm::translate(model, glm::vec3(0.0f + mainWindow.getmuevex(), 0.0f, -6.0f));
		modelAuto = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		chasisTexture.UseTexture();
		chasisAuto.RenderModel();
		model = modelAuto;

		//cofre
		model = glm::translate(model, glm::vec3(0.0f, 0.43f, 1.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cofreAuto.RenderModel();
		chasisTexture.UseTexture();
		model = modelAuto;

		//llanta delantera 1
		model = glm::translate(model, glm::vec3(-1.0f, -0.3f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		llantaTexture.UseTexture();
		llantaAuto.RenderModel();
		model = modelAuto;

		//llanta delantera 2
		model = glm::translate(model, glm::vec3(1.0f, -0.3f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		llantaTexture.UseTexture();
		llantaAuto.RenderModel();
		model = modelAuto;

		//llanta trasera 1
		model = glm::translate(model, glm::vec3(-1.0f, -0.3f, -1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		llantaTexture.UseTexture();
		llantaAuto.RenderModel();
		model = modelAuto;

		//llanta trasera 2
		model = glm::translate(model, glm::vec3(1.0f, -0.3f, -1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		llantaTexture.UseTexture();
		llantaAuto.RenderModel();
		model = modelAuto;

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
/*
//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		logofiTexture.UseTexture(); //textura con transparencia o traslucidez
		FIGURA A RENDERIZAR de OpenGL, si es modelo importado no se declara UseTexture
		glDisable(GL_BLEND);
*/