/* 
Animación por keyframes
La textura del skybox fue conseguida desde la página https ://opengameart.org/content/elyvisions-skyboxes?page=1
y edité en Gimp rotando 90 grados en sentido antihorario la imagen  sp2_up.png para poder ver continuidad.
Fuentes :
	https ://www.khronos.org/opengl/wiki/Keyframe_Animation
	http ://what-when-how.com/wp-content/uploads/2012/07/tmpcd0074_thumb.png
	*/

//PRACTICA 10 - MAGALLY ESCAMILLA
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

//PARA GUARDAR KEYS EN .TXT
#include <fstream>   
#include <sstream>   
#include <iostream>  

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
float movCoche;
float movOffset;
float rotllanta;
float rotllantaOffset;
bool avanza;
float angulovaria = 0.0f;

const char* archivoKeyFrames = "keyframesP10.txt";

//variables para keyframes
float reproduciranimacion = 0, habilitaranimacion, guardoFrame, reinicioFrame, contador = 0;
int cicloX, cicloY, cicloR, cicloTapaY, cicloTapaR, ciclo;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;


Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture FlechaTexture;

Model Kitt_M;
Model Llanta_M;
Model Blackhawk_M;

Model ufoUp;
Model ufoDown;
Texture ufoTexture;

Skybox skybox;

//AVANCES PROYECTO
Model Piso_M;
Texture PisoTexture;

//modelos y texturas principales
Model Angela_Desk;
Model Angela_Chair;
Texture Angela_Desk_Texture;
Texture Angela_Chair_Texture;

Model Ofrenda;
Texture FloresT;
Texture OneSinT;
Texture OfrendaT;

Model PiramideRing;
Model Ring;
Texture PiramideT;
Texture RingT;

//GALERIA
//centro
Model CalendarioDoom;
Model PenachoMan;
Model MegaHawlucha;
Texture CalendarioT;
Texture PenachoT;
Texture MegaHawluchaT;

//alrededores
Model ParedGaleria;
Model ExpositorGaleria;
Model Caballete1;
Model Caballete2;
Texture ParedGalT;
Texture ExpositorT;
Texture CaballeteT;


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

//función para teclado de keyframes 
void inputKeyframes(bool* keys);

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
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
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
	meshList.push_back(obj6);

	Mesh* obj7 = new Mesh();
	obj7->CreateMesh(numeroVertices, numeroIndices, 32, 6);
	meshList.push_back(obj7);

}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}


///////////////////////////////KEYFRAMES/////////////////////


bool animacion = false;

//NEW// Keyframes
float posXavion = 2.0, posYavion = 5.0, posZavion = -3.0;
float	movAvion_x = 0.0f, movAvion_y = 0.0f;
float giroAvion = 0;
float movYtapa = 0.0f, giroTapa = 0.0f;

#define MAX_FRAMES 100 //Número de cuadros máximos
int i_max_steps = 75; //Número de pasos entre cuadros para interpolación, a mayor número , más lento será el movimiento
int i_curr_steps = 0;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float movAvion_x;		//Variable para PosicionX
	float movAvion_y;		//Variable para PosicionY
	float movAvion_xInc;		//Variable para IncrementoX
	float movAvion_yInc;		//Variable para IncrementoY
	float giroAvion;		//Variable para GiroAvion
	float giroAvionInc;		//Variable para IncrementoGiroAvion

	//NUEVAS VARIABLES
	float movYtapa;		//Variable para movimiento vertical de la tapa
	float giroTapa;		//Variable para giro de la tapa
	float movTapa_Inc;		//Variable para IncrementoTapa
	float giroTapaInc;		//Variable para IncrementoGiroTapa

}FRAME;

FRAME KeyFrame[MAX_FRAMES]; //numero de frames a almacenar
int FrameIndex = 0;			//El número de cuadros guardados actualmente desde 0 para no sobreescribir
bool play = false; //reproducir
int playIndex = 0; //a partir de cual

