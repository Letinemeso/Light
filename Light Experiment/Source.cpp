#include <glew.h>
#include <glfw3.h>

#include <thread>
#include <chrono>

#include <iostream>

int main() {
	glfwInit();

	GLFWwindow* wind = glfwCreateWindow(400, 400, "light simulation", 0, 0);
	glfwMakeContextCurrent(wind);

	glfwSetWindowSizeLimits(wind, 400, 400, 400, 400);

	glewInit();

	const char* vshader =
		"#version 330 core\n"
		"layout (location = 0) in vec4 coords;\n"
		"layout (location = 1) in vec3 color;\n"
		"out vec4 outColor;\n"
		"void main(){"
		"outColor = vec4(color, 1.0f);\n"
		"gl_Position = coords;\n}";

	const char* fshader =
		"#version 330 core\n"
		"in vec4 outColor;\n"
		"out vec4 clr;\n"
		"void main(){\n"
		"clr = outColor;\n}";

	unsigned int vsh = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vsh, 1, &vshader, 0);
	glCompileShader(vsh);


	unsigned int fsh = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fsh, 1, &fshader, 0);
	glCompileShader(fsh);


	unsigned int program = glCreateProgram();
	glAttachShader(program, vsh);
	glAttachShader(program, fsh);
	glLinkProgram(program);
	glUseProgram(program);

	//////////
	/////////
	////////
	///////		SHADER COMPILING COMPLETE
	//////
	/////
	////
	///
	//
	

	std::cout << "\tWelcome to light simulation by .Snare\n\n"
		<< "There is a small square in top left corner which brightness\ndepends on distance between it and yellow square,\nwhich is in top right corner.\n\n"
		<< "You can control first (white) square with WASD buttons and\nanother (yellow) with arrow buttons.\n";

	//FIRST OBJECT
	float coords[] = {
		-0.8f, 0.7f, 0.0f, 1.0f,
		-0.7f, 0.7f, 0.0f, 1.0f,
		-0.7f, 0.8f, 0.0f, 1.0f,
		-0.8f, 0.8f, 0.0f, 1.0f
	};
	float fObjCenterX = -0.75f;
	float fObjCenterY = 0.75f;

	float standartColors[] = {
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f
	};

	float colors[] = {
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f
	};

	//SECOND OBJECT
	float coords2[] = {
		0.7f, 0.7f, 0.0f, 1.0f,
		0.8f, 0.7f, 0.0f, 1.0f,
		0.8f, 0.8f, 0.0f, 1.0f,
		0.7f, 0.8f, 0.0f, 1.0f
	};
	float sObjCenterX = 0.75f;
	float sObjCenterY = 0.75f;

	float colors2[] = {
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
	};


	//FIRST OBJECT INITIALIZATION

	unsigned int vertexArray;
	glGenVertexArrays(1, &vertexArray);

	unsigned int vertexBuffer;
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 16, coords, GL_STATIC_DRAW);

	glBindVertexArray(vertexArray);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);

	unsigned int colorBuffer;
	glGenBuffers(1, &colorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 16, colors, GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	//SECOND OBJECT INITIALIZATION

	unsigned int va2;
	glCreateVertexArrays(1, &va2);

	unsigned int vb2, cb2;

	glGenBuffers(1, &vb2);
	glGenBuffers(1, &cb2);

	glBindBuffer(GL_ARRAY_BUFFER, vb2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 16, coords2, GL_STATIC_DRAW);

	glBindVertexArray(va2);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);

	glBindBuffer(GL_ARRAY_BUFFER, cb2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 16, colors2, GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	float minDist = 0.2f, maxDist = 1.0f;
	float lightMult = 1.0f;


	while (!glfwWindowShouldClose(wind)) {
		glClear(GL_COLOR_BUFFER_BIT);

		//FIRST OBJECT CONTROLS
		if (glfwGetKey(wind, GLFW_KEY_D)) {
			if (fObjCenterX < 0.95f) {
				if ((abs(fObjCenterY - sObjCenterY) >= 0.08f) || (abs(fObjCenterX - sObjCenterX) >= 0.1f) || (fObjCenterX > sObjCenterX)) {
					for (int i = 0; i < 16; i += 4) {
						coords[i] += 0.01f;
					}
					fObjCenterX += 0.01f;
				}
			}
		}
		if (glfwGetKey(wind, GLFW_KEY_A)) {
			if (fObjCenterX > -0.95f) {
				if ((abs(fObjCenterY - sObjCenterY) >= 0.08f) || (abs(fObjCenterX - sObjCenterX) >= 0.1f) || (fObjCenterX < sObjCenterX)) {
					//std::cout << fObjCenterX << " " << sObjCenterX << "\n";
					for (int i = 0; i < 16; i += 4) {
						coords[i] -= 0.01f;
					}
					fObjCenterX -= 0.01f;
				}
			}
		}
		//std::cout << ((abs(fObjCenterY - sObjCenterY) >= 0.1f) ? (abs(fObjCenterY - sObjCenterY)) : (abs(fObjCenterY - sObjCenterY))) << "\t" << ((abs(fObjCenterX - sObjCenterX) >= 0.1f) ? "2 true\n" : "2 false\n");
		if (glfwGetKey(wind, GLFW_KEY_W)) {
			if (fObjCenterY < 0.95f) {
				if ((abs(fObjCenterY - sObjCenterY) >= 0.1f) || (abs(fObjCenterX - sObjCenterX) >= 0.08f) || (fObjCenterY > sObjCenterY)) {
					for (int i = 1; i < 16; i += 4) {
						coords[i] += 0.01f;
					}
					fObjCenterY += 0.01f;
				}
			}
		}
		if (glfwGetKey(wind, GLFW_KEY_S)) {
			if ((fObjCenterY > -0.95f)  ){
				if ((abs(fObjCenterY - sObjCenterY) >= 0.1f) || (abs(fObjCenterX - sObjCenterX) >= 0.08f)  || (fObjCenterY <= sObjCenterY)) {
						for (int i = 1; i < 16; i += 4) {
							coords[i] -= 0.01f;
						}
						fObjCenterY -= 0.01f;
				}
			}
		}
		//std::cout << fObjCenterY << "\t" << sObjCenterY << "\n";
		//std::cout << fObjCenterX << "\t" << sObjCenterX << "\n";

		//SECOND OBJECT CONTROLS
		if (glfwGetKey(wind, GLFW_KEY_RIGHT)) {
			if (sObjCenterX < 0.95f) {
				if ((abs(fObjCenterY - sObjCenterY) >= 0.087f) || (abs(fObjCenterX - sObjCenterX) >= 0.1f) || (fObjCenterX < sObjCenterX)) {
					for (int i = 0; i < 16; i += 4) {
						coords2[i] += 0.01f;
					}
					sObjCenterX += 0.01f;
				}
			}
		}
		if (glfwGetKey(wind, GLFW_KEY_LEFT)) {
			if (sObjCenterX > -0.95f) {
				if ((abs(fObjCenterY - sObjCenterY) >= 0.087f) || (abs(fObjCenterX - sObjCenterX) >= 0.1f) || (fObjCenterX > sObjCenterX)) {
					for (int i = 0; i < 16; i += 4) {
						coords2[i] -= 0.01f;
					}
					sObjCenterX -= 0.01f;
				}
			}
		}
		if (glfwGetKey(wind, GLFW_KEY_UP)) {
			if (sObjCenterY < 0.95f) {
				if ((abs(fObjCenterY - sObjCenterY) >= 0.1f) || (abs(fObjCenterX - sObjCenterX) >= 0.087f) || (fObjCenterY <= sObjCenterY)) {
					for (int i = 1; i < 16; i += 4) {
						coords2[i] += 0.01f;
					}
					sObjCenterY += 0.01f;
				}
			}
		}
		if (glfwGetKey(wind, GLFW_KEY_DOWN)) {
			if (sObjCenterY > -0.95f) {
				if ((abs(fObjCenterY - sObjCenterY) >= 0.1f) || (abs(fObjCenterX - sObjCenterX) >= 0.087f) || (fObjCenterY > sObjCenterY)) {
					for (int i = 1; i < 16; i += 4) {
						coords2[i] -= 0.01f;
					}
					sObjCenterY -= 0.01f;
				}
			}
		}




		float distance = sqrt(pow(fObjCenterX - sObjCenterX, 2) + pow(fObjCenterY - sObjCenterY, 2));

		//DISTANCE SCALING
		if (distance <= minDist) {
			lightMult = 1.0f;
		}
		if (distance >= maxDist) {
			lightMult = 0.02f;
		}
		if ((distance < maxDist) && (distance > minDist)) {
			lightMult = (maxDist - distance) / 0.8f;
			if (lightMult < 0.02f) {
				lightMult = 0.02f;
			}
		}


		//COLORS SETUP DEPENDING ON LIGHT
		for (int i = 0; i < 12; ++i) {
			colors[i] = standartColors[i] * lightMult;
		}

		glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 16, colors, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 16, coords, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ARRAY_BUFFER, vb2);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 16, coords2, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);


		glBindVertexArray(vertexArray);
		glDrawArrays(GL_QUADS, 0, 4);

		glBindVertexArray(va2);
		glDrawArrays(GL_QUADS, 0, 4);

		glBindVertexArray(0);



		std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 60));

		glfwPollEvents();
		glfwSwapBuffers(wind);
	}

	return 0;
}