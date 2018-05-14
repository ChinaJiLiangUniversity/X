#ifndef MYNORMALMAPPING_H
#define  MYNORMALMAPPING_H
#include "MybaseRenderOp.h"

#include "gldata.h"
#include "MyCommanFunc.h"
#include "MyCamera.h"
class  MyNormalMapping :public MybaseRenderOp
{

public:
	MyNormalMapping();
	~MyNormalMapping(){}

	void initOpengl();
	void DoDraw();
	void renderQuad();
public:
	MyGlData *op_data_;
	GLuint diffuseMap ;
	GLuint  normalMap;
	
	
};
#endif