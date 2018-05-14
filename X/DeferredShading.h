#ifndef MYDEFERREDSHADING_H
#define  MYDEFERREDSHADING_H
#include "MybaseRenderOp.h"

#include "gldata.h"
#include "MyCommanFunc.h"
#include "MyCamera.h"
class  MyDeferredShading:public MybaseRenderOp
{
	
public:
	MyDeferredShading();
	~MyDeferredShading(){}

	void initOpengl();
	void DoDraw();

public:
	MyGlData *op_data_;
	GLuint vertexbuffer;
	GLuint vertexColorBuffer;
	unsigned int woodTexture;
	MyCamera *ptr_camera_;

};
#endif