#include <iostream>
#include <math.h>
#include <GL/freeglut.h>
#include <GL/glext.h>
#include <IL/devil_cpp_wrapper.hpp>

#include "ObjLoader.h"

#ifdef UNICODE
	#define _T(x) Lx
#else
	#define _T(x) x
#endif

#define SIZE_X 1024
#define SIZE_Y 768

Object bulb;
Object table;
Object lamp;
Object notebook;
Object display;
Object vase;
Object library;
Object bin;
Object box;
Object metronome;
Object hand;
Object draught;

float angleX = 0.0;
float angleY = 0.0;
float directionX = 0.0;
float directionZ = -1.0;
float directionY = 0.0;
float xPos = 0.0;
float zPos = 0.0;
float yPos = 0.0;
float rotX;
float rotY;

unsigned int texBoxId;
unsigned int texTableId;
unsigned int texLibraryId;
unsigned int texVaseId;
unsigned int texBinId;
unsigned int texMetroId;
unsigned int texHandId;
unsigned int texFloorId;
unsigned int texWallId;
unsigned int texUpWallId;

int floorId;
int wallId;
int upFloorId;
int tableId;
int libraryId;
int lampId;
int binId;
int notebookId;
int vaseId;
int boxId;
int metronomeId;

int lightRoom;
int lightSun;
int lightLamp;
int metronomeState;

void resize(int width, int height)
{
	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();
	gluPerspective(60, (float)width/height, 0.1f, 1000.0f);

	glViewport(0, 0, width, height);

	glMatrixMode(GL_MODELVIEW);
}

void keyPressed(unsigned char key, int mouseX, int mouseY)
{
	float fraction = 0.2;

    switch(key)
    {
    case 27:
        exit(0);
        break;
    case 'w':
		xPos += directionX * fraction;
		zPos += directionZ * fraction;
		yPos += directionY * fraction;
		break;
	case 's':
		xPos -= directionX * fraction;
		zPos -= directionZ * fraction;
		yPos -= directionY * fraction;
		break;
	case 't':
		angleX -= 0.05f;
		directionX = sin(angleY);
		directionY = sin(angleX);
		directionZ = -cos(angleY);
		break;
	case 'g':
		angleX += 0.05f;
		directionX = sin(angleY);
		directionY = sin(angleX);
		directionZ = -cos(angleY);
		break;
	case 'a':
		angleY -= 0.05f;
		directionX = sin(angleY);
		directionZ = -cos(angleY);
		break;
	case 'd':
		angleY += 0.05f;
		directionX = sin(angleY);
		directionZ = -cos(angleY);
		break;
	case 0x20:
		yPos += fraction;
		break;
	case 'c':
		yPos -= fraction;
		break;
	case 'l':
		lightRoom ^= 1;
		break;
	case 'k':
		lightSun ^= 1;
		break;
	case 'j':
		lightLamp ^= 1;
		break;
	case 'm':
		metronomeState ^= 1;
		break;
	case 'f':
		glutFullScreenToggle();
		break;
	}
}

#ifdef UNICODE
int loadTexture(const wchar_t * filename, unsigned int texId)
#else
int loadTexture(const char * filename, unsigned int texId)
#endif
{
	ilImage image;
	bool loaded;

	loaded = image.Load(filename);
	if (!loaded) {
#ifdef UNICODE
		wcerr << "Texture " << filename << " not loaded." << endl;
#else
		cerr << "Texture " << filename << " not loaded." << endl;
#endif
	}

	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA8, image.Width(), image.Height(), GL_RGB, GL_UNSIGNED_BYTE, image.GetData());

	image.Close();

	return texId;
}

void drawObject(Object &obj, float r, float g, float b, float alpha)
{
	float color[] = {r, g, b, alpha};
	float white[] = {1.0, 1.0, 1.0, 1.0};

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
	glMaterialf(GL_FRONT, GL_SHININESS, 60);

	glBegin(GL_TRIANGLES);
	for (size_t i = 0; i < obj.vertices.size(); i++) {
		glNormal3f(obj.normals[i].x, obj.normals[i].y, obj.normals[i].z);
		glTexCoord2f(obj.texcoords[i].x, obj.texcoords[i].y);
		glVertex3f(obj.vertices[i].x, obj.vertices[i].y, obj.vertices[i].z);
	}
	glEnd();
}

void createUpFloor()
{
	upFloorId = glGenLists(1);
	glNewList(upFloorId, GL_COMPILE);

	glPushMatrix();
	glTranslatef(0.0f, 5.0f, 0.f);
	glScalef(0.25f, 0.1f, 0.25f);
	drawObject(draught, 0.8f, 0.8f, 0.8, 1.0f);
	glPopMatrix();

	glEndList();
}

