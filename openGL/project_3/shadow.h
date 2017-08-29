#ifndef SHADOW_H
#define SHADOW_H

#include <iostream>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>

#define TEXTURE				GL_TEXTURE7
#define TEXTURE_UNIFORM		7

#define RENDER_WIDTH		1280.0
#define RENDER_HEIGHT		720.0
#define SHADOW_MAP_RATIO	2

#define start_transform(x) do { \
			glPushMatrix(); \
			do {x} while (0); \
			glMatrixMode(GL_TEXTURE); \
			glActiveTexture(TEXTURE); \
			glPushMatrix(); \
			do {x} while (0); \
		} while (0)

#define end_transform() do { \
			glPopMatrix(); \
			glMatrixMode(GL_MODELVIEW); \
			glPopMatrix(); \
		} while (0)

void setTextureMatrix();
void generateShadowFBO(unsigned int &fbo, unsigned int &depthTexture);
void setupMatrices(float posX, float posY, float posZ, float lookAtX, float lookAtY, float lookAtZ);
void startPreRender(unsigned int &fbo, float pX, float pY, float pZ, float lX, float lY, float lZ);
void endPreRender();
void startFullRender(unsigned int &prg, unsigned int &depthTexture, unsigned int &uniShadow);
void endFullRender();

#endif
