#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <shader/shader_m.h>
#include <camera/camera_forwardback.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
// shaders
std::string strlocalpath = "F:/Code/OpenGL/projects/virtualcorridor/";
std::string str_vs = strlocalpath + "vertex_shader.vs";
const char* vsf = str_vs.c_str();
std::string str_fs = strlocalpath + "fragment_shader.fs";
const char* fsf = str_fs.c_str();
// textures
std::string str_walltex = strlocalpath + "textures/walltexture.jpg";
const char* walltex = str_walltex.c_str();
std::string str_skytex = strlocalpath + "textures/skytexture.jpg";
const char* skytex = str_skytex.c_str();
std::string str_groundtex = strlocalpath + "textures/groundtexture.jpg";
const char* groundtex = str_groundtex.c_str();

//callbacks
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

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

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	};
	// world space positions of our cubes
	glm::vec3 rectPositions[] = {
		glm::vec3(-1.5f,  0.3f,  0.0f),//left wall
		glm::vec3(1.5f,  0.3f, 0.0f),//right wall
		glm::vec3(0.0f, -0.4f, 0.0f),//ground

		glm::vec3(0.0f,  1.0f, -1.0f),
		glm::vec3(0.0f,  1.0f, -10.0f),
		glm::vec3(0.0f,  1.0f, -20.0f),
		glm::vec3(0.0f,  1.0f, -30.0f),

		glm::vec3(0.0f,  1.0f, -80.0), //background
	};
	unsigned int VBO, VAO_Wall,VAO_Ground,VAO_Image;

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


	// load and create a texture 
	// -------------------------
	unsigned int texGround, texSky, texWall;
	// texture 1
	// ---------
	glGenTextures(1, &texGround);
	glBindTexture(GL_TEXTURE_2D, texGround);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char* data = stbi_load(groundtex, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	
	// texture 2
	// ---------
	glGenTextures(1, &texWall);
	glBindTexture(GL_TEXTURE_2D, texWall);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	data = stbi_load(walltex, &width, &height, &nrChannels, 0);
	if (data)
	{
		// note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// texture 3
	// ---------
	glGenTextures(1, &texSky);
	glBindTexture(GL_TEXTURE_2D, texSky);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	data = stbi_load(skytex, &width, &height, &nrChannels, 0);
	if (data)
	{
		// note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
	// -------------------------------------------------------------------------------------------
	ourShader.use();
	ourShader.setInt("texGround", 0);
	ourShader.setInt("texWall", 1);
	ourShader.setInt("texSky", 2);


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
			else if (i == (rectnum-1)) //background
			{
				// bind vertices
				glBindVertexArray(VAO_Ground);

				// bind textures on corresponding texture units
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, texSky);

				model = glm::translate(model, rectPositions[i]);
				model = glm::scale(model, glm::vec3(1000.0, 1000.0, 1.0));
			}
			else //scene
			{
				// bind vertices
				glBindVertexArray(VAO_Image);

				// bind textures on corresponding texture units
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, texSky);
				
				model = glm::translate(model, rectPositions[i]);
				model = glm::scale(model, glm::vec3(16.0/4.6, 9.0/4.6, 1.0));
			}
			ourShader.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

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
	//if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	//	camera.ProcessKeyboard(LEFT, deltaTime);
	//if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	//	camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}
