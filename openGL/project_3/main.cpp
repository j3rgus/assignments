#include <math.h>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/glext.h>

#include <IL/devil_cpp_wrapper.hpp>

#include "ObjLoader.h"
#include "shader.h"
#include "shadow.h"

#define SIZE_X RENDER_WIDTH
#define SIZE_Y RENDER_HEIGHT

#ifdef UNICODE
	#define _T(x) Lx
#else
	#define _T(x) x
#endif

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

float pCam[3] = {0.0, 15.0, 15.0};
float lCam[3] = {0.0, 0.0, 0.0};
float pLight[3] = {0.0, 20.0, 0.0};
float dLight[3] = {0.0, -1.0, 0.0};

float radius = 30.0;
const float speedFactorHand = 250.0f;	// mensie cislo znamena rychlejsie
const float speedFactorCamera = 2500.0f;

unsigned int fboId;
unsigned int depthTextureId;
unsigned int program;
unsigned int shadowMapUniform;
unsigned int textureMapUniform;

unsigned int texBoxId;
unsigned int texTableId;
unsigned int texLibraryId;
unsigned int texVaseId;
unsigned int texBinId;
unsigned int texMetroId;
unsigned int texHandId;
unsigned int texFloorId;
unsigned int texWallId;
unsigned int texLampId;
unsigned int texDisplayId;
unsigned int texNotebookId;

int baseId;
int tableId;
int libraryId;
int lampId;
int binId;
int notebookId;
int vaseId;
int boxId;
int metronomeId;

ObjLoader objLoader;
int staticCamera;

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
    switch(key)
    {
    case 27:
        exit(0);
        break;

	case 'c':
		staticCamera ^= 1;
		break;

	case 'f':
		glutFullScreenToggle();
		break;

	case 'a':
		radius += 1.0f;
		break;

	case 'z':
		radius -= 1.0f;
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

void drawObject(Object &obj)
{
	glBegin(GL_TRIANGLES);
	for (size_t i = 0; i < obj.vertices.size(); i++) {
		glTexCoord2f(obj.texcoords[i].x, obj.texcoords[i].y);
		glVertex3f(obj.vertices[i].x, obj.vertices[i].y, obj.vertices[i].z);
	}
	glEnd();
}

void createBase()
{
	objLoader.Load("objects/draughts.obj");
	objLoader.saveObject(draught);

	texFloorId = loadTexture(_T("textures/floor.jpg"), 8);
	texWallId = loadTexture(_T("textures/wall.jpg"), 9);

	baseId = glGenLists(1);
	glNewList(baseId, GL_COMPILE);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texFloorId);

	start_transform(
		glScalef(0.25f, 0.1f, 0.25f);
	);
	drawObject(draught);
	end_transform();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texWallId);

	start_transform(
		glTranslatef(5.0f, 2.38f, 0.f);
		glRotatef(90, 0.0f, 0.0f, 1.0f);
		glScalef(0.131f, 0.1f, 0.25f);
	);
	drawObject(draught);
	end_transform();

	start_transform(
		glTranslatef(-5.0f, 2.38f, 0.f);
		glRotatef(90, 0.0f, 0.0f, 1.0f);
		glScalef(0.131f, 0.1f, 0.25f);
	);
	drawObject(draught);
	end_transform();

	start_transform(
		glTranslatef(0.0f, 2.38f, -5.0f);
		glRotatef(90, 1.0f, 0.0f, 0.0f);
		glScalef(0.25f, 0.1f, 0.131f);
	);
	drawObject(draught);
	end_transform();

	glEndList();
}

void createTables(float plane)
{
	objLoader.Load("objects/table.obj");
	objLoader.saveObject(table);

	texTableId = loadTexture(_T("textures/table.jpg"), 2);

	tableId = glGenLists(1);
	glNewList(tableId, GL_COMPILE);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texTableId);

	start_transform(
		glTranslatef(-plane * 0.5f + 1.5, 0.0f, -plane * 0.5f + 1.1f);
		glScalef(0.02f, 0.02f, 0.02f);
	);
	drawObject(table);
	end_transform();

	start_transform(
		glTranslatef(plane * 0.5f - 1.0f, 0.0f, -plane * 0.5f + 1.4f);
		glRotatef(90, 0.0f, 1.0f, 0.0f);
		glScalef(0.02f, 0.02f, 0.02f);
	);
	drawObject(table);
	end_transform();

	glEndList();
}

