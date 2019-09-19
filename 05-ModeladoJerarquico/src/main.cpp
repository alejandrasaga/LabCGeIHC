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

int screenWidth;
int screenHeight;

GLFWwindow * window;

Shader shader;
std::shared_ptr<FirstPersonCamera> camera(new FirstPersonCamera());

Sphere cabeza(20, 20);
Sphere ojo(20, 20);
Sphere articulacion(20, 20);
Sphere  articulacion3(20, 20); //articulacion 1 es del hombro y la 3 de la muneca derecha
Sphere articulacion2(20, 20);
Sphere articulacion4(20, 20); //articulacion 2 es del hombro y la 4 de la muneca izquierda
Cylinder cuerpoTronco(20, 20, 0.5, 0.5);
Cylinder brazoDer(20, 20, 0.5, 0.5);
Cylinder brazoIzq(20, 20, 0.5, 0.5);
Box baseDer, baseIzq; //cajas en las articulaciones de las munecas
Sphere articulacion5(20,20);
Box baseCentro;
Box box1;

bool exitApp = false;
int lastMousePosX, offsetX = 0;
int lastMousePosY, offsetY = 0;

float rot1 = 0.0, rot2 = 0.0, rot3 = 0.0, rot4 = 0.0;
float rotCabeza = 0.0, rotArt1 = 0.0, rotArt2 = 0.0, rotArt3 = 0.0, rotArt4 = 0.0; //Rotar R2D2
bool sentido = true;

float rot0 = 0.0, dz = 0.0;

double deltaTime;

// Se definen todos las funciones.
void reshapeCallback(GLFWwindow* Window, int widthRes, int heightRes);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void mouseButtonCallback(GLFWwindow* window, int button, int state, int mod);
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

	// Inicializar los buffers VAO, VBO, EBO
	cabeza.init();
	// Método setter que colocar el apuntador al shader
	cabeza.setShader(&shader);
	//Setter para poner el color de la geometria
	cabeza.setColor(glm::vec4(0.7, 0.7, 1.0, 1.0));

	// Inicializar los buffers VAO, VBO, EBO
	ojo.init();
	// Método setter que colocar el apuntador al shader
	ojo.setShader(&shader);
	//Setter para poner el color de la geometria
	ojo.setColor(glm::vec4(0.0, 0.0, 0.0, 0.0));

	cuerpoTronco.init();
	cuerpoTronco.setShader(&shader);
	cuerpoTronco.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

	articulacion.init(); /*articulacion arriba derecha*/
	articulacion.setShader(&shader);
	articulacion.setColor(glm::vec4(0.8,0.8,0.8,0.8));

	brazoDer.init();
	brazoDer.setShader(&shader);
	brazoDer.setColor(glm::vec4(0.9, 0.9, 0.9, 1.0));

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

	box1.init();
	box1.setShader(&shader);
	box1.setColor(glm::vec4(1.0, 1.0, 0.0, 1.0));
	camera->setPosition(glm::vec3(0.0, 0.0, 4.0));
}

void destroy() {
	glfwDestroyWindow(window);
	glfwTerminate();
	// --------- IMPORTANTE ----------
	// Eliminar los shader y buffers creados.

	// Destrucción de los VAO, EBO, VBO
	cabeza.destroy();
	cuerpoTronco.destroy();
	box1.destroy();

	shader.destroy();
}

void reshapeCallback(GLFWwindow* Window, int widthRes, int heightRes) {
	screenWidth = widthRes;
	screenHeight = heightRes;
	glViewport(0, 0, widthRes, heightRes);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_ESCAPE:
			exitApp = true;
			break;
		}
	}
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
	offsetX = xpos - lastMousePosX;
	offsetY = ypos - lastMousePosY;
	lastMousePosX = xpos;
	lastMousePosY = ypos;
}

void mouseButtonCallback(GLFWwindow* window, int button, int state, int mod) {
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

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS
		&& glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
		rot1 += 0.001;
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS
		&& glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		rot1 -= 0.001;

	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS 
		&& glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
		rot2 += 0.001;
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS 
		&& glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		rot2 -= 0.001;

	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS
		&& glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
		rot3 += 0.001;
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS
		&& glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		rot3 -= 0.001;

	//ROTACION DE ARTURITO
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS		/*ROTA LA CABEZA*/
		&& glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
		rotCabeza += 0.001;
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS
		&& glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		rotCabeza -= 0.001;

	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS		/*ROTA ARTICULACION DERECHA E IZQUIERDA PARA QUE AMBAS  */
		&& glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE) /*SE MUEVAN AL MISMO TIEMPO*/
		rotArt1 += 0.001;
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS
		&& glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		rotArt1 -= 0.001;

	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS		/*ROTA ARTICULACION centro*/
		&& glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
		rotArt2 += 0.001;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS
		&& glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		rotArt2 -= 0.001;

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		rot0 = 0.0001;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		rot0 = -0.0001;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		dz = 0.0001;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		dz = -0.0001;
	glfwPollEvents();
	return continueApplication;
}