void saveFrame(void){ //tecla L
 //guarda frame actual a partir de index

	printf("frameindex %d\n", FrameIndex);
	KeyFrame[FrameIndex].movAvion_x = movAvion_x;
	KeyFrame[FrameIndex].movAvion_y = movAvion_y;
	KeyFrame[FrameIndex].giroAvion = giroAvion;
	KeyFrame[FrameIndex].movYtapa = movYtapa;
	KeyFrame[FrameIndex].giroTapa = giroTapa;
	
	//para guardar las keyframes se escribe en el archivo .txt
	// std::ios::app genera un append, es decir, lo agrega al final del archivo.
	std::ofstream archivo(archivoKeyFrames, std::ios::app);

	if (archivo.is_open()) {
		archivo << movAvion_x << " " << movAvion_y << " " << giroAvion << " " << movYtapa << " " << giroTapa << "\n";
		archivo.close();
		printf("El frame %d se ha guardado en el archivo %s\n", FrameIndex, archivoKeyFrames);
	}
	else {
		// std::cerr es stream para errores
		std::cerr << "No se pudo abrir el archivo " << archivoKeyFrames << "\n";
	}
	
	FrameIndex++;
}

//carga las keyframes del archivo
void loadKeyframes(){

	std::ifstream file(archivoKeyFrames); // se abre el .txt para su lectura
	std::string line; //guarda en un string cada linea

	//Verifica si se puede abrir el archivo antes de intentar leerlo
	if (!file.is_open()) {
		printf("No se encontro el archivo %s.\n", archivoKeyFrames);
		return; 
	}

	//mientras haya lineas en el archivo, se lee el documento
	while (std::getline(file, line) && FrameIndex < MAX_FRAMES)	{
		std::istringstream coord(line);
		float movX, movY, giro, movTapa, giroT;

		// lee las 5 coordenadas
		if (coord >> movX >> movY >> giro >> movTapa >> giroT) {
			KeyFrame[FrameIndex].movAvion_x = movX;
			KeyFrame[FrameIndex].movAvion_y = movY;
			KeyFrame[FrameIndex].giroAvion = giro;
			KeyFrame[FrameIndex].movYtapa = movTapa;
			KeyFrame[FrameIndex].giroTapa = giroT;
			FrameIndex++; 
		}
		else {
			std::cerr << "Error al leer la linea del archivo: " << line << "\n";
		}
	}

	file.close();
}

//reinicia a posicion inicial
void resetElements(void) //Tecla 0
{

	movAvion_x = KeyFrame[0].movAvion_x;
	movAvion_y = KeyFrame[0].movAvion_y;
	giroAvion = KeyFrame[0].giroAvion;
	movYtapa = KeyFrame[0].movYtapa;
	giroTapa = KeyFrame[0].giroTapa;
}

//agrega frames entre keys
void interpolation(void)
{
	KeyFrame[playIndex].movAvion_xInc = (KeyFrame[playIndex + 1].movAvion_x - KeyFrame[playIndex].movAvion_x) / i_max_steps;
	KeyFrame[playIndex].movAvion_yInc = (KeyFrame[playIndex + 1].movAvion_y - KeyFrame[playIndex].movAvion_y) / i_max_steps;
	KeyFrame[playIndex].giroAvionInc = (KeyFrame[playIndex + 1].giroAvion - KeyFrame[playIndex].giroAvion) / i_max_steps;
	//nuevas variables
	KeyFrame[playIndex].movTapa_Inc = (KeyFrame[playIndex + 1].movYtapa - KeyFrame[playIndex].movYtapa) / i_max_steps;
	KeyFrame[playIndex].giroTapaInc = (KeyFrame[playIndex + 1].giroTapa - KeyFrame[playIndex].giroTapa) / i_max_steps;

}


void animate(void){
	//Movimiento del objeto con barra espaciadora
	if (play)
	{
		if (i_curr_steps >= i_max_steps) //fin de animación entre frames?
		{
			playIndex++;
			printf("playindex : %d\n", playIndex);
			if (playIndex > FrameIndex - 2)	//Fin de toda la animación con último frame?
			{
				printf("Frame index= %d\n", FrameIndex);
				printf("termino la animacion\n");
				playIndex = 0;
				play = false;
			}
			else //Interpolación del próximo cuadro
			{
				
				i_curr_steps = 0; //Resetea contador
				//Interpolar
				interpolation();
			}
		}
		else
		{
			//Dibujar Animación
			movAvion_x += KeyFrame[playIndex].movAvion_xInc ;
			movAvion_y += KeyFrame[playIndex].movAvion_yInc ;
			giroAvion += KeyFrame[playIndex].giroAvionInc;
			//nuevas variables
			movYtapa += KeyFrame[playIndex].movTapa_Inc;
			giroTapa += KeyFrame[playIndex].giroTapaInc;
			i_curr_steps++;
		}

	}
}