void createFloor()
{
	floorId = glGenLists(1);
	glNewList(floorId, GL_COMPILE);

	glPushMatrix();
	glScalef(0.25f, 0.1f, 0.25f);
	drawObject(draught, 0.8f, 0.6f, 0.4, 1.0f);
	glPopMatrix();
	glEndList();
}

void createWalls()
{
	wallId = glGenLists(1);
	glNewList(wallId, GL_COMPILE);

	glPushMatrix();
	glTranslatef(5.0f, 2.38f, 0.f);
	glRotatef(90, 0.0f, 0.0f, 1.0f);
	glScalef(0.131f, 0.1f, 0.25f);
	drawObject(draught, 0.8f, 0.6f, 0.4, 1.0f);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-5.0f, 2.38f, 0.f);
	glRotatef(90, 0.0f, 0.0f, 1.0f);
	glScalef(0.131f, 0.1f, 0.25f);
	drawObject(draught, 0.8f, 0.6f, 0.4, 1.0f);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0f, 2.38f, -5.0f);
	glRotatef(90, 1.0f, 0.0f, 0.0f);
	glScalef(0.25f, 0.1f, 0.131f);
	drawObject(draught, 0.8f, 0.6f, 0.4, 1.0f);
	glPopMatrix();

	glEndList();
}

void createBulb(float height)
{
	float yellow[] = {1.0, 1.0, 0.0, 1.0};
	float black[] = {0.0, 0.0, 0.0, 1.0};

	glPushMatrix();
	glTranslatef(0, height + 0.3f, 0);
	glColor3f(0.0f, 0.0f, 0.0f);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);
	glutSolidCube(0.3f);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, height, 0);
	glScalef(0.02f, 0.02f, 0.02f);
	if (lightRoom)
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, yellow);
	drawObject(bulb, 1.0f, 1.0f, 0.0f, 1.0f);
	glPopMatrix();

	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, black);

	glPushMatrix();
	glTranslatef(-4.15f, 2.1f, -3.9f);
	glRotatef(-30, 0.0f, 1.0f, 0.0f);
	glRotatef(20, 0.0f, 0.0f, 1.0f);
	glScalef(0.01f, 0.01f, 0.01f);
	if (lightLamp)
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, yellow);
	drawObject(bulb, 1.0f, 1.0f, 0.0f, 1.0f);
	glPopMatrix();

	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, black);

	glEndList();
}

void createTables(float plane)
{
	tableId = glGenLists(1);
	glNewList(tableId, GL_COMPILE);

	glPushMatrix();
	glTranslatef(-plane * 0.5f + 1.5, 0.0f, -plane * 0.5f + 1.1f);
	glScalef(0.02f, 0.02f, 0.02f);
	drawObject(table, 0.4f, 0.2f, 0, 1.0f);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(plane * 0.5f - 1.0f, 0.0f, -plane * 0.5f + 1.4f);
	glRotatef(90, 0.0f, 1.0f, 0.0f);
	glScalef(0.02f, 0.02f, 0.02f);
	drawObject(table, 0.4f, 0.2f, 0, 1.0f);
	glPopMatrix();

	glEndList();
}

void createLibrary(float plane)
{
	libraryId = glGenLists(1);
	glNewList(libraryId, GL_COMPILE);

	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -plane * 0.5f + 0.5f);
	glScalef(0.02f, 0.02f, 0.02f);
	drawObject(library, 0.4f, 0.2f, 0, 1.0f);
	glPopMatrix();

	glEndList();
}

void createLamps(float plane)
{
	lampId = glGenLists(1);
	glNewList(lampId, GL_COMPILE);

	glPushMatrix();
	glTranslatef(-plane * 0.5f + 0.7f, 1.4f, -plane * 0.5f + 1.0f);
	glRotatef(150, 0, 1, 0);
	glScalef(0.04f, 0.04f, 0.04f);
	drawObject(lamp, 0.0f, 0.0f, 1.0f, 1.0f);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(plane * 0.5f - 0.7f, 1.4f, -plane * 0.5f + 1.6f);
	glRotatef(-30, 0, 1, 0);
	glScalef(0.04f, 0.04f, 0.04f);
	drawObject(lamp, 0.0f, 1.0f, 0.0f, 1.0f);
	glPopMatrix();

	glEndList();
}

void createBin(float plane)
{
	binId = glGenLists(1);
	glNewList(binId, GL_COMPILE);

	glPushMatrix();
	glTranslatef(-plane * 0.5f + 3.2f, 0.0f, -plane * 0.5f + 0.5f);
	glScalef(0.02f, 0.02f, 0.02f);
	drawObject(bin, 0.5f, 0.5f, 0.5f, 1.0f);
	glPopMatrix();

	glEndList();
}