void createLibrary(float plane)
{
	objLoader.Load("objects/library.obj");
	objLoader.saveObject(library);

	texLibraryId = loadTexture(_T("textures/library.jpg"), 3);

	libraryId = glGenLists(1);
	glNewList(libraryId, GL_COMPILE);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texLibraryId);

	start_transform(
		glTranslatef(0.0f, 0.0f, -plane * 0.5f + 0.5f);
		glScalef(0.02f, 0.02f, 0.02f);
	);
	drawObject(library);
	end_transform();

	glEndList();
}

void createLamps(float plane)
{
	objLoader.Load("objects/lamp.obj");
	objLoader.saveObject(lamp);

	texLampId = loadTexture(_T("textures/lamp.jpg"), 10);

	lampId = glGenLists(1);
	glNewList(lampId, GL_COMPILE);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texLampId);

	start_transform(
		glTranslatef(-plane * 0.5f + 0.7f, 1.4f, -plane * 0.5f + 1.0f);
		glRotatef(150, 0, 1, 0);
		glScalef(0.04f, 0.04f, 0.04f);
	);
	drawObject(lamp);
	end_transform();

	start_transform(
		glTranslatef(plane * 0.5f - 0.7f, 1.4f, -plane * 0.5f + 1.6f);
		glRotatef(-30, 0, 1, 0);
		glScalef(0.04f, 0.04f, 0.04f);
	);
	drawObject(lamp);
	end_transform();

	glEndList();
}

void createBin(float plane)
{
	objLoader.Load("objects/bin.obj");
	objLoader.saveObject(bin);

	texBinId = loadTexture(_T("textures/bin.jpg"), 5);

	binId = glGenLists(1);
	glNewList(binId, GL_COMPILE);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texBinId);

	start_transform(
		glTranslatef(-plane * 0.5f + 3.2f, 0.0f, -plane * 0.5f + 0.5f);
		glScalef(0.02f, 0.02f, 0.02f);
	);
	drawObject(bin);
	end_transform();

	glEndList();
}

void createNotebook(float plane)
{
	objLoader.Load("objects/notebook.obj");
	objLoader.saveObject(notebook);
	objLoader.Load("objects/notebook-display.obj");
	objLoader.saveObject(display);

	texDisplayId = loadTexture(_T("textures/display.jpg"), 11);
	texNotebookId = loadTexture(_T("textures/notebook.jpg"), 12);

	notebookId = glGenLists(1);
	glNewList(notebookId, GL_COMPILE);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texNotebookId);

	start_transform(
		glTranslatef(-plane * 0.5f + 1.7, 1.45f, -plane * 0.5f + 1.0f);
		glScalef(0.03f, 0.03f, 0.03f);
	);
	drawObject(notebook);
	end_transform();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texDisplayId);

	start_transform(
		glTranslatef(-plane * 0.5f + 1.7, 1.45f, -plane * 0.5f + 1.0f);
		glScalef(0.03f, 0.03f, 0.03f);
	);
	drawObject(display);
	end_transform();

	glEndList();
}

void createVase(float plane)
{
	objLoader.Load("objects/vase.obj");
	objLoader.saveObject(vase);

	texVaseId = loadTexture(_T("textures/vase.jpg"), 4);

	vaseId = glGenLists(1);
	glNewList(vaseId, GL_COMPILE);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texVaseId);

	start_transform(
		glTranslatef(-plane * 0.5f + 0.8f, 0.02f, -plane * 0.3f + 2.0f);
		glScalef(0.05f, 0.05f, 0.05f);
	);
	drawObject(vase);
	end_transform();

	glEndList();
}

void createBoxes(float plane)
{
	objLoader.Load("objects/box.obj");
	objLoader.saveObject(box);

	texBoxId = loadTexture(_T("textures/box.jpg"), 1);

	boxId = glGenLists(1);
	glNewList(boxId, GL_COMPILE);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texBoxId);

	start_transform(
		glTranslatef(plane * 0.5f - 1.0f, 0.0f, -plane * 0.3f + 2.0f);
		glScalef(0.02f, 0.02f, 0.02f);
	);
	drawObject(box);
	end_transform();

	start_transform(
		glTranslatef(plane * 0.5f - 1.0f, 0.0f, plane * 0.3f - 2.0f);
		glScalef(0.02f, 0.02f, 0.02f);
	);
	drawObject(box);
	end_transform();

	glEndList();
}

