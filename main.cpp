#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <chrono>
#include <unistd.h>
#include <sstream>

#include <opencv2/opencv.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


//Engine includes
#include "./Engine/Window.h"

#include "./Engine/Lights/DirectionalLight.h"
#include "./Engine/Lights/PointLight.h"
#include "./Engine/Lights/SpotLight.h"

#include "./Engine/Modeling/Material.h"
#include "./Engine/Modeling/Mesh.h"
#include "./Engine/Modeling/Model.h"
#include "./Engine/Modeling/Shader.h"
#include "./Engine/Modeling/Skybox.h"
#include "./Engine/Modeling/Texture.h"

#include "./Engine/Utils/CommonValues.h"

// Game
#include "./Game/Arena.h"
#include "./Game/Snake.h"
#include "./Game/Camera.h"
#include "./Game/GameValues.h"

#include "./kiwinn/GeneticPool.h"

extern GLfloat now,deltaTime,lastTime;

GLuint WIDTH = 1366;
GLuint HEIGHT = 768;

GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
uniformSpecularIntensity = 0, uniformShininess = 0, uniformOmniLightPos = 0, uniformFarPlane = 0;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Shader directionalShadowShader;
Shader omniShadowShader;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;

Material shinyMaterial;
Material dullMaterial;

//snake snake;
Model* model_snake;

Snake snakeNNs[game_snakenn_pool_size];
GeneticPool genom;
int nb_alive = 0;
Arena arena;

DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

Skybox skybox;

unsigned int pointLightCount = 0;
unsigned int spotLightCount = 0;

// Vertex Shader
static const char* vShader = "Shaders/shader.vert";

// Fragment Shader
static const char* fShader = "Shaders/shader.frag";

// Video output settings
const int FPS = 30; // Frames per second
const int SLEEP_TIME = 1000000/FPS;
const int FOURCC = cv::VideoWriter::fourcc('H','2','6','4'); // Codec to use

GLubyte* image = new GLubyte[WIDTH * HEIGHT * 3];
cv::VideoWriter writer;

// Function to capture a frame from the OpenGL window
void captureFrame() {
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glReadPixels(0, 0, WIDTH, HEIGHT, GL_BGR_EXT, GL_UNSIGNED_BYTE , image);

   // glReadPixels(0, 0, WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, image);
	cv::Mat frame(cv::Size(WIDTH, HEIGHT), CV_8UC3, image);
	//cv::InputArray inputArray(frame);
	cv::flip(frame,frame,0);

    // Write text on the image
    int font = cv::FONT_HERSHEY_SIMPLEX;
    double font_scale = .5;
    int thickness = 2;
    cv::Scalar color(255, 255, 255);  // white color

    std::string text = "Neuro-evolution - snake arena";
    cv::Size text_size = cv::getTextSize(text, font, font_scale, thickness, nullptr);
    int text_x = (frame.cols - text_size.width) / 2;
    int text_y = (text_size.height + text_size.height ) / 2;
    cv::putText(frame, text, cv::Point(text_x, text_y), font, font_scale, color, thickness);

	writer.write(frame);
}


void calcAverageNormals(unsigned int * indices, unsigned int indiceCount, GLfloat * vertices, unsigned int verticeCount, 
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
	genom.init(game_snakenn_pool_size, game_snakenn_data_size);

	model_snake = new Model();
	model_snake->LoadModel("snake/snake");

	for(size_t i = 0 ; i < game_snakenn_pool_size; i++)
	{
		snakeNNs[i] = Snake();
		snakeNNs[i].init(model_snake);
		snakeNNs[i].possess(genom.getDNA(i));
	}
	arena.init();
}

void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	directionalShadowShader = Shader();
	directionalShadowShader.CreateFromFiles("Shaders/directional_shadow_map.vert", "Shaders/directional_shadow_map.frag");
	omniShadowShader = Shader();
	omniShadowShader.CreateFromFiles("Shaders/omni_directional_shadow_map.vert", "Shaders/omni_directional_shadow_map.geom", "Shaders/omni_directional_shadow_map.frag");
}

void RenderScene()
{

	arena.render(uniformModel,uniformSpecularIntensity,uniformShininess);
	for(size_t i = 0 ; i < game_snakenn_pool_size; i++)
	{
		snakeNNs[i].render(uniformModel,uniformSpecularIntensity,uniformShininess);
	}

}

