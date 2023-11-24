#include "objReader.h"
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Resize(int w, int h);
void Mouse(int button, int state, int x, int y);
void Motion(int xpos, int ypos);
void Keyboard(unsigned char key, int x, int y);
void Timer(int Value);
void make_vertexShaders(); //버텍스 세이더 만들기
void make_fragmentShaders(); //프래그먼트 세이더 만들기
void make_shaderProgram(); //세이더 프로그램 생성
void InitBuffer();
char* filetobuf(const char* file);

GLint height, width; //윈도우창 크기 
GLuint shaderProgram; //세이더 프로그램 이름
GLuint vertexShader;//버텍스 세이더 객체
GLuint fragmentShader;//프래그먼트 세이더 객체


struct OBJ {
	GLuint VAO, VBO_pos, VBO_normal, VBO_uv;
	std::vector<glm::vec3> m_vertices;
	std::vector<glm::vec2> m_uvs;
	std::vector<glm::vec3> m_normals;
};
OBJ tankbody;
OBJ block;
OBJ map;
OBJ item;
OBJ sphere;

//랜덤출력
using namespace std;

random_device rd;
default_random_engine dre(rd());
uniform_int_distribution<int> uid(-28, 28);


//함수선언
void convertDeviceXY2OpenglXY(int x, int y, float* xpos, float* ypos);
void start_page();

//충돌체크
bool collision_Chk(float aL, float aR, float aT, float aB, float bL, float bR, float bT, float bB);


bool collide_check_3(float aL, float aR, float aT, float aB, float aD, float aU, float bL, float bR, float bT, float bB, float bD, float bU);
void item_colliCHK();
bool mov_coliiCHK();
void item_setup();
void Setup_Block(); // 블럭 초기화
void get_Block(); // 블록 출력


//색상
struct COLOR
{
	float r = 1.0f;
	float g = 1.0f;
	float b = 1.0f;
};

//탱크
GLint tank_object = loadOBJ("try.obj", tankbody.m_vertices, tankbody.m_uvs, tankbody.m_normals);
struct Player_tank {
	int hp = 100;
	float x = 0.0, y = 0.0, z = 0.0;
}Player;

float rotate_bigY = 180; // 객체 자전

//----------------------------------------------------------------------------------------------------------
// 블록/맵
GLint mapobject = loadOBJ("cube.obj", map.m_vertices,map.m_uvs,map.m_normals);
GLint blockobject = loadOBJ("cube.obj", block.m_vertices,block.m_uvs,block.m_normals);


// 장애물
struct Map_Block
{
	bool exist = false;
	float height = 0;

	COLOR b_color;

	//좌표
	float x = 0.0;
	float y = 0.0;
	float z = 0.0;

	//크기
	float x_scale = 1.0;
	float y_scale = 2.0;
	float z_scale = 1.0;

};

Map_Block Block[30][30];

bool chaseOn = true;

//----------------------------------------------------------------------------------------------------------
//카메라
struct cameraLoc
{
	//pos
	float x = 1.0, y = 1.0, z = 3.0;
	// up
	float uX = 0.0, uY = 1.0, uZ = 0.0;

}CamPos;

int position = 1; //카메라 변경


//카메라
glm::vec3 cameraPos = glm::vec3(CamPos.x, CamPos.y, CamPos.z); //--- 카메라 위치
glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f); //--- 카메라 바라보는 방향
glm::vec3 cameraUp = glm::vec3(CamPos.uX, CamPos.uY, CamPos.uZ); //--- 카메라 위쪽 방향
glm::vec3 cameraFront = glm::vec3(0.0, 0.0, -1.0);


//마우스 콜백
float pitch, yaw;
bool firstMouse = true;
float xpos, ypos;
float lastX, lastY;

//------------------------------------------------------------------------------------------------------
//포탄 

GLint sphere_object = loadOBJ("sphere.obj", sphere.m_vertices,sphere.m_uvs,sphere.m_normals);

int k = 0; //포탄 넘버

struct Sphere_ {
	bool launch = false;
	float sphere_z = 0;
	float now_yaw = 0;
};

