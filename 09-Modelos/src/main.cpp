#define _USE_MATH_DEFINES
#include <cmath>
//glew include
#include <GL/glew.h>

//std includes
#include <string>
#include <iostream>

//glfw include
#include <GLFW/glfw3.h>

// program include
#include "Headers/TimeManager.h"

// Shader include
#include "Headers/Shader.h"

// Model geometric includes
#include "Headers/Sphere.h"
#include "Headers/Cylinder.h"
#include "Headers/Box.h"
#include "Headers/FirstPersonCamera.h"

//GLM include
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Headers/Texture.h"

//INCLUDE LOADER MODEL CLASS 
#include "Headers/Model.h"

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

int screenWidth;
int screenHeight;

GLFWwindow *window;

Shader shader;
//Shader de texturizado
Shader shaderTexture;
//Shader con iluminacion solo color
Shader shaderColorLighting;
//Shader con iluminacion y textura
Shader shaderTextureLighting;
// Descomentar
//Shader con materiales
Shader shaderMaterialLighting;
// Descomentar
//Shader con skybox
Shader shaderSkybox;
// Shader con multiples luces
Shader shaderMulLighting;

std::shared_ptr<FirstPersonCamera> camera(new FirstPersonCamera());

Sphere sphere1(20, 20);
Sphere sphere2(20, 20);
Sphere sphere3(20, 20);
Sphere sphereLamp(20, 20);
//BOB 
Box box4, box5, box6, box7;
Sphere sphere4(20, 20); //ojo bob blanco
Sphere sphere5(20, 20); //ojo parte negra bob
Sphere sphere6(20, 20); //ojo azul bob
Sphere sphere7(20, 20); //boca bob
Sphere sphere8(20, 20); 
Sphere sphere9(20, 20); 
Cylinder cylinder6(20, 20, 0.5, 0.5), cylinder5(20, 20, 0.5, 0.5), cylinder4(20, 20, 0.5, 0.5); //BRAZO DERECHO BOB
Box box8; //dientes bob
//
Sphere skyboxSphere(20, 20);
Cylinder cylinder1(20, 20, 0.5, 0.5);
Cylinder cylinder2(20, 20, 0.5, 0.5);
Cylinder cylinderMaterials(20, 20, 0.5, 0.5);
Box boxMaterials;
Box box1;
Box box2;
Box box3;
Cylinder buroHabit(20, 20, 0.5, 0.5);
Cylinder jacuzi(20, 20, 0.5, 0.5);
Box boxMaterials2;
Box casaExterior, casaExterior2, casaExterior3, casaExterior4; //paredes de la casa exterior
Box mosaicoBanio, paredBanio;//BANIO1
Box pisoHabitacion, paredHabitacion, muebleHabitacion;
Box cocinaPared, cocinaPiso, cocina, encimera;
Box salaPared, salaPiso, sillon;
Box paredExt, paredExt2;
Box pisoExt;
Box puerta;
Box ventana;
/*R2D2*/
Sphere cabeza(20, 20);
Sphere ojoR2D2(20, 20);
Sphere articulacion(20, 20);
Sphere  articulacion3(20, 20); //articulacion 1 es del hombro y la 3 de la muneca derecha
Sphere articulacion2(20, 20);
Sphere articulacion4(20, 20); //articulacion 2 es del hombro y la 4 de la muneca izquierda
Cylinder cuerpoTronco(20, 20, 0.5, 0.5);
Cylinder brazoDer(20, 20, 0.5, 0.5);
Cylinder brazoIzq(20, 20, 0.5, 0.5);
Cylinder brazoDetalle(20, 20, 0.5, 0.5);
Box baseDer, baseIzq; //cajas en las articulaciones de las munecas
Sphere articulacion5(20, 20);
Box baseCentro;


//Models complex instances
Model modelRock;
Model modelRailRoad;
Model modelAirCraft;
Model modelHongo;
Model modelReloj;
Model modelSilla;
Model modelMesita;
Model modelTaburete;
GLuint textureID1, textureID2, textureID3, textureID4;
//												 paredes exterior, mosaicoBanio,paredBanio, pisoHabit, paredHabit
GLuint textureID5, textureID6, textureID7, textureID9, textureID8;
//		cocinaPared,cocinaPiso,  marmolSala,	pisoSala, paredArbusto, paredRoca, pisoExterior, puertaPrinci
GLuint textureID10, textureID11, textureID12, textureID13, textureID14, textureID15, textureID16, textureID17;
//		puertas		ventana		cocina estufa, comida		horno estufa cama base	cama colchon, piscina
GLuint textureID18, textureID19, textureID20, textureID21, textureID22, textureID23, textureID24, textureID25;
// sillon			encimera
GLuint textureID26, textureID27;
GLuint skyboxTextureID;

GLenum types[6] = {
GL_TEXTURE_CUBE_MAP_POSITIVE_X,
GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
GL_TEXTURE_CUBE_MAP_NEGATIVE_Z };

std::string fileNames[6] = { "../Textures/sb_iceflow/iceflow_ft.tga", //FT es el float asociado al indice de numeracion
		"../Textures/sb_iceflow/iceflow_bk.tga",
		"../Textures/sb_iceflow/iceflow_up.tga",
		"../Textures/sb_iceflow/iceflow_dn.tga",
		"../Textures/sb_iceflow/iceflow_rt.tga",
		"../Textures/sb_iceflow/iceflow_lf.tga" };


bool exitApp = false;
int lastMousePosX, offsetX = 0;
int lastMousePosY, offsetY = 0;

float rot0 = 0.0, dz = 0.0;

float rot1 = 0.0, rot2 = 0.0, rot3 = 0.0, rot4 = 0.0;
float rotCabeza = 0.0, rotArt1 = 0.0, rotArt2 = 0.0, rotArt3 = 0.0, rotArt4 = 0.0; //Rotar R2D2
bool sentido = true;

double deltaTime;

// Se definen todos las funciones.
void reshapeCallback(GLFWwindow *Window, int widthRes, int heightRes);
void keyCallback(GLFWwindow *window, int key, int scancode, int action,
	int mode);
void mouseCallback(GLFWwindow *window, double xpos, double ypos);
void mouseButtonCallback(GLFWwindow *window, int button, int state, int mod);
void init(int width, int height, std::string strTitle, bool bFullScreen);
void destroy();
bool processInput(bool continueApplication = true);

