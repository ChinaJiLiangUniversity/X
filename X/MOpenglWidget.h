#ifndef MOPENGL_WIDGET_H
#define MOPENGL_WIDGET_H

#include <gl/glew.h>
#include "USShader.h"
#include <QVector>
#include <QVector3D>
#include <QPainter>
#include <QColor>
#include <QOpenGLTexture>
#include <QMatrix4x4>
#include <QtWidgets/QOpenGLWidget>
#include "bubble.h"
#include "ShapeData.h"
#include "ShapeGenerator.h"
#include "MyModel.h"
#include "MyCamera.h"
#include "MybaseRenderOp.h"
#include "DeferredShading.h"
#include <QEvent>
#include <QKeyEvent>





class MOpenglWidget :public  QOpenGLWidget
{
public:
	MOpenglWidget(QWidget* parent);
	~MOpenglWidget();
	enum MyEnum
	{
		ArrayBuffers,
		NumBuffers
	};
	enum VAO_IDS{ Triangles,NumVAOS};
	enum Attrib_IDS{ vPosition = 0, vColor = 1};

	GLuint  VAOS[NumVAOS];
	GLuint  buffers[NumBuffers];

	virtual void initializeGL()override;
	virtual void resizeGL(int w, int h)override;
	virtual void paintGL()override;

	virtual void mousePressEvent(QMouseEvent *e);
	virtual void mouseReleaseEvent(QMouseEvent *e);
	virtual void mouseDoubleClickEvent(QMouseEvent *);
	virtual void mouseMoveEvent(QMouseEvent *);
	virtual void wheelEvent(QWheelEvent*e);
	//void paintEvent(QPaintEvent *e);
	void createGeometry();
	void quad(qreal x1, qreal y1, qreal x2, qreal y2, qreal x3, qreal y3, qreal x4, qreal y4);
	void extrude(qreal x1, qreal y1, qreal x2, qreal y2);
	void createBubbles(int number);
	void focusInEvent(QFocusEvent * event);
	void keyPressEvent(QKeyEvent * event);
	void keyReleaseEvent(QKeyEvent * event);
	void sendDataToOpenGL();
	void installShaders();
	std::string readShaderCode(const char* fileName);
	bool checkShaderStatus(GLuint shaderID);
	bool checkProgramStatus(GLuint programID);
	bool checkStatus(GLuint objectID, PFNGLGETSHADERIVPROC objectPropertyGetterFunc, PFNGLGETSHADERINFOLOGPROC getInfoLogFunc, GLenum statusType);
	GLuint genComputeProg(GLuint texHandle);
	
	GLuint genRenderProg(GLuint texHandle);
	GLuint genTexture();
	void checkErrors(std::string desc);
	void processShader(std::string file_name);
	void setMat4(const std::string &name, const glm::mat4 &mat);
	void renderScene(const USShader &shader);
private:
	unsigned int depthMap;
	static float wheel_scale_speed_;
	std::string scene_name_;
	glm::mat4 transform;
	QPoint old_pos_;
	static float rotate_speed_;
	//	USID scene_id_;
	QOpenGLTexture *m_texture;
	GLuint program_handle_;

	QList<Bubble *> m_bubbles;
	GLuint program_handle2_;
	GLuint vshader_handle2_;
	GLuint fshader_handle2_;
	unsigned int VBO, cubeVAO, EBO;
	unsigned int lightVAO;
	unsigned int texture_id;

	double m_fAngle;
	double m_fScale;

	GLuint vao1_;
	GLuint vao2_;

	GLuint vbos1_;
	GLuint vbos2_;
	QVector<QVector3D> m_vertices;
	QVector<QVector3D> m_normals;
	QColor m_background;
	bool m_qtLogo;
	bool m_transparent;
	MyModel theModel;

	// test2
	GLuint renderHandle, computeHandle;
	GLuint output_image;
	GLuint  render_vao;
	GLuint  render_vbo;

	bool move_scene_ = false;
	float factor_ = 0.005;
public:
	MyCamera *ptr_camera;
	unsigned int quadVAO = 0;
	unsigned int quadVBO;
	unsigned int planeVBO;
	// 2. 加载纹理
	unsigned int woodTexture;

	// 3.创建深度测试纹理FBO

	unsigned int depthMapFBO;
	// 4. create depth texture
	unsigned int planeVAO;
	unsigned int centerposVAO;
	USShader shader;
	USShader simpleDepthShader;
	USShader debugDepthQuad;
	USShader pointshader;

	unsigned int shader_id;
	unsigned int simpleDepthShader_id;
	unsigned int debugDepthQuad_id;
	float centerPos[3];
	//float *centerPos2;
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
public:
	MybaseRenderOp *renderoperation_ = nullptr;
};


#endif