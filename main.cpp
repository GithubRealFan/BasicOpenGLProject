#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <iostream>
#include <glm/ext.hpp>
#include <vector>
#include "shader.h"
#include "shaderprogram.h"
using namespace std;

/*=================================================================================================
	DOMAIN
=================================================================================================*/

int FLAT_MODE = true;
bool NORMAL_VISIBLE = true;

// Window dimensions
const int InitWindowWidth = 800;
const int InitWindowHeight = 800;
int WindowWidth = InitWindowWidth;
int WindowHeight = InitWindowHeight;

// Last mouse cursor position
int LastMousePosX = 0;
int LastMousePosY = 0;

// Arrays that track which keys are currently pressed
bool key_states[256];
bool key_special_states[256];
bool mouse_states[8];

// Other parameters
bool draw_wireframe = false;

/*=================================================================================================
	SHADERS & TRANSFORMATIONS
=================================================================================================*/

ShaderProgram PassthroughShader;
ShaderProgram PerspectiveShader;
// 2 shaders added 
ShaderProgram SmoothShader;
ShaderProgram NotSmoothShader;

glm::mat4 PerspProjectionMatrix(1.0f);
glm::mat4 PerspViewMatrix(1.0f);
glm::mat4 PerspNormalMatrix(1.0f);

float perspZoom = 1.0f, perspSensitivity = 0.35f;
float perspRotationX = 0.0f, perspRotationY = 0.0f;

/*=================================================================================================
	OBJECTS
=================================================================================================*/


GLuint axis_VAO;
GLuint axis_VBO[4];

GLuint norms_VAO;
GLuint norms_VBO[4];

/*float R = 1.0f, r = 0.25f;
int N = 20, M = 10;

struct Vertex {
	float x = 0, y = 0, z = 0, w = 1;
	Vertex(float _x, float _y, float _z) : x(_x), y(_y), z(_z) { }
	void store_at(float* ptr) {
		ptr[0] = x;  ptr[1] = y;  ptr[2] = z;  ptr[3] = 1;
	}
};


float PI = 3.14159265358979323846F;
float TAU = 2 * PI;
std::vector<float> verts(N* M * 6 * 4);
vector<float> generateTorusVertices(float R, float r, int N, int M) {
	std::vector<float> verts(N * M * 6 * 4);  // xyzw for the 3 points of 2 triangles for each (i, j) torus location
	int k = 0;  // index to verts

	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < M; ++j) {

			auto vertex_at = [&](int i, int j) -> Vertex {
				float theta = TAU * i / (float)N,
					phi = TAU * j / (float)M;

				return {
					(R + r * cos(theta)) * cos(phi),
					(R + r * cos(theta)) * sin(phi),
					r * sin(theta)
				};
			};

			// make quad
			vector<Vertex> corners = {
				vertex_at(i, j),
				vertex_at(i, j + 1),
				vertex_at(i + 1, j + 1),
				vertex_at(i + 1, j)
			};

			// split quad into 2 triangles & store
			for (int index : {0, 1, 2, 2, 3, 1}) {
				corners[index].store_at(&verts[k]);
				k += 4;
			}
		}
	}
	return verts;
}*/

// vector<float> vert0 = {
// 	//x axis
// 	-1.0f,  0.0f,  1.0f, 1.0f,
// 	1.0f,  0.0f,  0.0f, 1.0f,
// 	//y axis
// 	0.0f, -1.0f,  0.0f, 1.0f,
// 	0.0f,  1.0f,  0.0f, 1.0f,
// 	//z axis
// 	0.0f,  0.0f, -1.0f, 1.0f,
// 	0.0f,  0.0f,  1.0f, 1.0f
// };

// auto verts = constructTorus(1.0f, 0.1f, 16, 16);

float R = .5f; //Big Radius
float r = 0.1f; //Innner Radius
int N = 8, M = 10;//M sides & N= number of triangles

vector<float> verts; // this stores all of torus in the end
vector<float> norms, verts_norm;
// bool updateFlag = true;

struct Point {  //point struct
	float x = 0, y = 0, z = 0, w = 1;
};