void DirectionalShadowMapPass(DirectionalLight* light)
{
	directionalShadowShader.UseShader();

	glViewport(0, 0, light->GetShadowMap()->GetShadowWidth(), light->GetShadowMap()->GetShadowHeight());

	light->GetShadowMap()->Write();
	glClear(GL_DEPTH_BUFFER_BIT);

	uniformModel = directionalShadowShader.GetModelLocation();
	directionalShadowShader.SetDirectionalLightTransform(light->CalculateLightTransform());

	directionalShadowShader.Validate();
	RenderScene();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OmniShadowMapPass(PointLight* light)
{
	glViewport(0, 0, light->GetShadowMap()->GetShadowWidth(), light->GetShadowMap()->GetShadowHeight());
	
	omniShadowShader.UseShader();
	uniformModel = omniShadowShader.GetModelLocation();
	uniformOmniLightPos = omniShadowShader.GetOmniLightPosLocation();
	uniformFarPlane = omniShadowShader.GetFarPlaneLocation();

	light->GetShadowMap()->Write();

	glClear(GL_DEPTH_BUFFER_BIT);

	glUniform3f(uniformOmniLightPos, light->GetPosition().x, light->GetPosition().y, light->GetPosition().z);
	glUniform1f(uniformFarPlane, light->GetFarPlane());
	omniShadowShader.SetOmniLightMatrices(light->CalculateLightTransform());

	omniShadowShader.Validate();
	RenderScene();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderPass(glm::mat4 projectionMatrix, glm::mat4 viewMatrix)
{
	glViewport(0, 0, WIDTH, HEIGHT);

	// Clear the window
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//skybox.DrawSkybox(viewMatrix,projectionMatrix);

	shaderList[0].UseShader();

	uniformModel = shaderList[0].GetModelLocation();
	uniformProjection = shaderList[0].GetProjectionLocation();
	uniformView = shaderList[0].GetViewLocation();
	uniformEyePosition = shaderList[0].GetEyePositionLocation();
	uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
	uniformShininess = shaderList[0].GetShininessLocation();


	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

	shaderList[0].SetDirectionalLight(&mainLight);
	shaderList[0].SetPointLights(pointLights, pointLightCount, 3, 0);
	shaderList[0].SetSpotLights(spotLights, spotLightCount, 3 + pointLightCount, pointLightCount);
	shaderList[0].SetDirectionalLightTransform(mainLight.CalculateLightTransform());

	mainLight.GetShadowMap()->Read(GL_TEXTURE2);


	shaderList[0].SetTexture(1);
	shaderList[0].SetDirectionalShadowMap(2);

	glm::vec3 lowerLight = camera.getCameraPosition();
	lowerLight.y -= 0.3f;
	spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

	shaderList[0].Validate();
	RenderScene();
}

void GameLoop(){
	if (CHECK_BIT(*mainWindow.getKeys(),GLFW_KEY_L))
	{
		spotLights[0].Toggle();
		mainWindow.Toggle(GLFW_KEY_L);
	}

	//snake.setbUp(CHECK_BIT(*mainWindow.getKeys(),GLFW_KEY_SPACE));
	
	if(arena.update()){
		for(size_t i = 0 ; i < game_snakenn_pool_size; i++)
		{
			snakeNNs[i].addScore(1.f);
		}
	}

	for(size_t i = 0 ; i < game_snakenn_pool_size; i++)
	{
		snakeNNs[i].update();

	}

	//check collisions
	nb_alive = 0;
	for(size_t i = 0 ; i < game_snakenn_pool_size; i++)
	{
		arena.checkCollision(&snakeNNs[i]);
		nb_alive+=!snakeNNs[i].isDead();
	}

	if(nb_alive < 1){
		genom.breed();
		for(size_t i = 0 ; i < game_snakenn_pool_size; i++)
		{
			snakeNNs[i].init(model_snake);
			snakeNNs[i].possess(genom.getDNA(i));
		}
		arena.init();
	}
}

int main() 
{

	mainWindow = Window(WIDTH, HEIGHT); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

    // Initialize OpenCV video writer
    writer.open("./media/output.mkv", FOURCC, FPS, cv::Size(WIDTH, HEIGHT), true);
    // writer.set(cv::VIDEOWRITER_PROP_QUALITY, 1.0);
    // writer.set(cv::VIDEOWRITER_PROP_QUALITY, 1.0);

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.f, 20.f, 0.f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, -90.0f, .0f, .0f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();

	shinyMaterial = Material(4.0f, 256);
	dullMaterial = Material(0.3f, 4);

	mainLight = DirectionalLight(2048, 2048,
								1.0f, 1.f, 1.f, 
								0.1f, 0.9f,
								-10.0f, -12.0f, 18.5f);

	pointLights[1] = PointLight(1024, 1024,
		0.1f, 100.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.4f,
		1.0f, 2.0f, 0.0f,
		0.3f, 0.2f, 0.1f);
//	pointLightCount++;
	pointLights[0] = PointLight(1024, 1024,
								0.1f, 100.0f, 
								0.0f, 1.0f, 0.0f,
								0.0f, 0.1f,
								-4.0f, 3.0f, 0.0f,
								0.3f, 0.2f, 0.1f);
//	pointLightCount++;

	
	spotLights[0] = SpotLight(1024, 1024,
						0.1f, 100.0f, 
						1.0f, 1.0f, 1.0f,
						0.2f, 2.0f,
						0.0f, 0.0f, 0.0f,
						0.0f, -1.0f, 0.0f,
						1.0f, 0.0f, 0.0f,
						20.0f);
//	spotLightCount++;
	spotLights[1] = SpotLight(1024, 1024,
		0.1f, 100.0f, 
		1.0f, 1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, -1.5f, 0.0f,
		-100.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		20.0f);
//	spotLightCount++;

	skybox = Skybox("cupertin-lake");

	glm::mat4 projection = glm::perspective(glm::radians(60.0f), (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	//glm::mat4 projection = glm::ortho(-5.f,5.f,-5.f,5.f,-5.f,5.f);

	// Loop until window closed
	while (!mainWindow.getShouldClose())
	{
		now = glfwGetTime(); // SDL_GetPerformanceCounter();
		deltaTime = now - lastTime; // (now - lastTime)*1000/SDL_GetPerformanceFrequency();
		lastTime = now;

		// Get + Handle User Input
		glfwPollEvents();

		camera.keyControl(mainWindow.getKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		GameLoop();

		DirectionalShadowMapPass(&mainLight);
		for (size_t i = 0; i < pointLightCount; i++)
		{
			OmniShadowMapPass(&pointLights[i]);
		}
		for (size_t i = 0; i < spotLightCount; i++)
		{
			OmniShadowMapPass(&spotLights[i]);
		}
		RenderPass(projection, camera.calculateViewMatrix());

		glUseProgram(0);

		mainWindow.swapBuffers();
	        // Write the frame to the video file
		captureFrame();

		int us = SLEEP_TIME - (glfwGetTime()-now)*1e6;
        if(us>0)usleep(us);
	}

	return 0;
}