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

//Descomentar
#include "Headers/Texture.h"

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
//Shader con materiales
Shader shaderMaterialLighting;
//Shader con skybox
Shader shaderSkybox;
//SHADER CON MUCHAS LUCES
Shader shaderMulLighting;

std::shared_ptr<FirstPersonCamera> camera(new FirstPersonCamera());

Sphere sphere1(20, 20);
Sphere sphere2(20, 20);
Sphere sphere3(20, 20);
Sphere sphereLamp(20, 20);
Sphere skyboxSphere(20, 20);
Cylinder buroHabit(20, 20, 0.5, 0.5);
Cylinder jacuzi(20, 20, 0.5, 0.5);
Box boxMaterials;
Box box3;
Box casaExterior, casaExterior2, casaExterior3, casaExterior4; //paredes de la casa exterior
Box mosaicoBanio, paredBanio;//BANIO1
Box pisoHabitacion, paredHabitacion, muebleHabitacion;
Box cocinaPared, cocinaPiso, cocina;
Box salaPared, salaPiso, sillon;
Box paredExt, paredExt2;
Box pisoExt;
Box puerta;
Box ventana;
//												 paredes exterior, mosaicoBanio,paredBanio, pisoHabit, paredHabit
GLuint textureID1, textureID2, textureID3, textureID4, textureID5, textureID6, textureID7, textureID9, textureID8;
//		cocinaPared,cocinaPiso,  marmolSala,	pisoSala, paredArbusto, paredRoca, pisoExterior, puertaPrinci
GLuint textureID10, textureID11, textureID12, textureID13, textureID14, textureID15, textureID16, textureID17;
//		puertas		ventana		cocina estufa, comida		horno estufa cama base	cama colchon, piscina
GLuint textureID18, textureID19, textureID20, textureID21, textureID22, textureID23, textureID24, textureID25;
// sillon
GLuint textureID26;
GLuint skyboxTextureID;

GLenum types[6] = { //enumeracion 
GL_TEXTURE_CUBE_MAP_POSITIVE_X,
GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
GL_TEXTURE_CUBE_MAP_NEGATIVE_Z };

