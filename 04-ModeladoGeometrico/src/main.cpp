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

Sphere sphere1(20, 20);//particiones de esfera, 20 horizontales, 20 veticales
Cylinder cylinder1(20, 20, 0.5, 0.5);// esfera de radio 0.5
Sphere sphere2(20, 20);
Sphere sphere3(20, 20);
Sphere sphere4(20, 20);
Sphere sphere5(20, 20);
Sphere sphere6(20, 20);
Sphere sphere7(20, 20);
Sphere sphere8(20, 20);
Box box1;
Box box2;
Box box3;
Box box4;
Cylinder cylinder2(20, 20, 0.1, 0.1);// esfera de radio 0.5
Cylinder cylinder3(20, 20, 0.1, 0.1);// esfera de radio 0.5

float rot1 = 0.0, rot2 = 0.0, rot3 = 0.0, rot4 = 0.0;

bool exitApp = false;
int lastMousePosX, offsetX = 0;
int lastMousePosY, offsetY = 0;

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
	//inicializar los buffers VAO,VBO,EBO
	sphere1.init();
	//METODO SETTER QUE COLOCA EL APUNTADOR AL SHADER 
	sphere1.setShader(&shader);
	//SETTER PARA PONER EL COLOR DE LA GEOMETRIA
	sphere1.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

	//inicializar los buffers VAO,VBO,EBO
	sphere2.init();
	//METODO SETTER QUE COLOCA EL APUNTADOR AL SHADER 
	sphere2.setShader(&shader);
	//SETTER PARA PONER EL COLOR DE LA GEOMETRIA
	sphere2.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

	//inicializar los buffers VAO,VBO,EBO
	sphere3.init();
	//METODO SETTER QUE COLOCA EL APUNTADOR AL SHADER 
	sphere3.setShader(&shader);
	//SETTER PARA PONER EL COLOR DE LA GEOMETRIA
	sphere3.setColor(glm::vec4(0.0, 0.0, 0.0, 0.0));

	//inicializar los buffers VAO,VBO,EBO
	sphere4.init();
	//METODO SETTER QUE COLOCA EL APUNTADOR AL SHADER 
	sphere4.setShader(&shader);
	//SETTER PARA PONER EL COLOR DE LA GEOMETRIA
	sphere4.setColor(glm::vec4(0.0, 0.0, 0.0, 0.0));

	//inicializar los buffers VAO,VBO,EBO
	sphere5.init();
	//METODO SETTER QUE COLOCA EL APUNTADOR AL SHADER 
	sphere5.setShader(&shader);
	//SETTER PARA PONER EL COLOR DE LA GEOMETRIA
	sphere5.setColor(glm::vec4(0.0f, 1.0f, 1.0f, 1.0f));

	//inicializar los buffers VAO,VBO,EBO
	sphere6.init();
	//METODO SETTER QUE COLOCA EL APUNTADOR AL SHADER 
	sphere6.setShader(&shader);
	//SETTER PARA PONER EL COLOR DE LA GEOMETRIA
	sphere6.setColor(glm::vec4(0.0f, 1.0f, 1.0f, 1.0f));


	//inicializar los buffers VAO,VBO,EBO
	sphere7.init();
	//METODO SETTER QUE COLOCA EL APUNTADOR AL SHADER 
	sphere7.setShader(&shader);
	//SETTER PARA PONER EL COLOR DE LA GEOMETRIA
	sphere7.setColor(glm::vec4(1.0, 1.0, 0.0, 0.0f));

	cylinder1.init();
	cylinder1.setShader(&shader);
	cylinder1.setColor(glm::vec4(1.0, 1.0, 0.0, 0.0));
	//corbata
	cylinder2.init();
	cylinder2.setShader(&shader);
	cylinder2.setColor(glm::vec4(1.0, 0.0, 0.0, 0.0));
	//bob
	box1.init();
	box1.setShader(&shader);
	box1.setColor(glm::vec4(1.0, 1.0, 0.0, 0.0));
	//camisa blanca
	box2.init();
	box2.setShader(&shader);
	box2.setColor(glm::vec4((1.0, 1.0, 1.0, 1.0)));
	// boca 
	//inicializar los buffers VAO,VBO,EBO
	sphere8.init();
	//METODO SETTER QUE COLOCA EL APUNTADOR AL SHADER 
	sphere8.setShader(&shader);
	//SETTER PARA PONER EL COLOR DE LA GEOMETRIA
	sphere8.setColor(glm::vec4(1.0, 0.0, 0.0, 0.0));
	// dientes
	box4.init();
	box4.setShader(&shader);
	box4.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));
	// pestañas
	cylinder3.init();
	cylinder3.setShader(&shader);
	cylinder3.setColor(glm::vec4(0.0, 0.0, 0.0, 0.0));
	//pantalones
	box3.init();
	box3.setShader(&shader);
	box3.setColor(glm::vec4((0.1f, 0.0f, 0.0f, 0.0f)));
	camera->

		setPosition(glm::vec3(2.0, 0.0, 4.0));
}

