#include <iostream>
#include <cmath>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models
#include "SOIL2/SOIL2.h"


// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
//objetos creados
#include "Carro.h"
#include "Tren.h"
#include "Silla.h"
#include "Mesa.h"
#include "pikachu.h"
#include "stand.h"
#include "persona.h"
// Function prototypes
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow *window, double xPos, double yPos);
void DoMovement();
void Animation();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(15.0f, 1.0f, 15.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(0.0f,2.0f, 0.0f),
	glm::vec3(0.0f,0.0f, 0.0f),
	glm::vec3(0.0f,0.0f,  0.0f),
	glm::vec3(0.0f,0.0f, 0.0f)
};



float vertices[] = {
    // Posiciones          // Normales           // UVs
    // Frente
    -0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   0.0f, 0.0f,
    // Atrás
    -0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   1.0f, 0.0f,
     0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
     0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   0.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   1.0f, 0.0f,
    // Izquierda
    -0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,   1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,   1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,   0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,   1.0f, 0.0f,
    // Derecha
     0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,   1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,   1.0f,  0.0f,  0.0f,   1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,   1.0f,  0.0f,  0.0f,   0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,   1.0f, 0.0f,
    // Abajo
    -0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,   0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,   1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,   1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,   1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,   0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,   0.0f, 1.0f,
    // Arriba
    -0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,   0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,   1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,   0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,   0.0f, 1.0f
};



glm::vec3 Light1 = glm::vec3(0);

//declaracion de objetos
Carro CarroAzul(glm::vec3(37.0f, 0.050f, 13.0f));
bool AnimCarro = false;

Tren TrenRojo(glm::vec3(-1.0f, 0.0f, -26.0f));
bool AnimTren = false;

Silla SillaPlegable(glm::vec3(6.8f, 0.050f, -0.53));
bool AnimSilla = false;      // Estado de animación de la silla
bool SillaAbierta = false;   // Estado actual de la silla (abierta/cerrada)
bool AnimacionEnProgreso = false;

Mesa MesaPlegable(glm::vec3(6.8f, 0.74f, -1.8f));
bool AnimMesa = false;           // Estado de animación de la mesa
bool MesaAbierta = false;        // Estado actual de la mesa (abierta/cerrada)
bool AnimacionMesaEnProgreso = false;  // Para evitar múltiples activaciones

Stand stand1(glm::vec3(6.8f, 0.03f, -1.5f));

Pikachu botarga(glm::vec3(19.0f, 0.050f, 2.7f));

Persona persona1(glm::vec3(6.8f, 0.13f, -1.1f));
Persona personaCaminando(glm::vec3(14.0f, 0.13f, 1.0f));
bool animPersonaCaminando = false;
int estadoCaminando = 0; // 0: Reposo, 1: Pierna Der. Adelante, 2: Pierna Izq. Adelante
float velocidadCaminata = 80.0f; // Velocidad de rotación de las extremidades
float limitePaso = 35.0f; // Ángulo máximo que pueden alcanzar las piernas
int direccionRuta = 0;

//Silla silla2(glm::vec3(0.0f, 0.050f, 0.3f));
//Mesa mesa2(glm::vec3(0.0f, 0.74f, -0.5f));
//Stand stand2(glm::vec3(0.0f, 0.03f, 0.0f));

// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