void applicationLoop() {
	bool psi = true;
	glm::mat4 model2 = glm::mat4(1.0f);
	while (psi) {
		psi = processInput(true);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)screenWidth / (float)screenHeight, 0.01f, 100.0f);
		glm::mat4 view = camera->getViewMatrix();

		shader.setMatrix4("projection", 1, false, glm::value_ptr(projection));
		shader.setMatrix4("view", 1, false, glm::value_ptr(view));

		model2 = rotate(model2, rot0, glm::vec3(0, 1, 0));
		model2 = glm::translate(model2, glm::vec3(0, dz, 0));
		//cuerpoTronco.enableWireMode();
		//r2d2
		cuerpoTronco.render(glm::scale(model2, glm::vec3(1.0,1.0,1.0))); //CUERPO DE R2D2
		
		glm::mat4 cabezaSC = glm::translate(model2, glm::vec3(0.0, 0.5, 0.0));
		cabezaSC = glm::rotate(cabezaSC, rotCabeza, glm::vec3(0.0,1.0,0.0)); //rota la cabeza de R2D2
		cabeza.render(glm::scale(cabezaSC,glm::vec3(1.0,0.7,1.0)));
		
		glm::mat4 ojoSC = glm::translate(cabezaSC,glm::vec3(0.0,0.1,0.4));
		ojo.render(glm::scale(ojoSC, glm::vec3(0.2, 0.2, 0.2)));
		
		glm::mat4 art1 = glm::translate(model2, glm::vec3(0.5,0.2,0.0)); /*articulacion derecho arriba*/
		art1 = glm::rotate(art1, rotArt1, glm::vec3(1.0, 0.0, 0.0));
		articulacion.render(glm::scale(art1, glm::vec3(0.2,0.2,0.2)));
		
		glm::mat4 braDer = glm::translate(art1, glm::vec3(0.15,-0.4,0.0)); /*brazo derecho con art1*/
		braDer = glm::rotate(braDer, glm::radians(-90.0f), glm::vec3(0.0, 1.0, 0.0));
		brazoDer.render(glm::scale(braDer, glm::vec3(0.2,1.0,0.2)));

		glm::mat4 art3 = glm::translate(braDer, glm::vec3(0.0, -0.6, 0.0)); /*articulacion derecho muneca*/
		art3 = glm::rotate(art3, rotArt3, glm::vec3(1.0, 0.0, 0.0));
		articulacion3.render(glm::scale(art3, glm::vec3(0.2, 0.2, 0.2)));

		glm::mat4 cajaDer = glm::translate(art3, glm::vec3(0.0, -0.2, 0.0));
		baseDer.render(glm::scale(art3, glm::vec3(0.3,0.15,0.3)));

		glm::mat4 art2 = glm::translate(model2, glm::vec3(-0.5, 0.2, 0.0)); /*articulacion izquierda arriba*/
		art2 = glm::rotate(art2, rotArt1, glm::vec3(1.0, 0.0, 0.0));
		articulacion2.render(glm::scale(art2, glm::vec3(0.2, 0.2, 0.2)));

		glm::mat4 braIzq = glm::translate(art2, glm::vec3(-0.15, -0.4, 0.0)); /*brazo izquierdo con art2*/
		braIzq = glm::rotate(braIzq, glm::radians(-90.0f), glm::vec3(0.0, 1.0, 0.0));
		brazoIzq.render(glm::scale(braIzq, glm::vec3(0.2, 1.0, 0.2)));

		glm::mat4 art4 = glm::translate(braIzq, glm::vec3(0.0, -0.6, 0.0)); /*articulacion izquierda muneca*/
		art4 = glm::rotate(art4, rotArt4, glm::vec3(1.0, 0.0, 0.0));
		articulacion4.render(glm::scale(art4, glm::vec3(0.2, 0.2, 0.2)));

		glm::mat4 cajaIzq = glm::translate(art4, glm::vec3(0.0, -0.2, 0.0));
		baseIzq.render(glm::scale(art4, glm::vec3(0.3, 0.15, 0.3)));

		glm::mat4 art5 = glm::translate(model2, glm::vec3(0.0, -0.6, 0.0)); /*articulacion izquierda muneca*/
		art5 = glm::rotate(art5, rotArt2, glm::vec3(1.0, 0.0, 0.0));
		articulacion5.render(glm::scale(art5, glm::vec3(0.2, 0.2, 0.2)));
		
		glm::mat4 cajaCentro = glm::translate(art5, glm::vec3(0.0, -0.2, 0.0));
		baseCentro.render(glm::scale(art5, glm::vec3(0.5, 0.3, 0.5)));

		shader.turnOff();

		dz = 0;
		rot0 = 0;

		glfwSwapBuffers(window);
	}
}

int main(int argc, char ** argv) {
	init(800, 700, "Window GLFW", false);
	applicationLoop();
	destroy();
	return 1;
}
