#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <map>
#include <glm/gtc/matrix_transform.hpp>
#include "Matrix.hpp"
#include <unistd.h>
#include "cubic.hpp"
using namespace glm;

void Cubic::downCross(){}
void Cubic::downFace(){}
void Cubic::secondLevel(){}
void Cubic::upCross(){}
void Cubic::upCrossCorrect(){}
void Cubic::upFace(){}
void Cubic::finish(){}

GLFWwindow* window;
const int WIDTH = 1024;
const int HEIGHT = 768;

vector<GLfloat> PosList;
vector<GLfloat> ColorList;

vec3 Color[10] = {vec3(1,1,1),vec3(1,1,0),vec3(1,0,0),vec3(1,0.7,0.22),vec3(0,0,1),vec3(0,1,0)};
map<char, vec3> char2col;
const vec3 Black = vec3(0,0,0);

vec3 operator*(vec3 x,GLfloat y){
	return vec3(x[0]*y,x[1]*y,x[2]*y);
}

const double Pi = acos(-1);
const double wspeed = Pi / 4;

GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path){

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if(VertexShaderStream.is_open()){
		std::string Line = "";
		while(getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}else{
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if(FragmentShaderStream.is_open()){
		std::string Line = "";
		while(getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;


	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}



	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}



	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	
	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);
	
	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

void Push(vector<GLfloat> &List,vec3 x){
	List.push_back(x[0]);
	List.push_back(x[1]);
	List.push_back(x[2]);
}

void addRect(vec3 a,vec3 b,vec3 c,vec3 d,vec3 color){
	vec3 Move = vec3(-1.5f,-1.5f,-1.5f);
	printf("addRect([%f,%f,%f][%f,%f,%f][%f,%f,%f][%f,%f,%f])\n",
			a[0],a[1],a[2],b[0],b[1],b[2],c[0],c[1],c[2],d[0],d[1],d[2]);
	Push(PosList,a + Move);
	Push(PosList,b + Move);
	Push(PosList,c + Move);
	Push(PosList,c + Move);
	Push(PosList,d + Move);
	Push(PosList,a + Move);
	for (int i = 1;i <= 6;i++){
		Push(ColorList,color);
	}
}

void MakeSurface(int *color, vec3 orin, vec3 right, vec3 down, vec3 back){
	const GLfloat eps = 0.02;
	const GLfloat eps2 = 0.02;
	vec3 pos1 = orin + right * eps + down * eps;
	vec3 pos2 = orin + right - right * eps + down * eps ;
	vec3 pos3 = orin + right + down - down * eps - right * eps;
	vec3 pos4 = orin + down + right * eps  - down * eps;
	for (GLint i = 0;i < 3;i++)
		for (GLint j = 0;j < 3;j++){
			vec3 delta = down * i + right * j;
			addRect(pos1 + delta, pos2 + delta, pos3 + delta, pos4 + delta, Color[color[i * 3 + j]]);

			addRect(pos1 + delta - down * eps2 - right * eps2 + back * eps2,
					pos1 + delta - down * eps2 + back * eps2,
					pos4 + delta + down * eps2 + back * eps2,
					pos4 + delta + down * eps2 - right * eps2 + back * eps2,
					Black);
			addRect(pos1 + delta - down * eps2 + back * eps2,
					pos1 + delta + back * eps2,
					pos2 + delta + back * eps2,
					pos2 + delta - down * eps2 + back * eps2,
					Black);
			addRect(pos2 + delta - down * eps2 + back * eps2,
					pos2 + delta - down * eps2 + right * eps2 + back * eps2,
					pos3 + delta + down * eps2 + right * eps2 + back * eps2,
					pos3 + delta + down * eps2 + back * eps2,
					Black);
			addRect(pos4 + delta + down * eps2 + back * eps2,
					pos4 + delta + back * eps2,
					pos3 + delta + back * eps2,
					pos3 + delta + down * eps2 + back * eps2,
					Black);
		}
}

void Generate(int color[54]){
	PosList.clear();
	ColorList.clear();
	MakeSurface(color + 9 * 0, vec3(0,3,3), vec3(1,0,0) , vec3(0,-1,0), vec3(0,0,-1));
	MakeSurface(color + 9 * 1, vec3(3,3,0), vec3(-1,0,0), vec3(0,-1,0), vec3(0,1,0));
	MakeSurface(color + 9 * 2, vec3(0,3,0), vec3(1,0,0) , vec3(0,0,1) , vec3(0,-1,0));
	MakeSurface(color + 9 * 3, vec3(0,0,3), vec3(1,0,0) , vec3(0,0,-1), vec3(0,1,0));
	MakeSurface(color + 9 * 4, vec3(0,3,0), vec3(0,0,1) , vec3(0,-1,0), vec3(1,0,0));
	MakeSurface(color + 9 * 5, vec3(3,3,3), vec3(0,0,-1), vec3(0,-1,0), vec3(-1,0,0));
}

int init(){
	char2col['W'] = Color[0];
	char2col['Y'] = Color[1];
	char2col['R'] = Color[2];
	char2col['O'] = Color[3];
	char2col['B'] = Color[4];
	char2col['G'] = Color[5];
	FILE *f = fopen("ColorTrans","r");
	freopen("ErrorLog.txt", "w", stdout);
	static char st[10];
	fscanf(f, "%s", st);
	for (int i = 0;i < 6;i++){
		Color[i] = char2col[st[i]];
	}
	if (!glfwInit()){
		fprintf(stderr, "Fails to initialize GLFW\n");
		return -1;
	}
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	window = glfwCreateWindow(1024, 768, "徐Boss的魔方还原器 Demo",NULL,NULL);
	if (window == NULL){
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glewExperimental = true;
	if (glewInit() != GLEW_OK){
		fprintf(stderr, "Failed to initialisze GLEW\n");
		return -1;
	}
	return 0;
}
GLfloat vertex_buffer_data[18*54*5],color_buffer_data[18*54*5];

void getColor(){
	static int colorArray[54];
	for (int i = 0;i < 54;i++) colorArray[i] = A.ordianary[i][0];
	Generate(colorArray);

	fprintf(stderr, "%d %d\n",PosList.size(), ColorList.size());
	for (int i = 0;i < PosList.size();i++) vertex_buffer_data[i] = PosList[i];
	for (int i = 0;i < ColorList.size();i++) color_buffer_data[i] = ColorList[i];
}

void _getColor(){
	PosList.clear();
	ColorList.clear();
	addRect(vec3(-1,-1,-1),vec3(-1,-1,1),vec3(-1,1,1),vec3(-1,1,-1),Color[1]);
//	addRect(vec3(1, -1,-1),vec3(1, -1,1),vec3(1 ,1,1),vec3(1,1,-1),Color[1]);

	for (int i = 0;i < PosList.size();i++) vertex_buffer_data[i] = PosList[i];
	for (int i = 0;i < ColorList.size();i++) color_buffer_data[i] = ColorList[i];
}
int cnt = 0;
vector<string> Eq;

void getEqation(){
	FILE *f = fopen("output.txt","r");
	int x;
	fscanf(f,"%d",&x);
	char s[100];
	for (int i = 0;i < x;i++){
		fscanf(f,"%s",s);
		Eq.push_back(string(s));
	}
}

mat4 getRotateModel(){
	mat4 cur = mat4(1.f);
	static double lastTime = glfwGetTime();
	double currentTime = glfwGetTime();
	static GLfloat rightangle = 0, upangle = 0;
	float deltaTime = float(currentTime - lastTime);

	if (glfwGetKey( window, GLFW_KEY_UP ) == GLFW_PRESS){
		upangle -= wspeed * deltaTime;
	}
	if (glfwGetKey( window, GLFW_KEY_DOWN ) == GLFW_PRESS){
		upangle += wspeed * deltaTime;
	}
	if (glfwGetKey( window, GLFW_KEY_RIGHT ) == GLFW_PRESS){
		rightangle += wspeed * deltaTime;
	}
	if (glfwGetKey( window, GLFW_KEY_LEFT ) == GLFW_PRESS){
		rightangle -= wspeed * deltaTime;
	}
	lastTime = currentTime;
	cur = rotate(cur, rightangle, vec3(0,1,0));
	cur = rotate(cur, upangle , vec3(0,0,1));
	return cur;
}

vec3 getLookAtPos(){
	mat4 cur = mat4(1.f);
	static vec3 orin = vec3(3,6,5);
	static double lastTime = glfwGetTime();
	double currentTime = glfwGetTime();
	GLfloat rightangle = 0, upangle = 0;
	float deltaTime = float(currentTime - lastTime);

	if (glfwGetKey( window, GLFW_KEY_UP ) == GLFW_PRESS){
		upangle += wspeed * deltaTime;
	}
	if (glfwGetKey( window, GLFW_KEY_DOWN ) == GLFW_PRESS){
		upangle -= wspeed * deltaTime;
	}
	if (glfwGetKey( window, GLFW_KEY_RIGHT ) == GLFW_PRESS){
		rightangle += wspeed * deltaTime;
	}
	if (glfwGetKey( window, GLFW_KEY_LEFT ) == GLFW_PRESS){
		rightangle -= wspeed * deltaTime;
	}
	lastTime = currentTime;
	if (rightangle != 0) cur = rotate(cur, rightangle, vec3(0,1,0));
	if (upangle != 0) cur = rotate(cur, upangle ,vec3(0,0,1));
	vec4 now = vec4(orin[0],orin[1],orin[2],0);
	now = cur * now;
	orin = vec3(now[0],now[1],now[2]);
	return orin;
}

int main()
{
	if (init()) return -1;
	Cubic_init("question");
	getEqation();
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS); 
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	GLuint programID = LoadShaders("vertex.shader", "fragment.shader");
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");
	mat4 Projection = perspective(
		radians(45.f),
		(float)WIDTH/HEIGHT,
		3.f,
		10.f
	);


	mat4 View = lookAt(
		vec3(0,0,8.366),
		vec3(0,0,0),
		vec3(0,1,0)
	);

	mat4 Model = mat4(1);
	mat4 MVP =  Projection * View * Model; 

	fprintf(stderr, "%d\n",sizeof(vertex_buffer_data));

	GLuint vertexbuffer, colorbuffer;
	int flag = 1;
	do{
		if (flag == 1){
			getColor();
			flag = 0;
		}
		glGenBuffers(1, &vertexbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);

		glGenBuffers(1, &colorbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data), color_buffer_data, GL_STATIC_DRAW);
		

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(programID);

		Model = getRotateModel();
	//	View = lookAt(getLookAtPos(), vec3(0,0,0), vec3(0,1,0));
		MVP = Projection * View * Model;

		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

		glDrawArrays(GL_TRIANGLES, 0, PosList.size() * 3);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
			
		glfwSwapBuffers(window);
		glfwPollEvents();

		glDeleteBuffers(1, &vertexbuffer);
		glDeleteBuffers(1, &colorbuffer);
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
			if (cnt == Eq.size()){
				fprintf(stderr,"Finished Already!\n");
			}else{
				A.rotate(Eq[cnt++]);
				flag = 1;
				while (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
					glfwPollEvents();
				}
			}
		}
	}
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &colorbuffer);
	glDeleteProgram(programID);
	glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();
	return 0;
}