// Implementacion de todas las funciones.
void init(int width, int height, std::string strTitle, bool bFullScreen) {

	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		exit(-1);
	}

	screenWidth = width;
	screenHeight = height;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (bFullScreen)
		window = glfwCreateWindow(width, height, strTitle.c_str(),
			glfwGetPrimaryMonitor(), nullptr);
	else
		window = glfwCreateWindow(width, height, strTitle.c_str(), nullptr,
			nullptr);

	if (window == nullptr) {
		std::cerr
			<< "Error to create GLFW window, you can try download the last version of your video card that support OpenGL 3.3+"
			<< std::endl;
		destroy();
		exit(-1);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);

	glfwSetWindowSizeCallback(window, reshapeCallback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Init glew
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		std::cerr << "Failed to initialize glew" << std::endl;
		exit(-1);
	}

	glViewport(0, 0, screenWidth, screenHeight);
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	shader.initialize("../Shaders/colorShader.vs", "../Shaders/colorShader.fs");
	shaderTexture.initialize("../Shaders/texturizado_res.vs",
		"../Shaders/texturizado_res.fs");
	shaderColorLighting.initialize("../Shaders/iluminacion_color_res.vs",
		"../Shaders/iluminacion_color_res.fs");
	shaderTextureLighting.initialize("../Shaders/iluminacion_texture_res.vs",
		"../Shaders/iluminacion_texture_res.fs");
	shaderMaterialLighting.initialize("../Shaders/iluminacion_material.vs",
		"../Shaders/iluminacion_material_res.fs");
	shaderSkybox.initialize("../Shaders/cubeTexture.vs",
		"../Shaders/cubeTexture.fs");
	shaderMulLighting.initialize("../Shaders/iluminacion_texture_res.vs",
		"../Shaders/multipleLights.fs");

	// Inicializar los buffers VAO, VBO, EBO
	sphere1.init();
	// Método setter que colocar el apuntador al shader
	sphere1.setShader(&shaderColorLighting);
	//Setter para poner el color de la geometria
	sphere1.setColor(glm::vec4(0.3, 0.3, 1.0, 1.0));

	// Inicializar los buffers VAO, VBO, EBO
	sphere2.init();
	sphere2.setShader(&shaderColorLighting);
	sphere2.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

	sphereLamp.init();
	sphereLamp.setShader(&shader);
	sphereLamp.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

	cylinder1.init();
	cylinder1.setShader(&shaderColorLighting);
	cylinder1.setColor(glm::vec4(0.3, 0.3, 1.0, 1.0));

	cylinder2.init();
	cylinder2.setShader(&shaderMulLighting); //solo se aplica a los objetos con tectura

	cylinderMaterials.init();
	cylinderMaterials.setShader(&shaderMaterialLighting);
	cylinderMaterials.setColor(glm::vec4(0.3, 0.3, 1.0, 1.0));

	boxMaterials.init();
	boxMaterials.setShader(&shaderMaterialLighting);

	// Inicializacion de la esfera del skybox
	skyboxSphere.init();
	skyboxSphere.setShader(&shaderSkybox);
	skyboxSphere.setScale(glm::vec3(20.0f, 20.0f, 20.0f));

	box1.init();
	// Settea el shader a utilizar
	box1.setShader(&shaderMulLighting); //modificamos esto
	box1.setColor(glm::vec4(1.0, 1.0, 0.0, 1.0));

	box2.init();
	box2.setShader(&shaderMulLighting);

	sphere3.init();
	sphere3.setShader(&shaderMulLighting);

	box3.init();
	box3.setShader(&shaderMulLighting);

	modelRock.loadModel("../models/rock/rock.obj");
	modelRock.setShader(&shaderMulLighting);

	modelRailRoad.loadModel("../models/railroad/railroad_track.obj");
	modelRailRoad.setShader(&shaderMulLighting);

	modelAirCraft.loadModel("../models/AirCraft_obj/E 45 Aircraft_obj.obj");
	modelAirCraft.setShader(&shaderMulLighting);

	modelHongo.loadModel("../models/daga/02_-_Knife.obj");
	modelHongo.setShader(&shaderMulLighting);

	modelReloj.loadModel("../models/reloj/Clock_obj.obj");
	modelReloj.setShader(&shaderMulLighting);

	modelMesita.loadModel("../models/Wood_Table/Wood_Table.obj");
	modelMesita.setShader(&shaderMulLighting);

	modelTaburete.loadModel("../models/24-taburetka/Taburetka.obj");
	modelTaburete.setShader(&shaderMulLighting);

	//PAREDES EXTERIORES DE LA CASA 
	casaExterior.init();
	casaExterior.setShader(&shaderMulLighting);
	casaExterior2.init();
	casaExterior2.setShader(&shaderMulLighting);
	casaExterior3.init();
	casaExterior3.setShader(&shaderMulLighting);
	casaExterior4.init();
	casaExterior4.setShader(&shaderMulLighting);
	//BANIO1
	mosaicoBanio.init();
	mosaicoBanio.setShader(&shaderMulLighting);
	paredBanio.init();
	paredBanio.setShader(&shaderMulLighting);
	//HABITACION IZQUIERDA CERCA BANIO
	pisoHabitacion.init();
	pisoHabitacion.setShader(&shaderMulLighting);
	paredHabitacion.init();
	paredHabitacion.setShader(&shaderMulLighting);
	//COCINA
	cocinaPared.init();
	cocinaPared.setShader(&shaderMulLighting);
	cocinaPiso.init();
	cocinaPiso.setShader(&shaderMulLighting);
	encimera.init();
	encimera.setShader(&shaderMulLighting);
	//SALA
	salaPared.init();
	salaPared.setShader(&shaderMulLighting);
	salaPiso.init();
	salaPiso.setShader(&shaderMulLighting);
	sillon.init();
	sillon.setShader(&shaderMulLighting);
	//CUARTO EXTERIOR
	paredExt.init();
	paredExt.setShader(&shaderMulLighting);
	paredExt2.init();
	paredExt2.setShader(&shaderMulLighting);
	pisoExt.init();
	pisoExt.setShader(&shaderMulLighting);
	puerta.init();
	puerta.setShader(&shaderMulLighting);
	//VENTANA
	ventana.init();
	ventana.setShader(&shaderMulLighting);
	//cocina muebles
	cocina.init();
	cocina.setShader(&shaderMulLighting);
	//habitacion muebles
	muebleHabitacion.init();
	muebleHabitacion.setShader(&shaderMulLighting);
	buroHabit.init();
	buroHabit.setShader(&shaderMulLighting);
	//jardin
	jacuzi.init();
	jacuzi.setShader(&shaderMulLighting);
	//TERMINA CASITA
	/*=======================*/
	/*INICIA BOB*/
	box4.init();
	box4.setShader(&shaderMulLighting); //CUERPO BOB
	box5.init();
	box5.setShader(&shader);
	box5.setColor(glm::vec4(1.0, 1.0, 1.0, 0.0)); //CAMISA BLANCA
	box6.init();
	box6.setShader(&shader);
	box6.setColor(glm::vec4(0.5, 0.25, 0.0, 0.0)); //PANTALONES DEL BOB
	cylinder4.init();
	cylinder4.setShader(&shaderMulLighting); //piernas
	cylinder5.init();
	cylinder5.setShader(&shader);
	cylinder5.setColor(glm::vec4(1.0, 0.1, 0.0, 0.0)); //CORBATA
	box7.init();
	box7.setShader(&shader);
	box7.setColor(glm::vec4(0.0, 0.0, 0.0, 0.0));//ZAPATOS
	sphere4.init();
	sphere4.setShader(&shader);
	sphere4.setColor(glm::vec4(1.0, 1.0, 1.0, 0.0));
	sphere5.init();
	sphere5.setShader(&shader);
	sphere5.setColor(glm::vec4(0.0, 0.0, 0.0, 0.0));
	sphere6.init();
	sphere6.setShader(&shader);
	sphere6.setColor(glm::vec4(0.0f, 0.1f, 1.0f, 1.0f)); //OJOS
	sphere7.init();
	sphere7.setShader(&shader);
	sphere7.setColor(glm::vec4(0.9f, 0.0f, 0.5f, 0.0f));//BOCA
	box8.init();
	box8.setShader(&shader);
	box8.setColor(glm::vec4(1.0, 1.0, 1.0, 0.0)); //DIENTES
	cylinder6.init();
	cylinder6.setShader(&shaderMulLighting);
	sphere8.init();
	sphere8.setShader(&shaderMulLighting);
	sphere9.init();
	sphere9.setShader(&shaderMulLighting);
	/*INICIA R2D2*/
	/*R2D2*/
	cabeza.init();
	cabeza.setShader(&shader);
	cabeza.setColor(glm::vec4(0.7, 0.7, 1.0, 1.0));

	ojoR2D2.init();
	ojoR2D2.setShader(&shader);
	ojoR2D2.setColor(glm::vec4(0.0, 0.0, 0.0, 0.0));

	cuerpoTronco.init();
	cuerpoTronco.setShader(&shader);
	cuerpoTronco.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

	articulacion.init(); /*articulacion arriba derecha*/
	articulacion.setShader(&shader);
	articulacion.setColor(glm::vec4(0.8, 0.8, 0.8, 0.8));

	brazoDer.init();
	brazoDer.setShader(&shader);
	brazoDer.setColor(glm::vec4(0.9, 0.9, 0.9, 1.0));

	brazoDetalle.init();
	brazoDetalle.setShader(&shader);
	brazoDetalle.setColor(glm::vec4(0.0, 0.0, 1.0, 1.0));

	articulacion3.init(); /*articulacion muneca derecha*/
	articulacion3.setShader(&shader);
	articulacion3.setColor(glm::vec4(0.8, 0.8, 0.8, 0.8));

	articulacion2.init(); /*articulacion arriba izquierda*/
	articulacion2.setShader(&shader);
	articulacion2.setColor(glm::vec4(0.8, 0.8, 0.8, 0.8));

	articulacion4.init(); /*articulacion arriba izquierda*/
	articulacion4.setShader(&shader);
	articulacion4.setColor(glm::vec4(0.8, 0.8, 0.8, 0.8));

	brazoIzq.init();
	brazoIzq.setShader(&shader);
	brazoIzq.setColor(glm::vec4(0.9, 0.9, 0.9, 1.0));

	articulacion5.init(); /*articulacion arriba izquierda*/
	articulacion5.setShader(&shader);
	articulacion5.setColor(glm::vec4(0.8, 0.8, 0.8, 0.8));

	baseDer.init();
	baseDer.setShader(&shader);
	baseDer.setColor(glm::vec4(0.45, 0.45, 1.0, 0.8));

	baseIzq.init();
	baseIzq.setShader(&shader);
	baseIzq.setColor(glm::vec4(0.45, 0.45, 1.0, 0.8));

	baseCentro.init();
	baseCentro.setShader(&shader);
	baseCentro.setColor(glm::vec4(0.45, 0.45, 1.0, 0.8));

	camera->setPosition(glm::vec3(0.0, 0.0, 4.0));

	// Descomentar
	// Definimos el tamanio de la imagen
	int imageWidth, imageHeight;
	// Definiendo la textura a utilizar
	Texture texture1("../Textures/sponge.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	FIBITMAP *bitmap = texture1.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	unsigned char *data = texture1.convertToData(bitmap, imageWidth,
		imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureID1);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureID1);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	texture1.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture texture2("../Textures/water.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = texture2.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = texture2.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureID2);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureID2);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	texture2.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture texture3("../Textures/goku.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	// Voltear la imagen
	bitmap = texture3.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = texture3.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureID3);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureID3);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	texture3.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture texture4("../Textures/texturaLadrillos.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	// Voltear la imagen
	bitmap = texture4.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = texture4.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureID4);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureID4);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	texture4.freeImage(bitmap);
	//TEXTURAS PARA LA CASA
	// TEXTURA DE PAREDES EXTERIORES
	Texture texture5("../Textures/maderaExt.jpg");
	bitmap = texture5.loadImage();
	data = texture5.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID5);
	glBindTexture(GL_TEXTURE_2D, textureID5);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture5.freeImage(bitmap);

	// TEXTURA DE PISO BANIO
	Texture texture6("../Textures/mosaicoPisoBanio.jpg");
	bitmap = texture6.loadImage();
	data = texture6.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID6);
	glBindTexture(GL_TEXTURE_2D, textureID6);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture6.freeImage(bitmap);
	//TEXTURA PARED BANIO
	Texture texture7("../Textures/marmolParedBanio.jpg");
	bitmap = texture7.loadImage();
	data = texture7.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID7);
	glBindTexture(GL_TEXTURE_2D, textureID7);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture7.freeImage(bitmap);
	//TEXTURA PARED HABITACION
	Texture texture8("../Textures/tapizHabitacion.jpg");
	bitmap = texture8.loadImage();
	data = texture8.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID8);
	glBindTexture(GL_TEXTURE_2D, textureID8);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture8.freeImage(bitmap);
	//PISO HABITACION
	Texture texture9("../Textures/maderaPisoHabitacion.jpg");
	bitmap = texture9.loadImage();
	data = texture9.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID9);
	glBindTexture(GL_TEXTURE_2D, textureID9);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture9.freeImage(bitmap);
	//TEXTURA PARED COCINA 
	Texture texture10("../Textures/paredCocina.jpg");
	bitmap = texture10.loadImage();
	data = texture10.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID10);
	glBindTexture(GL_TEXTURE_2D, textureID10);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture10.freeImage(bitmap);
	//PISO COCINA
	Texture texture11("../Textures/pisoCocina.jpg");
	bitmap = texture11.loadImage();
	data = texture11.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID11);
	glBindTexture(GL_TEXTURE_2D, textureID11);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture11.freeImage(bitmap);
	//TEXTURA 12 PARED SALA
	Texture texture12("../Textures/marmolSala.jpg");
	bitmap = texture12.loadImage();
	data = texture12.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID12);
	glBindTexture(GL_TEXTURE_2D, textureID12);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture12.freeImage(bitmap);
	//TEXTURA 13 PISO SALA
	Texture texture13("../Textures/pisoSala.jpg");
	bitmap = texture13.loadImage();
	data = texture13.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID13);
	glBindTexture(GL_TEXTURE_2D, textureID13);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture13.freeImage(bitmap);
	//TEXTURA 14 EXTERIOR
	Texture texture14("../Textures/paredJardin.jpg");
	bitmap = texture14.loadImage();
	data = texture14.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID14);
	glBindTexture(GL_TEXTURE_2D, textureID14);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture14.freeImage(bitmap);
	//TEXTURA 15 
	Texture texture15("../Textures/paredExt.jpg");
	bitmap = texture15.loadImage();
	data = texture15.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID15);
	glBindTexture(GL_TEXTURE_2D, textureID15);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture15.freeImage(bitmap);
	//TEXTURA 16 PISO PATIO
	Texture texture16("../Textures/pisoExt.jpg");
	bitmap = texture16.loadImage();
	data = texture16.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID16);
	glBindTexture(GL_TEXTURE_2D, textureID16);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture16.freeImage(bitmap);
	//TEXTURA 17 PUERTA PRINCIPAL
	Texture texture17("../Textures/puertaPrincipal.jpg");
	bitmap = texture17.loadImage();
	data = texture17.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID17);
	glBindTexture(GL_TEXTURE_2D, textureID17);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture17.freeImage(bitmap);
	//TEXTURA 18 PUERTA PRINCIPAL
	Texture texture18("../Textures/puerta.jpg");
	bitmap = texture18.loadImage();
	data = texture18.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID18);
	glBindTexture(GL_TEXTURE_2D, textureID18);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture18.freeImage(bitmap);
	//TEXTURA 19 PARA VENTANA
	Texture texture19("../Textures/ventana.png");
	bitmap = texture19.loadImage();
	data = texture19.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID19);
	glBindTexture(GL_TEXTURE_2D, textureID19);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture19.freeImage(bitmap);
	//TEXTURA 20
	Texture texture20("../Textures/estufa.jpg");
	bitmap = texture20.loadImage();
	data = texture20.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID20);
	glBindTexture(GL_TEXTURE_2D, textureID20);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture20.freeImage(bitmap);
	//COMIDA
	Texture texture21("../Textures/comida.png");
	bitmap = texture21.loadImage();
	data = texture21.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID21);
	glBindTexture(GL_TEXTURE_2D, textureID21);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture21.freeImage(bitmap);

	//HORNO ESTUFA
	Texture texture22("../Textures/horno.jpg");
	bitmap = texture22.loadImage();
	data = texture22.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID22);
	glBindTexture(GL_TEXTURE_2D, textureID22);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture22.freeImage(bitmap);
	//textura cama
	Texture texture23("../Textures/cama.jpg");
	bitmap = texture23.loadImage();
	data = texture23.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID23);
	glBindTexture(GL_TEXTURE_2D, textureID23);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture23.freeImage(bitmap);
	//textura cama colcha
	Texture texture24("../Textures/camaColcha.jpg");
	bitmap = texture24.loadImage();
	data = texture24.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID24);
	glBindTexture(GL_TEXTURE_2D, textureID24);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture24.freeImage(bitmap);
	//TEXTURA 25 JACUZZI
	Texture texture25("../Textures/water.jpg");
	bitmap = texture25.loadImage();
	data = texture25.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID25);
	glBindTexture(GL_TEXTURE_2D, textureID25);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture25.freeImage(bitmap);
	//TEXTURA PARA EL SILLON
	Texture texture26("../Textures/sillon.jpg");
	bitmap = texture26.loadImage();
	data = texture26.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID26);
	glBindTexture(GL_TEXTURE_2D, textureID26);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture26.freeImage(bitmap);
	//TEXTURA PARA EL SILLON
	Texture texture27("../Textures/metal2.jpg");
	bitmap = texture27.loadImage();
	data = texture27.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID27);
	glBindTexture(GL_TEXTURE_2D, textureID27);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture27.freeImage(bitmap);
	//FINALIZA TEXTURA PARA LA CASA

	// Carga de texturas para el skybox
	Texture skyboxTexture = Texture("");
	glGenTextures(1, &skyboxTextureID);
	// Tipo de textura CUBE MAP
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTextureID);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	for (int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(types); i++) {
		skyboxTexture = Texture(fileNames[i]);
		FIBITMAP *bitmap = skyboxTexture.loadImage(true);
		unsigned char *data = skyboxTexture.convertToData(bitmap, imageWidth,
			imageHeight);
		if (data) {
			glTexImage2D(types[i], 0, GL_RGBA, imageWidth, imageHeight, 0,
				GL_BGRA, GL_UNSIGNED_BYTE, data);
		}
		else
			std::cout << "Failed to load texture" << std::endl;
		skyboxTexture.freeImage(bitmap);
	}

}

