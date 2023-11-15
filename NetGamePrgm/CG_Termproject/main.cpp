#include "std.h"
#include "CRender.h"
#include "CMesh.h"
#include "CPlayer.h"
#include "CTexture.h"
#include "CShader.h"

struct player{
	int hp = 100;
	float x = 0.0, y = 0.0, z = 0.0;
	float scalex = 3;
}Player;
CTexture* texture;
CMesh* mesh;
CShader* shader;
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Resize(int w, int h);
void Mouse(int button, int state, int x, int y);
void Motion(int xpos, int ypos);
void Keyboard(unsigned char key, int x, int y);
void Timer(int Value);
void hpbar();
void InitBuffer();
GLint height, width; //윈도우창 크기 



//오브젝트 불러오기
std::vector< glm::vec3 > underbody_vertices[4];
std::vector< glm::vec2 > underbody_uvs[4];
std::vector< glm::vec3 > underbody_normals[4];


//랜덤출력
using namespace std;

random_device rd;
default_random_engine dre(rd());
uniform_int_distribution<int> uid(-28, 28);


//함수선언
void convertDeviceXY2OpenglXY(int x, int y, float* xpos, float* ypos);

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
float rotate_bigY = 180; // 객체 자전

//----------------------------------------------------------------------------------------------------------
// 블록/맵


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

//----------------------------------------------------------------------------------------------------------


bool chaseOn = true;
int zombie_cnt = 0;
float u_hpscalex = 2;
float u_hptransx = 0;
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

std::vector< glm::vec3 > sphere_vertices;
std::vector< glm::vec2 > sphere_uvs;
std::vector< glm::vec3 > sphere_normals;
GLint sphere_object = mesh->loadOBJ("sphere.obj", sphere_vertices, sphere_uvs, sphere_normals);

int k = 0; //포탄 넘버

struct Sphere {
	bool launch = false;
	float sphere_z = 0;
	float now_yaw = 0;
};

struct Sphere sphere[500]; // 포탄의 갯수 
glm::mat4 spheres_pos;
glm::vec4 sphere_position; //포탄의 위치 (충돌처리용)

//-------------------------------------------------------------------------------------------------------
//아이템 
std::vector< glm::vec3 > item_vertices[4];
std::vector< glm::vec2 > item_uvs[4];
std::vector< glm::vec3 > item_normals[4];
GLint cube = mesh->loadOBJ("cube.obj", item_vertices[0], item_uvs[0], item_normals[0]);

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
float scene_f = 1;
float scene_[]
{
-0.5f, -0.5f, 0.f, 0.0, 0.0, 1.0, 0.0, 0.0,
0.5f, -0.5f, 0.f, 0.0, 0.0, 1.0, 1.0, 0.0,
0.5f, 0.5f, 0.f, 0.0, 0.0, 1.0, 1.0, 1.0,
0.5f, 0.5f, 0.f, 0.0, 0.0, 1.0, 1.0, 1.0,
-0.5f, 0.5f, 0.f, 0.0, 0.0, 1.0, 0.0, 1.0,
-0.5f, -0.5f, 0.f, 0.0, 0.0, 1.0, 0.0, 0.0
};


//=======================================================================================================

bool start = true;
void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정
{ //--- 윈도우 생성하기
	height = 800;
	width = 1100;
	glutInit(&argc, argv); // glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // 디스플레이 모드 설정	glutInitWindowPosition(100, 100); // 윈도우의 위치 지정
	glutInitWindowSize(width, height); // 윈도우의 크기 지정
	glutCreateWindow("project");
	glewExperimental = GL_TRUE;
	glewInit();

	//make shaders
	shader = new CShader();
	shader->make_vertexShaders(); //--- 버텍스 세이더 만들기
	shader->make_fragmentShaders(); //--- 프래그먼트 세이더 만들기
	shader->make_shaderProgram(); //--- 세이더 프로그램 만들기

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


void convertDeviceXY2OpenglXY(int x, int y, float* xpos, float* ypos)
{
	int w = width;
	int h = height;
	*xpos = (float)((x - (float)w / 2.0) * (float)(1.0 / (float)(w / 2.0)));
	*ypos = -(float)((y - (float)h / 2.0) * (float)(1.0 / (float)(h / 2.0)));
	
}

void Mouse(int button, int state, int x, int y)
{
	//포탄 발사
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		sphere[k].now_yaw = yaw;
		sphere[k].launch = true;
		PlaySound(TEXT("gun.wav"), NULL, SND_FILENAME | SND_ASYNC);
	}

	//카메라 변경
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		position++;
		position %= 2;
	}

	glutPostRedisplay();
}
//====================================================================================================