/*using glm::mat4;
using glm::vec3;
class Line {
	int shaderProgram;
	unsigned int VBO, VAO;
	vector<float> vertices;
	vec3 startPoint;
	vec3 endPoint;
	mat4 MVP = mat4(1.0);
	vec3 lineColor;
public:
	Line(vec3 start, vec3 end) {

		startPoint = start;
		endPoint = end;
		lineColor = vec3(1, 1, 1);

		const char* vertexShaderSource = "#version 330 core\n"
			"layout (location = 0) in vec3 aPos;\n"
			"uniform mat4 MVP;\n"
			"void main()\n"
			"{\n"
			"   gl_Position = MVP * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
			"}\0";
		const char* fragmentShaderSource = "#version 330 core\n"
			"out vec4 FragColor;\n"
			"uniform vec3 color;\n"
			"void main()\n"
			"{\n"
			"   FragColor = vec4(color, 1.0f);\n"
			"}\n\0";

		// vertex shader
		int vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
		glCompileShader(vertexShader);
		// check for shader compile errors

		// fragment shader
		int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
		glCompileShader(fragmentShader);
		// check for shader compile errors

		// link shaders
		shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);
		// check for linking errors

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		vertices = {
			 start.x, start.y, start.z,
			 end.x, end.y, end.z,

		};

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

	}

	int setMVP(mat4 mvp) {
		MVP = mvp;
		return 1;
	}

	int setColor(vec3 color) {
		lineColor = color;
		return 1;
	}

	int draw() {
		glUseProgram(shaderProgram);

		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "MVP"), 1, GL_FALSE, &MVP[0][0]);
		glUniform3fv(glGetUniformLocation(shaderProgram, "color"), 1, &lineColor[0]);

		glBindVertexArray(VAO);
		glDrawArrays(GL_LINES, 0, 2);
		return 1;
	}

	~Line() {
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteProgram(shaderProgram);
	}
};
Line line;*/

float PI = 3.1416F;
vector<float> constructTorus(float R,float r,int N,int M) { //return a vector of type float
	std::vector<float> points,normalPoints;  //initialize a vector of type float name points.
	for (int i = 0; i < N; ++i) {  //num of triangles 
		for (int j = 0; j < M; ++j) { //sides

			auto makePoint = [&](int i, int j) { // //capture all variables within scope by reference
				Point P; // initialize/ create point p
				float theta = 2 * PI * i / (float)N;
				float phi = 2 * PI * j / (float)M; //convert theta and phi to radians

				P.x = (R + r * cos(theta)) * cos(phi);   //paramaterization
				P.y = (R + r * cos(theta)) * sin(phi);     //for points
				P.z = r * sin(theta);
				return P;
			};
		
			Point P = makePoint(i, j);             //points to connect two tri's
			Point Q = makePoint(i, j + 1);          
			Point R = makePoint(i + 1, j + 1);
			Point S = makePoint(i + 1, j);

			auto add = [&](Point w) {  // construct for add essentially to connect two tri's
				points.push_back(w.x);      
				points.push_back(w.y);// this is a add method for connecting all the sides
				points.push_back(w.z);
				points.push_back(1);
			};
			/*add(P);  add(Q);
			add(Q);  add(R);
			*/
			add(P);  add(Q);  add(R);   //connection make from one chunk
			add(R);  add(S);  add(P);


		}
	}
	return points;// , normalPoints; //returns to display
}
vector<float> ConstructNorms(int N,int M) {
	std::vector<float> normalPoints;  //initialize a vector of type float named ...

	for (int i = 0; i < N; ++i) {  //num of triangles 
		for (int j = 0; j < M; ++j) { //sides
			if (FLAT_MODE) {
				auto makePoint = [&](int i, int j) { // //capture all variables within scope by reference
					Point P; // initialize/ create point p
					float theta = 2 * PI * i / (float)N;
					float phi = 2 * PI * j / (float)M; //convert theta and phi to radians

					P.x = (R + r * cos(theta)) * cos(phi);   //paramaterization
					P.y = (R + r * cos(theta)) * sin(phi);     //for points
					P.z = r * sin(theta);
					return P;
				};
				Point p = makePoint(i, j);             //points to connect two tri's
				Point q = makePoint(i, j + 1);
				Point r = makePoint(i + 1, j + 1);
				Point s = makePoint(i + 1, j);

				glm::vec3 P(p.x, p.y, p.z), Q(q.x, q.y, q.z), R(r.x, r.y, r.z);

				glm::vec3 norm = glm::cross(Q - P, R - P);
				norm = glm::normalize(norm) * 0.05f;

				for (int i = 0; i < 6; ++i) {
					normalPoints.push_back(norm.x);
					normalPoints.push_back(norm.y);
					normalPoints.push_back(norm.z);
					normalPoints.push_back(0);
				}
			}
			else {
				//computes normals
				auto makeNormals = [&](int i, int j) {
					Point Normals;
					float theta = 2 * PI * i / (float)N;
					float phi = 2 * PI * j / (float)M;
					Normals.x = (cos(theta) * cos(phi));
					Normals.y = (cos(theta) * sin(phi));
					Normals.z = (sin(theta));
					return Normals;
				};
				Point n = makeNormals(i, j);
				Point o = makeNormals(i, j + 1);
				Point r = makeNormals(i + 1, j + 1);
				Point m = makeNormals(i + 1, j);
				// adds normals
				auto addNormals = [&](Point n) {
					normalPoints.push_back(n.x);
					normalPoints.push_back(n.y);
					normalPoints.push_back(n.z);
					normalPoints.push_back(0);
				};
				addNormals(n); addNormals(o); addNormals(r);
				addNormals(r); addNormals(m); addNormals(n);
			}
		}
	}
	return normalPoints;
}