/*std::string fileNames[6] = { "../Textures/mp_bloodvalley/blood-valley_ft.tga", //FT es el float asociado al indice de numeracion
		"../Textures/mp_bloodvalley/blood-valley_bk.tga",
		"../Textures/mp_bloodvalley/blood-valley_up.tga",
		"../Textures/mp_bloodvalley/blood-valley_dn.tga",
		"../Textures/mp_bloodvalley/blood-valley_rt.tga",
		"../Textures/mp_bloodvalley/blood-valley_lf.tga" };*/


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
	// Descomentar
	shaderMaterialLighting.initialize("../Shaders/iluminacion_material.vs",
		"../Shaders/iluminacion_material.fs");
	// Descomentar
	shaderSkybox.initialize("../Shaders/cubeTexture.vs",
		"../Shaders/cubeTexture.fs");
	shaderMulLighting.initialize("../Shaders/iluminacion_texture_res.vs",
		"../Shaders/multipleLights.fs");

	// Inicializar los buffers VAO, VBO, EBO
	sphere1.init();
	// Método setter que colocar el apuntador al shader
	sphere1.setShader(&shaderColorLighting); //AGREGAMOS COLOR E ILUMINACION
	//Setter para poner el color de la geometria
	sphere1.setColor(glm::vec4(0.3, 0.3, 1.0, 1.0));

	sphereLamp.init();
	sphereLamp.setShader(&shader);
	sphereLamp.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

	boxMaterials.init();
	boxMaterials.setShader(&shaderMaterialLighting);//no es necesario ponerle color porque le estamos diicendo que material va a tener

	//INICIALIZACION DE LA ESPERA DEL SKYBOX
	skyboxSphere.init();
	skyboxSphere.setShader(&shaderSkybox);
	skyboxSphere.setScale(glm::vec3(20.0f, 20.0f, 20.0f)); //INDICA LA ESCALA DEL OBJETO

	sphere3.init();
	sphere3.setShader(&shaderTextureLighting);

	box3.init();
	box3.setShader(&shaderTextureLighting);
	//PAREDES EXTERIORES DE LA CASA 
	casaExterior.init();
	casaExterior.setShader(&shaderTextureLighting);
	casaExterior2.init();
	casaExterior2.setShader(&shaderTextureLighting);
	casaExterior3.init();
	casaExterior3.setShader(&shaderTextureLighting);
	casaExterior4.init();
	casaExterior4.setShader(&shaderTextureLighting);
	//BANIO1
	mosaicoBanio.init();
	mosaicoBanio.setShader(&shaderTextureLighting);
	paredBanio.init();
	paredBanio.setShader(&shaderTextureLighting);
	//HABITACION IZQUIERDA CERCA BANIO
	pisoHabitacion.init();
	pisoHabitacion.setShader(&shaderTextureLighting);
	paredHabitacion.init();
	paredHabitacion.setShader(&shaderTextureLighting);
	//COCINA
	cocinaPared.init();
	cocinaPared.setShader(&shaderTextureLighting);
	cocinaPiso.init();
	cocinaPiso.setShader(&shaderTextureLighting);
	//SALA
	salaPared.init();
	salaPared.setShader(&shaderTextureLighting);
	salaPiso.init();
	salaPiso.setShader(&shaderTextureLighting);
	sillon.init();
	sillon.setShader(&shaderTextureLighting);
	//CUARTO EXTERIOR
	paredExt.init();
	paredExt.setShader(&shaderTextureLighting);
	paredExt2.init();
	paredExt2.setShader(&shaderTextureLighting);
	pisoExt.init();
	pisoExt.setShader(&shaderTextureLighting);
	puerta.init();
	puerta.setShader(&shaderTextureLighting);
	//VENTANA
	ventana.init();
	ventana.setShader(&shaderTextureLighting);
	//cocina muebles
	cocina.init();
	cocina.setShader(&shaderTextureLighting);
	//habitacion muebles
	muebleHabitacion.init();
	muebleHabitacion.setShader(&shaderTextureLighting);
	buroHabit.init();
	buroHabit.setShader(&shaderTextureLighting);
	//jardin
	jacuzi.init();
	jacuzi.setShader(&shaderTextureLighting);
	camera->setPosition(glm::vec3(0.0, 0.0, 4.0));

	int imageWidth, imageHeight;
	// TEXTURA DE PAREDES EXTERIORES
	Texture texture5("../Textures/maderaExt.jpg");
	FIBITMAP *bitmap = texture5.loadImage();
	unsigned char *data = texture5.convertToData(bitmap, imageWidth, imageHeight);
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
	Texture texture19("../Textures/ventana.jpg");
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

	Texture skyboxTexture = Texture("");
	glGenTextures(1, &skyboxTextureID);
	//TIPO DE TEXTURA CUBE MAP
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
		// Descomentar
		shaderMaterialLighting.setMatrix4("projection", 1, false,
			glm::value_ptr(projection));
		shaderMaterialLighting.setMatrix4("view", 1, false,
			glm::value_ptr(view));

		// Settea la matriz de vista y projection al shader con skybox
		// Descomentar
		shaderSkybox.setMatrix4("projection", 1, false,
			glm::value_ptr(projection));
		shaderSkybox.setMatrix4("view", 1, false,
			glm::value_ptr(glm::mat4(glm::mat3(view))));
		//setea la matriz de vista y proyeccion al shader con multiples luces
		shaderMulLighting.setMatrix4("projection", 1, false,
			glm::value_ptr(projection));
		shaderMulLighting.setMatrix4("view", 1, false,
			glm::value_ptr(glm::mat4(glm::mat3(view))));

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
			glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));

		// Propiedades de la luz para objetos con textura
		// Descomentar
		shaderMaterialLighting.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
		shaderMaterialLighting.setVectorFloat3("light.ambient", glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		shaderMaterialLighting.setVectorFloat3("light.diffuse", glm::value_ptr(glm::vec3(0.5, 0.5, 0.5)));
		shaderMaterialLighting.setVectorFloat3("light.specular", glm::value_ptr(glm::vec3(0.9, 0.9, 0.9)));

		//propiedades de la luz para los objetos con multiples luces
		shaderMulLighting.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
		shaderMulLighting.setVectorFloat3("directionalLight.ambient", glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		shaderMulLighting.setVectorFloat3("directionalLight.diffuse", glm::value_ptr(glm::vec3(0.5, 0.5, 0.5)));
		shaderMulLighting.setVectorFloat3("directionalLight.specular", glm::value_ptr(glm::vec3(0.9, 0.9, 0.9)));
		shaderMulLighting.setVectorFloat3("directionalLight.direction", glm::value_ptr(glm::vec3(0.0, 0.0, -1.0))); //indicamos direccion, no posicion

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
		// Descomentar
		shaderMaterialLighting.setVectorFloat3("light.position",
			glm::value_ptr(
				glm::vec4(
					lightModelmatrix
					* glm::vec4(0.0, 0.0, 0.0, 1.0))));
		sphereLamp.render(lightModelmatrix);

		model = glm::translate(model, glm::vec3(0, 0, dz));
		model = glm::rotate(model, rot0, glm::vec3(0, 1, 0));
		//PARED IZQUIERDA
		glm::mat4 modelCasa = glm::mat4(1.0); 
		modelCasa = glm::translate(model, glm::vec3(0.0, -3.0, 0.0));
		glBindTexture(GL_TEXTURE_2D, textureID5); 
		casaExterior.render(glm::scale(modelCasa, glm::vec3(0.01, 3.0, 15.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		//PARED DE ATRAS
		glm::mat4 modelCasa2 = glm::translate(modelCasa, glm::vec3(7.5, 0.0, -7.5));
		glBindTexture(GL_TEXTURE_2D, textureID5);
		casaExterior2.render(glm::scale(modelCasa2, glm::vec3(15.0, 3.0, 0.01)));
		glBindTexture(GL_TEXTURE_2D, 0);
		//PARED DERECHA
		glm::mat4 modelCasa3 = glm::translate(modelCasa2, glm::vec3(7.5, 0.0, 7.5));
		glBindTexture(GL_TEXTURE_2D, textureID5);
		casaExterior3.render(glm::scale(modelCasa3, glm::vec3(0.01, 3.0, 15.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		//PARED DE ENFRENTE
		glm::mat4 modelCasa4 = glm::translate(modelCasa3, glm::vec3(-7.5, 0.0, 7.5));
		glBindTexture(GL_TEXTURE_2D, textureID5);
		casaExterior4.render(glm::scale(modelCasa4, glm::vec3(15.0, 3.0, 0.01)));
		glBindTexture(GL_TEXTURE_2D, 0);

		//MOSAICO BANIO
		glm::mat4 mosaicoBano = glm::translate(modelCasa2, glm::vec3(-5.5, -1.49, 1.5));
		glBindTexture(GL_TEXTURE_2D, textureID6);
		mosaicoBanio.render(glm::scale(mosaicoBano, glm::vec3(4.0, 0.01, 3.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		//PARED BANIO IZQUIERA
		glm::mat4 paredesBanio = glm::translate(modelCasa, glm::vec3(0.01,0.0,-6.0));
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
		glm::mat4 paredHabit = glm::translate(modelCasa, glm::vec3(0.01,0.0,-1.5));
		glBindTexture(GL_TEXTURE_2D, textureID8);
		paredHabitacion.render(glm::scale(paredHabit, glm::vec3(0.01, 3.0, 6.0)));
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
		glm::mat4 pisoHabit = glm::translate(mosaicoBano, glm::vec3(1.5,0.0,4.5));
		glBindTexture(GL_TEXTURE_2D, textureID9);
		shaderTextureLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(0.0, 0.0)));
		pisoHabitacion.render(glm::scale(pisoHabit, glm::vec3(7.0, 0.01, 6.0)));
		//PARED IZQUIERDA COCINA
		glm::mat4 paredCocina = glm::translate(modelCasa, glm::vec3(0.01,0.0,4.5));
		shaderTextureLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(6.0, 3.0)));
		glBindTexture(GL_TEXTURE_2D, textureID10);
		cocinaPared.render(glm::scale(paredCocina, glm::vec3(0.015, 3.0, 6.0)));
		//PARED ATRAS COCINA
		paredCocina = glm::translate(modelCasa2, glm::vec3(-4.0, 0.0, 9.02));
		cocinaPared.render(glm::scale(paredCocina, glm::vec3(7.0, 3.0, 0.01)));
		//PARED DERECHA COCINA
		paredCocina = glm::translate(modelCasa3, glm::vec3(-8.0, 0.0, 4.5));
		cocinaPared.render(glm::scale(paredCocina, glm::vec3(0.01, 3.0, 6.0)));
		//PARED ENFRENTE COCINA
		paredCocina = glm::translate(modelCasa4, glm::vec3(-4.0, 0.0, -0.01));
		cocinaPared.render(glm::scale(paredCocina, glm::vec3(7.0, 3.0, 0.01)));
		glBindTexture(GL_TEXTURE_2D, 0);
		//PISO COCINA
		glm::mat4 pisoCocina = glm::translate(pisoHabit, glm::vec3(0.0, 0.0, 6.0));
		glBindTexture(GL_TEXTURE_2D, textureID11);
		cocinaPiso.render(glm::scale(pisoCocina, glm::vec3(7.0, 0.01, 6.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		//PARED SALA IZQUIERDA
		glm::mat4 paredSala = glm::translate(modelCasa, glm::vec3(7.01,0.0,3.5));
		glBindTexture(GL_TEXTURE_2D, textureID12);
		salaPared.render(glm::scale(paredSala, glm::vec3(0.01, 3.0, 8.0)));
		//PARED SALA ATRAS
		paredSala = glm::translate(modelCasa2, glm::vec3(3.5, 0.0, 7.0));
		salaPared.render(glm::scale(paredSala, glm::vec3(8.0, 3.0, 0.01)));
		//PARED SALA ENFRENTE
		paredSala = glm::translate(modelCasa4, glm::vec3(3.5, 0.0, -0.01));
		salaPared.render(glm::scale(paredSala, glm::vec3(8.0, 3.0, 0.01)));
		//PARED SALA DERECHA
		paredSala = glm::translate(modelCasa3, glm::vec3(-0.01, 0.0, 3.5));
		salaPared.render(glm::scale(paredSala, glm::vec3(0.01, 3.0, 8.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		//PISO SALA COMEDOR
		glm::mat4 pisoSala = glm::translate(pisoCocina, glm::vec3(7.5,0.0,-1.0));
		glBindTexture(GL_TEXTURE_2D, textureID13);
		salaPiso.render(glm::scale(pisoSala, glm::vec3(8.0, 0.01, 8.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		//PARED JARDIN (ARBUSTO)
		glm::mat4 arbusto = glm::translate(modelCasa2, glm::vec3(2.0,0.0,0.01));
		shaderTextureLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(3.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, textureID14);
		paredExt.render(glm::scale(arbusto, glm::vec3(11.0,3.0,0.2)));
		arbusto = glm::translate(modelCasa3, glm::vec3(-0.01, 0.0, -4.0)); //arbusto izquierdo
		shaderTextureLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(3.0, 1.0)));
		paredExt.render(glm::scale(arbusto, glm::vec3(0.01, 3.0, 7.0)));
		//ARBUSTO PEQUENO
		arbusto = glm::translate(modelCasa, glm::vec3(4.04, 0.0, -6.0));
		shaderTextureLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(1.0, 1.0)));
		paredExt.render(glm::scale(arbusto, glm::vec3(0.03, 3.0, 3.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		//PARED PATIO
		glm::mat4 parExt = glm::translate(modelCasa4, glm::vec3(3.5, 0.0, -8.02));
		shaderTextureLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(3.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, textureID15);
		paredExt.render(glm::scale(parExt, glm::vec3(8.0, 3.0, 0.01)));
		parExt = glm::translate(modelCasa, glm::vec3(7.01, 0.0, -2.5));
		shaderTextureLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(0.0, 0.0)));
		paredExt.render(glm::scale(parExt, glm::vec3(0.01, 3.0, 4.0)));
		parExt = glm::translate(modelCasa4, glm::vec3(-2.0, 0.0, -12.0));
		paredExt.render(glm::scale(parExt, glm::vec3(3.0, 3.0, 0.01)));
		glBindTexture(GL_TEXTURE_2D, 0);
		//PISO PATIO
		glm::mat4 pisoPatio = glm::translate(pisoSala, glm::vec3(0.0, 0.0, -6.0));
		shaderTextureLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(8.0, 2.0)));
		glBindTexture(GL_TEXTURE_2D, textureID16);
		pisoExt.render(glm::scale(pisoPatio, glm::vec3(8.0,0.01,4.0)));
		pisoPatio = glm::translate(pisoSala, glm::vec3(-2.0, 0.0, -9.5));
		pisoExt.render(glm::scale(pisoPatio, glm::vec3(12.0, 0.01, 3.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		//PUERTA PRINCIPAL
		glm::mat4 puertas = glm::translate(modelCasa4,glm::vec3(1.0,0.0,0.0));
		shaderTextureLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(0.0, 0.0)));
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
		glm::mat4 ventanas = glm::translate(modelCasa, glm::vec3(0.01,0.5,5.0));
		glBindTexture(GL_TEXTURE_2D, textureID19);
		ventana.render(glm::scale(ventanas, glm::vec3(0.05,1.5,3.0)));
		ventanas = glm::translate(modelCasa, glm::vec3(0.01, 0.5, -1.5));
		ventana.render(glm::scale(ventanas, glm::vec3(0.05, 1.5, 3.0)));
		ventanas = glm::translate(modelCasa3, glm::vec3(-0.01, 0.5, 3.5));
		ventana.render(glm::scale(ventanas, glm::vec3(0.05, 1.5, 3.0)));
		ventanas = glm::translate(modelCasa4, glm::vec3(5.0, 0.5, 0.01));
		ventana.render(glm::scale(ventanas, glm::vec3(3.0, 1.5, 0.07)));
		ventanas = glm::translate(modelCasa4, glm::vec3(-5.0, 0.5, 0.01));
		ventana.render(glm::scale(ventanas, glm::vec3(3.0, 1.5, 0.07)));
		glBindTexture(GL_TEXTURE_2D, 0);
		/*====================================*/
		glm::mat4 estufa = glm::translate(modelCasa, glm::vec3(0.5, -0.5, 5.0));
		glBindTexture(GL_TEXTURE_2D, textureID20);
		cocina.render(glm::scale(estufa, glm::vec3(1.0, 0.02, 1.5)));
		glBindTexture(GL_TEXTURE_2D, 0);
		//horno estufa
		estufa = glm::translate(modelCasa, glm::vec3(1.0,-1.0,5.0));
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
		jardin = glm::scale(jardin, glm::vec3(3.0,1.0,3.0));
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
		shaderTextureLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(4.0,1.0)));
		glBindTexture(GL_TEXTURE_2D, textureID26);
		sillon.render(glm::scale(sillones, glm::vec3(0.15,1.0,4.0))); //respaldo sillon 
		shaderTextureLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(1.0, 4.0)));
		sillones = glm::translate(modelCasa, glm::vec3(10.25, -1.35, 3.5));
		sillon.render(glm::scale(sillones, glm::vec3(0.5, 0.3, 4.0)));//asiento sillon
		shaderTextureLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(4.0, 1.0)));
		sillones = glm::translate(modelCasa2, glm::vec3(4.0, -1.0, 9.1));
		sillon.render(glm::scale(sillones, glm::vec3(3.0, 1.0, 0.15))); //respaldo sillon 2
		shaderTextureLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(4.0, 1.0)));
		sillones = glm::translate(modelCasa2, glm::vec3(4.0, -1.35, 9.25));
		sillon.render(glm::scale(sillones, glm::vec3(3.0, 0.3, 0.6)));//asiento sillon 2
		glBindTexture(GL_TEXTURE_2D, 0);

		//ENCIMERA DE LA COCINA
		shaderMaterialLighting.setVectorFloat3("material.ambient", glm::value_ptr(glm::vec3(0.05f, 0.05f, 0.05f)));
		shaderMaterialLighting.setVectorFloat3("material.diffuse", glm::value_ptr(glm::vec3(0.5f, 0.5f, 0.5f)));
		shaderMaterialLighting.setVectorFloat3("material.specular", glm::value_ptr(glm::vec3(0.7f, 0.7f, 0.7f)));
		shaderMaterialLighting.setFloat("material.shininess", 10.0f);
		glm::mat4 boxMaterialModel = glm::translate(modelCasa, glm::vec3(0.5f, -1.0f, 4.5f));
		boxMaterials.render(glm::scale(boxMaterialModel, glm::vec3(1.0,1.0,6.0)));
		boxMaterialModel = glm::translate(boxMaterialModel, glm::vec3(3.5f, 0.0f, 2.5f));
		boxMaterials.render(glm::scale(boxMaterialModel, glm::vec3(6.0, 1.0, 1.0)));
		boxMaterialModel = glm::translate(boxMaterialModel, glm::vec3(-1.5f, 0.0f, -5.0f));
		boxMaterials.render(glm::scale(boxMaterialModel, glm::vec3(3.0, 1.0, 1.0)));

		if (angle > 2 * M_PI)
			angle = 0.0;
		else
			angle += 0.0001;

		// Se Dibuja el Skybox
		GLint oldCullFaceMode;
		GLint oldDepthFuncMode;
		//SE DESHABILITA EL MODO DEL RECORTE DE CARAS OCULTAS PARA VER LAS ESFERAS DESDE ADENTRO
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