void createMetronome(float plane)
{
	objLoader.Load("objects/metronome.obj");
	objLoader.saveObject(metronome);

	texMetroId = loadTexture(_T("textures/metronome.jpg"), 6);

	metronomeId = glGenLists(1);
	glNewList(metronomeId, GL_COMPILE);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texMetroId);

	start_transform(
		glTranslatef(-plane * 0.5f + 0.8f, 0.0f, plane * 0.3f - 2.0f);
		glRotatef(90, 0.0f, 1.0f, 0.0f);
		glScalef(0.1f, 0.1f, 0.1f);
	);
	drawObject(metronome);
	end_transform();

	glEndList();
}

void drawHand(float plane)
{
	static float rotation = 20;

	rotation = 20 * sin(glutGet(GLUT_ELAPSED_TIME)/speedFactorHand);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texHandId);

	start_transform(
		glTranslatef(-plane * 0.5f + 0.9f, 0.0f, plane * 0.3f - 2.0f);
		glRotatef(rotation, 1.0f, 0.0f, 0.0f);
		glRotatef(90, 0.0f, 1.0f, 0.0f);
		glScalef(0.1f, 0.1f, 0.1f);
	);
	drawObject(hand);
	end_transform();
}

void drawObjects()
{
	glCallList(baseId);
	glCallList(tableId);
	glCallList(libraryId);
	glCallList(lampId);
	glCallList(binId);
	glCallList(notebookId);
	glCallList(vaseId);
	glCallList(boxId);
	glCallList(metronomeId);
	drawHand(10.0f);
}

void lightUpdate()
{
	pLight[0] = radius * cos(glutGet(GLUT_ELAPSED_TIME)/speedFactorCamera);
	pLight[2] = radius * sin(glutGet(GLUT_ELAPSED_TIME)/speedFactorCamera);
}

void render(void)
{
	lightUpdate();

	startPreRender(fboId,pLight[0],pLight[1],pLight[2],dLight[0],dLight[1],dLight[2]);
	glCullFace(GL_FRONT);
	glUniform1i(textureMapUniform, 0);
	drawObjects();
	setTextureMatrix();
	endPreRender();

	startFullRender(program, depthTextureId, shadowMapUniform);
	if (staticCamera)
		setupMatrices(pCam[0], pCam[1], pCam[2], lCam[0], lCam[1], lCam[2]);
	else
		setupMatrices(pLight[0],pLight[1],pLight[2],dLight[0],dLight[1],dLight[2]);
	glCullFace(GL_BACK);
	glUniform1i(textureMapUniform, 0);
	drawObjects();
	endFullRender();

    glutSwapBuffers();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE);
    
    glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) - SIZE_X) / 2,
		(glutGet(GLUT_SCREEN_HEIGHT) - SIZE_Y) / 2);
    glutInitWindowSize(SIZE_X, SIZE_Y);
    glutCreateWindow("Project 3");

	shaderInit();

    glutDisplayFunc(render);
	glutIdleFunc(render);
    glutKeyboardFunc(keyPressed);
	glutReshapeFunc(resize);
	glutSetCursor(GLUT_CURSOR_NONE);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	objLoader.Load("objects/metronome-hand.obj");
	objLoader.saveObject(hand);
	texHandId = loadTexture(_T("textures/hand.jpg"), 7);

	glClearColor(0,0,0,1.0f);

	generateShadowFBO(fboId, depthTextureId);

	program = loadProgram("shader/vs.glsl", "shader/fs.glsl");
	shadowMapUniform = glGetUniformLocation(program, "ShadowMap");
	textureMapUniform = glGetUniformLocation(program, "TextureMap");

	createBase();
	createTables(10.0f);
	createLibrary(10.0f);
	createLamps(10.0f);
	createBin(10.0f);
	createNotebook(10.0f);
	createVase(10.0f);
	createBoxes(10.0f);
	createMetronome(10.0f);

    glutMainLoop();

    return 0;
}