void destroy() {
	glfwDestroyWindow(window);
	glfwTerminate();
	// --------- IMPORTANTE ----------
	// Eliminar los shader y buffers creados.

	// Destrucción de los VAO, EBO, VBO
	sphere1.destroy();
	cylinder1.destroy();
	box1.destroy();

	shader.destroy();
}

void reshapeCallback(GLFWwindow *Window, int widthRes, int heightRes) {
	screenWidth = widthRes;
	screenHeight = heightRes;
	glViewport(0, 0, widthRes, heightRes);
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action,
	int mode) {
	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_ESCAPE:
			exitApp = true;
			break;
		}
	}
}

void mouseCallback(GLFWwindow *window, double xpos, double ypos) {
	offsetX = xpos - lastMousePosX;
	offsetY = ypos - lastMousePosY;
	lastMousePosX = xpos;
	lastMousePosY = ypos;
}

void mouseButtonCallback(GLFWwindow *window, int button, int state, int mod) {
	if (state == GLFW_PRESS) {
		switch (button) {
		case GLFW_MOUSE_BUTTON_RIGHT:
			std::cout << "lastMousePos.y:" << lastMousePosY << std::endl;
			break;
		case GLFW_MOUSE_BUTTON_LEFT:
			std::cout << "lastMousePos.x:" << lastMousePosX << std::endl;
			break;
		case GLFW_MOUSE_BUTTON_MIDDLE:
			std::cout << "lastMousePos.x:" << lastMousePosX << std::endl;
			std::cout << "lastMousePos.y:" << lastMousePosY << std::endl;
			break;
		}
	}
}

bool processInput(bool continueApplication) {
	if (exitApp || glfwWindowShouldClose(window) != 0) {
		return false;
	}

	TimeManager::Instance().CalculateFrameRate(false);
	deltaTime = TimeManager::Instance().DeltaTime;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera->moveFrontCamera(true, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera->moveFrontCamera(false, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera->moveRightCamera(false, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera->moveRightCamera(true, deltaTime);
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		camera->mouseMoveCamera(offsetX, offsetY, 0.01);
	offsetX = 0;
	offsetY = 0;

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		sentido = false;

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && sentido)
		rot1 += 0.001;
	else if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && !sentido)
		rot1 -= 0.001;
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS
		&& glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		rot2 += 0.001;
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS
		&& glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
		rot2 -= 0.001;
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS && sentido)
		rot3 += 0.001;
	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS && sentido)
		rot4 += 0.001;

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		rot0 = 0.0001;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		rot0 = -0.0001;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		dz = 0.0001;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		dz = -0.0001;

	sentido = true;

	glfwPollEvents();
	return continueApplication;
}

