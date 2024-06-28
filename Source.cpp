#include <iostream>
#include <chrono>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"


// Vertices coordinates

GLfloat vertices[] =
{ //     COORDINATES    //              
	-1.0f,  1.0f, 0.0f, // top left
	 1.0f,  1.0f, 0.0f, // top right
	-1.0f, -1.0f, 0.0f, // bottom left
	 1.0f, -1.0f, 0.0f  // bottom right
};

// Indices for vertices order
GLuint indices[] =
{
	0, 1, 2,
	1, 2, 3
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	//function to adjust height and width
	glViewport(0, 0, width, height);
}

bool keyPressed(GLFWwindow* window, int key)
{
	return glfwGetKey(window, key) == GLFW_PRESS;
}

/*
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	shaderProgram.Activate();

	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) 
	{
		GLfloat positionValue;
		glUniform1f(xPosUniformID, glGetUniformfv(shaderProgram.ID, glGetUniformLocation(shaderProgram.ID, "uniformName"), &positionValue));
	}
}
*/

int main()
{
	// Initialize GLFW
	glfwInit();

	int width = 800;
	int height = 800;

	// Tell GLFW what version of OpenGL we are using 
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a GLFWwindow object of width by height pixels, naming it "OpenGL"
	GLFWwindow* window = glfwCreateWindow(width, height, "OpenGL", NULL, NULL);
	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);
	// Sets the function to call when the window is resized
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//Load GLAD so it configures OpenGL
	gladLoadGL();

	// Generates Shader object using shaders defualt.vert and default.frag
	Shader shaderProgram("default.vert", "default.frag");

	// Generates Vertex Array Object and binds it
	VAO VAO1;
	VAO1.Bind();

	// Generates Vertex Buffer Object and links it to vertices. 
	// VBO and EBO objects are binded when the constructor is called
	VBO VBO1(vertices, sizeof(vertices));
	// Generates Element Buffer Object and links it to indices
	EBO EBO1(indices, sizeof(indices));

	// Links VBO attributes (position of vertices)
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);

	// Unbind all to prevent accidentally modifying them
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	// Gets ID of uniform called "iTime" and "iResolution"
	GLuint timeUniformID = glGetUniformLocation(shaderProgram.ID, "iTime");
	GLuint resolutionUniformID = glGetUniformLocation(shaderProgram.ID, "iResolution");

	GLuint xPosUniformID = glGetUniformLocation(shaderProgram.ID, "xPos");
	float xPosValue = 0.0f;
	GLuint yPosUniformID = glGetUniformLocation(shaderProgram.ID, "yPos");
	float yPosValue = 0.0f;
	GLuint zoomUniformID = glGetUniformLocation(shaderProgram.ID, "zoomFactor");
	float zoomFactorValue = 1.0f;
	GLuint maxIterUniformID = glGetUniformLocation(shaderProgram.ID, "maxIter");
	shaderProgram.Activate();
	glUniform1i(maxIterUniformID, 1000);
	glUniform1f(xPosUniformID, xPosValue);
	glUniform1f(yPosUniformID, yPosValue);
	glUniform1f(zoomUniformID, zoomFactorValue);

	int framebufferWidth, framebufferHeight;
	// Restart the GLFW timer and start main while loop 
	glfwSetTime(0.0);
	while (!glfwWindowShouldClose(window))
	{
		// Specify the color of the background
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		// Clean the back buffer and assign the new color to it
		glClear(GL_COLOR_BUFFER_BIT);
		// Tell OpenGL which Shader Program we want to use
		shaderProgram.Activate();
		// Bind the VAO so OpenGL knows to use it
		VAO1.Bind();

		// Assigns a value to the iTime and iResolution uniforms; NOTE: Must always be done after activating the Shader Program
		glUniform1f(timeUniformID, float(glfwGetTime()));
		glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
		glUniform2f(resolutionUniformID, GLfloat(framebufferWidth), GLfloat(framebufferHeight));

		// Draw primitives, number of indices, datatype of indices, index of indices
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();

		if (keyPressed(window, GLFW_KEY_RIGHT)) 
		{
			xPosValue += 2.24f / 1000.0f * zoomFactorValue;
			glUniform1f(xPosUniformID, xPosValue);
		}
		if (keyPressed(window, GLFW_KEY_LEFT))
		{
			xPosValue -= 2.24f / 1000.0f * zoomFactorValue;
			glUniform1f(xPosUniformID, xPosValue);
		}
		if (keyPressed(window, GLFW_KEY_UP))
		{
			yPosValue += 2.24f / 1000.0f * zoomFactorValue;
			glUniform1f(yPosUniformID, yPosValue);
		}
		if (keyPressed(window, GLFW_KEY_DOWN))
		{
			yPosValue -= 2.24f / 1000.0f * zoomFactorValue;
			glUniform1f(yPosUniformID, yPosValue);
		}
		if (keyPressed(window, GLFW_KEY_W))
		{
			zoomFactorValue *= 0.999f;
			zoomFactorValue = std::max(0.0000001f, zoomFactorValue);
			glUniform1f(zoomUniformID, zoomFactorValue);
		}
		if (keyPressed(window, GLFW_KEY_S))
		{
			zoomFactorValue /= 0.999f;
			zoomFactorValue = std::min(3.0f, zoomFactorValue);
			glUniform1f(zoomUniformID, zoomFactorValue);
		}
		
	}



	// Delete all the objects we've created
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shaderProgram.Delete();

	// Delete window before ending the program
	glfwDestroyWindow(window);

	// Terminate GLFW before ending the program
	glfwTerminate();

	return 0;
}