vector<float> ConstructNormVectors(float R, float r, int N, int M)
{
	std::vector<float> points;  //initialize a vector of type float name points.
	for (int i = 0; i < N; ++i) {  //num of triangles 
		for (int j = 0; j < M; ++j) { //sides

			auto makePoint = [&](int i, int j) { // //capture all variables within scope by reference
				Point P; // initialize/ create point p
				float theta = 2 * PI * i / (float)N;
				float phi = 2 * PI * j / (float)M; //convert theta and phi to radians

				P.x = (R + r * cos(theta)) * cos(phi);   //paramaterization
				P.y = (R + r * cos(theta)) * sin(phi);     //for points
				P.z = r * sin(theta);
				return P;
			};

			if (FLAT_MODE) {

				Point P = makePoint(i, j);             //points to connect two tri's
				Point Q = makePoint(i, j + 1);
				Point R = makePoint(i + 1, j + 1);
				Point S = makePoint(i + 1, j);

				auto add = [&](Point p, Point q, Point r) {  // construct for add essentially to connect two tri's
					glm::vec3 P(p.x, p.y, p.z), Q(q.x, q.y, q.z), R(r.x, r.y, r.z);
					glm::vec3 C = (P + Q + R) / 3.0f;

					points.push_back(C[0]);
					points.push_back(C[1]);
					points.push_back(C[2]);
					points.push_back(1);

					glm::vec3 norm = glm::cross(Q - P, R - P);
					norm = glm::normalize(norm) * 0.05f;

					points.push_back(C[0] + norm[0]);
					points.push_back(C[1] + norm[1]);
					points.push_back(C[2] + norm[2]);
					points.push_back(1);
				};
				add(P, Q, R);
				add(R, S, P);
			}
			else {
				Point P = makePoint(i, j);             //points to connect two tri's
				Point Q = makePoint(i, j + 1);

				auto add = [&](Point p, int i, int j) {  // construct for add essentially to connect two tri's
					glm::vec3 P(p.x, p.y, p.z);

					points.push_back(P[0]);
					points.push_back(P[1]);
					points.push_back(P[2]);
					points.push_back(1);


					float theta = 2 * PI * i / (float)N;
					float phi = 2 * PI * j / (float)M; //convert theta and phi to radians

					glm::vec3 norm;
					norm.x = (cos(theta) * cos(phi));
					norm.y = (cos(theta) * sin(phi));
					norm.z = (sin(theta));
					norm = glm::normalize(norm) * 0.05f;

					points.push_back(P[0] + norm[0]);
					points.push_back(P[1] + norm[1]);
					points.push_back(P[2] + norm[2]);
					points.push_back(1);
				};
				add(P, i, j);
				add(Q, i, j+1);
			}
		}
	}
	return points;// , normalPoints; //returns to display
}
float axis_vertices[] = {
	//x axis
	-1.0f,  0.0f,  0.0f, 1.0f,
	1.0f,  0.0f,  0.0f, 1.0f,
	//y axis
	0.0f, -1.0f,  0.0f, 1.0f,
	0.0f,  1.0f,  0.0f, 1.0f,
	//z axis
	0.0f,  0.0f, -1.0f, 1.0f,
	0.0f,  0.0f,  1.0f, 1.0f
};