void createNotebook(float plane)
{
	notebookId = glGenLists(1);
	glNewList(notebookId, GL_COMPILE);

	glPushMatrix();
	glTranslatef(-plane * 0.5f + 1.7, 1.45f, -plane * 0.5f + 1.0f);
	glScalef(0.03f, 0.03f, 0.03f);
	drawObject(notebook, 0.3f, 0.3f, 0.3f, 1.0f);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-plane * 0.5f + 1.7, 1.45f, -plane * 0.5f + 1.0f);
	glScalef(0.03f, 0.03f, 0.03f);
	drawObject(display, 0.0f, 0.0f, 0.0f, 1.0f);
	glPopMatrix();

	glEndList();
}

void createVase(float plane)
{
	float vaseColor[] = {0.5f, 0.25f, 0.01f, 1};

	vaseId = glGenLists(1);
	glNewList(vaseId, GL_COMPILE);

	glPushMatrix();
	glTranslatef(-plane * 0.5f + 0.8f, 0.02f, -plane * 0.3f + 2.0f);
	glScalef(0.05f, 0.05f, 0.05f);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, vaseColor);
	drawObject(vase, 0.5f, 0.25f, 0.01f, 1.0f);
	glPopMatrix();

	glEndList();
}

void createBoxes(float plane)
{
	boxId = glGenLists(1);
	glNewList(boxId, GL_COMPILE);

	glPushMatrix();
	glTranslatef(plane * 0.5f - 1.0f, 0.0f, -plane * 0.3f + 2.0f);
	glScalef(0.02f, 0.02f, 0.02f);
	drawObject(box, 0.6f, 0.4f, 0.1f, 1.0f);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(plane * 0.5f - 1.0f, 0.0f, plane * 0.3f - 2.0f);
	glScalef(0.02f, 0.02f, 0.02f);
	drawObject(box, 0.6f, 0.4f, 0.1f, 1.0f);
	glPopMatrix();

	glEndList();
}

void createMetronome(float plane)
{
	metronomeId = glGenLists(1);
	glNewList(metronomeId, GL_COMPILE);

	glPushMatrix();
	glTranslatef(-plane * 0.5f + 0.8f, 0.0f, plane * 0.3f - 2.0f);
	glRotatef(90, 0.0f, 1.0f, 0.0f);
	glScalef(0.1f, 0.1f, 0.1f);
	drawObject(metronome, 0.4f, 0.3f, 0.2f, 1.0f);
	glPopMatrix();

	glEndList();
}

void drawSun(float plane)
{
	float lightSunPos[] = {0.0f, 0.0f, -8.0f, 1.0f};
	float color[] = {1.0, 1.0, 0.0, 1.0};
	float black[] = {0.0, 0.0, 0.0, 1.0};
	static float rotation = 0;

	rotation += 0.2f;

	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
	glRotatef(rotation, 1.0, 0.0, 0.0);
	glTranslatef(0.0, 0.0, -8.0);
	glLightfv(GL_LIGHT1, GL_POSITION, lightSunPos);
	if (lightSun)
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, color);
	glutSolidSphere(0.6f, 16, 16);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, black);
	glPopMatrix();
}

void drawHand(float plane)
{
	static float time = 0.0f;
	static float rotation = 20;

	if (metronomeState)
		time++;
	rotation = 20 * sin(time/15.0f);

	glPushMatrix();
	glTranslatef(-plane * 0.5f + 0.9f, 0.0f, plane * 0.3f - 2.0f);
	glRotatef(rotation, 1.0f, 0.0f, 0.0f);
	glRotatef(90, 0.0f, 1.0f, 0.0f);
	glScalef(0.1f, 0.1f, 0.1f);
	drawObject(hand, 0.8f, 0.8f, 1.0f, 1.0f);
	glPopMatrix();
}