struct Sphere_ sphere_[500]; // 포탄의 갯수 
glm::mat4 spheres_pos;
glm::vec4 sphere_position; //포탄의 위치 (충돌처리용)


GLint itemobject = loadOBJ("cube.obj", item.m_vertices,item.m_uvs,item.m_normals);


struct Heart {
	bool exist = true;
	float x = 3.0;
	float y = 0.0;
	float z = 0.0;
};

struct Wheel {
	bool exist = true;
	float x = 5.0;
	float y = 0.0;
	float z = 0.0;
};

struct God {
	bool exist = true;
	float x = 7.0;
	float y = 0.0;
	float z = 0.0;
};

struct ICE {
	bool exist = true;
	float x = 9.0;
	float y = 0.0;
	float z = 0.0;
};

struct Heart heart[3];
struct Wheel wheel[3];
struct God god[3];
struct ICE ice[3];

//일정시간만 아이템 유지
int time_god = 0;
bool collide_god = false;
int time_ice = 0;
bool collide_ice = false;

//=======================================================================================================

bool start = true;

void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정
{ //--- 윈도우 생성하기
	height = 700;
	width = 700;
	glutInit(&argc, argv); // glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // 디스플레이 모드 설정
	glutInitWindowPosition(100, 100); // 윈도우의 위치 지정
	glutInitWindowSize(width, height); // 윈도우의 크기 지정
	glutCreateWindow("project");
	glewExperimental = GL_TRUE;
	glewInit();

	//make shaders
	make_vertexShaders(); //--- 버텍스 세이더 만들기
	make_fragmentShaders(); //--- 프래그먼트 세이더 만들기
	make_shaderProgram(); //--- 세이더 프로그램 만들기

	InitBuffer();
	item_setup();




	//call back
	glutDisplayFunc(drawScene); // 출력 함수의 지정
	glutReshapeFunc(Reshape); // 다시 그리기 함수 지정
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);//마우스 콜백
	glutPassiveMotionFunc(Motion);//마우스 움직임
	glutTimerFunc(30, Timer, 1); //애니매이션 타이머


	glutMainLoop(); // 이벤트 처리 시작 
}

//====================================================================================================

GLuint VAO_map[4], VBO_mapPos[4], VBO_mapnormal[4];
GLuint VAO_sphere, VBO_spherepos, VBO_spherenormal;
GLuint VAO_item[4], VBO_itemPos[4], VBO_itemnormal[4];