void applicationLoop() {
	bool psi = true;

	glm::mat4 model = glm::mat4(1.0f);
	float offX = 0.0;
	float angle = 0.0;
	float ratio = 5.0;
	while (psi) {
		psi = processInput(true);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 projection = glm::perspective(glm::radians(45.0f),
			(float)screenWidth / (float)screenHeight, 0.01f, 100.0f);
		glm::mat4 view = camera->getViewMatrix();

		// Settea la matriz de vista y projection al shader con solo color
		shader.setMatrix4("projection", 1, false, glm::value_ptr(projection));
		shader.setMatrix4("view", 1, false, glm::value_ptr(view));
		// Settea la matriz de vista y projection al shader con solo textura
		shaderTexture.setMatrix4("projection", 1, false,
			glm::value_ptr(projection));
		shaderTexture.setMatrix4("view", 1, false, glm::value_ptr(view));

		// Settea la matriz de vista y projection al shader con iluminacion solo color
		shaderColorLighting.setMatrix4("projection", 1, false,
			glm::value_ptr(projection));
		shaderColorLighting.setMatrix4("view", 1, false, glm::value_ptr(view));

		// Settea la matriz de vista y projection al shader con iluminacion con textura
		shaderTextureLighting.setMatrix4("projection", 1, false,
			glm::value_ptr(projection));
		shaderTextureLighting.setMatrix4("view", 1, false,
			glm::value_ptr(view));

		// Settea la matriz de vista y projection al shader con iluminacion con material
		shaderMaterialLighting.setMatrix4("projection", 1, false,
			glm::value_ptr(projection));
		shaderMaterialLighting.setMatrix4("view", 1, false,
			glm::value_ptr(view));

		// Settea la matriz de vista y projection al shader con skybox
		shaderSkybox.setMatrix4("projection", 1, false,
			glm::value_ptr(projection));
		shaderSkybox.setMatrix4("view", 1, false,
			glm::value_ptr(glm::mat4(glm::mat3(view))));
		// Settea la matriz de vista y projection al shader con multiples luces
		shaderMulLighting.setMatrix4("projection", 1, false,
			glm::value_ptr(projection));
		shaderMulLighting.setMatrix4("view", 1, false,
			glm::value_ptr(view));

		// Propiedades de la luz para objetos con color
		shaderColorLighting.setVectorFloat3("viewPos",
			glm::value_ptr(camera->getPosition()));
		shaderColorLighting.setVectorFloat3("light.ambient",
			glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		shaderColorLighting.setVectorFloat3("light.diffuse",
			glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		shaderColorLighting.setVectorFloat3("light.specular",
			glm::value_ptr(glm::vec3(0.9, 0.0, 0.0)));

		// Propiedades de la luz para objetos con textura
		shaderTextureLighting.setVectorFloat3("viewPos",
			glm::value_ptr(camera->getPosition()));
		shaderTextureLighting.setVectorFloat3("light.ambient",
			glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		shaderTextureLighting.setVectorFloat3("light.diffuse",
			glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		shaderTextureLighting.setVectorFloat3("light.specular",
			glm::value_ptr(glm::vec3(0.9, 0.0, 0.0)));

		// Propiedades de la luz para objetos con textura
		shaderMaterialLighting.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
		shaderMaterialLighting.setVectorFloat3("light.ambient", glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		shaderMaterialLighting.setVectorFloat3("light.diffuse", glm::value_ptr(glm::vec3(0.5, 0.5, 0.5)));
		shaderMaterialLighting.setVectorFloat3("light.specular", glm::value_ptr(glm::vec3(0.9, 0.9, 0.9)));

		// Propiedades de la luz para objetos con multiples luces
		shaderMulLighting.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));//aqio le movimos para bajar la intensidad de la luz
		shaderMulLighting.setVectorFloat3("directionalLight.light.ambient", glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		shaderMulLighting.setVectorFloat3("directionalLight.light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.4, 0.4)));
		shaderMulLighting.setVectorFloat3("directionalLight.light.specular", glm::value_ptr(glm::vec3(0.7, 0.7, 0.7)));
		shaderMulLighting.setVectorFloat3("directionalLight.direction", glm::value_ptr(glm::vec3(-1.0, 0.0, 0.0))); //sentido del vector
		//ESTO ES PARA LA LUZ SPOTLIGHT LAMPARAAAAAAAAAS
		shaderMulLighting.setInt("spotLightCount", 1);
		shaderMulLighting.setVectorFloat3("spotLights[0].position", glm::value_ptr(camera->getPosition()));
		shaderMulLighting.setVectorFloat3("spotLights[0].direction", glm::value_ptr(camera->getFront()));
		shaderMulLighting.setVectorFloat3("spotLights[0].light.ambient", glm::value_ptr(glm::vec3(0.01, 0.01, 0.01)));
		shaderMulLighting.setVectorFloat3("spotLights[0].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.4, 0.4)));
		shaderMulLighting.setVectorFloat3("spotLights[0].light.specular", glm::value_ptr(glm::vec3(0.6, 0.6, 0.6)));
		shaderMulLighting.setFloat("spotLights[0].cutOff", cos(glm::radians(12.5)));
		shaderMulLighting.setFloat("spotLights[0].outerCutOff", cos(glm::radians(15.0)));
		shaderMulLighting.setFloat("spotLights[0].constant", 1.0);
		shaderMulLighting.setFloat("spotLights[0].linear", 0.3);
		shaderMulLighting.setFloat("spotLights[0].quadratic", 0.001);

		glm::mat4 lightModelmatrix = glm::rotate(glm::mat4(1.0f), angle,
			glm::vec3(1.0f, 0.0f, 0.0f));
		lightModelmatrix = glm::translate(lightModelmatrix,
			glm::vec3(0.0f, 0.0f, -ratio));
		// Posicion luz para objetos con color
		shaderColorLighting.setVectorFloat3("light.position",
			glm::value_ptr(
				glm::vec4(
					lightModelmatrix
					* glm::vec4(0.0, 0.0, 0.0, 1.0))));

		// Posicion luz para objetos con textura
		shaderTextureLighting.setVectorFloat3("light.position",
			glm::value_ptr(
				glm::vec4(
					lightModelmatrix
					* glm::vec4(0.0, 0.0, 0.0, 1.0))));

		// Posicion luz para objetos con materiales
		shaderMaterialLighting.setVectorFloat3("light.position",
			glm::value_ptr(
				glm::vec4(
					lightModelmatrix
					* glm::vec4(0.0, 0.0, 0.0, 1.0))));
		sphereLamp.render(lightModelmatrix);

		model = glm::translate(model, glm::vec3(0, 0, dz));
		model = glm::rotate(model, rot0, glm::vec3(0, 1, 0));
		//box1.enableWireMode();
		// Usamos la textura ID 1
		glBindTexture(GL_TEXTURE_2D, textureID1);
		box1.render(glm::scale(model, glm::vec3(1.0, 1.0, 0.1)));
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		// Articulacion 1
		glm::mat4 j1 = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));
		j1 = glm::rotate(j1, rot1, glm::vec3(0, 0, 1));
		j1 = glm::rotate(j1, rot2, glm::vec3(0, 1, 0));
		sphere1.enableWireMode();
		sphere1.render(glm::scale(j1, glm::vec3(0.1, 0.1, 0.1)));

		// Hueso 1
		glm::mat4 l1 = glm::translate(j1, glm::vec3(0.25f, 0.0, 0.0));
		l1 = glm::rotate(l1, glm::radians(90.0f), glm::vec3(0, 0, 1.0));
		//cylinder1.enableWireMode();
		cylinder1.render(glm::scale(l1, glm::vec3(0.1, 0.5, 0.1)));

		// Articulacion 2
		glm::mat4 j2 = glm::translate(j1, glm::vec3(0.5, 0.0f, 0.0f));
		j2 = glm::rotate(j2, rot3, glm::vec3(0.0, 0.0, 1.0));
		j2 = glm::rotate(j2, rot4, glm::vec3(1.0, 0.0, 0.0));
		sphere1.enableWireMode();
		sphere1.render(glm::scale(j2, glm::vec3(0.1, 0.1, 0.1)));

		// Hueso 2
		glm::mat4 l2 = glm::translate(j2, glm::vec3(0.25, 0.0, 0.0));
		l2 = glm::rotate(l2, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
		//cylinder1.enableWireMode();
		cylinder1.render(glm::scale(l2, glm::vec3(0.1, 0.5, 0.1)));

		// Ojo
		glm::mat4 ojo = glm::translate(model, glm::vec3(0.25, 0.25, 0.05));
		//sphere1.enableWireMode();
		sphere1.render(glm::scale(ojo, glm::vec3(0.2, 0.2, 0.1)));

		glm::mat4 ojo2 = glm::translate(model, glm::vec3(-0.25, 0.25, 0.05));
		//sphere2.enableWireMode();
		sphere2.render(glm::scale(ojo2, glm::vec3(0.2, 0.2, 0.1)));

		glm::mat4 modelAgua = glm::mat4(1.0);
		modelAgua = glm::translate(modelAgua, glm::vec3(0.0, -3.0, 0.0));
		modelAgua = glm::scale(modelAgua, glm::vec3(5.0, 0.01, 5.0));
		// Se activa la textura del agua
		glBindTexture(GL_TEXTURE_2D, textureID2);
		//cambiamos el shader con multiples luches
		shaderMulLighting.setFloat("offsetX", offX);
		box2.render(modelAgua);
		glBindTexture(GL_TEXTURE_2D, 0);
		shaderMulLighting.setFloat("offsetX", 0);

		glm::mat4 modelSphere = glm::mat4(1.0);
		modelSphere = glm::translate(modelSphere, glm::vec3(3.0, 0.0, 0.0));
		glBindTexture(GL_TEXTURE_2D, textureID3);
		sphere3.render(modelSphere);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 modelCylinder = glm::mat4(1.0);
		modelCylinder = glm::translate(modelCylinder,
			glm::vec3(-3.0, 0.0, 0.0));
		// Envolvente desde el indice 0, el tamanio es 20 * 20 * 6
		// Se usa la textura 1 ( Bon sponja)
		glBindTexture(GL_TEXTURE_2D, textureID1);
		cylinder2.render(0, cylinder2.getSlices() * cylinder2.getStacks() * 6,
			modelCylinder);
		// Tapa Superior desde el indice : 20 * 20 * 6, el tamanio de indices es 20 * 3
		// Se usa la textura 2 ( Agua )
		glBindTexture(GL_TEXTURE_2D, textureID2);
		cylinder2.render(cylinder2.getSlices() * cylinder2.getStacks() * 6,
			cylinder2.getSlices() * 3, modelCylinder);
		// Tapa inferior desde el indice : 20 * 20 * 6 + 20 * 3, el tamanio de indices es 20 * 3
		// Se usa la textura 3 ( Goku )
		glBindTexture(GL_TEXTURE_2D, textureID3);
		cylinder2.render(
			cylinder2.getSlices() * cylinder2.getStacks() * 6
			+ cylinder2.getSlices() * 3, cylinder2.getSlices() * 3,
			modelCylinder);
		glBindTexture(GL_TEXTURE_2D, 0);

		// Render del cubo con textura de ladrillos y con repeticion en x
		glm::mat4 cubeTextureModel = glm::mat4(1.0);
		cubeTextureModel = glm::translate(cubeTextureModel,
			glm::vec3(3.0, 2.0, 3.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(cubeTextureModel);
		glBindTexture(GL_TEXTURE_2D, 0);

		// Render del cyindro con materiales
		glm::mat4 cylinderMaterialModel = glm::mat4(1.0);
		cylinderMaterialModel = glm::translate(cylinderMaterialModel, glm::vec3(3.0, 2.0, -3.0));
		shaderMaterialLighting.setVectorFloat3("material.ambient", glm::value_ptr(glm::vec3(0.61424f, 0.04136f, 0.04136f)));
		shaderMaterialLighting.setVectorFloat3("material.diffuse", glm::value_ptr(glm::vec3(0.61424f, 0.04136f, 0.04136f)));
		shaderMaterialLighting.setVectorFloat3("material.specular", glm::value_ptr(glm::vec3(0.727811f, 0.626959f, 0.626959f)));
		shaderMaterialLighting.setFloat("material.shininess", 76.8f);
		cylinderMaterials.render(cylinderMaterialModel);

		glm::mat4 boxMaterialModel = glm::mat4(1.0f);
		boxMaterialModel = glm::translate(boxMaterialModel, glm::vec3(-3.0, 2.0, -3.0));
		shaderMaterialLighting.setVectorFloat3("material.ambient", glm::value_ptr(glm::vec3(0.61424f, 0.04136f, 0.04136f)));
		shaderMaterialLighting.setVectorFloat3("material.diffuse", glm::value_ptr(glm::vec3(0.61424f, 0.04136f, 0.04136f)));
		shaderMaterialLighting.setVectorFloat3("material.specular", glm::value_ptr(glm::vec3(0.727811f, 0.626959f, 0.626959f)));
		shaderMaterialLighting.setFloat("material.shininess", 76.8f);
		boxMaterials.render(boxMaterialModel);

		//Models complex render
		glm::mat4 matrixModelRock = glm::mat4(1.0);
		matrixModelRock = glm::translate(matrixModelRock, glm::vec3(-3.0, 0.0, 4.0));
		modelRock.render(matrixModelRock);
		//Forze to enable the unit texture to 0 always -------------IMPORTANTE
		glActiveTexture(GL_TEXTURE0);
		//esto es para las vias del tren
		glm::mat4 matrixModelRailRoad = glm::mat4(1.0);
		matrixModelRailRoad = glm::translate(matrixModelRailRoad, glm::vec3(3.0, 0.0, 4.0));
		modelRailRoad.render(matrixModelRailRoad);
		//Forze to enable the unit texture to 0 always -------------IMPORTANTE
		glActiveTexture(GL_TEXTURE0);
		//OTRO MODELO
		glm::mat4 matrixAirCraft = glm::mat4(1.0);
		matrixAirCraft = glm::translate(matrixAirCraft, glm::vec3(4.0, -4.0, 7.0));
		modelAirCraft.render(matrixAirCraft);
		glActiveTexture(GL_TEXTURE0);

		glm::mat4 matrixHongo = glm::mat4(1.0);
		matrixHongo = glm::translate(matrixHongo, glm::vec3(-5.0, -4.0, -5.0));
		matrixHongo = glm::scale(matrixHongo, glm::vec3(0.5, 0.5, 0.5));
		modelHongo.render(matrixHongo);
		glActiveTexture(GL_TEXTURE0);
		
		//CASITA
		model = glm::translate(model, glm::vec3(0, 0, dz));
		model = glm::rotate(model, rot0, glm::vec3(0, 1, 0));
		//PARED IZQUIERDA
		glm::mat4 modelCasa = glm::mat4(1.0);
		modelCasa = glm::translate(model, glm::vec3(-10.0, 0.0, 15.0));
		glBindTexture(GL_TEXTURE_2D, textureID5);
		glm::mat4 modelCasaIzq = glm::translate(modelCasa, glm::vec3(0.0, 0.0, 0.0));
		modelCasaIzq = glm::translate(modelCasa, glm::vec3(0.0, 0.0, -5.25));
		shaderMulLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(5.0, 3.0)));
		casaExterior.render(glm::scale(modelCasaIzq, glm::vec3(0.01, 3.0, 4.5)));
		modelCasaIzq = glm::translate(modelCasa, glm::vec3(0.0, 0.0, 1.75));
		shaderMulLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(4.0, 3.0)));
		casaExterior.render(glm::scale(modelCasaIzq, glm::vec3(0.01, 3.0, 3.5)));
		modelCasaIzq = glm::translate(modelCasa, glm::vec3(0.0, 0.0, 7.0));
		shaderMulLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(1.0, 3.0)));
		casaExterior.render(glm::scale(modelCasaIzq, glm::vec3(0.01, 3.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		shaderMulLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(0.0, 0.0)));
		//PARED DE ATRAS
		glm::mat4 modelCasa2 = glm::translate(modelCasa, glm::vec3(7.5, 0.0, -7.5));
		glBindTexture(GL_TEXTURE_2D, textureID5);
		casaExterior2.render(glm::scale(modelCasa2, glm::vec3(15.0, 3.0, 0.01)));
		glBindTexture(GL_TEXTURE_2D, 0);
		//PARED DERECHA
		glm::mat4 modelCasa3 = glm::translate(modelCasa2, glm::vec3(7.5, 0.0, 7.5));
		glBindTexture(GL_TEXTURE_2D, textureID5);
		glm::mat4 modelCasaDer = glm::translate(modelCasa2, glm::vec3(7.5, 0.0, 7.5));
		modelCasaDer = glm::translate(modelCasaDer, glm::vec3(0.0, 0.0, 0.75));
		casaExterior3.render(glm::scale(modelCasaDer, glm::vec3(0.01, 3.0, 2.5)));
		modelCasaDer = glm::translate(modelCasaDer, glm::vec3(0.0, 0.0, 5.5));
		casaExterior3.render(glm::scale(modelCasaDer, glm::vec3(0.01, 3.0, 2.5)));
		glBindTexture(GL_TEXTURE_2D, 0);
		//PARED DE ENFRENTE
		glm::mat4 modelCasa4 = glm::translate(modelCasa3, glm::vec3(-7.5, 0.0, 7.5));
		glm::mat4 modelCasaFrente = glm::translate(modelCasa3, glm::vec3(-7.5, 0.0, 7.5));
		glBindTexture(GL_TEXTURE_2D, textureID5);
		casaExterior4.render(glm::scale(modelCasaFrente, glm::vec3(7.0, 3.0, 0.01)));
		modelCasaFrente = glm::translate(modelCasa4, glm::vec3(7.0, 0.0, 0.0));
		shaderMulLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(1.0, 3.0)));
		casaExterior4.render(glm::scale(modelCasaFrente, glm::vec3(1.0, 3.0, 0.01)));
		modelCasaFrente = glm::translate(modelCasa4, glm::vec3(-7.0, 0.0, 0.0));
		casaExterior4.render(glm::scale(modelCasaFrente, glm::vec3(1.0, 3.0, 0.01)));
		shaderMulLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(0.0, 0.0)));
		glBindTexture(GL_TEXTURE_2D, 0);

		//MOSAICO BANIO
		glm::mat4 mosaicoBano = glm::translate(modelCasa2, glm::vec3(-5.5, -1.49, 1.5));
		glBindTexture(GL_TEXTURE_2D, textureID6);
		mosaicoBanio.render(glm::scale(mosaicoBano, glm::vec3(4.0, 0.01, 3.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		//PARED BANIO IZQUIERA
		glm::mat4 paredesBanio = glm::translate(modelCasa, glm::vec3(0.01, 0.0, -6.0));
		glBindTexture(GL_TEXTURE_2D, textureID7);
		paredBanio.render(glm::scale(paredesBanio, glm::vec3(0.01, 3.0, 3.0)));
		//PARED BANIO ATRAS
		paredesBanio = glm::translate(modelCasa2, glm::vec3(-5.5, 0.0, 0.01));
		paredBanio.render(glm::scale(paredesBanio, glm::vec3(4.0, 3.0, 0.01)));
		//PARED BANIO DERECHA
		paredesBanio = glm::translate(modelCasa3, glm::vec3(-11.0, 0.0, -6.0));
		paredBanio.render(glm::scale(paredesBanio, glm::vec3(0.01, 3.0, 3.0)));
		//PARED BANIO ENFRENTE
		paredesBanio = glm::translate(modelCasa4, glm::vec3(-5.5, 0.0, -12.0));
		paredBanio.render(glm::scale(paredesBanio, glm::vec3(4.0, 3.0, 0.01)));
		glBindTexture(GL_TEXTURE_2D, 0);
		//PARED IZQUIERDA HABITACION
		glm::mat4 paredHabit = glm::translate(modelCasa, glm::vec3(0.01, 0.0, 1.0));
		glBindTexture(GL_TEXTURE_2D, textureID8);
		paredHabitacion.render(glm::scale(paredHabit, glm::vec3(0.01, 3.0, 2.0)));
		paredHabit = glm::translate(modelCasa, glm::vec3(0.01, 0.0, -4.0));
		paredHabitacion.render(glm::scale(paredHabit, glm::vec3(0.01, 3.0, 2.0)));
		//PARED ATRAS HABITACION
		paredHabit = glm::translate(modelCasa2, glm::vec3(-4.0, 0.0, 3.01));
		paredHabitacion.render(glm::scale(paredHabit, glm::vec3(7.0, 3.0, 0.01)));
		//PARED DERECHA HABITACION
		paredHabit = glm::translate(modelCasa3, glm::vec3(-8.0, 0.0, -1.5));
		paredHabitacion.render(glm::scale(paredHabit, glm::vec3(0.01, 3.0, 6.01)));
		//PARED ENFRENTE HABITACION
		paredHabit = glm::translate(modelCasa4, glm::vec3(-4.0, 0.0, -6.0));
		paredHabitacion.render(glm::scale(paredHabit, glm::vec3(7.0, 3.0, 0.01)));
		glBindTexture(GL_TEXTURE_2D, 0);
		//PISO HABITACION
		glm::mat4 pisoHabit = glm::translate(mosaicoBano, glm::vec3(1.5, 0.0, 4.5));
		glBindTexture(GL_TEXTURE_2D, textureID9);
		shaderMulLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(0.0, 0.0)));
		pisoHabitacion.render(glm::scale(pisoHabit, glm::vec3(7.0, 0.01, 6.0)));
		//PARED IZQUIERDA COCINA
		glm::mat4 paredCocina = glm::translate(modelCasa, glm::vec3(0.01, 0.0, 7.0));
		shaderMulLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(1.0, 3.0)));
		glBindTexture(GL_TEXTURE_2D, textureID10);
		cocinaPared.render(glm::scale(paredCocina, glm::vec3(0.015, 3.0, 1.0)));
		//PARED IZQUIERDA 2 COCINA 
		paredCocina = glm::translate(modelCasa, glm::vec3(0.01, 0.0, 2.5));
		shaderMulLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 3.0)));
		cocinaPared.render(glm::scale(paredCocina, glm::vec3(0.015, 3.0, 2.0)));
		//PARED ATRAS COCINA
		paredCocina = glm::translate(modelCasa2, glm::vec3(-4.0, 0.0, 9.02));
		shaderMulLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(6.0, 3.0)));
		cocinaPared.render(glm::scale(paredCocina, glm::vec3(7.0, 3.0, 0.01)));
		//PARED DERECHA COCINA
		paredCocina = glm::translate(modelCasa3, glm::vec3(-8.0, 0.0, 4.5));
		cocinaPared.render(glm::scale(paredCocina, glm::vec3(0.01, 3.0, 6.0)));
		//PARED ENFRENTE COCINA
		paredCocina = glm::translate(modelCasa4, glm::vec3(-7.0, 0.0, -0.01));
		shaderMulLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(1.0, 3.0)));
		cocinaPared.render(glm::scale(paredCocina, glm::vec3(1.0, 3.0, 0.01)));
		//PARED ENFRENTE 2 COCINA 
		paredCocina = glm::translate(modelCasa4, glm::vec3(-2.0, 0.0, -0.01));
		shaderMulLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(3.0, 3.0)));
		cocinaPared.render(glm::scale(paredCocina, glm::vec3(3.0, 3.0, 0.01)));
		glBindTexture(GL_TEXTURE_2D, 0);
		//PISO COCINA
		glm::mat4 pisoCocina = glm::translate(pisoHabit, glm::vec3(0.0, 0.0, 6.0));
		glBindTexture(GL_TEXTURE_2D, textureID11);
		shaderMulLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(6.0, 3.0)));
		cocinaPiso.render(glm::scale(pisoCocina, glm::vec3(7.0, 0.01, 6.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		
		//PARED SALA IZQUIERDA
		glm::mat4 paredSala = glm::translate(modelCasa, glm::vec3(7.01, 0.0, 3.5));
		glBindTexture(GL_TEXTURE_2D, textureID12);
		salaPared.render(glm::scale(paredSala, glm::vec3(0.01, 3.0, 8.0)));
		//PARED SALA ATRAS
		paredSala = glm::translate(modelCasa2, glm::vec3(3.5, 0.0, 7.0));
		salaPared.render(glm::scale(paredSala, glm::vec3(8.0, 3.0, 0.01)));
		//PARED SALA ENFRENTE
		paredSala = glm::translate(modelCasa4, glm::vec3(1.5, 0.0, -0.01));
		salaPared.render(glm::scale(paredSala, glm::vec3(4.0, 3.0, 0.01)));
		paredSala = glm::translate(modelCasa4, glm::vec3(7.0, 0.0, -0.01));
		salaPared.render(glm::scale(paredSala, glm::vec3(1.0, 3.0, 0.01)));
		//PARED SALA DERECHA
		paredSala = glm::translate(modelCasa3, glm::vec3(-0.01, 0.0, 0.75));
		salaPared.render(glm::scale(paredSala, glm::vec3(0.01, 3.0, 2.5)));
		paredSala = glm::translate(modelCasa3, glm::vec3(-0.01, 0.0, 6.25));
		salaPared.render(glm::scale(paredSala, glm::vec3(0.01, 3.0, 2.5)));
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 matrixReloj = glm::mat4(1.0);
		matrixReloj = glm::translate(paredSala, glm::vec3(-7.93, 1.0, -1.2));
		matrixReloj = glm::scale(matrixReloj, glm::vec3(0.5, 0.5, 0.5));
		shaderMulLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(0.0, 0.0)));
		modelReloj.render(matrixReloj);
		glActiveTexture(GL_TEXTURE0);
	
		//PISO SALA COMEDOR
		glm::mat4 pisoSala = glm::translate(pisoCocina, glm::vec3(7.5, 0.0, -1.0));
		glBindTexture(GL_TEXTURE_2D, textureID13);
		salaPiso.render(glm::scale(pisoSala, glm::vec3(8.0, 0.01, 8.0)));
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 matrixMesita = glm::mat4(1.0);
		matrixMesita = glm::translate(pisoSala, glm::vec3(0.75, 0.0, 0.0));
		matrixMesita = glm::scale(matrixMesita, glm::vec3(2.0, 1.0, 2.0));
		shaderMulLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(0.0, 0.0)));
		modelMesita.render(matrixMesita);
		glActiveTexture(GL_TEXTURE0);

		glm::mat4 comedor = glm::translate(pisoCocina, glm::vec3(0.5, 1.0, -1.0));
		glBindTexture(GL_TEXTURE_2D, textureID23);
		buroHabit.render(glm::scale(comedor, glm::vec3(2.5, 0.1, 2.5)));
		comedor = glm::translate(pisoCocina, glm::vec3(0.5, 0.5, -1.0));
		buroHabit.render(glm::scale(comedor, glm::vec3(0.2, 1.0, 0.2)));
		glActiveTexture(GL_TEXTURE0);

		comedor = glm::translate(pisoCocina, glm::vec3(0.5, 1.0, -1.0));
		glm::mat4 matrixTaburete = glm::mat4(1.0);
		matrixTaburete = glm::translate(comedor, glm::vec3(1.5, -1.0, 0.2));
		matrixTaburete = glm::scale(matrixTaburete, glm::vec3(1.5,1.5, 1.5));
		shaderMulLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(0.0, 0.0)));
		modelTaburete.render(matrixTaburete);
		glActiveTexture(GL_TEXTURE0);
		//PARED JARDIN (ARBUSTO)
		glm::mat4 arbusto = glm::translate(modelCasa2, glm::vec3(2.0, 0.0, 0.01));
		shaderMulLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(3.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, textureID14);
		paredExt.render(glm::scale(arbusto, glm::vec3(11.0, 3.0, 0.2)));
		arbusto = glm::translate(modelCasa3, glm::vec3(-0.01, 0.0, -4.0)); //arbusto izquierdo
		shaderMulLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(3.0, 1.0)));
		paredExt.render(glm::scale(arbusto, glm::vec3(0.01, 3.0, 7.0)));
		//ARBUSTO PEQUENO
		arbusto = glm::translate(modelCasa, glm::vec3(4.04, 0.0, -6.0));
		shaderMulLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(1.0, 1.0)));
		paredExt.render(glm::scale(arbusto, glm::vec3(0.03, 3.0, 3.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		//PARED PATIO
		glm::mat4 parExt = glm::translate(modelCasa4, glm::vec3(3.5, 0.0, -8.02));
		shaderMulLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(3.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, textureID15);
		paredExt.render(glm::scale(parExt, glm::vec3(8.0, 3.0, 0.01)));
		parExt = glm::translate(modelCasa, glm::vec3(7.01, 0.0, -2.5));
		shaderMulLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(0.0, 0.0)));
		paredExt.render(glm::scale(parExt, glm::vec3(0.01, 3.0, 4.0)));
		parExt = glm::translate(modelCasa4, glm::vec3(-2.0, 0.0, -12.0));
		paredExt.render(glm::scale(parExt, glm::vec3(3.0, 3.0, 0.01)));
		glBindTexture(GL_TEXTURE_2D, 0);
		//PISO PATIO
		glm::mat4 pisoPatio = glm::translate(pisoSala, glm::vec3(0.0, 0.0, -6.0));
		shaderMulLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(8.0, 2.0)));
		glBindTexture(GL_TEXTURE_2D, textureID16);
		pisoExt.render(glm::scale(pisoPatio, glm::vec3(8.0, 0.01, 4.0)));
		pisoPatio = glm::translate(pisoSala, glm::vec3(-2.0, 0.0, -9.5));
		pisoExt.render(glm::scale(pisoPatio, glm::vec3(12.0, 0.01, 3.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		//PUERTA PRINCIPAL
		glm::mat4 puertas = glm::translate(modelCasa4, glm::vec3(1.0, 0.0, 0.0));
		shaderMulLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(0.0, 0.0)));
		glBindTexture(GL_TEXTURE_2D, textureID17);
		puerta.render(glm::scale(puertas, glm::vec3(3.0, 3.0, 0.04)));
		glBindTexture(GL_TEXTURE_2D, 0);
		//PUERTAS CASA DENTRO
		puertas = glm::translate(modelCasa, glm::vec3(7.0, 0.0, 4.0));
		glBindTexture(GL_TEXTURE_2D, textureID18);
		puerta.render(glm::scale(puertas, glm::vec3(0.04, 3.0, 1.25)));//PUERTA COCINA
		puertas = glm::translate(modelCasa, glm::vec3(7.0, 0.0, 0.5)); //PUERTA CUARTO
		puerta.render(glm::scale(puertas, glm::vec3(0.04, 3.0, 1.25)));
		puertas = glm::translate(modelCasa2, glm::vec3(-6.0, 0.0, 3.0));
		puerta.render(glm::scale(puertas, glm::vec3(1.25, 3.0, 0.04)));
		puertas = glm::translate(modelCasa2, glm::vec3(6.0, 0.0, 7.0));
		puerta.render(glm::scale(puertas, glm::vec3(1.25, 3.0, 0.06)));
		glBindTexture(GL_TEXTURE_2D, 0);
		//VENTANAS
		glm::mat4 ventanas = glm::translate(modelCasa, glm::vec3(0.01, 0.0, 5.0));
		glBindTexture(GL_TEXTURE_2D, textureID19);
		ventana.render(glm::scale(ventanas, glm::vec3(0.05, 3.0, 3.0)));
		ventanas = glm::translate(modelCasa, glm::vec3(0.01, 0.0, -1.5));
		ventana.render(glm::scale(ventanas, glm::vec3(0.05, 3.0, 3.0)));
		ventanas = glm::translate(modelCasa3, glm::vec3(-0.01, 0.0, 3.5));
		ventana.render(glm::scale(ventanas, glm::vec3(0.05, 3.0, 3.0)));
		ventanas = glm::translate(modelCasa4, glm::vec3(5.0, 0.0, 0.01));
		ventana.render(glm::scale(ventanas, glm::vec3(3.0, 3.0, 0.07)));
		ventanas = glm::translate(modelCasa4, glm::vec3(-5.0, 0.0, 0.01));
		ventana.render(glm::scale(ventanas, glm::vec3(3.0, 3.0, 0.07)));
		glBindTexture(GL_TEXTURE_2D, 0);
		/*====================================*/
		glm::mat4 estufa = glm::translate(modelCasa, glm::vec3(0.5, -0.5, 5.0));
		glBindTexture(GL_TEXTURE_2D, textureID20);
		cocina.render(glm::scale(estufa, glm::vec3(1.0, 0.02, 1.5)));
		glBindTexture(GL_TEXTURE_2D, 0);
		//encimeraglm::vec3(-8.0, 0.0, -1.5)
		glm::mat4 encimeras = glm::translate(modelCasa, glm::vec3(0.5,-1.0,4.5));
		glBindTexture(GL_TEXTURE_2D, textureID27);
		encimera.render(glm::scale(encimeras, glm::vec3(1.0,1.0,6.0)));
		encimeras = glm::translate(modelCasa4, glm::vec3(-4.0, -1.0, -0.5));
		encimera.render(glm::scale(encimeras, glm::vec3(7.0, 1.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		//horno estufa
		estufa = glm::translate(modelCasa, glm::vec3(1.0, -1.0, 5.0));
		glBindTexture(GL_TEXTURE_2D, textureID22);
		cocina.render(glm::scale(estufa, glm::vec3(0.01, 1.0, 1.5)));
		glBindTexture(GL_TEXTURE_2D, 0);
		//comida
		estufa = glm::translate(modelCasa4, glm::vec3(-4.0, -0.4, -0.5));
		glBindTexture(GL_TEXTURE_2D, textureID21);
		cocina.render(glm::scale(estufa, glm::vec3(1.5, 0.0001, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		//MUEBLES HABITACION
		glm::mat4 muebles = glm::translate(modelCasa, glm::vec3(2.0, -1.25, -1.5));
		glBindTexture(GL_TEXTURE_2D, textureID23);
		muebleHabitacion.render(glm::scale(muebles, glm::vec3(3.5, 0.25, 2.0))); //base cama
		glm::mat4 muebles2 = glm::translate(modelCasa, glm::vec3(0.5, -1.25, 0.15)); //buro
		buroHabit.render(glm::scale(muebles2, glm::vec3(0.8, 1.0, 0.8)));
		muebles2 = glm::translate(modelCasa, glm::vec3(0.5, -1.25, -3.15)); //buro
		buroHabit.render(glm::scale(muebles2, glm::vec3(0.8, 1.0, 0.8)));
		glBindTexture(GL_TEXTURE_2D, 0);
		muebles = glm::translate(modelCasa, glm::vec3(2.0, -1.0, -1.5));
		glBindTexture(GL_TEXTURE_2D, textureID24);
		muebleHabitacion.render(glm::scale(muebles, glm::vec3(3.5, 0.5, 2.0))); //base cama
		glBindTexture(GL_TEXTURE_2D, 0);
		//JACUZI
		glm::mat4 jardin = glm::translate(modelCasa2, glm::vec3(5.5, -1.0, 2.0));
		jardin = glm::scale(jardin, glm::vec3(3.0, 1.0, 3.0));
		glBindTexture(GL_TEXTURE_2D, textureID23);
		jacuzi.render(0, jacuzi.getSlices() * jacuzi.getStacks() * 6,
			jardin);
		glBindTexture(GL_TEXTURE_2D, textureID25);
		jacuzi.render(jacuzi.getSlices() * jacuzi.getStacks() * 6,
			jacuzi.getSlices() * 3,
			jardin);
		glBindTexture(GL_TEXTURE_2D, 0);
		//SILLONES DE LA SALA
		glm::mat4 sillones = glm::translate(modelCasa, glm::vec3(10.0, -1.0, 3.5));
		shaderMulLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(4.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, textureID26);
		sillon.render(glm::scale(sillones, glm::vec3(0.15, 1.0, 4.0))); //respaldo sillon 
		shaderMulLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(1.0, 4.0)));
		sillones = glm::translate(modelCasa, glm::vec3(10.25, -1.35, 3.5));
		sillon.render(glm::scale(sillones, glm::vec3(0.5, 0.3, 4.0)));//asiento sillon
		shaderMulLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(4.0, 1.0)));
		sillones = glm::translate(modelCasa2, glm::vec3(4.0, -1.0, 9.1));
		sillon.render(glm::scale(sillones, glm::vec3(3.0, 1.0, 0.15))); //respaldo sillon 2
		shaderMulLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(4.0, 1.0)));
		sillones = glm::translate(modelCasa2, glm::vec3(4.0, -1.35, 9.25));
		sillon.render(glm::scale(sillones, glm::vec3(3.0, 0.3, 0.6)));//asiento sillon 2
		glBindTexture(GL_TEXTURE_2D, 0);

		
		//FIN CASITA
		//BOB ESPONJA CHIDO
		glm::mat4 model2 = glm::mat4(1.0f); //BOB ESPONJA
		model2 = glm::translate(arbusto, glm::vec3(7.0, 0.0, 2.0));
		glBindTexture(GL_TEXTURE_2D, textureID1);
		//CUERPO DE BOB
		box4.render(glm::scale(model2, glm::vec3(1.0, 1.0, 0.1)));
		glm::mat4 c2 = glm::translate(model2, glm::vec3(0.0, -0.5, 0.01));
		box5.render(glm::scale(c2, glm::vec3(1.0, 0.04, 0.1)));
		glm::mat4 c3 = glm::translate(c2, glm::vec3(0.0, -0.11, 0.0));
		box6.render(glm::scale(c3, glm::vec3(1.0, 0.25, 0.1)));
		glm::mat4 c4 = glm::translate(c3, glm::vec3(-0.3, -0.1, 0.0));
		box6.render(glm::scale(c4, glm::vec3(0.11, 0.13, 0.0)));
		glm::mat4 c5 = glm::translate(c3, glm::vec3(0.3, -0.1, 0.0));
		box6.render(glm::scale(c5, glm::vec3(0.11, 0.13, 0.0)));
		
		//PIERNAS
		// pie derecho 
		glm::mat4 p1 = glm::translate(c5, glm::vec3(0.01, -0.19, 0.0));
		p1 = glm::rotate(p1, glm::radians(0.2f), glm::vec3(0.0, 0.0, 1.0));
		cylinder4.render(glm::scale(p1, glm::vec3(0.048, 0.25, 0.048)));
		// pie izquierdo
		glm::mat4 p2 = glm::translate(c4, glm::vec3(0.01, -0.19, 0.0));
		p2 = glm::rotate(p2, glm::radians(0.2f), glm::vec3(0.0, 0.0, 1.0));
		cylinder4.render(glm::scale(p2, glm::vec3(0.048, 0.25, 0.048)));
		//corbata
		glm::mat4 ci1 = glm::translate(model2, glm::vec3(0.0, -0.5, 0.09));
		cylinder5.render(glm::scale(ci1, glm::vec3(0.1, 0.07, 0.1)));
		//ZAPATOS
		glm::mat4 z1 = glm::translate(p1, glm::vec3(0.0, -0.15, 0.0));
		box7.render(glm::scale(z1, glm::vec3(0.1, 0.05, 0.2)));
		glm::mat4 z2 = glm::translate(p2, glm::vec3(0.0, -0.15, 0.0));
		box7.render(glm::scale(z2, glm::vec3(0.1, 0.05, 0.2)));
		//ojos
		//ojo 2
		glm::mat4 ojo3 = glm::translate(model2, glm::vec3(0.15, 0.15, 0.05));
		sphere4.render(glm::scale(ojo3, glm::vec3(0.3, 0.3, 0.1)));
		//ojo parte azul
		glm::mat4 ojo5 = glm::translate(model2, glm::vec3(0.15, 0.15, 0.08));
		sphere6.render(glm::scale(ojo5, glm::vec3(0.15, 0.15, 0.05)));
		//ojo2 pequeño parte negra
		glm::mat4 ojo4 = glm::translate(model2, glm::vec3(0.15, 0.15, 0.09));
		sphere5.render(glm::scale(ojo4, glm::vec3(0.08, 0.08, 0.05)));
		//ojo 2
		glm::mat4 ojo6 = glm::translate(model2, glm::vec3(-0.15, 0.15, 0.05));
		sphere4.render(glm::scale(ojo6, glm::vec3(0.3, 0.3, 0.1)));
		//ojo parte azul
		glm::mat4 ojo7 = glm::translate(model2, glm::vec3(-0.15, 0.15, 0.08));
		sphere6.render(glm::scale(ojo7, glm::vec3(0.15, 0.15, 0.05)));
		//ojo2 pequeño parte negra
		glm::mat4 ojo8 = glm::translate(model2, glm::vec3(-0.15, 0.15, 0.09));
		sphere5.render(glm::scale(ojo8, glm::vec3(0.08, 0.08, 0.05)));
		// boca 
		glm::mat4 boca = glm::translate(model2, glm::vec3(0.0, -0.2, 0.08));
		sphere7.render(glm::scale(boca, glm::vec3(0.1, 0.1, 0.0)));
		// dientes 
		glm::mat4 d1 = glm::translate(boca, glm::vec3(-0.02, 0.03, 0.0));
		box8.render(glm::scale(d1, glm::vec3(0.025, 0.03, 0.01)));
		glm::mat4 d2 = glm::translate(boca, glm::vec3(0.02, 0.03, 0.0));
		box8.render(glm::scale(d2, glm::vec3(0.025, 0.03, 0.01)));
		//
		//BRAZO DERECHO
		//articulacion mano derecha hombro 
		glm::mat4 DH = glm::translate(model2, glm::vec3(0.5f, 0.0f, 0.0f));
		sphere8.render(glm::scale(DH, glm::vec3(0.1, 0.1, 0.1)));
		DH = glm::rotate(DH, rot1, glm::vec3(0, 0, 1));
		DH = glm::rotate(DH, rot2, glm::vec3(0.0, 1.0, 0.0));
		//BRAZO DERECHO
		glm::mat4 BD = glm::translate(DH, glm::vec3(0.1f, 0.0, 0.0));
		BD = glm::rotate(BD, glm::radians(90.0f), glm::vec3(0, 0, 1.0));
		cylinder6.render(glm::scale(BD, glm::vec3(0.05, 0.4, 0.05)));
		//articulacion mano derecha codo
		glm::mat4 AD = glm::translate(DH, glm::vec3(0.3, 0.0, 0.0));
		sphere9.render(glm::scale(AD, glm::vec3(0.1, 0.1, 0.1)));
		//mano derecha 2
		glm::mat4 MD = glm::translate(AD, glm::vec3(0.01, 0.2, 0.0));
		cylinder6.render(glm::scale(MD, glm::vec3(0.05, 0.4, 0.05)));
		//BRAZO IZQUIERDO
		glm::mat4 DH2 = glm::translate(model2, glm::vec3(-0.5f, 0.0f, 0.0f));
		sphere8.render(glm::scale(DH2, glm::vec3(0.1, 0.1, 0.1)));
		DH = glm::rotate(DH2, rot1, glm::vec3(0, 0, 1));
		DH = glm::rotate(DH2, rot2, glm::vec3(0.0, 1.0, 0.0));
		//
		glm::mat4 BD2 = glm::translate(DH2, glm::vec3(-0.1f, 0.0, 0.0));
		BD2 = glm::rotate(BD2, glm::radians(90.0f), glm::vec3(0, 0, 1.0));
		cylinder6.render(glm::scale(BD2, glm::vec3(0.05, 0.4, 0.05)));
		//articulacion mano derecha codo
		glm::mat4 AD2 = glm::translate(DH2, glm::vec3(-0.3, 0.0, 0.0));
		sphere9.render(glm::scale(AD2, glm::vec3(0.1, 0.1, 0.1)));
		//mano derecha 2
		glm::mat4 MD2 = glm::translate(AD2, glm::vec3(-0.01, 0.2, 0.0));
		cylinder6.render(glm::scale(MD2, glm::vec3(0.05, 0.4, 0.05)));
		/*AQUI TERMINA BOB ESPONJA*/
		/**************************************************************/
		/**								R2D2						 **/
		/**************************************************************/
		//model2 = glm::translate(model2, glm::vec3(1.0, 0.0, 0.0));
		model2 = rotate(model2, rot0, glm::vec3(0, 1, 0));
		model2 = glm::translate(paredSala, glm::vec3(-2.0, -0.5, 0));

		//cuerpoTronco.enableWireMode();
		//r2d2
		cuerpoTronco.render(glm::scale(model2, glm::vec3(1.0, 1.0, 1.0))); //CUERPO DE R2D2

		glm::mat4 pecho = glm::translate(model2, glm::vec3(0.0, 0.1, 0.5)); //linea azul pecho
		pecho = glm::rotate(pecho, glm::radians(-90.0f), glm::vec3(0.0, 1.0, 0.0));
		brazoDetalle.render(glm::scale(pecho, glm::vec3(0.0, 0.05, 0.3)));

		glm::mat4 pecho2 = glm::translate(model2, glm::vec3(0.0, 0.0, 0.5)); //linea azul pecho
		pecho2 = glm::rotate(pecho2, glm::radians(-90.0f), glm::vec3(0.0, 1.0, 0.0));
		brazoDetalle.render(glm::scale(pecho2, glm::vec3(0.0, 0.05, 0.3)));

		glm::mat4 pecho3 = glm::translate(model2, glm::vec3(0.0, -0.1, 0.5)); //linea azul pecho
		pecho3 = glm::rotate(pecho3, glm::radians(-90.0f), glm::vec3(0.0, 1.0, 0.0));
		brazoDetalle.render(glm::scale(pecho3, glm::vec3(0.0, 0.05, 0.3)));

		glm::mat4 cabezaSC = glm::translate(model2, glm::vec3(0.0, 0.5, 0.0));
		cabezaSC = glm::rotate(cabezaSC, rotCabeza, glm::vec3(0.0, 1.0, 0.0)); //rota la cabeza de R2D2
		cabeza.render(glm::scale(cabezaSC, glm::vec3(1.0, 0.7, 1.0)));

		glm::mat4 ojoSC = glm::translate(cabezaSC, glm::vec3(0.0, 0.1, 0.4));
		ojoR2D2.render(glm::scale(ojoSC, glm::vec3(0.2, 0.2, 0.2)));

		glm::mat4 art1 = glm::translate(model2, glm::vec3(0.5, 0.2, 0.0)); /*articulacion derecho arriba*/
		art1 = glm::rotate(art1, rotArt1, glm::vec3(1.0, 0.0, 0.0));
		articulacion.render(glm::scale(art1, glm::vec3(0.2, 0.2, 0.2)));

		glm::mat4 braDer = glm::translate(art1, glm::vec3(0.15, -0.4, 0.0)); /*brazo derecho con art1*/
		braDer = glm::rotate(braDer, glm::radians(-90.0f), glm::vec3(0.0, 1.0, 0.0));
		brazoDer.render(glm::scale(braDer, glm::vec3(0.2, 1.0, 0.2)));

		glm::mat4 braDerDet = glm::translate(art1, glm::vec3(0.18, -0.4, 0.0)); /*brazo derecho con art1*/
		braDerDet = glm::rotate(braDerDet, glm::radians(-90.0f), glm::vec3(0.0, 1.0, 0.0));
		brazoDetalle.render(glm::scale(braDerDet, glm::vec3(0.1, 0.8, 0.2)));


		glm::mat4 art3 = glm::translate(braDer, glm::vec3(0.0, -0.6, 0.0)); /*articulacion derecho muneca*/
		art3 = glm::rotate(art3, rotArt3, glm::vec3(1.0, 0.0, 0.0));
		articulacion3.render(glm::scale(art3, glm::vec3(0.2, 0.2, 0.2)));

		glm::mat4 cajaDer = glm::translate(art3, glm::vec3(0.0, -0.2, 0.0));
		baseDer.render(glm::scale(art3, glm::vec3(0.3, 0.15, 0.3)));

		glm::mat4 art2 = glm::translate(model2, glm::vec3(-0.5, 0.2, 0.0)); /*articulacion izquierda arriba*/
		art2 = glm::rotate(art2, rotArt1, glm::vec3(1.0, 0.0, 0.0));
		articulacion2.render(glm::scale(art2, glm::vec3(0.2, 0.2, 0.2)));

		glm::mat4 braIzq = glm::translate(art2, glm::vec3(-0.15, -0.4, 0.0)); /*brazo izquierdo con art2*/
		braIzq = glm::rotate(braIzq, glm::radians(-90.0f), glm::vec3(0.0, 1.0, 0.0));
		brazoIzq.render(glm::scale(braIzq, glm::vec3(0.2, 1.0, 0.2)));

		glm::mat4 braIzqDet = glm::translate(art2, glm::vec3(-0.18, -0.4, 0.0)); /*brazo izquierdo detalle azul con art2*/
		braIzqDet = glm::rotate(braIzqDet, glm::radians(-90.0f), glm::vec3(0.0, 1.0, 0.0));
		brazoDetalle.render(glm::scale(braIzqDet, glm::vec3(0.1, 0.8, 0.2)));

		glm::mat4 art4 = glm::translate(braIzq, glm::vec3(0.0, -0.6, 0.0)); /*articulacion izquierda muneca*/
		art4 = glm::rotate(art4, rotArt4, glm::vec3(1.0, 0.0, 0.0));
		articulacion4.render(glm::scale(art4, glm::vec3(0.2, 0.2, 0.2)));

		glm::mat4 cajaIzq = glm::translate(art4, glm::vec3(0.0, -0.2, 0.0));
		baseIzq.render(glm::scale(art4, glm::vec3(0.3, 0.15, 0.3)));

		glm::mat4 art5 = glm::translate(model2, glm::vec3(0.0, -0.6, 0.0)); /*articulacion izquierda muneca*/
		art5 = glm::rotate(art5, rotArt2, glm::vec3(1.0, 0.0, 0.0));
		articulacion5.render(glm::scale(art5, glm::vec3(0.2, 0.2, 0.2)));



		if (angle > 2 * M_PI)
			angle = 0.0;
		else
			angle += 0.0001;

		// Se Dibuja el Skybox
		GLint oldCullFaceMode;
		GLint oldDepthFuncMode;
		// deshabilita el modo del recorte de caras ocultas para ver las esfera desde adentro
		glGetIntegerv(GL_CULL_FACE_MODE, &oldCullFaceMode);
		glGetIntegerv(GL_DEPTH_FUNC, &oldDepthFuncMode);
		shaderSkybox.setFloat("skybox", 0);
		glCullFace(GL_FRONT);
		glDepthFunc(GL_LEQUAL);
		skyboxSphere.render();
		glCullFace(oldCullFaceMode);
		glDepthFunc(oldDepthFuncMode);

		dz = 0;
		rot0 = 0;
		offX += 0.001;

		glfwSwapBuffers(window);
	}
}

int main(int argc, char **argv) {
	init(800, 700, "Window GLFW", false);
	applicationLoop();
	destroy();
	return 1;
}