float axis_colors[] = {
	//x axis
	1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 1.0f,
	//y axis
	0.0f, 1.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 0.0f, 1.0f,
	//z axis
	0.0f, 0.0f, 1.0f, 1.0f,
	0.0f, 0.0f, 1.0f, 1.0f
};

/*=================================================================================================
	HELPER FUNCTIONS
=================================================================================================*/

void window_to_scene(int wx, int wy, float& sx, float& sy)
{
	sx = (2.0f * (float)wx / WindowWidth) - 1.0f;
	sy = 1.0f - (2.0f * (float)wy / WindowHeight);
}

/*=================================================================================================
	SHADERS
=================================================================================================*/

/**
 * Properties of the light.
 */
struct LightProperties
{
	GLfloat direction[3];
	GLfloat ambientColor[4];
	GLfloat diffuseColor[4];
	GLfloat specularColor[4];
};

/**
 * Properties of the material, basically all the color factors without the emissive color component.
 */
struct MaterialProperties
{
	GLfloat ambientColor[4];
	GLfloat diffuseColor[4];
	GLfloat specularColor[4];
	GLfloat specularExponent;
};

/**
 * Locations for the light properties.
 */
struct LightLocations
{
	GLint directionLocation;
	GLint ambientColorLocation;
	GLint diffuseColorLocation;
	GLint specularColorLocation;
};

/**
 * Locations for the material properties.
 */
struct MaterialLocations
{
	GLint ambientColorLocation;
	GLint diffuseColorLocation;
	GLint specularColorLocation;
	GLint specularExponentLocation;
};

static struct LightLocations g_light;
static struct MaterialLocations g_material;