GLvoid InitBuffer() {

	//---------------------------------------------------------------------------------------------------------------------------
	//탱크 버퍼

	glGenVertexArrays(1, &VAO_map[0]);
	glBindVertexArray(VAO_map[0]);

	glGenBuffers(1, &VBO_mapPos[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_mapPos[0]);
	glBufferData((GL_ARRAY_BUFFER),tankbody.m_vertices.size()* sizeof(glm::vec3), &tankbody.m_vertices[0], GL_STATIC_DRAW);
	GLint pAttribute = glGetAttribLocation(shaderProgram, "aPos");
	glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
	glEnableVertexAttribArray(pAttribute);

	glGenBuffers(1, &VBO_mapnormal[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_mapnormal[0]);
	glBufferData(GL_ARRAY_BUFFER,tankbody.m_normals.size()* sizeof(glm::vec3), &tankbody.m_normals[0], GL_STATIC_DRAW);
	GLint nAttribute = glGetAttribLocation(shaderProgram, "aNormal");
	glVertexAttribPointer(nAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(nAttribute);



	//맵 버퍼---------------------------------------------------------------------------------------------------------------------------

	glGenVertexArrays(1, &VAO_map[1]);
	glBindVertexArray(VAO_map[1]);

	glGenBuffers(1, &VBO_mapPos[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_mapPos[1]);
	glBufferData((GL_ARRAY_BUFFER),map.m_vertices.size() * sizeof(glm::vec3), &map.m_vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &VBO_mapnormal[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_mapnormal[1]);
	glBufferData(GL_ARRAY_BUFFER,map.m_normals.size() * sizeof(glm::vec3), &map.m_normals[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(1);


	//블록
	glGenVertexArrays(1, &VAO_map[2]);
	glBindVertexArray(VAO_map[2]);

	glGenBuffers(1, &VBO_mapPos[2]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_mapPos[2]);
	glBufferData((GL_ARRAY_BUFFER),block.m_vertices.size()* sizeof(glm::vec3), &block.m_vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &VBO_mapnormal[2]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_mapnormal[2]);
	glBufferData(GL_ARRAY_BUFFER,tankbody.m_normals.size() * sizeof(glm::vec3), &tankbody.m_normals[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(1);
	
	//-------------------------------------------------------------------------------------------------------------------------------
	//포탄
	glGenVertexArrays(1, &VAO_sphere);
	glBindVertexArray(VAO_sphere);

	glGenBuffers(1, &VBO_spherepos);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_spherepos);
	glBufferData((GL_ARRAY_BUFFER), sphere.m_vertices.size() * sizeof(glm::vec3), &sphere.m_vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
	glEnableVertexAttribArray(pAttribute);

	glGenBuffers(1, &VBO_spherenormal);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_spherenormal);
	glBufferData(GL_ARRAY_BUFFER,sphere.m_normals.size() * sizeof(glm::vec3), &sphere.m_normals[0], GL_STATIC_DRAW);
	glVertexAttribPointer(nAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(nAttribute);

	//아이템 -------------------------------------------------------------------------------------------------------------------
	glGenVertexArrays(1, &VAO_item[0]);
	glBindVertexArray(VAO_item[0]);

	glGenBuffers(1, &VBO_itemPos[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_itemPos[0]);
	glBufferData((GL_ARRAY_BUFFER), item.m_vertices.size() * sizeof(glm::vec3), &item.m_vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
	glEnableVertexAttribArray(pAttribute);

	glGenBuffers(1, &VBO_itemnormal[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_itemnormal[0]);
	glBufferData(GL_ARRAY_BUFFER, item.m_normals.size() * sizeof(glm::vec3), &item.m_normals[0], GL_STATIC_DRAW);
	glVertexAttribPointer(nAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(nAttribute);


	// -------------------------------------------------------------------------------------------------------------------------
	//light
	glUseProgram(shaderProgram);
	unsigned int lightPosLocation = glGetUniformLocation(shaderProgram, "lightPos"); //--- lightPos 값 전달: (0.0, 0.0, 5.0);
	glUniform3f(lightPosLocation, 0.0, 0.0, 2.0);
	unsigned int lightColorLocation = glGetUniformLocation(shaderProgram, "lightColor"); //--- lightColor 값 전달: (1.0, 1.0, 1.0) 백색
	glUniform3f(lightColorLocation, 1.0, 1.0, 1.0);


	unsigned int viewPosLocation = glGetUniformLocation(shaderProgram, "viewPos"); //--- viewPos 값 전달: 카메라 위치
	glUniform3f(viewPosLocation, CamPos.x, CamPos.y, CamPos.z);
}
//====================================================================================================


void start_page() {

	glClearColor(0.0, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	//뷰 변환
	glm::mat4 view = glm::mat4(1.0f);
	cameraPos.x = 0.0; //카메라 위치를 탱크 위치로 고정
	cameraPos.z = 0.0;
	cameraPos.y = 65.0; //카메라를 위쪽(y축)으로 조정

	glm::vec3 Diretion = glm::vec3(0.0, 0.0, 0.0);
	cameraUp = glm::vec3(0.0, 0.0, -1.0); //--- 카메라 위쪽 방향


	view = glm::lookAt(cameraPos, Diretion, cameraUp);
	unsigned int viewLocation = glGetUniformLocation(shaderProgram, "viewTransform"); //--- 뷰잉 변환 설정
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);


	//투영변환
	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);
	projection = glm::translate(projection, glm::vec3(0.0, 0.0, -3.0));//--- 공간을 약간 뒤로 미뤄줌
	unsigned int projectionLocation = glGetUniformLocation(shaderProgram, "projectionTransform"); //--- 투영 변환 값 설정
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

}



GLvoid drawObj() {


	//탱크
	glBindVertexArray(VAO_map[0]);
	glm::mat4 model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(Player.x, Player.y, Player.z));
	model = glm::rotate(model, glm::radians(rotate_bigY), glm::vec3(0.0, 1.0, 0.0));
	int modeltrans = glGetUniformLocation(shaderProgram, "modeltrans");
	glUniformMatrix4fv(modeltrans, 1, GL_FALSE, glm::value_ptr(model));
	unsigned int objColorLocation = glGetUniformLocation(shaderProgram, "objectColor"); //--- object Color값 전달: (1.0, 0.5, 0.3)의 색
	glUniform3f(objColorLocation, 0.4, 0.8, 0.4);


	/*
	//35931 (vertex number)
	//body 
	glBindVertexArray(VAO_map[0]);
	glDrawArrays(GL_TRIANGLES, 0, 21000);


	glm::mat4 head = glm::mat4(1.0);
	head = glm::rotate(head, glm::radians(yaw), glm::vec3(0.0, 1.0, 0.0));
	head = model * head;
	glUniformMatrix4fv(modeltrans, 1, GL_FALSE, glm::value_ptr(head));
	//head
	glBindVertexArray(VAO_map[0]);
	glDrawArrays(GL_TRIANGLES, 21000, tank_object);
	*/

	glDrawArrays(GL_TRIANGLES, 0, tank_object);

	//-------------------------------------------------------------------------------------------------------------------------------
	//맵
	glBindVertexArray(VAO_map[1]);
	glm::mat4 map_floor = glm::mat4(1.0);
	map_floor = glm::translate(map_floor, glm::vec3(0, -1.5, 0));
	map_floor = glm::scale(map_floor, glm::vec3(30.0, 0.1, 30.0));

	unsigned int floor_model = glGetUniformLocation(shaderProgram, "modeltrans");
	glUniformMatrix4fv(floor_model, 1, GL_FALSE, glm::value_ptr(map_floor));

	unsigned int mapColor = glGetUniformLocation(shaderProgram, "objectColor");
	glUniform3f(objColorLocation, 0.3, 0.3, 0.3);

	glDrawArrays(GL_TRIANGLES, 0, map.m_vertices.size());


	//장애물 - 블록
	get_Block();

	
	//--------------------------------------------------------------------------------------------------
	//포탄
	if (sphere_[k].launch) {
		glm::mat4 spheres_scale = glm::mat4(1.0);
		spheres_pos = glm::mat4(1.0);
		if (!(Player.x == 0 && Player.y == 0 && Player.z == 0)) { //탱크의 위치가 원점이 아닌 경우 
			spheres_pos = glm::translate(spheres_pos, glm::vec3(Player.x, Player.y, Player.z)); //원점에서 회전 후 탱크의 이동으로 위치 변경 
		}
		spheres_pos = glm::rotate(spheres_pos, glm::radians(sphere_[k].now_yaw), glm::vec3(0.0, 1.0, 0.0));
		spheres_pos = glm::translate(spheres_pos, glm::vec3(0.0, 0.0, sphere_[k].sphere_z));
		spheres_scale = glm::scale(spheres_scale, glm::vec3(0.07, 0.07, 0.07));
		spheres_pos = spheres_pos * spheres_scale;
		glUniformMatrix4fv(modeltrans, 1, GL_FALSE, glm::value_ptr(spheres_pos));
		glBindVertexArray(VAO_sphere);
		glUniform3f(objColorLocation, 1.0, 1.0, 1.0);
		glDrawArrays(GL_TRIANGLES, 0, sphere_object);
	}
	//---------------------------------------------------------------------------------------------------------
	//아이템


	//체력 회복
	for (int i = 0; i < 3; i++) {
		if (heart[i].exist) {
			glm::mat4 hearts = glm::mat4(1.0);
			hearts = glm::translate(hearts, glm::vec3(heart[i].x, heart[i].y, heart[i].z));
			hearts = glm::scale(hearts, glm::vec3(0.2, 0.2, 0.2));
			glUniformMatrix4fv(modeltrans, 1, GL_FALSE, glm::value_ptr(hearts));
			glBindVertexArray(VAO_item[0]);
			glUniform3f(objColorLocation, 1.0, 0.0, 0.0);
			glDrawArrays(GL_TRIANGLES, 0, itemobject);
		}
	}

	//속도 증가
	for (int i = 0; i < 3; i++) {
		if (wheel[i].exist) {
			glm::mat4 velocity = glm::mat4(1.0);
			velocity = glm::translate(velocity, glm::vec3(wheel[i].x, wheel[i].y, wheel[i].z));
			velocity = glm::scale(velocity, glm::vec3(0.2, 0.2, 0.2));
			glUniformMatrix4fv(modeltrans, 1, GL_FALSE, glm::value_ptr(velocity));
			glBindVertexArray(VAO_item[0]);
			glUniform3f(objColorLocation, 0.0, 0.0, 1.0);
			glDrawArrays(GL_TRIANGLES, 0, itemobject);
		}
	}

	//무적 상태
	for (int i = 0; i < 3; i++) {
		if (god[i].exist) {
			glm::mat4 godmode = glm::mat4(1.0);
			godmode = glm::translate(godmode, glm::vec3(god[i].x, god[i].y, god[i].z));
			godmode = glm::scale(godmode, glm::vec3(0.2, 0.2, 0.2));
			glUniformMatrix4fv(modeltrans, 1, GL_FALSE, glm::value_ptr(godmode));
			glBindVertexArray(VAO_item[0]);
			glUniform3f(objColorLocation, 0.0, 1.0, 0.0);
			glDrawArrays(GL_TRIANGLES, 0, itemobject);
		}
	}

	//좀비 얼리기
	for (int i = 0; i < 3; i++) {
		if (ice[i].exist) {
			glm::mat4 freeze = glm::mat4(1.0);
			freeze = glm::translate(freeze, glm::vec3(ice[i].x, ice[i].y, ice[i].z));
			freeze = glm::scale(freeze, glm::vec3(0.2, 0.2, 0.2));
			glUniformMatrix4fv(modeltrans, 1, GL_FALSE, glm::value_ptr(freeze));
			glBindVertexArray(VAO_item[0]);
			glUniform3f(objColorLocation, 1.0, 1.0, 0.0);
			glDrawArrays(GL_TRIANGLES, 0, itemobject);
		}
	}

}



GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수
{
	//배경색 지정 
	glClearColor(0.3, 0.5f, 0.8f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	//깊이 검사 
	glEnable(GL_DEPTH_TEST);
	glUseProgram(shaderProgram);

	if (start) {
		glViewport(0, 0, width, height);
		start_page();
		drawObj();

	}

	else {


		//-------------------------------------------------------------------------------------------------------------------------------


		//뷰 변환
		glm::mat4 view = glm::mat4(1.0f);
		cameraPos.x = Player.x; //카메라 위치를 탱크 위치로 고정
		cameraPos.z = Player.z;
		cameraPos.y = CamPos.y; //카메라를 위쪽(y축)으로 조정

		//1인칭
		glViewport(0, 0, width, height);
		glm::vec3 cameraPos_1;
		cameraPos_1.x = Player.x; //카메라 위치를 탱크 위치로 고정
		cameraPos_1.z = Player.z;
		cameraPos_1.z -= 1.3;
		cameraPos_1.y = 0.7; //카메라를 위쪽(y축)으로 조정
		glm::vec3 cameraFront_1 = glm::vec3(0.0, 0.0, -1.0);

		//1인칭
		if (position == 0) {
			view = glm::lookAt(cameraPos_1, cameraPos_1 + cameraFront_1, cameraUp);
		}
		//3인칭
		else if (position == 1) {
			view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		}

		/*view = glm::lookAt(cameraPos, cameraDirection, cameraUp);*/
		unsigned int viewLocation = glGetUniformLocation(shaderProgram, "viewTransform"); //--- 뷰잉 변환 설정
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
		drawObj();

		
		//-------------------------------------------------------------------------------------------------------------------------------
		//투영변환
		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);
		projection = glm::translate(projection, glm::vec3(0.0, 0.0, -3.0));//--- 공간을 약간 뒤로 미뤄줌
		unsigned int projectionLocation = glGetUniformLocation(shaderProgram, "projectionTransform"); //--- 투영 변환 값 설정
		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);


		drawObj();

		//충돌처리----------------------------------------------------------------------------------------------
		//충돌 처리용 포탄 위치 기록
		glm::vec3 temp_position = glm::vec3(1.0);
		sphere_position = spheres_pos * glm::vec4(temp_position, 1.0);


		item_colliCHK();

		//-------------------------------------------------------------------------------------------------
	}
	glutSwapBuffers(); // 화면에 출력하기
}

//====================================================================================================
//애니메이션
void Timer(int Value) {

	

	//포탄 
	if (sphere_[k].launch) {
		sphere_[k].sphere_z -= 1.0;
		if (sphere_[k].sphere_z <= -15.0f) {
			k++;
			sphere_[k].launch = false;
		}
	}

	//무적시간 타이머
	if (collide_god) {
		time_god += 1;
		if (time_god == 1000) {
			collide_god = false;
		}
	}
	//얼리기 타이머
	if (collide_ice) {
		time_ice += 1;
		if (time_god == 1000) {
			chaseOn = true;
			collide_ice = false;
		}
	}


	glutTimerFunc(30, Timer, 1);
	glutPostRedisplay();

}



float speed = 0.3;
void Keyboard(unsigned char key, int x, int y)
{
	switch (key) {

		//탱크 이동
	case 'w':
		if (mov_coliiCHK()) Player.z += 0.3;
		else Player.z -= 0.3;
		break;
	case 'a':
		if (mov_coliiCHK()) Player.x += 0.3;
		else Player.x -= 0.3;
		break;
	case 's':
		if (mov_coliiCHK()) Player.z -= 0.3;
		else Player.z += 0.3; break;
	case 'd':
		if (mov_coliiCHK()) Player.x -= 0.3;
		else Player.x += 0.3;
		break;

	case 'W':
		if (mov_coliiCHK()) Player.z += 0.3;
		else Player.z -= 0.3;
		break;
	case 'A':
		if (mov_coliiCHK()) Player.x += 0.3;
		else Player.x -= 0.3;
		break;
	case 'S':
		if (mov_coliiCHK()) Player.z -= 0.3;
		else Player.z += 0.3; break;
	case 'D':
		if (mov_coliiCHK()) Player.x -= 0.3;
		else Player.x += 0.3;
		break;

		//좀비 따라옴 멈춤
	case 'c':
		if (chaseOn) chaseOn = false;
		else chaseOn = true;
		break;


		//게임 시작
	case '1':
		if (start) start = false;
		else start = true;
		break;

		//종료
	case 'q':
		glutLeaveMainLoop();
		break;
	case 'Q':
		glutLeaveMainLoop();
		break;
	}



	glutPostRedisplay();

}
//==충돌 체크=============================================================================================
bool collision_Chk(float aL, float aR, float aT, float aB, float bL, float bR, float bT, float bB) {

	if (bB <= aT || bT >= aB || bR <= aL || bL >= aR) return false;
	return true;
}

//벽&탱크
bool mov_coliiCHK() {
	for (int i = 0; i < 30; i++)
	{
		for (int j = 0; j < 30; j++)
		{
			if (Block[i][j].exist)
			{
				if (collision_Chk(Block[i][j].x - 1.0, Block[i][j].x + 1.0, Block[i][j].z - 2.0, Block[i][j].z + 2.0,
					Player.x - 0.5, Player.x + 0.5, Player.z - 0.5, Player.z + 0.5))
				{

					return true;
				}
			}

		}

	}
	return false;

}


//3차원 충돌 (수정 필요..)
bool collide_check_3(float aL, float aR, float aT, float aB, float aD, float aU, float bL, float bR, float bT, float bB, float bD, float bU) {
	if (aL <= bR && aR >= bL
		&& aB <= bT && aT >= bB
		&& aD <= bU && aU >= bD)
		return true;
	return false;
}






void item_colliCHK() {
	//체력 회복
	for (int i = 0; i < 3; i++)
	{
		if (heart[i].exist)
		{
			if (collision_Chk(heart[i].x - 0.3, heart[i].x + 0.3, heart[i].z - 0.3, heart[i].z + 0.3,
				Player.x - 0.3, Player.x + 0.3, Player.z - 0.3, Player.z + 0.3))
			{
				PlaySound(TEXT("item.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
				heart[i].exist = false;
				Player.hp += 30;
			}
		}
	}

	//속도 증가
	for (int i = 0; i < 3; i++)
	{
		if (wheel[i].exist)
		{
			if (collision_Chk(wheel[i].x - 0.3, wheel[i].x + 0.3, wheel[i].z - 0.3, wheel[i].z + 0.3,
				Player.x - 0.3, Player.x + 0.3, Player.z - 0.3, Player.z + 0.3))
			{
				PlaySound(TEXT("item.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
				wheel[i].exist = false;
				speed = 0.7;
			}
		}
	}

	//무적 상태
	for (int i = 0; i < 3; i++)
	{
		if (god[i].exist)
		{
			if (collision_Chk(god[i].x - 0.3, god[i].x + 0.3, god[i].z - 0.3, god[i].z + 0.3,
				Player.x - 0.3, Player.x + 0.3, Player.z - 0.3, Player.z + 0.3))
			{
				PlaySound(TEXT("item.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
				god[i].exist = false;
				collide_god = true;
			}
		}
	}


	

}


void item_setup() {


	float temp_x = 0.0;
	float temp_z = 0.0;
	for (int i = 0; i < 3; i++)
	{
		temp_x = uid(dre);
		temp_z = uid(dre);

		heart[i].x = temp_x;
		heart[i].z = temp_z;

		//-------------------------
		temp_x = uid(dre);
		temp_z = uid(dre);

		wheel[i].x = temp_x;
		wheel[i].z = temp_z;

		//-------------------------
		temp_x = uid(dre);
		temp_z = uid(dre);

		god[i].x = temp_x;
		god[i].z = temp_z;

		//-------------------------
		temp_x = uid(dre);
		temp_z = uid(dre);

		ice[i].x = temp_x;
		ice[i].z = temp_z;
	}

}

//==탱크 움직임=============================================================================================

void Mouse(int button, int state, int x, int y)
{
	//포탄 발사
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		sphere_[k].now_yaw = yaw;
		sphere_[k].launch = true;
		PlaySound(TEXT("gun.wav"), NULL, SND_FILENAME | SND_ASYNC);
	}

	//카메라 변경
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		position++;
		position %= 2;
	}

	glutPostRedisplay();
}

void Motion(int xpos, int ypos) {

	//convertDeviceXY2OpenglXY(x, y, &xpos, &ypos);
	//convertDeviceXY2OpenglXY(lastx, lasty, &lastX, &lastY);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.5;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	//limit 설정 
	if (yaw > 89.0f)
		yaw = 89.0f;
	if (yaw < -89.0f)
		yaw = -89.0f;

	//카메라 뷰 360회전 
	glm::vec3 front = glm::vec3(0.0, 0.0, -1.0);
	front.x = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	//front.y = sin(glm::radians(pitch));*/
	front.z = cos(glm::radians(yaw)) * cos(glm::radians(pitch));



	cameraFront = glm::normalize(front);

	glutPostRedisplay();
}





//==맵 블럭 초기화========================================================================================

void Setup_Block() {


	for (int i = 0; i < 30; i++)
	{
		for (int j = 0; j < 30; j++)
		{
			Block[i][j].b_color.r = 0.2;
			Block[i][j].b_color.g = 0.1;
			Block[i][j].b_color.b = 0.0;


			Block[i][j].x = -29 + j * 2;
			Block[i][j].z = -29 + i * 2;
		}
	}


	// 테두리 벽

	for (int i = 0; i < 30; i++)
	{
		Block[0][i].exist = true;
		Block[i][0].exist = true;

		Block[29][i].exist = true;
		Block[i][29].exist = true;
	}
}

void make_map() {

	for (int i = 0; i < 3; i++)
	{
		//작은 꺽쇄
		Block[8][8 + i].exist = true;
		Block[8 + i][8].exist = true;

		Block[20][8 + i].exist = true;
		Block[20 - i][8].exist = true;


		Block[8][20 - i].exist = true;
		Block[8 + i][20].exist = true;


		Block[20][20 - i].exist = true;
		Block[20 - i][20].exist = true;

	}


	for (int i = 0; i < 2; i++)
	{
		//중앙 사각형
		//Block[13][14 + i].exist = true;
		//Block[16][14 + i].exist = true;
		//
		//Block[14+i][13].exist = true;
		//Block[14+i][16].exist = true;
	}



	for (int i = 0; i < 4; i++)
	{
		//큰 꺽쇄
		Block[4][4 + i].exist = true;
		Block[4 + i][4].exist = true;

		Block[25][4 + i].exist = true;
		Block[25 - i][4].exist = true;


		Block[4][25 - i].exist = true;
		Block[4 + i][25].exist = true;


		Block[25][25 - i].exist = true;
		Block[25 - i][25].exist = true;
	}

}

void get_Block() {


	Setup_Block();
	make_map();

	glBindVertexArray(VAO_map[2]);

	for (int i = 0; i < 30; i++)
	{
		for (int j = 0; j < 30; j++)
		{
			if (Block[i][j].exist)
			{
				glm::mat4 map_block = glm::mat4(1.0);
				map_block = glm::translate(map_block, glm::vec3(Block[i][j].x, Block[i][j].y, Block[i][j].z));
				map_block = glm::scale(map_block, glm::vec3(Block[i][j].x_scale, Block[i][j].y_scale, Block[i][j].z_scale));


				unsigned int block_model = glGetUniformLocation(shaderProgram, "modeltrans");
				glUniformMatrix4fv(block_model, 1, GL_FALSE, glm::value_ptr(map_block));

				unsigned int blockColor = glGetUniformLocation(shaderProgram, "objectColor");
				glUniform3f(blockColor, Block[i][j].b_color.r, Block[i][j].b_color.g, Block[i][j].b_color.b);

				glDrawArrays(GL_TRIANGLES, 0, block.m_vertices.size());
			}


		}
	}


}

//==콜백 함수==========================================================================================

GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수 
{

	glViewport(0, 0, w, h);
}

GLvoid Resize(int w, int h)
{
	width = w;
	height = h;
	glViewport(0, 0, width, height);
}

//==쉐이더 함수========================================================================================

void make_vertexShaders() {


	GLchar* vertexSource;

	//버텍스 세이더 읽어와서 객체 만들고 컴파일하기
	vertexSource = filetobuf("vertex.glsl");
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader); //버텍스 컴파일 


	//에러코드 작성
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		std::cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}

}


void make_fragmentShaders() {

	GLchar* fragmentSource;

	//프래그먼트 세이더 읽어와서 객체 만들고 컴파일하기
	fragmentSource = filetobuf("fragment.glsl");
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader); //버텍스 컴파일 


	//에러코드 작성
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		std::cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}

}


void make_shaderProgram() {

	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	glLinkProgram(shaderProgram);


	//프로그램 생성 후 객체 삭제 가능
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	//에러코드 작성
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(shaderProgram, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(shaderProgram, 512, NULL, errorLog);
		std::cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}
	glUseProgram(shaderProgram);

}

//파일 읽기-----------------------
char* filetobuf(const char* file)
{
	FILE* fptr;
	long length;
	char* buf;
	fptr = fopen(file, "rb"); // Open file for reading 
	if (!fptr) // Return NULL on failure 
		return NULL;
	fseek(fptr, 0, SEEK_END); // Seek to the end of the file 
	length = ftell(fptr); // Find out how many bytes into the file we are 
	buf = (char*)malloc(length + 1); // Allocate a buffer for the entire length of the file and a null terminator 
	fseek(fptr, 0, SEEK_SET); // Go back to the beginning of the file 
	fread(buf, length, 1, fptr);// Read the contents of the file in to the buffer 
	fclose(fptr); // Close the file 
	buf[length] = 0;
	// Null terminator 
	return buf; // Return the buffer 
}

//==========================================================================================

void convertDeviceXY2OpenglXY(int x, int y, float* xpos, float* ypos)
{
	int w = width;
	int h = height;
	*xpos = (float)((x - (float)w / 2.0) * (float)(1.0 / (float)(w / 2.0)));
	*ypos = -(float)((y - (float)h / 2.0) * (float)(1.0 / (float)(h / 2.0)));
}