///////////////* FIN KEYFRAMES*////////////////////////////



int main()
{
	mainWindow = Window(1024, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.5f, 0.5f);

	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();


	Kitt_M = Model();
	Kitt_M.LoadModel("Models/kitt_optimizado.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/llanta_optimizada.obj");
	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");
	
	ufoUp = Model();
	ufoUp.LoadModel("Models/wilyUP.obj");
	ufoDown = Model();
	ufoDown.LoadModel("Models/wilyDWN.obj");
	ufoTexture = Texture("Textures/wilyUfoT.png");
	ufoTexture.LoadTextureA();

	//**************PROYECTO****************

	//terreno
	Piso_M = Model();
	Piso_M.LoadModel("Models/Piso.obj");
	PisoTexture = Texture("Textures/textura piso.png");
	PisoTexture.LoadTextureA();

	//recepcion
	Angela_Desk = Model();
	Angela_Desk.LoadModel("Models/escritorio.obj");
	Angela_Desk_Texture = Texture("Textures/Textura_Desk.png");
	Angela_Desk_Texture.LoadTextureA();
	Angela_Chair = Model();
	Angela_Chair.LoadModel("Models/sillaAngela.obj");
	Angela_Chair_Texture = Texture("Textures/Silla_Texture.png");
	Angela_Chair_Texture.LoadTextureA();


	//ofrenda
	Ofrenda = Model();
	Ofrenda.LoadModel("Models/ofrenda.obj");
	FloresT = Texture("Textures/FloresCempasuchilTextura.png");
	FloresT.LoadTextureA();
	OfrendaT = Texture("Textures/BakeOfrenda.png");
	OfrendaT.LoadTextureA();
	OneSinT = Texture("Textures/OneSinTextura.png");
	OneSinT.LoadTextureA();


	//piramide y ring
	PiramideRing = Model();
	PiramideRing.LoadModel("Models/piramide.obj");
	PiramideT = Texture("Textures/PiramideOfrenda.png");
	PiramideT.LoadTextureA();
	Ring = Model();
	Ring.LoadModel("Models/ring.obj");
	RingT = Texture("Textures/RingBake.png");
	RingT.LoadTextureA();

	//GALERIA
	//centro
	CalendarioDoom = Model();
	CalendarioDoom.LoadModel("Models/DoomsdayCalendar.obj");
	CalendarioT = Texture("Textures/DoomsdayCalT.png");
	CalendarioT.LoadTextureA();

	PenachoMan = Model();
	PenachoMan.LoadModel("Models/PenachoMan.obj");
	PenachoT = Texture("Textures/PenachoT.png");
	PenachoT.LoadTextureA();

	MegaHawlucha = Model();
	MegaHawlucha.LoadModel("Models/MegaHawlucha.obj");
	MegaHawluchaT = Texture("Textures/Estatua.png");
	MegaHawluchaT.LoadTextureA();

	//alrededores
	ParedGaleria = Model();
	ParedGaleria.LoadModel("Models/BaseGal1.obj");
	ParedGalT = Texture("Textures/BaseGaleria1T.png");
	ParedGalT.LoadTextureA();

	ExpositorGaleria = Model();
	ExpositorGaleria.LoadModel("Models/BaseGal2.obj");
	ExpositorT = Texture("Textures/BaseGaleria2T.png");
	ExpositorT.LoadTextureA();

	Caballete1 = Model();
	Caballete1.LoadModel("Models/CaballeteGal1.obj");
	Caballete2 = Model();
	Caballete2.LoadModel("Models/CaballeteGal2.obj");
	CaballeteT = Texture("Textures/caballete1T.png");
	CaballeteT.LoadTextureA();

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/sp2_rt.png");
	skyboxFaces.push_back("Textures/Skybox/sp2_lf.png");
	skyboxFaces.push_back("Textures/Skybox/sp2_dn.png");
	skyboxFaces.push_back("Textures/Skybox/sp2_up.png");
	skyboxFaces.push_back("Textures/Skybox/sp2_bk.png");
	skyboxFaces.push_back("Textures/Skybox/sp2_ft.png");

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

	//luz ufo
	spotLights[2] = SpotLight(1.0f, 1.0f, 0.0f,
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;


	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset=0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	
	movCoche = 0.0f;
	movOffset = 0.01f;
	rotllanta = 0.0f;
	rotllantaOffset = 10.0f;
	glm::vec3 posUfo = glm::vec3(3.0f, 0.0f, 3.0f);
	glm::vec3 positionluz = glm::vec3(0);


	
	//Se agregan nuevos frames 
	loadKeyframes();

	printf("\nTeclas para uso de Keyframes:\nA.- Presionar barra espaciadora para reproducir animacion.\n");
	printf("B.- Presionar 0 para volver a habilitar reproduccion de la animacion\n");
	printf("C.- Presiona L para guardar frame\nD.- Presiona P para habilitar guardar nuevo frame\n");
	printf("\nTeclas para crear animaciones : \n1.- Presiona 1 para mover en X positivo\n");
	printf("2.- Presiona 2 para mover en X negativo\n3.- Presiona 3 para mover en Y positivo\n");
	printf("4.- Presiona 4 para mover en Y negativo\n5.- Presiona 5 para rotar el UFO\n");
	printf("6.- Presiona 6 para alzar la tapa\n7.- Presiona 7 para bajar la tapa\n");
	printf("8.- Presiona 8 para rotar la tapa\n9.- Presiona R para habilitar mover nuevamente");


	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec2 toffset = glm::vec2(0.0f, 0.0f);
	glm::vec3 lowerLight = glm::vec3(0.0f,0.0f,0.0f);

	glm::mat4 modelPiso(1.0);
	glm::mat4 elementos(1.0);
	glm::mat4 elementoLocal(1.0);
	glm::mat4 caballetes(1.0);

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		angulovaria += 0.5f*deltaTime;

		if (movCoche < 30.0f)
		{
			movCoche -= movOffset * deltaTime;
			//printf("avanza%f \n ",movCoche);
		}
		rotllanta += rotllantaOffset * deltaTime;


		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		//-------Para Keyframes
		inputKeyframes(mainWindow.getsKeys());
		animate();

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
		uniformTextureOffset = shaderList[0].getOffsetLocation();

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
		modelaux = glm::mat4(1.0);
		modelPiso = glm::mat4(1.0);
		caballetes = glm::mat4(1.0);
		elementos = glm::mat4(1.0);
		elementoLocal = glm::mat4(1.0);
		toffset = glm::vec2(0.0f, 0.0f);
		color = glm::vec3(1.0f, 1.0f, 1.0f);

		
		/*
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
		*/
		


		//------------------PROYECTO-----------------------------------
		//Piso modelado con Blender
		modelPiso = glm::mat4(1.0);
		modelPiso = glm::translate(modelPiso, glm::vec3(0.0f, -2.0f, 0.0f));
		modelPiso = glm::scale(modelPiso, glm::vec3(0.7f, 0.7f, 0.7f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelPiso));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		PisoTexture.UseTexture();
		Piso_M.RenderModel();



		//Recepcion
		elementos = glm::mat4(1.0);
		elementos = glm::translate(elementos, glm::vec3(-15.0f, -2.0f, 18.0f));
		elementos = glm::scale(elementos, glm::vec3(1.3f, 1.3f, 1.3f));
		elementos = glm::rotate(elementos, 30 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		elementoLocal = elementos;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		Angela_Desk_Texture.UseTexture();
		Angela_Desk.RenderModel();

		elementos = glm::translate(elementos, glm::vec3(0.0f, 0.0f, -2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		Angela_Chair_Texture.UseTexture();
		Angela_Chair.RenderModel();

		//Ofrenda
		elementos = glm::mat4(1.0);
		elementos = glm::translate(elementos, glm::vec3(-5.0f, -2.0f, -35.0f));//Siempre se tiene que tener -1 en Y para estar sobre el piso
		elementos = glm::scale(elementos, glm::vec3(1.5f, 1.5f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		FloresT.UseTexture();
		OneSinT.UseTexture();
		OfrendaT.UseTexture();
		Ofrenda.RenderModel();

		//Ubicacion central de la piramide
		elementos = glm::mat4(1.0);
		elementos = glm::translate(elementos, glm::vec3(125.5f, -2.0f, -25.0f));//Siempre se tiene que tener -1 en Y para estar sobre el piso
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		PiramideT.UseTexture();
		PiramideRing.RenderModel();

		//Ring de lucha
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		RingT.UseTexture();
		Ring.RenderModel();
		//Ubicaciones centrales de la galeria
		//------------------------------------------ 
		//MegaHawlucha
		elementos = glm::mat4(1.0);
		elementos = glm::translate(elementos, glm::vec3(20.0f, -2.0f, 18.0f));
		elementoLocal = elementos;
		//model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		MegaHawluchaT.UseTexture();
		MegaHawlucha.RenderModel();

		//Calendario
		elementos = elementoLocal;
		elementos = glm::translate(elementos, glm::vec3(15.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		CalendarioT.UseTexture();
		CalendarioDoom.RenderModel();

		//Penacho (con base)
		elementos = elementoLocal;
		elementos = glm::translate(elementos, glm::vec3(-10.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		PenachoT.UseTexture();
		PenachoMan.RenderModel();

		//Limite abajo, poner paredes
		elementos = glm::mat4(1.0);
		elementos = glm::translate(elementos, glm::vec3(40.0f, -2.0f, 35.0f));//Siempre se tiene que tener -1 en Y para estar sobre el piso
		elementoLocal = elementos;
		//elementos = glm::scale(elementos, glm::vec3(0.5f, 0.5f, 0.5f));
		elementos = glm::rotate(elementos, 160 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		ParedGalT.UseTexture();
		ParedGaleria.RenderModel();

		elementos = elementoLocal;
		elementos = glm::translate(elementos, glm::vec3(10.0f, 0.0f, 0.0f));//Siempre se tiene que tener -1 en Y para estar sobre el piso
		elementos = glm::rotate(elementos, 200 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		ParedGalT.UseTexture();
		ParedGaleria.RenderModel();

		elementos = elementoLocal;
		elementos = glm::translate(elementos, glm::vec3(-10.0f, 0.0f, 0.0f));//Siempre se tiene que tener -1 en Y para estar sobre el piso
		elementos = glm::rotate(elementos, 200 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		ParedGalT.UseTexture();
		ParedGaleria.RenderModel();


		//Limite arriba. FILA DE CABALLETES.
			//1
			caballetes = glm::mat4(1.0);
			caballetes = glm::translate(caballetes, glm::vec3(10.0f, -2.0f, 3.0f));
			modelaux = caballetes;
			caballetes = glm::scale(caballetes, glm::vec3(1.0f, 1.3f, 1.0f));
			caballetes = glm::rotate(caballetes, 15 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(caballetes));
			CaballeteT.UseTexture();
			//textura de cuadro en cuestión
			Caballete1.RenderModel();

			//2
			caballetes = modelaux;
			caballetes = glm::translate(caballetes, glm::vec3(5.0f, 0.0f, 0.0f));
			modelaux = caballetes;
			caballetes = glm::scale(caballetes, glm::vec3(1.5f, 1.5f, 1.5f));
			caballetes = glm::rotate(caballetes, -20 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(caballetes));
			CaballeteT.UseTexture();
			//textura de cuadro en cuestión
			Caballete2.RenderModel();

			//3
			caballetes = modelaux;
			caballetes = glm::translate(caballetes, glm::vec3(3.0f, 0.0f, 0.0f));
			modelaux = caballetes;
			caballetes = glm::scale(caballetes, glm::vec3(0.8f, 0.8f, 0.8f));
			caballetes = glm::rotate(caballetes, 10 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(caballetes));
			CaballeteT.UseTexture();
			//textura de cuadro en cuestión
			Caballete2.RenderModel();

			//4
			caballetes = modelaux;
			caballetes = glm::translate(caballetes, glm::vec3(4.0f, 0.0f, 0.0f));
			modelaux = caballetes;
			caballetes = glm::rotate(caballetes, 15 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(caballetes));
			CaballeteT.UseTexture();
			//textura de cuadro en cuestión
			Caballete1.RenderModel();

			//5
			caballetes = modelaux;
			caballetes = glm::translate(caballetes, glm::vec3(3.0f, 0.0f, 0.0f));
			modelaux = caballetes;
			caballetes = glm::scale(caballetes, glm::vec3(1.5f, 1.5f, 1.5f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(caballetes));
			CaballeteT.UseTexture();
			//textura de cuadro en cuestión
			Caballete1.RenderModel();

			//6
			caballetes = modelaux;
			caballetes = glm::translate(caballetes, glm::vec3(6.0f, 0.0f, 0.0f));
			modelaux = caballetes;
			caballetes = glm::scale(caballetes, glm::vec3(1.0f, 1.3f, 1.5f));
			caballetes = glm::rotate(caballetes, -10 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(caballetes));
			CaballeteT.UseTexture();
			//textura de cuadro en cuestión
			Caballete2.RenderModel();

			//7
			caballetes = modelaux;
			caballetes = glm::translate(caballetes, glm::vec3(3.0f, 0.0f, 0.0f));
			modelaux = caballetes;
			caballetes = glm::scale(caballetes, glm::vec3(0.8f, 0.8f, 0.8f));
			caballetes = glm::rotate(caballetes, 10 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(caballetes));
			CaballeteT.UseTexture();
			//textura de cuadro en cuestión
			Caballete1.RenderModel();

			//8
			caballetes = modelaux;
			caballetes = glm::translate(caballetes, glm::vec3(4.0f, 0.0f, 0.0f));
			modelaux = caballetes;
			caballetes = glm::rotate(caballetes, 15 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(caballetes));
			CaballeteT.UseTexture();
			//textura de cuadro en cuestión
			Caballete2.RenderModel();

			//9
			caballetes = modelaux;
			caballetes = glm::translate(caballetes, glm::vec3(5.0f, 0.0f, 0.0f));
			modelaux = caballetes;
			caballetes = glm::scale(caballetes, glm::vec3(1.0f, 1.4f, 1.0f));
			caballetes = glm::rotate(caballetes, -20 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(caballetes));
			CaballeteT.UseTexture();
			//textura de cuadro en cuestión
			Caballete1.RenderModel();

		//----------------------------------------------------------------------------

		//Instancia del coche 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(movCoche-50.0f, 0.5f, -2.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Kitt_M.RenderModel();

		//Llanta delantera izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(7.0f, -0.5f, 8.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		color = glm::vec3(0.5f, 0.5f, 0.5f);//llanta con color gris
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//Llanta trasera izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(15.5f, -0.5f, 8.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//Llanta delantera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(7.0f, -0.5f, 1.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//Llanta trasera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(15.5f, -0.5f, 1.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//UFO parte inferior, nodo padre
		model = glm::mat4(1.0);
		posUfo = glm::vec3(posXavion + movAvion_x, posYavion + movAvion_y, posZavion);
		model = glm::translate(model, posUfo);
		modelaux = model;
		model = glm::rotate(model, giroAvion * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ufoTexture.UseTexture();
		ufoDown.RenderModel();
		

		//UFO parte superior. cambia rotación y posición vertical
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f + movYtapa, 0.0f));
		model = glm::rotate(model, giroTapa * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ufoTexture.UseTexture();
		ufoUp.RenderModel();
		
		//luz
		positionluz = glm::vec3(model[3]);
		spotLights[2].SetPos(positionluz);
		
		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}


void inputKeyframes(bool* keys){
	//reproduce
	if (keys[GLFW_KEY_SPACE]) 
	{
		if (reproduciranimacion < 1) //bandera de control
		{
			if (play == false && (FrameIndex > 1))
			{
				resetElements();
				//First Interpolation				
				interpolation();
				play = true;
				playIndex = 0;
				i_curr_steps = 0;
				reproduciranimacion++;
				printf("\n presiona 0 para habilitar reproducir de nuevo la animación'\n");
				habilitaranimacion = 0;

			}
			else
			{
				play = false;

			}
		}
	}
	
	//reincia
	if (keys[GLFW_KEY_0]) 
	{
		if (habilitaranimacion < 1 && reproduciranimacion>0) //banderas
		{
			printf("Ya puedes reproducir de nuevo la animación con la tecla de barra espaciadora'\n");
			reproduciranimacion = 0;
		}
	}

	//guardar
	if (keys[GLFW_KEY_L])
	{
		if (guardoFrame < 1) //bandera
		{
			saveFrame();
			printf("movAvion_x es: %f\n", movAvion_x);
			printf("movAvion_y es: %f\n", movAvion_y);
			printf("giroAvion es: %f\n", giroAvion);
			printf("presiona P para habilitar guardar otro frame'\n");
			guardoFrame++;
			reinicioFrame = 0;
		}
	}
	
	//autoriza guardado
	if (keys[GLFW_KEY_P]) //para una sola vez, es press && GLFW_PRESS
	{
		if (reinicioFrame < 1) //bandera
		{
			guardoFrame = 0;
			reinicioFrame++;
			printf("Ya puedes guardar otro frame presionando la tecla L'\n");
		}
	}

	//mueve +1 en x
	if (keys[GLFW_KEY_1]){
		if (cicloX < 1)
		{
			movAvion_x += 1.0f;
			printf("\n movAvion_x es: %f\n", movAvion_x);
			cicloX++;
			cicloR = 0;
			printf("\n Presiona la tecla R para poder habilitar la variable\n");
		}

	}

	//mueve -1 en x
	if (keys[GLFW_KEY_2]) {
		if (cicloX < 1)
		{
			movAvion_x -= 1.0f;
			printf("\n movAvion_x es: %f\n", movAvion_x);
			cicloX++;
			cicloR = 0;
			printf("\n Presiona la tecla R para poder habilitar la variable\n");
		}

	}

	//mueve +1 en y
	if (keys[GLFW_KEY_3]){
		if (cicloY < 1)
		{
			movAvion_y += 1.0f;
			printf("\n movAvion_y es: %f\n", movAvion_y);
			cicloY++;
			cicloR = 0;
			printf("\n Presiona la tecla R para poder habilitar la variable\n");
		}

	}

	//mueve -1 en y
	if (keys[GLFW_KEY_4]){
		if (cicloY < 1)
		{
			movAvion_y -= 1.0f;
			printf("\n movAvion_y es: %f\n", movAvion_y);
			cicloY++;
			cicloR = 0;
			printf("\n Presiona la tecla R para poder habilitar la variable\n");
		}

	}
	
	//rotacion de 60
	if (keys[GLFW_KEY_5]) {
		if (ciclo < 1)
		{
			giroAvion += 60.0f;
			printf("\n giroAvion es: %f\n", giroAvion);
			ciclo++;
			cicloR = 0;
			printf("\n Presiona la tecla R para poder habilitar la variable\n");
		}

	}

	//mueve tapa +0.5 en y
	if (keys[GLFW_KEY_6]) {
		if (cicloTapaY < 1)
		{
			movYtapa += 0.5f;
			printf("\n movYtapa es: %f\n", movYtapa);
			cicloTapaY++;
			cicloR = 0;
			printf("\n Presiona la tecla R para poder habilitar la variable\n");
		}

	}

	//mueve -0.5 en y
	if (keys[GLFW_KEY_7]) {
		if (cicloTapaY < 1)
		{
			movYtapa -= 0.5f;
			printf("\n movYtapa es: %f\n", movYtapa);
			cicloTapaY++;
			cicloR = 0;
			printf("\n Presiona la tecla R para poder habilitar la variable\n");
		}

	}

	//rotacion de tapa de 30
	if (keys[GLFW_KEY_8]) {
		if (cicloTapaR < 1)
		{
			giroTapa += 30.0f;
			printf("\n giroTapa es: %f\n", giroTapa);
			cicloTapaR++;
			cicloR = 0;
			printf("\n Presiona la tecla R para poder habilitar la variable\n");
		}

	}

	//autoriza cambios
	//se puede mover en x, y, rotar a la vez, pero solo una direccion
	if (keys[GLFW_KEY_R]) 
	{
		if (cicloR < 1) //bandera
		{
			cicloX = 0;
			cicloY = 0;
			ciclo = 0;
			cicloTapaY = 0;
			cicloTapaR = 0;
			cicloR++;
			printf("\n Ya puedes modificar tu variable presionando la tecla 1\n");
		}
	}

}