void CreateTransformationMatrices(void)
{
	// PROJECTION MATRIX
	PerspProjectionMatrix = glm::perspective<float>(glm::radians(60.0f), (float)WindowWidth / (float)WindowHeight, 0.01f, 1000.0f);

	// VIEW MATRIX
	glm::vec3 eye(0.0, 0.0, 2.0);
	glm::vec3 center(0.0, 0.0, 0.0);
	glm::vec3 up(0.0, 1.0, 0.0);

	PerspViewMatrix = glm::lookAt(eye, center, up);

	glm::mat4 PerspModelMatrix = glm::mat4(1.0);
	PerspModelMatrix = glm::rotate(PerspModelMatrix, glm::radians(perspRotationX), glm::vec3(1.0, 0.0, 0.0));
	PerspModelMatrix = glm::rotate(PerspModelMatrix, glm::radians(perspRotationY), glm::vec3(0.0, 1.0, 0.0));
	PerspModelMatrix = glm::scale(PerspModelMatrix, glm::vec3(perspZoom));

	PerspViewMatrix *= PerspModelMatrix;

	// MODEL MATRIX
	PerspNormalMatrix = glm::mat3(PerspViewMatrix);

	PerspectiveShader.SetUniform("u_projectionMatrix", glm::value_ptr(PerspProjectionMatrix), 4, GL_FALSE, 1);
	PerspectiveShader.SetUniform("u_modelViewMatrix", glm::value_ptr(PerspViewMatrix), 4, GL_FALSE, 1);
	PerspectiveShader.SetUniform("u_normalMatrix", glm::value_ptr(PerspNormalMatrix), 3, GL_FALSE, 1);

	// This is a white light.
	struct LightProperties light = { { 0.0f, 1.0f, 1.0f }, { 0.3f, 0.3f, 0.3f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } };

	// Blue color material with white specular color.
	struct MaterialProperties material = { { 0.0f, 0.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, 10.0f };

	glm::vec3 light_direction = glm::mat3(PerspViewMatrix) * glm::normalize(glm::vec3(light.direction[0], light.direction[1], light.direction[2]));

	// Set up light ...
	PerspectiveShader.SetUniform("u_light.direction", glm::value_ptr(light_direction), 3, 1);
	PerspectiveShader.SetUniform("u_light.ambientColor", light.ambientColor, 4, 1);
	PerspectiveShader.SetUniform("u_light.diffuseColor", light.diffuseColor, 4, 1);
	PerspectiveShader.SetUniform("u_light.specularColor", light.specularColor, 4, 1);

	// ... and material values.
	PerspectiveShader.SetUniform("u_material.ambientColor", material.ambientColor, 4, 1);
	PerspectiveShader.SetUniform("u_material.diffuseColor", material.diffuseColor, 4, 1);
	PerspectiveShader.SetUniform("u_material.specularColor", material.specularColor, 4, 1);
	PerspectiveShader.SetUniform("u_material.specularExponent", &material.specularExponent, 1, 1);
}

void CreateShaders(void)
{
	cout << "A" << endl;
	// Renders without any transformations
	PassthroughShader.Create("./shaders/simple.vert", "./shaders/simple.frag");
	cout << "B" << endl;

	// Renders using perspective projection
	//PerspectiveShader.Create("./shaders/persp.vert", "./shaders/persp.frag");
	PerspectiveShader.Create("./shaders/phong.vert.glsl", "./shaders/phong.frag.glsl");
	
	//PerspectiveShader.Create("./shaders/persplight.vert","./shaders/persplight.frag");
	cout << "C" << endl;
	// I added this
//	SmoothShader.Create("./shaders/smooth.vert", "./shaders/smooth.frag");
//	NotSmoothShader.Create("./shaders/not_smooth.vert", "./shaders/not_smooth.frag");



}

/*=================================================================================================
	BUFFERS
=================================================================================================*/


void CreateAxisBuffers(void)
{
	norms = ConstructNorms(N,M);
	verts = constructTorus(R,r,N,M); // 1.0f, 0.1f, N, M);
	verts_norm = ConstructNormVectors(R, r, N, M);
	//verts = generateTorusVertices( R,  r,  N,  M); 
	vector<float> rgb;

	glGenVertexArrays(1, &axis_VAO);
	glBindVertexArray(axis_VAO);

	glGenBuffers(3, &axis_VBO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, axis_VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verts.size(), &verts[0], GL_STATIC_DRAW);
    
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	// created a vector type float rgb
	for (auto v : verts) { // goes through the entire vector and fills

		rgb.push_back(0.f);
		rgb.push_back(0.2f);
		rgb.push_back(0.5f);
		rgb.push_back(1.f);

	}

	glBindBuffer(GL_ARRAY_BUFFER, axis_VBO[1]);
	// size is equal to amount of verts 
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * rgb.size(), &rgb[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, axis_VBO[2]);
	// size is equal to amount of verts 
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * norms.size(), &norms[0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(2);

	glGenVertexArrays(1, &norms_VAO);
	glBindVertexArray(norms_VAO);

	glGenBuffers(2, &norms_VBO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, norms_VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verts_norm.size(), &verts_norm[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	rgb.clear();
	for (int i = 0, n = verts_norm.size() / 4; i < n; ++i) {
		rgb.push_back(1.f);
		rgb.push_back(0.f);
		rgb.push_back(0.f);
		rgb.push_back(1.f);
	}

	glBindBuffer(GL_ARRAY_BUFFER, norms_VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * rgb.size(), &rgb[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

/*=================================================================================================
	CALLBACKS
=================================================================================================*/

//-----------------------------------------------------------------------------
// CALLBACK DOCUMENTATION
// https://www.opengl.org/resources/libraries/glut/spec3/node45.html
// http://freeglut.sourceforge.net/docs/api.php#WindowCallback
//-----------------------------------------------------------------------------

void idle_func()
{
	//uncomment below to repeatedly draw new frames
	glutPostRedisplay();
}

void reshape_func(int width, int height)
{
	WindowWidth = width;
	WindowHeight = height;

	glViewport(0, 0, width, height);
	glutPostRedisplay();
}

void keyboard_func(unsigned char key, int x, int y)
{
	key_states[key] = true;

	switch (key)
	{
	case '1':
	{
		draw_wireframe = !draw_wireframe;
		if (draw_wireframe == true)
			std::cout << "Wireframes on.\n";
		else
			std::cout << "Wireframes off.\n";
		break;
	}
	case 'q': {
		++N; //increments # of triangles
		CreateAxisBuffers();
		break;
	}
	case 'a':{
		--N;    //dec. num tri's
		CreateAxisBuffers();
		break;
			}
	
	case 'w': {
		r += 0.1f; //INCREMENTS LIL r
		CreateAxisBuffers();
		break;
	}
	case 's': {
		r -=0.1F;  //DEC LIL r
		CreateAxisBuffers();
		break;
	}
	case 'e': {
		R+=0.1f;  //INC R
		CreateAxisBuffers();
		break; 
	}
	case 'd': {
		R-=0.1f; //DECREMENTS R
		CreateAxisBuffers();
		break;
	}

	case 'j':
		++M; //increments sides
		CreateAxisBuffers();
		break;
	case'm':
		--M;
		CreateAxisBuffers();
		break;
		// Exit on escape key press
	case 'c':
		NORMAL_VISIBLE = !NORMAL_VISIBLE;
		break;
	case 'z':
		FLAT_MODE = true;
		CreateAxisBuffers();
		break;
	case 'x':
		FLAT_MODE = false;
		CreateAxisBuffers();
		break;
	case '\x1B':
	{
		exit(EXIT_SUCCESS);
		break;
	}
	}
}

void key_released(unsigned char key, int x, int y)
{
	key_states[key] = false;
}

void key_special_pressed(int key, int x, int y)
{
	key_special_states[key] = true;
}

void key_special_released(int key, int x, int y)
{
	key_special_states[key] = false;
}

void mouse_func(int button, int state, int x, int y)
{
	// Key 0: left button
	// Key 1: middle button
	// Key 2: right button
	// Key 3: scroll up
	// Key 4: scroll down

	if (x < 0 || x > WindowWidth || y < 0 || y > WindowHeight)
		return;

	float px, py;
	window_to_scene(x, y, px, py);

	if (button == 3)
	{
		perspZoom += 0.03f;
	}
	else if (button == 4)
	{
		if (perspZoom - 0.03f > 0.0f)
			perspZoom -= 0.03f;
	}

	mouse_states[button] = (state == GLUT_DOWN);

	LastMousePosX = x;
	LastMousePosY = y;
}

void passive_motion_func(int x, int y)
{
	if (x < 0 || x > WindowWidth || y < 0 || y > WindowHeight)
		return;

	float px, py;
	window_to_scene(x, y, px, py);

	LastMousePosX = x;
	LastMousePosY = y;
}

void active_motion_func(int x, int y)
{
	if (x < 0 || x > WindowWidth || y < 0 || y > WindowHeight)
		return;

	float px, py;
	window_to_scene(x, y, px, py);

	if (mouse_states[0] == true)
	{
		perspRotationY += (x - LastMousePosX) * perspSensitivity;
		perspRotationX += (y - LastMousePosY) * perspSensitivity;
	}

	LastMousePosX = x;
	LastMousePosY = y;
}

/*=================================================================================================
	RENDERING
=================================================================================================*/

void display_func(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	PerspectiveShader.Use();
	CreateTransformationMatrices();


	if (draw_wireframe == true)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glBindVertexArray(axis_VAO);
	glDrawArrays(GL_TRIANGLES, 0, verts.size());
	glBindVertexArray(0);

	if (draw_wireframe == true)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	if (NORMAL_VISIBLE) {
		glBindVertexArray(norms_VAO);
		glDrawArrays(GL_LINES, 0, verts_norm.size());
		glBindVertexArray(0);
	}

	glutSwapBuffers();
}

/*=================================================================================================
	INIT
=================================================================================================*/

void init(void)
{
	// Print some info
	std::cout << "Vendor:         " << glGetString(GL_VENDOR) << "\n";
	std::cout << "Renderer:       " << glGetString(GL_RENDERER) << "\n";
	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << "\n";
	std::cout << "GLSL Version:   " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n\n";

	// Set OpenGL settings
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // background color
	glEnable(GL_DEPTH_TEST); // enable depth test
	glEnable(GL_CULL_FACE); // enable back-face culling

	// Create shaders
	CreateShaders();

	// Create buffers
	CreateAxisBuffers();

	std::cout << "Finished initializing...\n\n";
}

/*=================================================================================================
	MAIN
=================================================================================================*/

int main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitWindowPosition(100, 100);
	glutInitWindowSize(InitWindowWidth, InitWindowHeight);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

	glutCreateWindow("CSE-170 Computer Graphics");

	// Initialize GLEW
	GLenum ret = glewInit();
	if (ret != GLEW_OK) {
		std::cerr << "GLEW initialization error." << std::endl;
		glewGetErrorString(ret);
		return -1;
	}

	glutDisplayFunc(display_func);
	glutIdleFunc(idle_func);
	glutReshapeFunc(reshape_func);
	glutKeyboardFunc(keyboard_func);
	glutKeyboardUpFunc(key_released);
	glutSpecialFunc(key_special_pressed);
	glutSpecialUpFunc(key_special_released);
	glutMouseFunc(mouse_func);
	glutMotionFunc(active_motion_func);
	glutPassiveMotionFunc(passive_motion_func);

	init();

	glutMainLoop();

	return EXIT_SUCCESS;
}

