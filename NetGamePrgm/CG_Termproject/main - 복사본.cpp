//#include "std.h"
//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"
//GLvoid drawScene(GLvoid);
//GLvoid Reshape(int w, int h);
//GLvoid Resize(int w, int h);
//void Mouse(int button, int state, int x, int y);
//void Motion(int xpos, int ypos);
//void Keyboard(unsigned char key, int x, int y);
//void Timer(int Value);
//void hpbar();
//void make_vertexShaders(); //���ؽ� ���̴� �����
//void make_fragmentShaders(); //�����׸�Ʈ ���̴� �����
//void make_shaderProgram(); //���̴� ���α׷� ����
//void InitBuffer();
//char* filetobuf(const char* file);
//void InitTexture(const char* filename, unsigned int texture);
//GLint height, width; //������â ũ�� 
//GLuint shaderProgram; //���̴� ���α׷� �̸�
//GLuint t_shaderProgram; //���̴� ���α׷� �̸�
//
//GLuint vertexShader;//���ؽ� ���̴� ��ü
//GLuint fragmentShader;//�����׸�Ʈ ���̴� ��ü
//GLuint t_fragmentShader;//�����׸�Ʈ ���̴� ��ü
//
////������Ʈ �ҷ�����
//std::vector< glm::vec3 > underbody_vertices[4];
//std::vector< glm::vec2 > underbody_uvs[4];
//std::vector< glm::vec3 > underbody_normals[4];
//
//
////�������
//using namespace std;
//
//random_device rd;
//default_random_engine dre(rd());
//uniform_int_distribution<int> uid(-28, 28);
//
//
////�Լ�����
//void convertDeviceXY2OpenglXY(int x, int y, float* xpos, float* ypos);
//void start_page();
//
////�浹üũ
//bool collision_Chk(float aL, float aR, float aT, float aB, float bL, float bR, float bT, float bB);
//
//
//bool collide_check_3(float aL, float aR, float aT, float aB, float aD, float aU, float bL, float bR, float bT, float bB, float bD, float bU);
//
//void item_colliCHK();
//bool mov_coliiCHK();
//
//void item_setup();
//
//
//void Setup_Block(); // �� �ʱ�ȭ
//void get_Block(); // ��� ���
//
////����
//struct COLOR
//{
//	float r = 1.0f;
//	float g = 1.0f;
//	float b = 1.0f;
//};
//
////��ũ
//GLint tank_object = loadOBJ("m26.obj", underbody_vertices[0], underbody_uvs[0], underbody_normals[0]);
//struct Player_tank {
//	int hp = 100;
//	float x = 0.0, y = 0.0, z = 0.0;
//	float scalex=3;
//}Player;
//
//float rotate_bigY = 180; // ��ü ����
//
////----------------------------------------------------------------------------------------------------------
//// ���/��
//GLint map = loadOBJ("cube.obj", underbody_vertices[1], underbody_uvs[1], underbody_normals[1]);
//GLint block = loadOBJ("cube.obj", underbody_vertices[2], underbody_uvs[2], underbody_normals[2]);
//
//
//// ��ֹ�
//struct Map_Block
//{
//	bool exist = false;
//	float height = 0;
//
//	COLOR b_color;
//
//	//��ǥ
//	float x = 0.0;
//	float y = 0.0;
//	float z = 0.0;
//
//	//ũ��
//	float x_scale = 1.0;
//	float y_scale = 2.0;
//	float z_scale = 1.0;
//
//};
//
//Map_Block Block[30][30];
//
////----------------------------------------------------------------------------------------------------------
//
//
//bool chaseOn = true;
//int zombie_cnt = 0;
//float u_hpscalex =2;
//float u_hptransx = 0;
////----------------------------------------------------------------------------------------------------------
////ī�޶�
//struct cameraLoc
//{
//	//pos
//	float x = 1.0, y = 1.0, z = 3.0;
//	// up
//	float uX = 0.0, uY = 1.0, uZ = 0.0;
//
//}CamPos;
//
//int position = 1; //ī�޶� ����
//
//
////ī�޶�
//glm::vec3 cameraPos = glm::vec3(CamPos.x, CamPos.y, CamPos.z); //--- ī�޶� ��ġ
//glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f); //--- ī�޶� �ٶ󺸴� ����
//glm::vec3 cameraUp = glm::vec3(CamPos.uX, CamPos.uY, CamPos.uZ); //--- ī�޶� ���� ����
//glm::vec3 cameraFront = glm::vec3(0.0, 0.0, -1.0);
//
//
////���콺 �ݹ�
//float pitch, yaw;
//bool firstMouse = true;
//float xpos, ypos;
//float lastX, lastY;
//
////------------------------------------------------------------------------------------------------------
////��ź 
//
//std::vector< glm::vec3 > sphere_vertices;
//std::vector< glm::vec2 > sphere_uvs;
//std::vector< glm::vec3 > sphere_normals;
//GLint sphere_object = mesh->loadOBJ("sphere.obj", sphere_vertices, sphere_uvs, sphere_normals);
//
//int k = 0; //��ź �ѹ�
//
//struct Sphere {
//	bool launch = false;
//	float sphere_z = 0;
//	float now_yaw = 0;
//};
//
//struct Sphere sphere[500]; // ��ź�� ���� 
//glm::mat4 spheres_pos;
//glm::vec4 sphere_position; //��ź�� ��ġ (�浹ó����)
//
////-------------------------------------------------------------------------------------------------------
////������ 
//std::vector< glm::vec3 > item_vertices[4];
//std::vector< glm::vec2 > item_uvs[4];
//std::vector< glm::vec3 > item_normals[4];
//GLint cube = loadOBJ("cube.obj", item_vertices[0], item_uvs[0], item_normals[0]);
//
//typedef struct obj {
//	unsigned int texture;
//	GLuint VAO, VBO_pos, VBO_normal, VBO_uv;
//	std::vector< glm::vec3 > out_vertices;
//	std::vector< glm::vec2 > out_uvs;
//	std::vector< glm::vec3 > out_normals;
//};
//obj brick;
//obj scene;
//obj tank;
//struct Heart {
//	bool exist = true;
//	float x = 3.0;
//	float y = 0.0;
//	float z = 0.0;
//};
//
//struct Wheel {
//	bool exist = true;
//	float x = 5.0;
//	float y = 0.0;
//	float z = 0.0;
//};
//
//struct God {
//	bool exist = true;
//	float x = 7.0;
//	float y = 0.0;
//	float z = 0.0;
//};
//
//struct ICE {
//	bool exist = true;
//	float x = 9.0;
//	float y = 0.0;
//	float z = 0.0;
//};
//
//struct Heart heart[3];
//struct Wheel wheel[3];
//struct God god[3];
//struct ICE ice[3];
//
////�����ð��� ������ ����
//int time_god = 0;
//bool collide_god = false;
//int time_ice = 0;
//bool collide_ice = false;
//float scene_f = 1;
//float scene_[]
//{
//-0.5f, -0.5f, 0.f, 0.0, 0.0, 1.0, 0.0, 0.0,
//0.5f, -0.5f, 0.f, 0.0, 0.0, 1.0, 1.0, 0.0,
//0.5f, 0.5f, 0.f, 0.0, 0.0, 1.0, 1.0, 1.0,
//0.5f, 0.5f, 0.f, 0.0, 0.0, 1.0, 1.0, 1.0,
//-0.5f, 0.5f, 0.f, 0.0, 0.0, 1.0, 0.0, 1.0,
//-0.5f, -0.5f, 0.f, 0.0, 0.0, 1.0, 0.0, 0.0
//};
//
//
////=======================================================================================================
//
//bool start = true;
//void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ����
//{ //--- ������ �����ϱ�
//	height = 800;
//	width = 1100;
//	glutInit(&argc, argv); // glut �ʱ�ȭ
//	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // ���÷��� ��� ����
//	glutInitWindowPosition(100, 100); // �������� ��ġ ����
//	glutInitWindowSize(width, height); // �������� ũ�� ����
//	glutCreateWindow("project");
//	glewExperimental = GL_TRUE;
//	glewInit();
//	glewInit();
//	
//	//make shaders
//	make_vertexShaders(); //--- ���ؽ� ���̴� �����
//	make_fragmentShaders(); //--- �����׸�Ʈ ���̴� �����
//	make_shaderProgram(); //--- ���̴� ���α׷� �����
//
//	InitBuffer();
//	item_setup();
//
//	
//	//call back
//	glutDisplayFunc(drawScene); // ��� �Լ��� ����
//	glutReshapeFunc(Reshape); // �ٽ� �׸��� �Լ� ����
//	glutKeyboardFunc(Keyboard);
//	glutMouseFunc(Mouse);//���콺 �ݹ�
//	glutPassiveMotionFunc(Motion);//���콺 ������
//	glutTimerFunc(30, Timer, 1); //�ִϸ��̼� Ÿ�̸�
//
//
//	glutMainLoop(); // �̺�Ʈ ó�� ���� 
//}
//
////====================================================================================================
//
//GLuint VAO_map[4], VBO_mapPos[4], VBO_mapnormal[4];
//GLuint VAO_sphere, VBO_spherepos, VBO_spherenormal;
//GLuint VAO_item[4], VBO_itemPos[4], VBO_itemnormal[4];
//
//
//GLvoid InitBuffer() {
//	glGenVertexArrays(1, &scene.VAO);
//	glGenBuffers(1, &scene.VBO_pos);
//	glGenBuffers(1, &scene.VBO_normal);
//	glGenBuffers(1, &scene.VBO_uv);
//
//	glUseProgram(t_shaderProgram);
//	glBindVertexArray(scene.VAO);
//	glBindBuffer(GL_ARRAY_BUFFER, scene.VBO_pos);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(scene_), scene_, GL_STATIC_DRAW);
//	GLint pAttribute8 = glGetAttribLocation(t_shaderProgram, "aPos");
//	glVertexAttribPointer(pAttribute8, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
//	glEnableVertexAttribArray(pAttribute8);
//
//
//	glBindBuffer(GL_ARRAY_BUFFER, scene.VBO_normal);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(scene_), scene_, GL_STATIC_DRAW);
//	GLint nAttribute8 = glGetAttribLocation(t_shaderProgram, "aNormal");
//	glVertexAttribPointer(nAttribute8, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
//	glEnableVertexAttribArray(nAttribute8);
//
//	glBindBuffer(GL_ARRAY_BUFFER, scene.VBO_uv);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(scene_), scene_, GL_STATIC_DRAW);
//	GLint cAttribute8 = glGetAttribLocation(t_shaderProgram, "aTexCoord");
//	glVertexAttribPointer(cAttribute8, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
//	glEnableVertexAttribArray(cAttribute8);
//	//---------------------------------------------------------------------------------------------------------------------------
//	//��ũ ����
//
//	glGenVertexArrays(1, &VAO_map[0]);
//	glBindVertexArray(VAO_map[0]);
//
//	glGenBuffers(1, &VBO_mapPos[0]);
//	glBindBuffer(GL_ARRAY_BUFFER, VBO_mapPos[0]);
//	glBufferData((GL_ARRAY_BUFFER), underbody_vertices[0].size() * sizeof(glm::vec3), &underbody_vertices[0][0], GL_STATIC_DRAW);
//	GLint pAttribute = glGetAttribLocation(shaderProgram, "aPos");
//	glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
//	glEnableVertexAttribArray(pAttribute);
//
//	glGenBuffers(1, &VBO_mapnormal[0]);
//	glBindBuffer(GL_ARRAY_BUFFER, VBO_mapnormal[0]);
//	glBufferData(GL_ARRAY_BUFFER, underbody_normals[0].size() * sizeof(glm::vec3), &underbody_normals[0][0], GL_STATIC_DRAW);
//	GLint nAttribute = glGetAttribLocation(shaderProgram, "aNormal");
//	glVertexAttribPointer(nAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
//	glEnableVertexAttribArray(nAttribute);
//
//	glBindBuffer(GL_ARRAY_BUFFER, VBO_mapnormal[0]);
//	glBufferData(GL_ARRAY_BUFFER, underbody_normals[0].size() * sizeof(glm::vec3), &underbody_normals[0][0], GL_STATIC_DRAW);
//	GLint cAttribute9 = glGetAttribLocation(shaderProgram, "aTexCoord");
//	glVertexAttribPointer(cAttribute9, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
//	glEnableVertexAttribArray(cAttribute9);
//
//	//�� ����---------------------------------------------------------------------------------------------------------------------------
//
//	glGenVertexArrays(1, &VAO_map[1]);
//	glBindVertexArray(VAO_map[1]);
//
//	glGenBuffers(1, &VBO_mapPos[1]);
//	glBindBuffer(GL_ARRAY_BUFFER, VBO_mapPos[1]);
//	glBufferData((GL_ARRAY_BUFFER), underbody_vertices[1].size() * sizeof(glm::vec3), &underbody_vertices[1][0], GL_STATIC_DRAW);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
//	glEnableVertexAttribArray(0);
//
//	glGenBuffers(1, &VBO_mapnormal[1]);
//	glBindBuffer(GL_ARRAY_BUFFER, VBO_mapnormal[1]);
//	glBufferData(GL_ARRAY_BUFFER, underbody_normals[1].size() * sizeof(glm::vec3), &underbody_normals[1][0], GL_STATIC_DRAW);
//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
//	glEnableVertexAttribArray(1);
//
//
//	//���
//	glGenVertexArrays(1, &VAO_map[2]);
//	glBindVertexArray(VAO_map[2]);
//
//	glGenBuffers(1, &VBO_mapPos[2]);
//	glBindBuffer(GL_ARRAY_BUFFER, VBO_mapPos[2]);
//	glBufferData((GL_ARRAY_BUFFER), underbody_vertices[2].size() * sizeof(glm::vec3), &underbody_vertices[2][0], GL_STATIC_DRAW);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
//	glEnableVertexAttribArray(0);
//
//	glGenBuffers(1, &VBO_mapnormal[2]);
//	glBindBuffer(GL_ARRAY_BUFFER, VBO_mapnormal[2]);
//	glBufferData(GL_ARRAY_BUFFER, underbody_normals[2].size() * sizeof(glm::vec3), &underbody_normals[2][0], GL_STATIC_DRAW);
//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
//	glEnableVertexAttribArray(1);
//	//-------------------------------------------------------------------------------------------------------------------------------
//	
//	//-------------------------------------------------------------------------------------------------------------------------------
//	//��ź
//	glGenVertexArrays(1, &VAO_sphere);
//	glBindVertexArray(VAO_sphere);
//
//	glGenBuffers(1, &VBO_spherepos);
//	glBindBuffer(GL_ARRAY_BUFFER, VBO_spherepos);
//	glBufferData((GL_ARRAY_BUFFER), sphere_vertices.size() * sizeof(glm::vec3), &sphere_vertices[0], GL_STATIC_DRAW);
//	glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
//	glEnableVertexAttribArray(pAttribute);
//
//	glGenBuffers(1, &VBO_spherenormal);
//	glBindBuffer(GL_ARRAY_BUFFER, VBO_spherenormal);
//	glBufferData(GL_ARRAY_BUFFER, sphere_normals.size() * sizeof(glm::vec3), &sphere_normals[0], GL_STATIC_DRAW);
//	glVertexAttribPointer(nAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
//	glEnableVertexAttribArray(nAttribute);
//
//	//������ -------------------------------------------------------------------------------------------------------------------
//	glGenVertexArrays(1, &VAO_item[0]);
//	glBindVertexArray(VAO_item[0]);
//
//	glGenBuffers(1, &VBO_itemPos[0]);
//	glBindBuffer(GL_ARRAY_BUFFER, VBO_itemPos[0]);
//	glBufferData((GL_ARRAY_BUFFER), item_vertices[0].size() * sizeof(glm::vec3), &item_vertices[0][0], GL_STATIC_DRAW);
//	glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
//	glEnableVertexAttribArray(pAttribute);
//
//	glGenBuffers(1, &VBO_itemnormal[0]);
//	glBindBuffer(GL_ARRAY_BUFFER, VBO_itemnormal[0]);
//	glBufferData(GL_ARRAY_BUFFER, item_normals[0].size() * sizeof(glm::vec3), &item_normals[0][0], GL_STATIC_DRAW);
//	glVertexAttribPointer(nAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
//	glEnableVertexAttribArray(nAttribute);
//
//
//	// -------------------------------------------------------------------------------------------------------------------------
//	//light
//	glUseProgram(shaderProgram);
//	unsigned int lightPosLocation = glGetUniformLocation(shaderProgram, "lightPos"); //--- lightPos �� ����: (0.0, 0.0, 5.0);
//	glUniform3f(lightPosLocation, 0.0, 0.0, 5.0);
//	unsigned int lightColorLocation = glGetUniformLocation(shaderProgram, "lightColor"); //--- lightColor �� ����: (1.0, 1.0, 1.0) ���
//	glUniform3f(lightColorLocation, 1.0, 1.0, 1.0);
//
//
//	unsigned int viewPosLocation = glGetUniformLocation(shaderProgram, "viewPos"); //--- viewPos �� ����: ī�޶� ��ġ
//	glUniform3f(viewPosLocation, CamPos.x, CamPos.y, CamPos.z);
//}
////====================================================================================================
//
//
//void start_page() {
//
//	glClearColor(0.0, 0.0f, 0.0f, 0.0f);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//
//	//�� ��ȯ
//	glm::mat4 view = glm::mat4(1.0f);
//	cameraPos.x = 0.0; //ī�޶� ��ġ�� ��ũ ��ġ�� ����
//	cameraPos.z = 0.5;
//	cameraPos.y = CamPos.y - 0.5; //ī�޶� ����(y��)���� ����
//
//	glm::vec3 Diretion = glm::vec3(1.0, 0.0, -1.0);
//
//
//	view = glm::lookAt(cameraPos, Diretion, cameraUp);
//	unsigned int viewLocation = glGetUniformLocation(shaderProgram, "viewTransform"); //--- ���� ��ȯ ����
//	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
//
//
//	//������ȯ
//	glm::mat4 projection = glm::mat4(1.0f);
//	projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);
//	projection = glm::translate(projection, glm::vec3(0.0, 0.0, -3.0));//--- ������ �ణ �ڷ� �̷���
//	unsigned int projectionLocation = glGetUniformLocation(shaderProgram, "projectionTransform"); //--- ���� ��ȯ �� ����
//	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);
//
//
//
//	glm::mat4 model = glm::mat4(1.0);
//	model = glm::translate(model, glm::vec3(0.0, 0.0, 0.0));
//	model = glm::rotate(model, glm::radians(rotate_bigY), glm::vec3(0.0, 1.0, 0.0));
//	int modeltrans = glGetUniformLocation(shaderProgram, "modeltrans");
//	glUniformMatrix4fv(modeltrans, 1, GL_FALSE, glm::value_ptr(model));
//	unsigned int objColorLocation = glGetUniformLocation(shaderProgram, "objectColor"); //--- object Color�� ����: (1.0, 0.5, 0.3)�� ��
//	glUniform3f(objColorLocation, 0.5, 0.8, 0.4);
//
//	glBindVertexArray(VAO_map[0]);
//	glDrawArrays(GL_TRIANGLES, 0, tank_object);
//
//	glBindVertexArray(VAO_map[3]);
//	glm::mat4 zombies = glm::mat4(1.0);
//	zombies = glm::translate(zombies, glm::vec3(0.0, 0, -2.0));
//	zombies = glm::rotate(zombies, glm::radians(0.0f), glm::vec3(0.0, 1.0, 0.0));
//	zombies = glm::scale(zombies, glm::vec3(1.0, 1.0, 1.0));
//
//	unsigned int zombies_model = glGetUniformLocation(shaderProgram, "modeltrans");
//	glUniformMatrix4fv(zombies_model, 1, GL_FALSE, glm::value_ptr(zombies));
//
//	glUniform3f(objColorLocation, 0.2, 0.4, 0.0);
//
//	glDrawArrays(GL_TRIANGLES, 0, underbody_vertices[3].size());
//
//}
//
//
//
//GLvoid drawObj() {
//
//
//	//��ũ
//	glBindVertexArray(VAO_map[0]);
//	glm::mat4 model = glm::mat4(1.0);
//	model = glm::translate(model, glm::vec3(Player.x, Player.y, Player.z));
//	model = glm::rotate(model, glm::radians(rotate_bigY), glm::vec3(0.0, 1.0, 0.0));
//	model = glm::scale(model, glm::vec3(1,1,1));
//
//	int modeltrans = glGetUniformLocation(shaderProgram, "modeltrans");
//	glUniformMatrix4fv(modeltrans, 1, GL_FALSE, glm::value_ptr(model));
//	unsigned int objColorLocation = glGetUniformLocation(shaderProgram, "objectColor"); //--- object Color�� ����: (1.0, 0.5, 0.3)�� ��
//	glUniform3f(objColorLocation,2,2,2);
//	InitTexture("t_yellow.png", tank.texture);
//	glBindTexture(GL_TEXTURE_2D, tank.texture);
//	glBindVertexArray(VAO_map[0]);
//	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//	glDrawArrays(GL_TRIANGLES, 0, 6);
//	glUseProgram(shaderProgram);
//
//	glm::mat4 head = glm::mat4(1.0);
//	head = glm::rotate(head, glm::radians(yaw), glm::vec3(0.0, 1.0, 0.0));
//	head = model * head;
//	glUniformMatrix4fv(modeltrans, 1, GL_FALSE, glm::value_ptr(head));
//	//head
//	glBindVertexArray(VAO_map[0]);
//	glDrawArrays(GL_TRIANGLES, 21000, tank_object);
//
//
//	glDrawArrays(GL_TRIANGLES, 0, tank_object);
//
//	//-------------------------------------------------------------------------------------------------------------------------------
//	//��
//	glBindVertexArray(VAO_map[1]);
//	glm::mat4 map_floor = glm::mat4(1.0);
//	map_floor = glm::translate(map_floor, glm::vec3(0, -1.5, 0));
//	map_floor = glm::scale(map_floor, glm::vec3(30.0, 0.1, 30.0));
//
//	unsigned int floor_model = glGetUniformLocation(shaderProgram, "modeltrans");
//	glUniformMatrix4fv(floor_model, 1, GL_FALSE, glm::value_ptr(map_floor));
//
//	unsigned int mapColor = glGetUniformLocation(shaderProgram, "objectColor");
//	glUniform3f(objColorLocation, 0.3, 0.3, 0.3);
//	glDrawArrays(GL_TRIANGLES, 0, underbody_vertices[1].size());
//	glDrawArrays(GL_TRIANGLES, 0, underbody_vertices[2].size());
//				InitTexture("t_yellow.png", brick.texture);
//				glBindTexture(GL_TEXTURE_2D, brick.texture);
//				glBindVertexArray(VAO_map[1]);
//				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//				glDrawArrays(GL_TRIANGLES, 0, 6);
//				glUseProgram(shaderProgram);
//	//��ֹ� - ���
//	get_Block();
//
//	//-------------------------------------------------------------------------------------------------------------------------------
//	
//
//	//--------------------------------------------------------------------------------------------------
//	//��ź
//	if (sphere[k].launch) {
//		glm::mat4 spheres_scale = glm::mat4(1.0);
//		spheres_pos = glm::mat4(1.0);
//		if (!(Player.x == 0 && Player.y == 0 && Player.z == 0)) { //��ũ�� ��ġ�� ������ �ƴ� ��� 
//			spheres_pos = glm::translate(spheres_pos, glm::vec3(Player.x, Player.y, Player.z)); //�������� ȸ�� �� ��ũ�� �̵����� ��ġ ���� 
//		}
//		spheres_pos = glm::rotate(spheres_pos, glm::radians(sphere[k].now_yaw), glm::vec3(0.0, 1.0, 0.0));
//		spheres_pos = glm::translate(spheres_pos, glm::vec3(0.0, 0.0, sphere[k].sphere_z));
//		spheres_scale = glm::scale(spheres_scale, glm::vec3(0.07, 0.07, 0.07));
//		spheres_pos = spheres_pos * spheres_scale;
//		glUniformMatrix4fv(modeltrans, 1, GL_FALSE, glm::value_ptr(spheres_pos));
//		glBindVertexArray(VAO_sphere);
//		glUniform3f(objColorLocation, 1.0, 1.0, 1.0);
//		glDrawArrays(GL_TRIANGLES, 0, sphere_object);
//	}
//	//---------------------------------------------------------------------------------------------------------
//	//������
//
//
//	//ü�� ȸ��
//	for (int i = 0; i < 3; i++) {
//		if (heart[i].exist) {
//			glm::mat4 hearts = glm::mat4(1.0);
//			hearts = glm::translate(hearts, glm::vec3(heart[i].x, heart[i].y, heart[i].z));
//			hearts = glm::scale(hearts, glm::vec3(0.2, 0.2, 0.2));
//			glUniformMatrix4fv(modeltrans, 1, GL_FALSE, glm::value_ptr(hearts));
//			glBindVertexArray(VAO_item[0]);
//			glUniform3f(objColorLocation, 1.0, 0.0, 0.0);
//			glDrawArrays(GL_TRIANGLES, 0, cube);
//		}
//	}
//
//	//�ӵ� ����
//	for (int i = 0; i < 3; i++) {
//		if (wheel[i].exist) {
//			glm::mat4 velocity = glm::mat4(1.0);
//			velocity = glm::translate(velocity, glm::vec3(wheel[i].x, wheel[i].y, wheel[i].z));
//			velocity = glm::scale(velocity, glm::vec3(0.2, 0.2, 0.2));
//			glUniformMatrix4fv(modeltrans, 1, GL_FALSE, glm::value_ptr(velocity));
//			glBindVertexArray(VAO_item[0]);
//			glUniform3f(objColorLocation, 0.0, 0.0, 1.0);
//			glDrawArrays(GL_TRIANGLES, 0, cube);
//		}
//	}
//
//	//���� ����
//	for (int i = 0; i < 3; i++) {
//		if (god[i].exist) {
//			glm::mat4 godmode = glm::mat4(1.0);
//			godmode = glm::translate(godmode, glm::vec3(god[i].x, god[i].y, god[i].z));
//			godmode = glm::scale(godmode, glm::vec3(0.2, 0.2, 0.2));
//			glUniformMatrix4fv(modeltrans, 1, GL_FALSE, glm::value_ptr(godmode));
//			glBindVertexArray(VAO_item[0]);
//			glUniform3f(objColorLocation, 0.0, 1.0, 0.0);
//			glDrawArrays(GL_TRIANGLES, 0, cube);
//		}
//	}
//
//
//}
//
//
//
//GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ�
//{
//	//���� ���� 
//	glClearColor(0.3, 0.5f, 0.8f, 0.0f);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//	unsigned int modelLocation = glGetUniformLocation(shaderProgram, "modeltrans");
//	//glUseProgram(t_shaderProgram);
//
//	//���� �˻� 
//	glEnable(GL_DEPTH_TEST);
//	glUseProgram(shaderProgram);
//
//	if (start) {
//		glViewport(0, 0, width, height);
//		start_page();
//	}
//
//	else {
//
//
//		//-------------------------------------------------------------------------------------------------------------------------------
//
//
//		//�� ��ȯ
//		glm::mat4 view = glm::mat4(1.0f);
//		cameraPos.x = Player.x; //ī�޶� ��ġ�� ��ũ ��ġ�� ����
//		cameraPos.z = Player.z;
//		cameraPos.y = CamPos.y; //ī�޶� ����(y��)���� ����
//
//		//1��Ī
//		glViewport(0, 0, width, height);
//		glm::vec3 cameraPos_1;
//		cameraPos_1.x = Player.x; //ī�޶� ��ġ�� ��ũ ��ġ�� ����
//		cameraPos_1.z = Player.z;
//		cameraPos_1.z -= 1.3;
//		cameraPos_1.y = 0.7; //ī�޶� ����(y��)���� ����
//		glm::vec3 cameraFront_1 = glm::vec3(0.0, 0.0, -1.0);
//
//		//1��Ī
//		if (position == 0) {
//			view = glm::lookAt(cameraPos_1, cameraPos_1 + cameraFront_1, cameraUp);
//		}
//		//3��Ī
//		else if (position == 1) {
//			view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
//		}
//
//		/*view = glm::lookAt(cameraPos, cameraDirection, cameraUp);*/
//		unsigned int viewLocation = glGetUniformLocation(shaderProgram, "viewTransform"); //--- ���� ��ȯ ����
//		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
//		drawObj();
//
//		//////�̴ϸ�
//		//glViewport(width * 3 / 4, height * 3 / 4, width / 4, height / 4);
//		//view = glm::mat4(1.0f);
//		//glm::vec3 map_pos = glm::vec3(0.0, 65.0, 0); //--- ī�޶� ��ġ
//		//glm::vec3 mapDir = glm::vec3(0.0, 0.0f, 0.0f); //--- ī�޶� �ٶ󺸴� ����
//		//glm::vec3 mapUp = glm::vec3(0.0f, 0.0f, -1.0f); //--- ī�޶� ���� ����
//		//view = glm::lookAt(map_pos, mapDir, mapUp);
//		//glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
//
//
//		//-------------------------------------------------------------------------------------------------------------------------------
//		//������ȯ
//		glm::mat4 projection = glm::mat4(1.0f);
//		projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);
//		projection = glm::translate(projection, glm::vec3(0.0, 0.0, -3.0));//--- ������ �ణ �ڷ� �̷���
//		unsigned int projectionLocation = glGetUniformLocation(shaderProgram, "projectionTransform"); //--- ���� ��ȯ �� ����
//		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);
//
//
//		drawObj();
//
//		//�浹ó��----------------------------------------------------------------------------------------------
//		//�浹 ó���� ��ź ��ġ ���
//		glm::vec3 temp_position = glm::vec3(1.0);
//		sphere_position = spheres_pos * glm::vec4(temp_position, 1.0);
//
//	
//	
//		item_colliCHK();
//
//
//		{glViewport(0, 0, width, height);
//		view = glm::mat4(1.0f);
//		glm::vec3 map_pos = glm::vec3(0.0, 0.0, 1.0); //--- ī�޶� ��ġ
//		glm::vec3 mapDir = glm::vec3(0.0, 0.0f, 0.0f); //--- ī�޶� �ٶ󺸴� ����
//		glm::vec3 mapUp = glm::vec3(0.0f, 1.0f, 0.0f); //--- ī�޶� ���� ����
//		view = glm::lookAt(map_pos, mapDir, mapUp);
//
//		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
//		unsigned int viewLocation = glGetUniformLocation(t_shaderProgram, "viewTransform");
//			glBindVertexArray(VAO_map[0]);
//		glm::mat4 model = glm::mat4(1.0);
//		model = glm::translate(model, glm::vec3(u_hptransx,3,-5));
//		model = glm::rotate(model, glm::radians(rotate_bigY), glm::vec3(0.0, 1.0, 0.0));
//		model = glm::scale(model, glm::vec3(u_hpscalex, 0.2, 0.5));
//
//		int modeltrans = glGetUniformLocation(shaderProgram, "modeltrans");
//		glUniformMatrix4fv(modeltrans, 1, GL_FALSE, glm::value_ptr(model));
//		unsigned int objColorLocation = glGetUniformLocation(shaderProgram, "objectColor"); //--- object Color�� ����: (1.0, 0.5, 0.3)�� ��
//		glUniform3f(objColorLocation, 2, 2, 2);
//		InitTexture("hpbar2.png", scene.texture);
//		glBindTexture(GL_TEXTURE_2D, scene.texture);
//		glBindVertexArray(scene.VAO);
//		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//		glDrawArrays(GL_TRIANGLES, 0, 6);
//		glUseProgram(shaderProgram); }
//	}
//	glutSwapBuffers(); // ȭ�鿡 ����ϱ�
//}
//
////====================================================================================================
////�ִϸ��̼�
//void Timer(int Value) {
//
//	
//
//	//��ź 
//	if (sphere[k].launch) {
//		sphere[k].sphere_z -= 1.0;
//		if (sphere[k].sphere_z <= -15.0f) {
//			k++;
//			sphere[k].launch = false;
//		}
//	}
//
//	//�����ð� Ÿ�̸�
//	if (collide_god) {
//		time_god += 1;
//		if (time_god == 1000) {
//			collide_god = false;
//		}
//	}
//	hpbar();
//	glutTimerFunc(30, Timer, 1);
//	glutPostRedisplay();
//
//}
//
//
//float speed = 0.3;
//void Keyboard(unsigned char key, int x, int y)
//{
//	switch (key) {
//
//		//��ũ �̵�
//	case 'w':
//		if (mov_coliiCHK()) Player.z += 0.3;
//		else Player.z -= 0.3;
//		break;
//	case 'a':
//		if (mov_coliiCHK()) Player.x += 0.3;
//		else Player.x -= 0.3;
//		break;
//	case 's':
//		if (mov_coliiCHK()) Player.z -= 0.3;
//		else Player.z += 0.3; break;
//	case 'd':
//		if (mov_coliiCHK()) Player.x -= 0.3;
//		else Player.x += 0.3;
//		break;
//
//	case 'W':
//		if (mov_coliiCHK()) Player.z += 0.3;
//		else Player.z -= 0.3;
//		break;
//	case 'A':
//		if (mov_coliiCHK()) Player.x += 0.3;
//		else Player.x -= 0.3;
//		break;
//	case 'S':
//		if (mov_coliiCHK()) Player.z -= 0.3;
//		else Player.z += 0.3; break;
//	case 'D':
//		if (mov_coliiCHK()) Player.x -= 0.3;
//		else Player.x += 0.3;
//		break;
//
//		//���� ����� ����
//	case 'c':
//		if (chaseOn) chaseOn = false;
//		else chaseOn = true;
//		break;
//
//
//		//���� ����
//	case '1':
//		if (start) start = false;
//		else start = true;
//		break;
//		//������
//	case '2':
//		Player.hp -= 5;
//		cout << Player.hp << endl;
//		break;
//		//����
//	case 'q':
//		glutLeaveMainLoop();
//		break;
//	case 'Q':
//		glutLeaveMainLoop();
//		break;
//	}
//
//
//
//	glutPostRedisplay();
//
//}
////==�浹 üũ=============================================================================================
//bool collision_Chk(float aL, float aR, float aT, float aB, float bL, float bR, float bT, float bB) {
//
//	if (bB <= aT || bT >= aB || bR <= aL || bL >= aR) return false;
//	return true;
//}
//
//
//
//
////3���� �浹 (���� �ʿ�..)
//bool collide_check_3(float aL, float aR, float aT, float aB, float aD, float aU, float bL, float bR, float bT, float bB, float bD, float bU) {
//	if (aL <= bR && aR >= bL
//		&& aB <= bT && aT >= bB
//		&& aD <= bU && aU >= bD)
//		return true;
//	return false;
//}
//
//
//
//
//
//
//void item_colliCHK() {
//	//ü�� ȸ��
//	for (int i = 0; i < 3; i++)
//	{
//		if (heart[i].exist)
//		{
//			if (collision_Chk(heart[i].x - 0.3, heart[i].x + 0.3, heart[i].z - 0.3, heart[i].z + 0.3,
//				Player.x - 0.3, Player.x + 0.3, Player.z - 0.3, Player.z + 0.3))
//			{
//				PlaySound(TEXT("item.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
//				heart[i].exist = false;
//				Player.hp += 30;
//			}
//		}
//	}
//
//	//�ӵ� ����
//	for (int i = 0; i < 3; i++)
//	{
//		if (wheel[i].exist)
//		{
//			if (collision_Chk(wheel[i].x - 0.3, wheel[i].x + 0.3, wheel[i].z - 0.3, wheel[i].z + 0.3,
//				Player.x - 0.3, Player.x + 0.3, Player.z - 0.3, Player.z + 0.3))
//			{
//				PlaySound(TEXT("item.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
//				wheel[i].exist = false;
//				speed = 0.7;
//			}
//		}
//	}
//
//	//���� ����
//	for (int i = 0; i < 3; i++)
//	{
//		if (god[i].exist)
//		{
//			if (collision_Chk(god[i].x - 0.3, god[i].x + 0.3, god[i].z - 0.3, god[i].z + 0.3,
//				Player.x - 0.3, Player.x + 0.3, Player.z - 0.3, Player.z + 0.3))
//			{
//				PlaySound(TEXT("item.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
//				god[i].exist = false;
//				collide_god = true;
//			}
//		}
//	}
//
//
//	
//
//}
//
//
//void item_setup() {
//
//
//	float temp_x = 0.0;
//	float temp_z = 0.0;
//	for (int i = 0; i < 3; i++)
//	{
//		temp_x = uid(dre);
//		temp_z = uid(dre);
//
//		heart[i].x = temp_x;
//		heart[i].z = temp_z;
//
//		//-------------------------
//		temp_x = uid(dre);
//		temp_z = uid(dre);
//
//		wheel[i].x = temp_x;
//		wheel[i].z = temp_z;
//
//		//-------------------------
//		temp_x = uid(dre);
//		temp_z = uid(dre);
//
//		god[i].x = temp_x;
//		god[i].z = temp_z;
//
//		//-------------------------
//		temp_x = uid(dre);
//		temp_z = uid(dre);
//
//		ice[i].x = temp_x;
//		ice[i].z = temp_z;
//	}
//
//}
//
////==��ũ ������=============================================================================================
//
//void Mouse(int button, int state, int x, int y)
//{
//	//��ź �߻�
//	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
//		sphere[k].now_yaw = yaw;
//		sphere[k].launch = true;
//		PlaySound(TEXT("gun.wav"), NULL, SND_FILENAME | SND_ASYNC);
//	}
//
//	//ī�޶� ����
//	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
//		position++;
//		position %= 2;
//	}
//
//	glutPostRedisplay();
//}
//
//void Motion(int xpos, int ypos) {
//
//	//convertDeviceXY2OpenglXY(x, y, &xpos, &ypos);
//	//convertDeviceXY2OpenglXY(lastx, lasty, &lastX, &lastY);
//
//	if (firstMouse)
//	{
//		lastX = xpos;
//		lastY = ypos;
//		firstMouse = false;
//	}
//
//	float xoffset = xpos - lastX;
//	float yoffset = lastY - ypos;
//	lastX = xpos;
//	lastY = ypos;
//
//	float sensitivity = 0.5;
//	xoffset *= sensitivity;
//	yoffset *= sensitivity;
//
//	yaw += xoffset;
//	pitch += yoffset;
//
//	//limit ���� 
//	if (yaw > 89.0f)
//		yaw = 89.0f;
//	if (yaw < -89.0f)
//		yaw = -89.0f;
//
//	//ī�޶� �� 360ȸ�� 
//	glm::vec3 front = glm::vec3(0.0, 0.0, -1.0);
//	front.x = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
//	//front.y = sin(glm::radians(pitch));*/
//	front.z = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
//
//
//
//	cameraFront = glm::normalize(front);
//
//	glutPostRedisplay();
//}
//
//
//
//
//
////==�� �� �ʱ�ȭ========================================================================================
//
//void Setup_Block() {
//
//
//	for (int i = 0; i < 30; i++)
//	{
//		for (int j = 0; j < 30; j++)
//		{
//			Block[i][j].b_color.r = 0.2;
//			Block[i][j].b_color.g = 0.1;
//			Block[i][j].b_color.b = 0.0;
//
//
//			Block[i][j].x = -29 + j * 2;
//			Block[i][j].z = -29 + i * 2;
//		}
//	}
//
//
//	// �׵θ� ��
//
//	for (int i = 0; i < 30; i++)
//	{
//		Block[0][i].exist = true;
//		Block[i][0].exist = true;
//
//		Block[29][i].exist = true;
//		Block[i][29].exist = true;
//	}
//}
//
//void make_map() {
//
//	for (int i = 0; i < 3; i++)
//	{
//		//���� ����
//		Block[8][8 + i].exist = true;
//		Block[8 + i][8].exist = true;
//
//		Block[20][8 + i].exist = true;
//		Block[20 - i][8].exist = true;
//
//
//		Block[8][20 - i].exist = true;
//		Block[8 + i][20].exist = true;
//
//
//		Block[20][20 - i].exist = true;
//		Block[20 - i][20].exist = true;
//
//	}
//
//
//	for (int i = 0; i < 2; i++)
//	{
//		//�߾� �簢��
//		//Block[13][14 + i].exist = true;
//		//Block[16][14 + i].exist = true;
//		//
//		//Block[14+i][13].exist = true;
//		//Block[14+i][16].exist = true;
//	}
//
//
//
//	for (int i = 0; i < 4; i++)
//	{
//		//ū ����
//		Block[4][4 + i].exist = true;
//		Block[4 + i][4].exist = true;
//
//		Block[25][4 + i].exist = true;
//		Block[25 - i][4].exist = true;
//
//
//		Block[4][25 - i].exist = true;
//		Block[4 + i][25].exist = true;
//
//
//		Block[25][25 - i].exist = true;
//		Block[25 - i][25].exist = true;
//	}
//
//}
//
//void get_Block() {
//
//
//	Setup_Block();
//	make_map();
//
//	glBindVertexArray(VAO_map[2]);
//
//	for (int i = 0; i < 30; i++)
//	{
//		for (int j = 0; j < 30; j++)
//		{
//			if (Block[i][j].exist)
//			{
//				glm::mat4 map_block = glm::mat4(1.0);
//				map_block = glm::translate(map_block, glm::vec3(Block[i][j].x, Block[i][j].y, Block[i][j].z));
//				map_block = glm::scale(map_block, glm::vec3(Block[i][j].x_scale, Block[i][j].y_scale, Block[i][j].z_scale));
//
//
//				unsigned int block_model = glGetUniformLocation(shaderProgram, "modeltrans");
//				glUniformMatrix4fv(block_model, 1, GL_FALSE, glm::value_ptr(map_block));
//
//				unsigned int blockColor = glGetUniformLocation(shaderProgram, "objectColor");
//				glUniform3f(blockColor, Block[i][j].b_color.r, Block[i][j].b_color.g, Block[i][j].b_color.b);
//
//		
//
//			}
//
//
//		}
//	}
//
//
//}
//
////==�ݹ� �Լ�==========================================================================================
//
//GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ� 
//{
//
//	glViewport(0, 0, w, h);
//}
//
//GLvoid Resize(int w, int h)
//{
//	width = w;
//	height = h;
//	glViewport(0, 0, width, height);
//}
//
////==���̴� �Լ�========================================================================================
//
//void make_vertexShaders() {
//
//
//	GLchar* vertexSource;
//
//	//���ؽ� ���̴� �о�ͼ� ��ü ����� �������ϱ�
//	vertexSource = filetobuf("vertex.glsl");
//	vertexShader = glCreateShader(GL_VERTEX_SHADER);
//	glShaderSource(vertexShader, 1, &vertexSource, NULL);
//	glCompileShader(vertexShader); //���ؽ� ������ 
//
//
//	//�����ڵ� �ۼ�
//	GLint result;
//	GLchar errorLog[512];
//	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
//	if (!result)
//	{
//		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
//		std::cerr << "ERROR: vertex shader ������ ����\n" << errorLog << std::endl;
//		return;
//	}
//
//}
//
//
//void make_fragmentShaders() {
//
//	GLchar* fragmentSource;
//
//	//�����׸�Ʈ ���̴� �о�ͼ� ��ü ����� �������ϱ�
//	fragmentSource = filetobuf("fragment.glsl");
//	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
//	glCompileShader(fragmentShader); //���ؽ� ������
//
//
//	GLchar* t_fragmentSource;
//
//	//�����׸�Ʈ ���̴� �о�ͼ� ��ü ����� �������ϱ�
//	t_fragmentSource = filetobuf("t_fragment.glsl");
//	t_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//	glShaderSource(t_fragmentShader, 1, &t_fragmentSource, NULL);
//	glCompileShader(t_fragmentShader); //���ؽ� ������ 
//
//
//	//�����ڵ� �ۼ�
//	GLint result;
//	GLchar errorLog[512];
//	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
//	if (!result)
//	{
//		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
//		std::cerr << "ERROR: vertex shader ������ ����\n" << errorLog << std::endl;
//		return;
//	}
//
//
//	//�����ڵ� �ۼ�
//	GLint t_result;
//	GLchar t_errorLog[512];
//	glGetShaderiv(t_fragmentShader, GL_COMPILE_STATUS, &t_result);
//	if (!result)
//	{
//		glGetShaderInfoLog(t_fragmentShader, 512, NULL, t_errorLog);
//		std::cerr << "ERROR: vertex shader ������ ����\n" << t_errorLog << std::endl;
//		return;
//	}
//
//}
//
//
//void make_shaderProgram() {
//
//	shaderProgram = glCreateProgram();
//
//	glAttachShader(shaderProgram, vertexShader);
//	glAttachShader(shaderProgram, fragmentShader);
//
//	glLinkProgram(shaderProgram);
//
//
//	t_shaderProgram = glCreateProgram();
//
//	glAttachShader(t_shaderProgram, vertexShader);
//	glAttachShader(t_shaderProgram, t_fragmentShader);
//
//	glLinkProgram(t_shaderProgram);
//
//	//���α׷� ���� �� ��ü ���� ����
//	glDeleteShader(vertexShader);
//	glDeleteShader(fragmentShader);
//	glDeleteShader(t_fragmentShader);
//
//
//
//
//
//	//�����ڵ� �ۼ�
//	GLint result;
//	GLchar errorLog[512];
//	glGetShaderiv(shaderProgram, GL_COMPILE_STATUS, &result);
//	if (!result)
//	{
//		glGetShaderInfoLog(shaderProgram, 512, NULL, errorLog);
//		std::cerr << "ERROR: vertex shader ������ ����\n" << errorLog << std::endl;
//		return;
//	}
//
//
//
//	//glUseProgram(shaderProgram);
//	//�����ڵ� �ۼ�
//	GLint t_result;
//	GLchar t_errorLog[512];
//	glGetShaderiv(t_shaderProgram, GL_COMPILE_STATUS, &t_result);
//	if (!result)
//	{
//		glGetShaderInfoLog(t_shaderProgram, 512, NULL, t_errorLog);
//		std::cerr << "ERROR: vertex shader ������ ����\n" << t_errorLog << std::endl;
//		return;
//	}
//	//glUseProgram(t_shaderProgram);
//
//}
//
////���� �б�-----------------------
//char* filetobuf(const char* file)
//{
//	FILE* fptr;
//	long length;
//	char* buf;
//	fptr = fopen(file, "rb"); // Open file for reading 
//	if (!fptr) // Return NULL on failure 
//		return NULL;
//	fseek(fptr, 0, SEEK_END); // Seek to the end of the file 
//	length = ftell(fptr); // Find out how many bytes into the file we are 
//	buf = (char*)malloc(length + 1); // Allocate a buffer for the entire length of the file and a null terminator 
//	fseek(fptr, 0, SEEK_SET); // Go back to the beginning of the file 
//	fread(buf, length, 1, fptr);// Read the contents of the file in to the buffer 
//	fclose(fptr); // Close the file 
//	buf[length] = 0;
//	// Null terminator 
//	return buf; // Return the buffer 
//}
//
////==========================================================================================
//
//void convertDeviceXY2OpenglXY(int x, int y, float* xpos, float* ypos)
//{
//	int w = width;
//	int h = height;
//	*xpos = (float)((x - (float)w / 2.0) * (float)(1.0 / (float)(w / 2.0)));
//	*ypos = -(float)((y - (float)h / 2.0) * (float)(1.0 / (float)(h / 2.0)));
//}
//void InitTexture(const char* filename, unsigned int texture)
//{
//
//
//	glGenTextures(1, &texture);
//	stbi_set_flip_vertically_on_load(true);
//	//glBindTexture(GL_TEXTURE_2D, texture);
//	// �ؽ�ó wrapping/filtering �ɼ� ����(���� ���ε��� �ؽ�ó ��ü�� ����)
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	// �ؽ�ó �ε� �� ����
//	int width, height, nrChannels;
//	unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);
//	if (data)
//	{
//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
//		glGenerateMipmap(GL_TEXTURE_2D);
//	}
//	else
//	{
//		std::cout << "Failed to load texture" << std::endl;
//	}
//	stbi_image_free(data);
//}
//void hpbar() {
//	int hpbardir = 1;
//	if (u_hpscalex >0.01) {
//		hpbardir = 1;
//	}
//	else {
//		hpbardir = 0;
//
//	}
//		u_hpscalex -= 0.002*hpbardir;
//		u_hptransx -= 0.001 *hpbardir;
//		cout << u_hpscalex << endl;
//}