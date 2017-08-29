#include "shadow.h"

void setTextureMatrix()
{
	static double modelView[16];
	static double projection[16];
	const GLdouble bias[16] = {	
		0.5, 0.0, 0.0, 0.0, 
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0};

	glGetDoublev(GL_MODELVIEW_MATRIX, modelView);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);

	glMatrixMode(GL_TEXTURE);
	glActiveTexture(TEXTURE);

	glLoadIdentity();	
	glLoadMatrixd(bias);

	glMultMatrixd(projection);
	glMultMatrixd(modelView);

	glMatrixMode(GL_MODELVIEW);
}

void generateShadowFBO(unsigned int &fbo, unsigned int &depthTexture)
{
	int shadowMapWidth = RENDER_WIDTH * SHADOW_MAP_RATIO;
	int shadowMapHeight = RENDER_HEIGHT * SHADOW_MAP_RATIO;
	GLenum FBOstatus;

	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowMapWidth, shadowMapHeight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D, depthTexture, 0);

	FBOstatus = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER);

	if(FBOstatus != GL_FRAMEBUFFER_COMPLETE)
		std::cerr << "GL_FRAMEBUFFER_COMPLETE failed\n";

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void setupMatrices(float posX,float posY,float posZ,float lookAtX,float lookAtY,float lookAtZ)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45,RENDER_WIDTH/RENDER_HEIGHT,10,40000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(posX, posY, posZ, lookAtX, lookAtY, lookAtZ,0,1,0);
}

void startPreRender(unsigned int &fbo, float pX, float pY, float pZ, float lX, float lY, float lZ)
{
	glBindFramebuffer(GL_FRAMEBUFFER,fbo);
	glViewport(0,0,RENDER_WIDTH * SHADOW_MAP_RATIO,RENDER_HEIGHT* SHADOW_MAP_RATIO);
	glClear(GL_DEPTH_BUFFER_BIT);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	setupMatrices(pX,pY,pZ,lX,lY,lZ);
}

void endPreRender()
{
	glBindFramebuffer(GL_FRAMEBUFFER,0);
	glViewport(0,0,RENDER_WIDTH,RENDER_HEIGHT);

	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void startFullRender(unsigned int &prg, unsigned int &depthTexture, unsigned int &uniShadow)
{
	glUseProgram(prg);
	glUniform1i(uniShadow, TEXTURE_UNIFORM);
	glActiveTexture(TEXTURE);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
}

void endFullRender()
{
	glUseProgram(0);
}