int main()
{
	// Init GLFW
	glfwInit();
	
	// Set all the required options for GLFW
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Sotano edificio A", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	// GLFW Options
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}
	//inicializamos objetos
	CarroAzul.Inicializar();
	TrenRojo.Inicializar();
	SillaPlegable.Inicializar();
	MesaPlegable.Inicializar();
	botarga.Inicializar();
	stand1.Inicializar();
	persona1.Inicializar();
	personaCaminando.Inicializar();
	/*silla2.Inicializar();
	mesa2.Inicializar();
	stand2.Inicializar();*/
	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);



	Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
	Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");
	
	//models
	Model Sotano((char*)"Models/SotanoA.obj");

	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// 3. Atributo de Coordenadas de Textura
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// Set texture units
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.difuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.specular"), 1);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);
	
	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{

		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();
		Animation();

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	   
		// OpenGL options
		glEnable(GL_DEPTH_TEST);

		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();

        glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "specular"),1);

		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"),0.6f,0.6f,0.6f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.6f, 0.6f, 0.6f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"),0.3f, 0.3f, 0.3f);


		// Point light 1
	    glm::vec3 lightColor;
		lightColor.x= abs(sin(glfwGetTime() *Light1.x));
		lightColor.y= abs(sin(glfwGetTime() *Light1.y));
		lightColor.z= sin(glfwGetTime() *Light1.z);

		
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lightColor.x,lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lightColor.x,lightColor.y,lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 0.2f, 0.2f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.045f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"),0.075f);


		// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.2f, 0.2f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.2f, 0.2f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.3f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.7f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.0f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(18.0f)));
		

		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 5.0f);

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();

		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


		glm::mat4 model(1);
		
		//Carga de modelo 
        view = camera.GetViewMatrix();	
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Sotano.Draw(lightingShader);
		

		//Dibujar modelos 
		CarroAzul.rotacion = glm::vec3(0.0f, 180.0f, 0.0f);
		CarroAzul.escala = glm::vec3(1.1f, 1.1f, 1.1f);
		CarroAzul.Draw(lightingShader, VAO);
		
		TrenRojo.escala = glm::vec3(3.0f,2.2f,2.5f);
		TrenRojo.Draw(lightingShader, VAO);
		
		SillaPlegable.rotacion = glm::vec3(0.0f, 180.0f, 0.0f);
		SillaPlegable.Draw(lightingShader, VAO);
		
		MesaPlegable.Draw(lightingShader, VAO);
		
		botarga.escala = glm::vec3(0.47f, 0.47f, 0.47f);
		botarga.Draw(lightingShader, VAO);
		
		stand1.rotacion = glm::vec3(0.0f, 180.0f, 0.0f);
		stand1.escala = glm::vec3(1.0f, 0.7f, 1.0f);
		stand1.Draw(lightingShader, VAO);
		
		persona1.rotacion = glm::vec3(0.0f, 180.0f, 0.0f);
		persona1.escala = glm::vec3(0.42f, 0.42f, 0.42f);
		persona1.Draw(lightingShader, VAO);

		//float desplazamientoX = 5.0f; // Espacio entre cada set

		//for (int i = 0; i < 4; i++) {
		//	glm::mat4 matrizSet = glm::mat4(1.0f);

		//	// Posicionamos cada set en el eje X
		//	matrizSet = glm::translate(matrizSet, glm::vec3(i * desplazamientoX, 0.0f, -5.0f));

		//	// Lógica para cambiar el tamaño
		//	if (i >= 2) {
		//		matrizSet = glm::scale(matrizSet, glm::vec3(0.6f, 0.7f, 0.6f));
		//	}

		//	// Dibujamos usando la matriz padre
		//	stand2.Draw(lightingShader, VAO, matrizSet);
		//	mesa2.Draw(lightingShader, VAO, matrizSet);
		//	silla2.anguloPlegado = 180.0f;
		//	silla2.Draw(lightingShader, VAO, matrizSet);
		//}
		personaCaminando.rotacion = glm::vec3(0.0f, 180.0f, 0.0f);
		personaCaminando.escala = glm::vec3(0.42f, 0.42f, 0.42f);
		personaCaminando.Draw(lightingShader, VAO);

		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();

		
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// Draw the light object (using light's vertex attributes)
		
			model = glm::mat4(1);
			model = glm::translate(model, pointLightPositions[0]);
			model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		
		glBindVertexArray(0);
		// Swap the screen buffers
		glfwSwapBuffers(window);
	}


	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();



	return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement()
{

	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);

	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);


	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);


	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);


	}

	
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}

	if (keys[GLFW_KEY_SPACE])
	{
		active = !active;
		if (active)
		{
			Light1 = glm::vec3(1.0f, 1.0f, 0.0f);
			
		}
		else
		{
			Light1 = glm::vec3(0);//Cuado es solo un valor en los 3 vectores pueden dejar solo una componente
		}
	}
	
	if (keys[GLFW_KEY_M])
	{
		AnimCarro = !AnimCarro;
	}

	if (keys[GLFW_KEY_O] && !AnimacionEnProgreso)
	{
		AnimSilla = !AnimSilla;// Alterna entre abrir y cerrar
		SillaAbierta = !SillaAbierta;
		AnimacionEnProgreso = true;
	}
	else if (!keys[GLFW_KEY_O])
	{
		AnimacionEnProgreso = false;
	}

	if (keys[GLFW_KEY_L] && !AnimacionMesaEnProgreso)
	{
		AnimMesa = !AnimMesa;        // Cambia el estado de animación
		MesaAbierta = !MesaAbierta;   // Cambia el estado de la mesa
		AnimacionMesaEnProgreso = true;
	}
	else if (!keys[GLFW_KEY_L])
	{
		AnimacionMesaEnProgreso = false;
	}
	if (key == GLFW_KEY_C && action == GLFW_PRESS) {
		animPersonaCaminando = !animPersonaCaminando;
	}
}
void Animation() {
	
	if (AnimCarro)
	{
		// Las llantas rotan sobre su propio eje
		CarroAzul.rotacionLlantas -= 100.0f * deltaTime;
		// El carro avanza sobre el eje X
		CarroAzul.avance += 2.0f * deltaTime;
	}
	if (AnimMesa) {
		// Abrir mesa (0° a 180°)
		if (MesaAbierta && MesaPlegable.anguloApertura < 180.0f) {
			MesaPlegable.anguloApertura += 60.0f * deltaTime;
		}
		// Cerrar mesa (180° a 90°)
		else if (!MesaAbierta && MesaPlegable.anguloApertura > 90.0f) {
			MesaPlegable.anguloApertura -= 60.0f * deltaTime;
		}
		else {
			AnimMesa = false;  // Termina animación
		}
	}
	if (AnimSilla) {
		// Si la silla está abierta, ciérrala
		if (SillaAbierta && SillaPlegable.anguloPlegado < 80.0f) {
			SillaPlegable.anguloPlegado += 50.0f * deltaTime;
		}
		// Si la silla está cerrada, ábrela
		else if (!SillaAbierta && SillaPlegable.anguloPlegado > 0.0f) {
			SillaPlegable.anguloPlegado -= 50.0f * deltaTime;
		}
		// Cuando termina la animación, desactiva AnimSilla
		else if ((SillaAbierta && SillaPlegable.anguloPlegado >= 80.0f) ||
			(!SillaAbierta && SillaPlegable.anguloPlegado <= 0.0f)) {
			AnimSilla = false;
		}
	}
	//Caminata
	if (animPersonaCaminando) {
		// Si acaba de iniciar, pasamos del estado de reposo (0) al primer movimiento (1)
		if (estadoCaminando == 0) {
			estadoCaminando = 1;
		}

		// Pierna derecha adelante, pierna izquierda atrás
		if (estadoCaminando == 1) {
			personaCaminando.rotPiernaDerX += velocidadCaminata * deltaTime;
			personaCaminando.rotPiernaIzqX -= velocidadCaminata * deltaTime;

			// Los brazos hacen el movimiento contrario a las piernas
			personaCaminando.rotHombroDerX -= velocidadCaminata * deltaTime;
			personaCaminando.rotHombroIzqX += velocidadCaminata * deltaTime;

			// Condición para cambiar de estado (Llegó al límite del paso)
			if (personaCaminando.rotPiernaDerX >= limitePaso) {
				estadoCaminando = 2; // Cambiamos al movimiento contrario
			}
		}
		// Pierna izquierda adelante, pierna derecha atrás
		else if (estadoCaminando == 2) {
			personaCaminando.rotPiernaDerX -= velocidadCaminata * deltaTime;
			personaCaminando.rotPiernaIzqX += velocidadCaminata * deltaTime;

			// Los brazos hacen el movimiento contrario
			personaCaminando.rotHombroDerX += velocidadCaminata * deltaTime;
			personaCaminando.rotHombroIzqX -= velocidadCaminata * deltaTime;

			// Condición para regresar al primer estado
			if (personaCaminando.rotPiernaDerX <= -limitePaso) {
				estadoCaminando = 1;
			}
		}

		//DESPLAZAMIENTO
		if (direccionRuta == 0) {
			// Avanza hacia -Z
			personaCaminando.posicion.z -= 1.5f * deltaTime;
			personaCaminando.rotacion.y = 0.0f; // Orientación original

			// Llegó a la coordenada límite en -Z?
			if (personaCaminando.posicion.z <= -10.0f) { 
				direccionRuta = 1; // Cambiamos el estado para que comience a regresar
			}
		}
		else if (direccionRuta == 1) {
			// Regresa hacia +Z
			personaCaminando.posicion.z += 1.5f * deltaTime;

			// Le damos la media vuelta al modelo para que no camine de espaldas
			personaCaminando.rotacion.y = -180.0f;

			// Regresó a su punto de origen?
			if (personaCaminando.posicion.z >= 0.0f) { // <-- LÍMITE DE ORIGEN
				direccionRuta = 0; // Reiniciamos el ciclo para que vuelva a ir hacia -Z
			}
		}
		//vercion 1 de caminata: personaCaminando.posicion.z -= 1.5f * deltaTime;
		
		// Flexionar ligeramente las rodillas/codos mientras camina 
		personaCaminando.rotRodillaDer = 15.0f; // Pequeña flexión estática
		personaCaminando.rotRodillaIzq = 15.0f;
		personaCaminando.rotCodoDer = -15.0f;
		personaCaminando.rotCodoIzq = -15.0f;

	}
	else {
		// Cuando se desactiva la animación, mandamos un cero en cada una de sus componentes (reposo)
		estadoCaminando = 0;
		personaCaminando.rotPiernaDerX = 0.0f;
		personaCaminando.rotPiernaIzqX = 0.0f;
		personaCaminando.rotHombroDerX = 0.0f;
		personaCaminando.rotHombroIzqX = 0.0f;

		personaCaminando.rotRodillaDer = 0.0f;
		personaCaminando.rotRodillaIzq = 0.0f;
		personaCaminando.rotCodoDer = 0.0f;
		personaCaminando.rotCodoIzq = 0.0f;
	}
}

void MouseCallback(GLFWwindow *window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}