GLuint VAO_map[4], VBO_mapPos[4], VBO_mapnormal[4];
GLuint VAO_sphere, VBO_spherepos, VBO_spherenormal;
GLuint VAO_item[4], VBO_itemPos[4], VBO_itemnormal[4];

typedef struct obj {
	unsigned int texture;
	GLuint VAO, VBO_pos, VBO_normal, VBO_uv;
	std::vector< glm::vec3 > out_vertices;
	std::vector< glm::vec2 > out_uvs;
	std::vector< glm::vec3 > out_normals;
};
obj brick;
obj scene;
obj tank;
obj map;
GLint map_object = mesh->loadOBJ("cube.obj",map.out_vertices,map.out_uvs,map.out_normals);
GLint tank_object = mesh->loadOBJ("try.obj", tank.out_vertices, tank.out_uvs, tank.out_normals);


GLvoid InitBuffer() {
	glGenVertexArrays(1, &scene.VAO);
	glGenBuffers(1, &scene.VBO_pos);
	glGenBuffers(1, &scene.VBO_normal);
	glGenBuffers(1, &scene.VBO_uv);

	glUseProgram(shader->Get_shaderProgram());
	glBindVertexArray(scene.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, scene.VBO_pos);
	glBufferData(GL_ARRAY_BUFFER, sizeof(scene_), scene_, GL_STATIC_DRAW);
	GLint pAttribute8 = glGetAttribLocation(shader->Get_shaderProgram(), "aPos");
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	glBindBuffer(GL_ARRAY_BUFFER, scene.VBO_normal);
	glBufferData(GL_ARRAY_BUFFER, sizeof(scene_), scene_, GL_STATIC_DRAW);
	GLint nAttribute8 = glGetAttribLocation(shader->Get_shaderProgram(), "aNormal");
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, scene.VBO_uv);
	glBufferData(GL_ARRAY_BUFFER, sizeof(scene_), scene_, GL_STATIC_DRAW);
	GLint cAttribute8 = glGetAttribLocation(shader->Get_shaderProgram(), "aTexCoord");
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	//---------------------------------------------------------------------------------------------------------------------------
	//탱크 버퍼



	glGenVertexArrays(1, &tank.VAO);
	glBindVertexArray(tank.VAO);

	glGenBuffers(1, &tank.VBO_pos);
	glBindBuffer(GL_ARRAY_BUFFER, tank.VBO_pos);
	glBufferData((GL_ARRAY_BUFFER), tank.out_vertices.size() * sizeof(glm::vec3), &tank.out_vertices.front(), GL_STATIC_DRAW);
	GLint pAttribute = glGetAttribLocation(shader->Get_shaderProgram(), "aPos");
	glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
	glEnableVertexAttribArray(pAttribute);

	glGenBuffers(1, &tank.VBO_normal);
	glBindBuffer(GL_ARRAY_BUFFER, tank.VBO_normal);
	glBufferData(GL_ARRAY_BUFFER, tank.out_normals.size() * sizeof(glm::vec3), &tank.out_normals.front(), GL_STATIC_DRAW);
	GLint nAttribute = glGetAttribLocation(shader->Get_shaderProgram(), "aNormal");
	glVertexAttribPointer(nAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(nAttribute);

	glGenBuffers(1, &tank.VBO_uv);
	glBindBuffer(GL_ARRAY_BUFFER, tank.VBO_uv);
	glBufferData(GL_ARRAY_BUFFER, tank.out_uvs.size() * sizeof(glm::vec2), &tank.out_uvs.front(), GL_STATIC_DRAW);
	GLint cAttribute9 = glGetAttribLocation(shader->Get_shaderProgram(), "aTexCoord");
	glVertexAttribPointer(cAttribute9, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
	glEnableVertexAttribArray(cAttribute9);
	//탱크 버퍼

	/*glGenVertexArrays(1, &tank.VAO);
	glBindVertexArray(tank.VAO);

	glGenBuffers(1, &tank.VBO_pos);
	glBindBuffer(GL_ARRAY_BUFFER, tank.VBO_pos);
	glBufferData((GL_ARRAY_BUFFER), tank.out_vertices.size() * sizeof(glm::vec3), &tank.out_vertices.front(), GL_STATIC_DRAW);
	GLint pAttribute = glGetAttribLocation(shader->Get_shaderProgram(), "aPos");
	glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
	glEnableVertexAttribArray(pAttribute);

	glGenBuffers(1, &tank.VBO_normal);
	glBindBuffer(GL_ARRAY_BUFFER, tank.VBO_normal);
	glBufferData(GL_ARRAY_BUFFER, tank.out_normals.size() * sizeof(glm::vec3), &tank.out_normals.front(), GL_STATIC_DRAW);
	GLint nAttribute = glGetAttribLocation(shader->Get_shaderProgram(), "aNormal");
	glVertexAttribPointer(nAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(nAttribute);

	glGenBuffers(1, &tank.VBO_uv);
	glBindBuffer(GL_ARRAY_BUFFER, tank.VBO_uv);
	glBufferData(GL_ARRAY_BUFFER, tank.out_uvs.size() * sizeof(glm::vec2), &tank.out_uvs.front(), GL_STATIC_DRAW);
	GLint cAttribute9 = glGetAttribLocation(shader->Get_shaderProgram(), "aTexCoord");
	glVertexAttribPointer(cAttribute9, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
	glEnableVertexAttribArray(cAttribute9);*/


	//맵 버퍼---------------------------------------------------------------------------------------------------------------------------

	glGenVertexArrays(1, &map.VAO);
	glBindVertexArray(map.VAO);

	glGenBuffers(1, &map.VBO_pos);
	glBindBuffer(GL_ARRAY_BUFFER, map.VBO_pos);
	glBufferData((GL_ARRAY_BUFFER), map.out_vertices.size() * sizeof(glm::vec3), &map.out_vertices.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &map.VBO_normal);
	glBindBuffer(GL_ARRAY_BUFFER, map.VBO_normal);
	glBufferData(GL_ARRAY_BUFFER, map.out_normals.size() * sizeof(glm::vec3), &map.out_normals.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(1);


	

	//-------------------------------------------------------------------------------------------------------------------------------
	//포탄
	glGenVertexArrays(1, &VAO_sphere);
	glBindVertexArray(VAO_sphere);

	glGenBuffers(1, &VBO_spherepos);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_spherepos);
	glBufferData((GL_ARRAY_BUFFER), sphere_vertices.size() * sizeof(glm::vec3), &sphere_vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
	glEnableVertexAttribArray(pAttribute);

	glGenBuffers(1, &VBO_spherenormal);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_spherenormal);
	glBufferData(GL_ARRAY_BUFFER, sphere_normals.size() * sizeof(glm::vec3), &sphere_normals[0], GL_STATIC_DRAW);
	glVertexAttribPointer(nAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(nAttribute);

	//아이템 -------------------------------------------------------------------------------------------------------------------
	//glGenVertexArrays(1, &VAO_item[0]);
	//glBindVertexArray(VAO_item[0]);

	//glGenBuffers(1, &VBO_itemPos[0]);
	//glBindBuffer(GL_ARRAY_BUFFER, VBO_itemPos[0]);
	//glBufferData((GL_ARRAY_BUFFER), item_vertices[0].size() * sizeof(glm::vec3), &item_vertices[0][0], GL_STATIC_DRAW);
	//glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
	//glEnableVertexAttribArray(pAttribute);

	//glGenBuffers(1, &VBO_itemnormal[0]);
	//glBindBuffer(GL_ARRAY_BUFFER, VBO_itemnormal[0]);
	//glBufferData(GL_ARRAY_BUFFER, item_normals[0].size() * sizeof(glm::vec3), &item_normals[0][0], GL_STATIC_DRAW);
	//glVertexAttribPointer(nAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	//glEnableVertexAttribArray(nAttribute);


	// -------------------------------------------------------------------------------------------------------------------------
	//light
	glUseProgram(shader->Get_shaderProgram());
	unsigned int lightPosLocation = glGetUniformLocation(shader->Get_shaderProgram(), "lightPos"); //--- lightPos 값 전달: (0.0, 0.0, 5.0);
	glUniform3f(lightPosLocation, 0.0, 0.0, 5.0);
	unsigned int lightColorLocation = glGetUniformLocation(shader->Get_shaderProgram(), "lightColor"); //--- lightColor 값 전달: (1.0, 1.0, 1.0) 백색
	glUniform3f(lightColorLocation, 1.0, 1.0, 1.0);


	unsigned int viewPosLocation = glGetUniformLocation(shader->Get_shaderProgram(), "viewPos"); //--- viewPos 값 전달: 카메라 위치
	glUniform3f(viewPosLocation, CamPos.x, CamPos.y, CamPos.z);
}
//====================================================================================================



GLvoid drawObj() {


	glBindVertexArray(tank.VAO);
	glm::mat4 model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(Player.x, Player.y, Player.z));
	model = glm::rotate(model, glm::radians(rotate_bigY), glm::vec3(0.0, 1.0, 0.0));
	model = glm::scale(model, glm::vec3(1, 1, 1));

	int modeltrans = glGetUniformLocation(shader->Get_shaderProgram(), "modeltrans");
	glUniformMatrix4fv(modeltrans, 1, GL_FALSE, glm::value_ptr(model));
	unsigned int objColorLocation = glGetUniformLocation(shader->Get_shaderProgram(), "objectColor"); //--- object Color값 전달: (1.0, 0.5, 0.3)의 색
	glUniform3f(objColorLocation, 2, 2, 2);
	texture->InitTexture("t_tank2.png", &tank.texture);
	glBindTexture(GL_TEXTURE_2D, tank.texture);
	glBindVertexArray(tank.VAO);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glUseProgram(shader->Get_shaderProgram());

	glm::mat4 head = glm::mat4(1.0);
	head = glm::rotate(head, glm::radians(yaw), glm::vec3(0.0, 1.0, 0.0));
	head = model * head;
	glUniformMatrix4fv(modeltrans, 1, GL_FALSE, glm::value_ptr(head));
	//head
	glBindVertexArray(tank.VAO);
	glDrawArrays(GL_TRIANGLES, 21000, tank_object);


	glDrawArrays(GL_TRIANGLES, 0, tank_object);

	//-------------------------------------------------------------------------------------------------------------------------------
	//맵
	glBindVertexArray(map.VAO);
	glm::mat4 map_floor = glm::mat4(1.0);
	map_floor = glm::translate(map_floor, glm::vec3(0, -1.5, 0));
	map_floor = glm::scale(map_floor, glm::vec3(30.0, 0.1, 30.0));

	unsigned int floor_model = glGetUniformLocation(shader->Get_shaderProgram(), "modeltrans");
	glUniformMatrix4fv(floor_model, 1, GL_FALSE, glm::value_ptr(map_floor));

	unsigned int mapColor = glGetUniformLocation(shader->Get_shaderProgram(), "objectColor");
	glUniform3f(objColorLocation, 0.3, 0.3, 0.3);
	glDrawArrays(GL_TRIANGLES, 0,map.out_vertices.size());
	//glDrawArrays(GL_TRIANGLES, 0, underbody_vertices[2].size());
	texture->InitTexture("hpbar2.png", &brick.texture);
	glBindTexture(GL_TEXTURE_2D, brick.texture);
	glBindVertexArray(map.VAO);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glUseProgram(shader->Get_shaderProgram());
	//장애물 - 블록
	get_Block();

	//-------------------------------------------------------------------------------------------------------------------------------


	//--------------------------------------------------------------------------------------------------
	//포탄
	if (sphere[k].launch) {
		glm::mat4 spheres_scale = glm::mat4(1.0);
		spheres_pos = glm::mat4(1.0);
		if (!(Player.x == 0 && Player.y == 0 && Player.z == 0)) { //탱크의 위치가 원점이 아닌 경우 
			spheres_pos = glm::translate(spheres_pos, glm::vec3(Player.x, Player.y, Player.z)); //원점에서 회전 후 탱크의 이동으로 위치 변경 
		}
		spheres_pos = glm::rotate(spheres_pos, glm::radians(sphere[k].now_yaw), glm::vec3(0.0, 1.0, 0.0));
		spheres_pos = glm::translate(spheres_pos, glm::vec3(0.0, 0.0, sphere[k].sphere_z));
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
			glDrawArrays(GL_TRIANGLES, 0, cube);
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
			glDrawArrays(GL_TRIANGLES, 0, cube);
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
			glDrawArrays(GL_TRIANGLES, 0, cube);
		}
	}


}



GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수
{
	//배경색 지정 
	glClearColor(0.3, 0.5f, 0.8f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	unsigned int modelLocation = glGetUniformLocation(shader->Get_shaderProgram(), "modeltrans");
	//glUseProgram(t_shaderProgram);

	//깊이 검사 
	glEnable(GL_DEPTH_TEST);
	glUseProgram(shader->Get_shaderProgram());

	if (start) {
		glViewport(0, 0, width, height);
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
		unsigned int viewLocation = glGetUniformLocation(shader->Get_shaderProgram(), "viewTransform"); //--- 뷰잉 변환 설정
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
		drawObj();

		//////미니맵
		//glViewport(width * 3 / 4, height * 3 / 4, width / 4, height / 4);
		//view = glm::mat4(1.0f);
		//glm::vec3 map_pos = glm::vec3(0.0, 65.0, 0); //--- 카메라 위치
		//glm::vec3 mapDir = glm::vec3(0.0, 0.0f, 0.0f); //--- 카메라 바라보는 방향
		//glm::vec3 mapUp = glm::vec3(0.0f, 0.0f, -1.0f); //--- 카메라 위쪽 방향
		//view = glm::lookAt(map_pos, mapDir, mapUp);
		//glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);


		//-------------------------------------------------------------------------------------------------------------------------------
		//투영변환
		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);
		projection = glm::translate(projection, glm::vec3(0.0, 0.0, -3.0));//--- 공간을 약간 뒤로 미뤄줌
		unsigned int projectionLocation = glGetUniformLocation(shader->Get_shaderProgram(), "projectionTransform"); //--- 투영 변환 값 설정
		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);


		drawObj();

		//충돌처리----------------------------------------------------------------------------------------------
		//충돌 처리용 포탄 위치 기록
		glm::vec3 temp_position = glm::vec3(1.0);
		sphere_position = spheres_pos * glm::vec4(temp_position, 1.0);



		item_colliCHK();


		{glViewport(0, 0, width, height);
		view = glm::mat4(1.0f);
		glm::vec3 map_pos = glm::vec3(0.0, 0.0, 1.0); //--- 카메라 위치
		glm::vec3 mapDir = glm::vec3(0.0, 0.0f, 0.0f); //--- 카메라 바라보는 방향
		glm::vec3 mapUp = glm::vec3(0.0f, 1.0f, 0.0f); //--- 카메라 위쪽 방향
		view = glm::lookAt(map_pos, mapDir, mapUp);

		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
		unsigned int viewLocation = glGetUniformLocation(shader->Get_shaderProgram(), "viewTransform");
		glBindVertexArray(VAO_map[0]);
		glm::mat4 model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(u_hptransx, 3, -5));
		model = glm::rotate(model, glm::radians(rotate_bigY), glm::vec3(0.0, 1.0, 0.0));
		model = glm::scale(model, glm::vec3(u_hpscalex, 0.2, 0.5));

		int modeltrans = glGetUniformLocation(shader->Get_shaderProgram(), "modeltrans");
		glUniformMatrix4fv(modeltrans, 1, GL_FALSE, glm::value_ptr(model));
		unsigned int objColorLocation = glGetUniformLocation(shader->Get_shaderProgram(), "objectColor"); //--- object Color값 전달: (1.0, 0.5, 0.3)의 색
		glUniform3f(objColorLocation, 2, 2, 2);
		texture->InitTexture("hpbar2.png", &scene.texture);
		glBindTexture(GL_TEXTURE_2D, scene.texture);
		glBindVertexArray(scene.VAO);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUseProgram(shader->Get_shaderProgram()); }
	}
	glutSwapBuffers(); // 화면에 출력하기
}

//====================================================================================================
//애니메이션
void Timer(int Value) {



	//포탄 
	if (sphere[k].launch) {
		sphere[k].sphere_z -= 1.0;
		if (sphere[k].sphere_z <= -15.0f) {
			k++;
			sphere[k].launch = false;
		}
	}

	//무적시간 타이머
	if (collide_god) {
		time_god += 1;
		if (time_god == 1000) {
			collide_god = false;
		}
	}
	hpbar();
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
		//디버깅용

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


void get_Block() {


	Setup_Block();
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


				unsigned int block_model = glGetUniformLocation(shader->Get_shaderProgram(), "modeltrans");
				glUniformMatrix4fv(block_model, 1, GL_FALSE, glm::value_ptr(map_block));

				unsigned int blockColor = glGetUniformLocation(shader->Get_shaderProgram(), "objectColor");
				glUniform3f(blockColor, Block[i][j].b_color.r, Block[i][j].b_color.g, Block[i][j].b_color.b);



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


void hpbar() {
	int hpbardir = 1;
	if (u_hpscalex > 0.01) {
		hpbardir = 1;
	}
	else {
		hpbardir = 0;

	}
	u_hpscalex -= 0.002 * hpbardir;
	u_hptransx -= 0.001 * hpbardir;
	
}