void render(void)
{
	float lightRoomPos[] = { 0.0f, 4.5f, 0.0f, 1.0f };
	float lightLampPos[] = { -4.15f, 2.1f, -3.9f, 1.0f };
	float lightLampDir[] = { 0.5f, -1.0f, 0.3f, 0.0f };

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (lightRoom == 1)
		glEnable(GL_LIGHT0);
	else
		glDisable(GL_LIGHT0);

	if (lightSun == 1)
		glEnable(GL_LIGHT1);
	else
		glDisable(GL_LIGHT1);

	if (lightLamp == 1)
		glEnable(GL_LIGHT2);
	else
		glDisable(GL_LIGHT2);


	glLoadIdentity();
	gluLookAt(xPos, yPos, zPos, xPos+directionX, yPos+directionY, zPos+directionZ, 0, 1, 0);

	glLightfv(GL_LIGHT0, GL_POSITION, lightRoomPos);

	glLightfv(GL_LIGHT2, GL_POSITION, lightLampPos);
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, lightLampDir);
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 30.0f);
	glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 8.0f);

	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, texTableId);
	glCallList(tableId);
	glBindTexture(GL_TEXTURE_2D, texLibraryId);
	glCallList(libraryId);
	glBindTexture(GL_TEXTURE_2D, texBoxId);
	glCallList(boxId);
	glBindTexture(GL_TEXTURE_2D, texVaseId);
	glCallList(vaseId);
	glBindTexture(GL_TEXTURE_2D, texBinId);
	glCallList(binId);
	glBindTexture(GL_TEXTURE_2D, texMetroId);
	glCallList(metronomeId);
	glBindTexture(GL_TEXTURE_2D, texFloorId);
	glCallList(floorId);
	glBindTexture(GL_TEXTURE_2D, texWallId);
	glCallList(wallId);
	glBindTexture(GL_TEXTURE_2D, texUpWallId);
	glCallList(upFloorId);
	glBindTexture(GL_TEXTURE_2D, texHandId);
	drawHand(10.0f);
	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_TEXTURE_2D);

	glCallList(lampId);
	glCallList(notebookId);

	drawSun(10.0);
	createBulb(4.5f);

    glutSwapBuffers();
}

int main(int argc, char** argv)
{
	ObjLoader objLoader;
	float ambientRoomLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	float diffuseRoomLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	float specularRoomLight[] = {0.5, 0.5, 0.5, 1.0};

	float ambientSunLight[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	float diffuseSunLight[] = { 0.4f, 0.4f, 0.4f, 1.0f };
	float specularSunLight[] = {0.5, 0.5, 0.5, 1.0};

	float ambientLampLight[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	float diffuseLampLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	float specularLampLight[] = {0.2, 0.2, 0.2, 1.0};

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE);
    
    glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) - SIZE_X) / 2,
		(glutGet(GLUT_SCREEN_HEIGHT) - SIZE_Y) / 2);
    glutInitWindowSize(SIZE_X, SIZE_Y);    
    glutCreateWindow("Project 2");

    glutDisplayFunc(render);
	glutIdleFunc(render);
    glutKeyboardFunc(keyPressed);
	glutReshapeFunc(resize);
	glutSetCursor(GLUT_CURSOR_NONE);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);

	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseRoomLight);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientRoomLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularRoomLight);

	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseSunLight);
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambientSunLight);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specularSunLight);

	glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuseLampLight);
	glLightfv(GL_LIGHT2, GL_AMBIENT, ambientLampLight);
	glLightfv(GL_LIGHT2, GL_SPECULAR, specularLampLight);

	objLoader.Load("objects/draughts.obj");
	objLoader.saveObject(draught);
	objLoader.Load("objects/table.obj");
	objLoader.saveObject(table);
	objLoader.Load("objects/bulb.obj");
	objLoader.saveObject(bulb);
	objLoader.Load("objects/lamp.obj");
	objLoader.saveObject(lamp);
	objLoader.Load("objects/notebook.obj");
	objLoader.saveObject(notebook);
	objLoader.Load("objects/notebook-display.obj");
	objLoader.saveObject(display);
	objLoader.Load("objects/vase.obj");
	objLoader.saveObject(vase);
	objLoader.Load("objects/library.obj");
	objLoader.saveObject(library);
	objLoader.Load("objects/bin.obj");
	objLoader.saveObject(bin);
	objLoader.Load("objects/box.obj");
	objLoader.saveObject(box);
	objLoader.Load("objects/metronome.obj");
	objLoader.saveObject(metronome);
	objLoader.Load("objects/metronome-hand.obj");
	objLoader.saveObject(hand);

	texBoxId = loadTexture("textures/box.jpg", 1);
	texTableId = loadTexture("textures/table.jpg", 2);
	texLibraryId = loadTexture("textures/library.jpg", 3);
	texVaseId = loadTexture("textures/vase.jpg", 4);
	texBinId = loadTexture("textures/bin.jpg", 5);
	texMetroId = loadTexture("textures/metronome.jpg", 6);
	texHandId = loadTexture("textures/hand.jpg", 7);
	texFloorId = loadTexture("textures/floor.jpg", 8);
	texWallId = loadTexture("textures/wall.jpg", 9);
	texUpWallId = loadTexture("textures/upwall.jpg", 10);

	createFloor();
	createUpFloor();
	createWalls();
	createBulb(4.5f);
	createTables(10.0f);
	createLibrary(10.0f);
	createLamps(10.0f);
	createBin(10.0f);
	createNotebook(10.0f);
	createVase(10.0f);
	createBoxes(10.0f);
	createMetronome(10.0f);

	lightRoom = 0;
	lightSun = 0;
	lightLamp = 0;
	metronomeState = 0;

    glutMainLoop();

    return 0;
}
