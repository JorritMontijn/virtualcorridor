#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <shader/shader_m.h>
//#include <camera/camera_forwardback.h>
#include <camera/camera_forwardback.h>
#include <vector>

#include <iostream>

using namespace std;

// define functions
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
unsigned int loadTexture(const char* path);
unsigned int loadCubemap(vector<std::string> faces);


// settings
const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 900;
// shaders
std::string strlocalpath = "F:/Code/OpenGL/projects/virtualcorridor/";
std::string str_vs = strlocalpath + "vertex_shader.vs";
const char* vsf = str_vs.c_str();
std::string str_fs = strlocalpath + "fragment_shader.fs";
const char* fsf = str_fs.c_str();
std::string str_vss = strlocalpath + "skybox.vs";
const char* vs_skybox = str_vss.c_str();
std::string str_fss = strlocalpath + "skybox.fs";
const char* fs_skybox = str_fss.c_str();

// textures
std::string str_walltex = strlocalpath + "textures/walltexture.jpg";
const char* walltex = str_walltex.c_str();
std::string str_skytex = strlocalpath + "textures/skytexture.jpg";
const char* skytex = str_skytex.c_str();
std::string str_groundtex = strlocalpath + "textures/groundtexture.jpg";
const char* groundtex = str_groundtex.c_str();

