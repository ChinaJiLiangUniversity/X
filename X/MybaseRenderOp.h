#ifndef MY_BASE_RENDER_OP_H
#define MY_BASE_RENDER_OP_H
#include "USShader.h"

class MybaseRenderOp
{
public:
	MybaseRenderOp(){}
	~MybaseRenderOp(){}
	virtual void initOpengl();
	virtual void DoDraw();
public:
	USShader shader;
};


#endif