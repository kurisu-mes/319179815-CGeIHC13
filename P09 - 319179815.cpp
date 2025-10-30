/*
Practica 9 - Animacion
Magally Escamilla Suarez
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
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"
//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

//variables para animación
//flecha
float toffsetflechau = 0.0f;
float toffsetflechav = 0.0f;

//numeros
int velCambio = 150; //cada cuando se cambia
int cuentaCambio = 0; //contador para cambio
int cuentaOffset = 0; //posicion para el offset
int cuentaNum = 0; //en que numero vamos
bool cambioDecada = true; //cambiar del 1 al 2
float toffsetnumerou = 0.0f;
float toffsetnumerov = 0.0f;
float toffsetnumerocambiau = 0.0;
float toffsetnumerocambiav = 0.0;
float posicionesOffsetH[] = { 0.0f, 0.25f, 0.5f, 0.75f };

//dragon
bool estadoTiamat = true; //si avanza o retrocede
float posXTiamat = 0.0f; //posicion x
float movXTiamat = 0.01f;//factor de movimiento X
float posYTiamat = 0.0f; //posicion y

float angulovaria = 0.0f;
float dragonavance = 0.0f;

//puertas arco
float posXPuertaR = 0.0f;
float posXPuertaD1 = 0.0f, posZPuertaD1 = 0.0f;
float posXPuertaD2 = 0.0, posZPuertaD2 = 0.0;
float movPuerta = 0.2f;

//DADO
float rotacion1 = 0.0f, rotacion2 = 0.0f;
float altura = 1.0f, desplazamiento = 0.0f;
float posXDado = 0.0f, posYDado = 0.0f, posZDado = 0.0f;
float movDado = 3.0f;
bool numeroAleatorio = false;


//letras
						//		P	 R		O	Y		E	C		T	O      ESP	C		G	 E		I	  H		C	ESP	
float posicionesLetrasX[] = {0.6f, 0.0f, 0.4f, 0.2f, 0.8f, 0.4f, 0.4f, 0.4f, 0.6f, 0.4f, 0.2f, 0.8f, 0.6f, 0.4f, 0.4f, 0.6f};
float posicionesLetrasY[] = {0.4f, 0.2f, 0.4f, 0.0f, 0.8f, 0.8f, 0.2f, 0.4f, 0.0f, 0.8f, 0.6f, 0.8f, 0.6f, 0.6f, 0.8f, 0.0f };
int velCambioLetra = 150; //cada cuando se cambia
int cuentaCambioLetra = 0; //contador para cambio
int pos1 = 0, pos2 = 1, pos3 = 2 , pos4 = 3, pos5 = 4; //en que numero vamos

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture FlechaTexture;
Texture NumerosTexture;
Texture Numero1Texture;
Texture Numero2Texture;
Texture LetrasArcoTexture;
Texture ArcoTexture;
Texture PuertaTexture;
Texture d8Texture;

Model TiamatCuerpo;
Model TiamatAla1;
Model TiamatAla2;

Model Arco;
Model PuertaR;
Model PuertaD1;
Model PuertaD2;


Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;

//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";

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


	unsigned int flechaIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat flechaVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int scoreIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat scoreVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int numeroIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat numeroVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.67f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		0.25f, 0.67f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		0.25f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,			0.0f, -1.0f, 0.0f,

	};

	unsigned int arcoIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat arcoVertices[] = {
		-1.0f, -1.0f, 0.0f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,		0.2f, 0.0f,		0.0f, -1.0f, 0.0f, 
		1.0f, 1.0f, 0.0f,		0.2f, 0.2f,		0.0f, -1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,		0.0f, 0.2f,		0.0f, -1.0f, 0.0f,
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

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	Mesh* obj5 = new Mesh();
	obj5->CreateMesh(flechaVertices, flechaIndices, 32, 6);
	meshList.push_back(obj5);

	Mesh* obj6 = new Mesh();
	obj6->CreateMesh(scoreVertices, scoreIndices, 32, 6);
	meshList.push_back(obj6); // todos los números

	Mesh* obj7 = new Mesh();
	obj7->CreateMesh(numeroVertices, numeroIndices, 32, 6);
	meshList.push_back(obj7); // solo un número

	Mesh* obj8 = new Mesh();
	obj8->CreateMesh(arcoVertices, arcoIndices, 32, 6);
	meshList.push_back(obj8); // letrero cartel

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
		 0.5f, -0.5f,  0.5f,		0.750f, 0.507f,		-1.0f,	1.0f,	0.0f,	//2
		 0.5f, -0.5f, -0.5f,		0.985f, 0.744f, 	-1.0f,	1.0f,	0.0f,	//3
		 0.0f,  0.43f, 0.0f,		0.510f, 0.744f, 	-1.0f,	1.0f,	0.0f,	//0

		 // left up: 6
		 //x		y		z		 S		T			NX		NY		NZ
		 0.0f,  0.43f,  0.0f,	0.490f, 0.744f, 	1.0f,	1.0f,	0.0f,	//0
		 -0.5f, -0.5f,  0.5f,	0.250f, 0.507f, 	1.0f,	1.0f,	0.0f,	//1
		 -0.5f, -0.5f, -0.5f,	0.015f, 0.744f, 	1.0f,	1.0f,	0.0f,	//4

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
		 0.0f,  -1.43f, 0.0f,	0.510f, 0.255f, 	-1.0f,	-1.0f,	0.0f,	//5
		 0.5f, -0.5f,   0.5f,	0.750f, 0.492f,		-1.0f,	-1.0f,	0.0f,	//2
		 0.5f, -0.5f,  -0.5f,	0.985f, 0.255f, 	-1.0f,	-1.0f,	0.0f,	//3

		 // left down: 7 
		 //x		y		z		 S		T			NX		NY		NZ
		-0.5f, -0.5f,   0.5f,	0.250f, 0.492f, 	1.0f,	-1.0f,	0.0f,	//1
		-0.5f, -0.5f,  -0.5f,	0.015f, 0.255f,		1.0f,	-1.0f,	0.0f,	//4
		 0.0f,  -1.43f, 0.0f,	0.490f, 0.255f, 	1.0f,	-1.0f,	0.0f,	//5

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


void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CrearDadoPiramide();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 5.0f, 30.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 0.5f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();
	FlechaTexture = Texture("Textures/flechas.tga");
	FlechaTexture.LoadTextureA();
	NumerosTexture = Texture("Textures/numerosbase.tga");
	NumerosTexture.LoadTextureA();
	Numero1Texture = Texture("Textures/numero1.tga");
	Numero1Texture.LoadTextureA();
	Numero2Texture = Texture("Textures/numero2.tga");
	Numero2Texture.LoadTextureA();

	//tiamat separada
	TiamatCuerpo = Model();
	TiamatCuerpo.LoadModel("Models/CuerpoTiamat.obj");
	TiamatAla1 = Model();
	TiamatAla1.LoadModel("Models/Ala1Tiamat.obj");
	TiamatAla2 = Model();
	TiamatAla2.LoadModel("Models/Ala2Tiamat.obj");

	//ARCO Y PUERTAS
	Arco = Model();
	Arco.LoadModel("Models/stoneArch.obj");
	PuertaR = Model();
	PuertaR.LoadModel("Models/puertaRotatoria.obj");
	PuertaD1 = Model();
	PuertaD1.LoadModel("Models/puertaDesliz1.obj");
	PuertaD2 = Model();
	PuertaD2.LoadModel("Models/puertaDesliz2.obj");

	LetrasArcoTexture = Texture("Textures/megamanFont.tga");
	LetrasArcoTexture.LoadTextureA();
	ArcoTexture = Texture("Textures/StoneArchT.png");
	ArcoTexture.LoadTextureA();
	PuertaTexture = Texture("Textures/Door.png");
	PuertaTexture.LoadTextureA();

	//dado
	d8Texture = Texture("Textures/d8_textura.tga");
	d8Texture.LoadTextureA();

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);

	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		0.0f, 2.5f, 1.5f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;

	//luz fija
	spotLights[1] = SpotLight(0.0f, 0.0f, 1.0f,
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

	glm::vec3 lowerLight(0.0f, 0.0f, 0.0f);

	glm::mat4 model(1.0);
	glm::mat4 modelDrag(1.0);
	glm::mat4 modelArco(1.0);
	glm::mat4 modelaux(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec2 toffset = glm::vec2(0.0f, 0.0f);

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
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		uniformTextureOffset = shaderList[0].getOffsetLocation(); // para la textura con movimiento

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);

		//Reinicializando variables cada ciclo de reloj
		model = glm::mat4(1.0);
		modelDrag = glm::mat4(1.0);
		modelArco = glm::mat4(1.0);
		modelaux = glm::mat4(1.0);
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		toffset = glm::vec2(0.0f, 0.0f);

		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

		//movimiento Dragon X
		if (estadoTiamat) {
			posXTiamat -= movXTiamat;
			if (posXTiamat <= -20.0f)
				estadoTiamat = false;
		}
		else {
			posXTiamat += movXTiamat;
			if (posXTiamat >= 20.0f)
				estadoTiamat = true;
		}

		//movimiento Dragon Y (tambien para alas)
		angulovaria += 3.0f * deltaTime;
		posYTiamat = sin(glm::radians(angulovaria));

		//cuerpoDragon
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(posXTiamat, 7.0f + posYTiamat, 6.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		modelDrag = model;
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		TiamatCuerpo.RenderModel();

		//ala1
		model = modelDrag;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -0.5f));
		model = glm::rotate(model, posYTiamat, glm::vec3(1.0f, 0.0f, 0.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		TiamatAla1.RenderModel();

		//ala2
		model = modelDrag;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.5f));
		model = glm::rotate(model, -posYTiamat, glm::vec3(1.0f, 0.0f, 0.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		TiamatAla2.RenderModel();

		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//movimiento Puertas
		if (mainWindow.getEstadoPuertas()) {
			//cerradas, se abren

			if (posXPuertaD1 > -3.0f) posXPuertaD1 -= movPuerta * deltaTime;
			else if (posZPuertaD1 < 3.5f) posZPuertaD1 += movPuerta * deltaTime;

			if (posXPuertaD2 < 0.5f) posXPuertaD2 += movPuerta * deltaTime;
			if (posZPuertaD2 < 2.0f) posZPuertaD2 += movPuerta * deltaTime;
			if (posXPuertaR > -90.0f) posXPuertaR -= 1.0f * deltaTime;
		}
		else {
			//abiertas, se abren
			if (posZPuertaD1 > 0.0f) posZPuertaD1 -= movPuerta * deltaTime;
			else if (posXPuertaD1 < 0.0f) posXPuertaD1 += movPuerta * deltaTime;

			if (posZPuertaD2 > 0.0f) posZPuertaD2 -= movPuerta * deltaTime;
			if (posXPuertaD2 > 0.0f) posXPuertaD2 -= movPuerta * deltaTime;
			if (posXPuertaR < 0.0f) posXPuertaR += 1.0f * deltaTime;
		}


		//ARCO
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.0f, -2.0f, -10.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelArco = model;
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ArcoTexture.UseTexture();
		Arco.RenderModel();

		//puerta rotatoria
		model = modelArco;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.5f));
		model = glm::rotate(model, posXPuertaR * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PuertaTexture.UseTexture();
		PuertaR.RenderModel();

		//puerta desliz 1
		model = modelArco;
		model = glm::translate(model, glm::vec3(posXPuertaD1 + 0.0f, 0.0f, posZPuertaD1 + 5.5f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PuertaTexture.UseTexture();
		PuertaD1.RenderModel();

		//puerta desliz 2
		model = glm::translate(model, glm::vec3(posXPuertaD2 + 0.0f, 0.0f, posZPuertaD2 + (-3.0f)));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PuertaTexture.UseTexture();
		PuertaD2.RenderModel();

		//cambio de letra
		if (cuentaCambioLetra >= velCambioLetra) {
			pos1++;
			if (pos1 > 15) pos1 = 0;

			pos2++;
			if (pos2 > 15) 	pos2 = 0;

			pos3++;
			if (pos3 > 15) pos3 = 0;

			pos4++;
			if (pos4 > 15) 	pos4 = 0;

			pos5++;
			if (pos5 > 15) 	pos5 = 0;

			cuentaCambioLetra = 0;
		}
		cuentaCambioLetra++;

		//letrero del arco, letra 1
		model = modelArco;
		toffset = glm::vec2(posicionesLetrasX[pos1], posicionesLetrasY[pos1]);
		model = glm::translate(model, glm::vec3(-3.5f, 11.0f, -4.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		LetrasArcoTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[7]->RenderMesh();

		//letrero del arco, letra 2
		toffset = glm::vec2(posicionesLetrasX[pos2], posicionesLetrasY[pos2]);
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, -0.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		LetrasArcoTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[7]->RenderMesh();

		//letrero del arco, letra 3
		toffset = glm::vec2(posicionesLetrasX[pos3], posicionesLetrasY[pos3]);
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		LetrasArcoTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[7]->RenderMesh();

		//letrero del arco, letra 4
		toffset = glm::vec2(posicionesLetrasX[pos4], posicionesLetrasY[pos4]);
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		LetrasArcoTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[7]->RenderMesh();

		//letrero del arco, letra 5
		toffset = glm::vec2(posicionesLetrasX[pos5], posicionesLetrasY[pos5]);
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		LetrasArcoTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[7]->RenderMesh();

		//ANIMACION FLECHAS
		toffsetflechau += 0.005;
		toffsetflechav = 0.000;

		if (toffsetflechau > 1.0)
			toffsetflechau = 0.0;
		toffset = glm::vec2(toffsetflechau, toffsetflechav);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-2.0f, 1.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		FlechaTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[4]->RenderMesh();

		//NUMEROS
		//plano con todos los números
		toffsetnumerou = 0.0;
		toffsetnumerov = 0.0;
		toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-6.0f, 2.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		NumerosTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		//número cambiante 1 - 8
		if (cuentaCambio >= velCambio) {
			cuentaNum++;
			cuentaOffset++;
			//verificar que esté dentro del rango
			if (cuentaOffset >= 4) {
				cuentaOffset = 0;
				if (cuentaNum >= 4)
					toffsetnumerocambiav = -0.33;
				if (cuentaNum >= 8) {
					cuentaNum = 0;
					cambioDecada = !cambioDecada;
					toffsetnumerocambiav = 0.0;
				}
			}
			toffsetnumerocambiau = posicionesOffsetH[cuentaOffset];
			cuentaCambio = 0;
		}
		cuentaCambio++;
		toffset = glm::vec2(toffsetnumerocambiau, toffsetnumerocambiav);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, 10.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		NumerosTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[6]->RenderMesh();

		//cambiar automáticamente entre textura número 1 y número 2
		toffsetnumerou = 0.0;
		toffsetnumerov = 0.0;
		toffset = glm::vec2(toffsetnumerou, toffsetnumerov);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-13.0f, 10.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		// cambiar entre 1 y 2
		if (cambioDecada)
			Numero1Texture.UseTexture();
		else
			Numero2Texture.UseTexture();

		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		//**********EJERCICIO OPCIONAL: ANIMACION DE DADO
		srand(time(NULL));

		if (mainWindow.getEstadoDado()) {
			//Se inclina para un lado o para otro (al azar)
			if (!numeroAleatorio) {
				rotacion1 = (rand() % 181) - 80; //valor de 90 a -90
				rotacion2 = (rand() % 181) - 90; //valor de 90 a -90
				numeroAleatorio = true;
			}
			//Cae mientras se inclina
			if (rotacion1 > 0 && posXDado < rotacion1)	posXDado += movDado * deltaTime; //rotacion positiva
			else if (rotacion1 < 0 && posXDado > rotacion1) posXDado -= movDado * deltaTime; //rotacion negativa

			if (altura > -0.5) altura -= 0.05 * deltaTime;
			else if (desplazamiento < 2.0) {
				desplazamiento += 0.05 * deltaTime;
				if (rotacion2 > 0 && posZDado < rotacion2) posZDado += movDado * deltaTime; //rotacion positiva
				else if (rotacion2 < 0 && posZDado > rotacion2) posZDado -= movDado * deltaTime; //rotacion negativa
			}
		}
		else {
			//reiniciar 
			altura = 1.0f;
			posXDado = 0.0f;
			posYDado = 0.0f;
			posZDado = 0.0f;
			desplazamiento = 0.0f;
			numeroAleatorio = false;
		}

		//dado 8 caras
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(desplazamiento, altura, 0.0f));
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, posXDado * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, posZDado * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, posZDado * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		d8Texture.UseTexture();
		meshList[8]->RenderMesh();

		glDisable(GL_BLEND);
		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