void destroy() {
	glfwDestroyWindow(window);
	glfwTerminate();
	// --------- IMPORTANTE ----------
	// Eliminar los shader y buffers creados.
	// DESTRUCCION DE LOS VAO,EBO  Y VBO
	sphere1.destroy();
	cylinder1.destroy();
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

	glfwPollEvents();
	return continueApplication;
}

void applicationLoop() {
	bool psi = true;
	while (psi) {
		psi = processInput(true);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)screenWidth / (float)screenHeight, 0.01f, 100.0f);
		glm::mat4 view = camera->getViewMatrix();

		shader.turnOn();

		shader.setMatrix4("projection", 1, false, glm::value_ptr(projection));
		shader.setMatrix4("view", 1, false, glm::value_ptr(view));

		glm::mat4 model = glm::mat4(1.0f);
		// DIBUJADO DE LA GEOMETRIA Y RECIBE LA MATRIZ DE TANSFORMACION 
		//sphere2.enableWireMode(); // PARA VISUALIZAR CON LINEAS 
		//sphere2.render(model);


		/*cylinder1.render(model);
		cylinder1.enableWireMode();*/
		//box1.enableWireMode();// DIMESION CAJA 1.0

		box1.render(glm::scale(model, glm::vec3(1.0, 1.0, 0.1)));
		//camisa
		//box2.enableWireMode();// DIMESION CAJA 1.0
		glm::mat4 c2 = glm::translate(model, glm::vec3(0.0, -0.5, 0.01));
		box2.render(glm::scale(c2, glm::vec3(1.0, 0.04, 0.1)));
		//pantalon
		glm::mat4 c3 = glm::translate(c2, glm::vec3(0.0, -0.11, 0.0));
		box3.render(glm::scale(c3, glm::vec3(1.0, 0.25, 0.1)));

		glm::mat4 c4 = glm::translate(c3, glm::vec3(-0.3, -0.1, 0.0));
		box3.render(glm::scale(c4, glm::vec3(0.11, 0.13, 0.0)));

		glm::mat4 c5 = glm::translate(c3, glm::vec3(0.3, -0.1, 0.0));
		box3.render(glm::scale(c5, glm::vec3(0.11, 0.13, 0.0)));

		// pie derecho 
		glm::mat4 p1 = glm::translate(c5, glm::vec3(0.01, -0.19, 0.0));
		p1 = glm::rotate(p1, glm::radians(0.2f), glm::vec3(0.0, 0.0, 1.0));
		cylinder1.render(glm::scale(p1, glm::vec3(0.048, 0.25, 0.048)));
		// pie izquierdo
		glm::mat4 p2 = glm::translate(c4, glm::vec3(0.01, -0.19, 0.0));
		p2 = glm::rotate(p2, glm::radians(0.2f), glm::vec3(0.0, 0.0, 1.0));
		cylinder1.render(glm::scale(p2, glm::vec3(0.048, 0.25, 0.048)));




		//corbata
		glm::mat4 ci1 = glm::translate(model, glm::vec3(0.0, -0.5, 0.09));
		cylinder2.render(glm::scale(ci1, glm::vec3(0.1, 0.07, 0.1)));

		//articulacion mano derech hombro 
		glm::mat4 j1 = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));
		sphere1.render(glm::scale(j1, glm::vec3(0.1, 0.1, 0.1)));
		j1 = glm::rotate(j1, rot1, glm::vec3(0, 0, 1));
		j1 = glm::rotate(j1, rot2, glm::vec3(0.0, 1.0, 0.0));

		//para mano derecha
		glm::mat4 l1 = glm::translate(j1, glm::vec3(0.1f, 0.0, 0.0));
		l1 = glm::rotate(l1, glm::radians(90.0f), glm::vec3(0, 0, 1.0));
		cylinder1.render(glm::scale(l1, glm::vec3(0.05, 0.4, 0.05)));

		//articulacion mano derecha codo
		glm::mat4 j2 = glm::translate(l1, glm::vec3(0.2f, 0.0, 0.0));
		sphere7.render(glm::scale(j2, glm::vec3(0.07, 0.07, 0.1)));


		//mano derecha 2
		glm::mat4 l2 = glm::translate(j2, glm::vec3(0.01, -0.18, 0.0));
		l2 = glm::rotate(j2, glm::radians(0.2f), glm::vec3(0.0, 0.0, 1.0));
		cylinder1.render(glm::scale(l2, glm::vec3(0.048, 0.4, 0.048)));

		//articulacion mano izquierda hombro 
		glm::mat4 j3 = glm::translate(model, glm::vec3(-0.5f, 0.0f, 0.0f));
		sphere1.render(glm::scale(j3, glm::vec3(0.1, 0.1, 0.1)));

		//articulacion mano izquierda codo
		glm::mat4 j4 = glm::translate(model, glm::vec3(-0.8f, 0.0, 0.0));
		sphere7.render(glm::scale(j4, glm::vec3(0.07, 0.07, 0.1)));
		//para mano izquierda
		glm::mat4 l3 = glm::translate(j3, glm::vec3(-0.1f, 0.0, 0.0));
		l3 = glm::rotate(l3, glm::radians(90.0f), glm::vec3(0, 0, 1.0));
		cylinder1.render(glm::scale(l3, glm::vec3(0.05, 0.4, 0.05)));
		//mano derecha 2
		glm::mat4 l4 = glm::translate(j4, glm::vec3(-0.01, -0.18, 0.0));
		l4 = glm::rotate(l4, glm::radians(0.2f), glm::vec3(0.0, 0.0, 1.0));
		cylinder1.render(glm::scale(l4, glm::vec3(0.048, 0.4, 0.048)));


		//ojo 2
		glm::mat4 ojo = glm::translate(model, glm::vec3(0.15, 0.15, 0.05));
		//sphere2.enableWireMode();
		sphere2.render(glm::scale(ojo, glm::vec3(0.3, 0.3, 0.1)));

		//ojo2 pequeño parte negra
		glm::mat4 ojo1 = glm::translate(model, glm::vec3(0.15, 0.15, 0.09));
		//sphere3.enableWireMode();
		sphere3.render(glm::scale(ojo1, glm::vec3(0.08, 0.08, 0.05)));

		//ojo2 pequeño parte azul
		glm::mat4 ojo5 = glm::translate(model, glm::vec3(0.15, 0.15, 0.08));
		//sphere3.enableWireMode();
		sphere6.render(glm::scale(ojo5, glm::vec3(0.15, 0.15, 0.05)));

		//ojo1
		glm::mat4 ojo2 = glm::translate(model, glm::vec3(-0.15, 0.15, 0.05));
		//sphere1.enableWireMode();
		sphere1.render(glm::scale(ojo2, glm::vec3(0.3, 0.3, 0.1)));
		//ojo1 pequeño parte negra
		glm::mat4 ojo3 = glm::translate(model, glm::vec3(-0.15, 0.15, 0.09));
		//sphere3.enableWireMode();
		sphere4.render(glm::scale(ojo3, glm::vec3(0.08, 0.08, 0.05)));

		//ojo1 pequeño parte azul
		glm::mat4 ojo4 = glm::translate(model, glm::vec3(-0.15, 0.15, 0.08));
		//sphere3.enableWireMode();
		sphere5.render(glm::scale(ojo4, glm::vec3(0.15, 0.15, 0.05)));
		// boca 
		//sphere1.enableWireMode();

		glm::mat4 boca = glm::translate(model, glm::vec3(0.0, -0.2, 0.08));
		sphere8.render(glm::scale(boca, glm::vec3(0.2, 0.09, 0.0)));
		// dientes 
		glm::mat4 d1 = glm::translate(boca, glm::vec3(-0.02, 0.03, 0.0));
		box4.render(glm::scale(d1, glm::vec3(0.025, 0.03, 0.01)));

		glm::mat4 d2 = glm::translate(boca, glm::vec3(0.02, 0.03, 0.0));
		box4.render(glm::scale(d2, glm::vec3(0.025, 0.03, 0.01)));
		// Zapatos
		glm::mat4 z1 = glm::translate(p1, glm::vec3(0.0, -0.15, 0.0));
		box3.render(glm::scale(z1, glm::vec3(0.1, 0.05, 0.2)));

		glm::mat4 z2 = glm::translate(p2, glm::vec3(0.0, -0.15, 0.0));
		box3.render(glm::scale(z2, glm::vec3(0.1, 0.05, 0.2)));
		//nariz 
		glm::mat4 n1 = glm::translate(boca, glm::vec3(0.0f, 0.2, 0.05));
		cylinder1.render(glm::scale(n1, glm::vec3(0.05, 0.05, 0.2)));

		//pestañas
		glm::mat4 pe1 = glm::translate(ojo2, glm::vec3(0.0f, 0.16, 0.05));
		cylinder3.render(glm::scale(pe1, glm::vec3(0.05, 0.05, 0.2)));

		glm::mat4 pe2 = glm::translate(ojo2, glm::vec3(-0.06f, 0.155, 0.05));
		cylinder3.render(glm::scale(pe2, glm::vec3(0.05, 0.05, 0.2)));

		glm::mat4 pe3 = glm::translate(ojo2, glm::vec3(0.06f, 0.155, 0.05));
		cylinder3.render(glm::scale(pe3, glm::vec3(0.05, 0.05, 0.2)));

		// pestañas 2 
		glm::mat4 pe4 = glm::translate(ojo1, glm::vec3(0.0f, 0.16, 0.0));
		cylinder3.render(glm::scale(pe4, glm::vec3(0.05, 0.05, 0.2)));

		glm::mat4 pe5 = glm::translate(ojo1, glm::vec3(-0.06f, 0.15, 0.0));
		cylinder3.render(glm::scale(pe5, glm::vec3(0.05, 0.05, 0.2)));

		glm::mat4 pe6 = glm::translate(ojo1, glm::vec3(0.06f, 0.15, 0.0));
		cylinder3.render(glm::scale(pe6, glm::vec3(0.05, 0.05, 0.2)));


		shader.turnOff();

		glfwSwapBuffers(window);
	}
}

int main(int argc, char ** argv) {
	init(800, 700, "Window GLFW", false);
	applicationLoop();
	destroy();
	return 1;
}