// skybox
std::string str_back = strlocalpath + "textures/skybox/back.jpg";
const char* backtex = str_back.c_str();
std::string str_bottom = strlocalpath + "textures/skybox/bottom.jpg";
const char* bottomtex = str_bottom.c_str();
std::string str_front = strlocalpath + "textures/skybox/front.jpg";
const char* fronttex = str_front.c_str();
std::string str_left = strlocalpath + "textures/skybox/left.jpg";
const char* lefttex = str_left.c_str();
std::string str_right = strlocalpath + "textures/skybox/right.jpg";
const char* righttex = str_right.c_str();
std::string str_top = strlocalpath + "textures/skybox/top.jpg";
const char* toptex = str_top.c_str();

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile our shader zprogram
	// ------------------------------------
	Shader ourShader(vsf, fsf);
	Shader skyboxShader(vs_skybox, fs_skybox);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float verticesWall[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  100.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  100.0f, 0.5f,
		 0.5f,  0.5f, -0.5f,  100.0f, 0.5f,
		-0.5f,  0.5f, -0.5f,  0.0f, 0.5f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	};
	float verticesGround[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  100.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  100.0f, 100.0f,
		 0.5f,  0.5f, -0.5f,  100.0f, 100.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 100.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	};
	float verticesImages[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	};
	float skyboxdist = 80.0f;
	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};
	for (unsigned int i = 0; i < (sizeof(skyboxVertices)/ sizeof(float)); i++)
	{
		skyboxVertices[i] *= skyboxdist;
	}

	// world space positions of our cubes
	glm::vec3 rectPositions[] = {
		glm::vec3(-1.5f,  0.3f,  0.0f),//left wall
		glm::vec3(1.5f,  0.3f, 0.0f),//right wall
		glm::vec3(0.0f, -0.4f, 0.0f),//ground

		glm::vec3(0.0f,  1.0f, -10.0f),
		glm::vec3(0.0f,  1.0f, -20.0f),
		glm::vec3(0.0f,  1.0f, -30.0f),
		glm::vec3(0.0f,  1.0f, -40.0f),

		//glm::vec3(0.0f,  1.0f, -80.0), //background
	};
	unsigned int VBO, VAO_Wall, VAO_Ground, VAO_Image;

	//wall vertices
	glGenVertexArrays(1, &VAO_Wall);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO_Wall);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesWall), verticesWall, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//ground vertices
	glGenVertexArrays(1, &VAO_Ground);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO_Ground);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesGround), verticesGround, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//image vertices
	glGenVertexArrays(1, &VAO_Image);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO_Image);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesImages), verticesImages, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// skybox VAO
	unsigned int skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	// load and create a texture 
	// -------------------------
	unsigned int texGround, texSky, texWall;
	// texture 1
	// ---------
	texGround = loadTexture(groundtex);

	// texture 2
	// ---------
	texSky = loadTexture(skytex);

	// texture 3
	// ---------
	texWall = loadTexture(walltex);

	// tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
	// -------------------------------------------------------------------------------------------
	ourShader.use();
	ourShader.setInt("texGround", 0);
	ourShader.setInt("texWall", 1);
	ourShader.setInt("texSky", 2);

	// 4 images
	std::string str_stim;
	std::string str_stimtex;
	std::vector<unsigned int> texStim = { 0,0,0,0};
	for (unsigned int i = 0; i < texStim.size(); i++)
	{
		str_stim = strlocalpath + "textures/stimuli/stim" + std::to_string(i+1) + ".jpg";
		const char* stimtex = str_stim.c_str();
		texStim[i] = loadTexture(stimtex);
		str_stimtex = "texStim[" + std::to_string(i) + "]";
		ourShader.setInt(str_stimtex, i+3);
	}
	
	// tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
	// -------------------------------------------------------------------------------------------
	
	vector<std::string> faces
	{
		righttex,
		lefttex,
		toptex,
		bottomtex,
		fronttex,
		backtex
	};
	unsigned int cubemapTexture = loadCubemap(faces);

	// shader configuration
	// --------------------
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);


	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// activate shader
		ourShader.use();

		// pass projection matrix to shader (note that in this case it could change every frame)
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		ourShader.setMat4("projection", projection);

		// camera/view transformation
		glm::mat4 view = camera.GetViewMatrix();
		ourShader.setMat4("view", view);


		// render rectangles
		const int rectnum = sizeof(rectPositions) / (sizeof(float) * 3);
		//std::cout << rectnum << std::endl;
		for (unsigned int i = 0; i < rectnum; i++)
		{

			// calculate the model matrix for each object and pass it to shader before drawing
			glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
			if (i == 0) //left wall
			{
				// bind vertices
				glBindVertexArray(VAO_Wall);

				// bind textures on corresponding texture units
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, texWall);
				//set location
				model = glm::translate(model, rectPositions[i]);
				float angle = 90.0f;
				model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
				model = glm::scale(model, glm::vec3(1000.0, 1.0, 0.5));
			}
			else if (i == 1) //right wall
			{
				// bind vertices
				glBindVertexArray(VAO_Wall);

				// bind textures on corresponding texture units
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, texWall);
				//set location
				model = glm::translate(model, rectPositions[i]);
				float angle = -90.0f;
				model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
				model = glm::scale(model, glm::vec3(1000.0, 1.0, 0.5));
			}
			else if (i == 2) //ground
			{
				// bind vertices
				glBindVertexArray(VAO_Ground);

				// bind textures on corresponding texture units
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, texGround);
				//set location
				model = glm::translate(model, rectPositions[i]);
				float angle = 90.0f;
				model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::scale(model, glm::vec3(1000.0, 1000.0, 0.5));
			}
			/*else if (i == (rectnum - 1)) //background
			{
				// bind vertices
				glBindVertexArray(VAO_Ground);

				// bind textures on corresponding texture units
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, texSky);

				model = glm::translate(model, rectPositions[i]);
				model = glm::scale(model, glm::vec3(1000.0, 1000.0, 1.0));
			}*/
			else //scene
			{
				
				// bind vertices
				glBindVertexArray(VAO_Image);

				// bind textures on corresponding texture units
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, texStim[i-3]);

				model = glm::translate(model, rectPositions[i]);
				model = glm::scale(model, glm::vec3(16.0 / 4.6, 9.0 / 4.6, 1.0));
				
			}
			ourShader.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// draw skybox as last
		glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
		skyboxShader.use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
		skyboxShader.setMat4("view", view);
		skyboxShader.setMat4("projection", projection);
		// skybox cube
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // set depth function back to default

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO_Wall);
	glDeleteVertexArrays(1, &VAO_Ground);
	glDeleteVertexArrays(1, &VAO_Image);
	glDeleteBuffers(1, &VBO);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 3);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);

		std::cout << "Texture [w h c] is " << width << " " << height << " " << nrComponents << std::endl;
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

// loads a cubemap texture from 6 individual texture faces
// order:
// +X (right)
// -X (left)
// +Y (top)
// -Y (bottom)
// +Z (front) 
// -Z (back)
// -------------------------------------------------------
unsigned int loadCubemap(vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}