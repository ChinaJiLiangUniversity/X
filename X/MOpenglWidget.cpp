#include "MOpenglWidget.h"
#include "MyCamera.h"

#include <QOpenGLContext>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <QApplication>
#include <QCoreApplication>
#include <QDir>
#include <QKeyEvent>
using namespace std;
using glm::vec3;
using glm::vec4;
using glm::mat4;

const uint NUM_VERTICES_PER_TRI = 3;
const uint NUM_FLOATS_PER_VERTICE = 9;
const uint VERTEX_BYTE_SIZE = NUM_FLOATS_PER_VERTICE * sizeof(float);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
float MOpenglWidget::rotate_speed_ = 1.5f;
float MOpenglWidget::wheel_scale_speed_ = 0.0083f * 0.1f;
// camera
// float lastX = SCR_WIDTH / 2.0f;
// float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 1.0f;
float lastFrame = 0.0f;

// lighting

glm::vec3 lightPos(-2.0f, 4.0f, -1.0f);


// data  
float planeVertices[] = {
	// positions            // normals         // texcoords
	25.0f, -0.5f, 25.0f, 0.0f, 1.0f, 0.0f, 25.0f, 0.0f,
	-25.0f, -0.5f, 25.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
	-25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f, 0.0f, 25.0f,

	25.0f, -0.5f, 25.0f, 0.0f, 1.0f, 0.0f, 25.0f, 0.0f,
	-25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f, 0.0f, 25.0f,
	25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f, 25.0f, 25.0f
};

static const GLfloat g_vertex_buffer_data[] = {
	-1.0f, -1.0f, 0.0f,
	1.0f, -1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
};

// 立方体数据
static const GLfloat g_vertex_buffer_data2[] = {
	-1.0f, -1.0f, -1.0f, // triangle 1 : begin
	-1.0f, -1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f, // triangle 1 : end
	1.0f, 1.0f, -1.0f, // triangle 2 : begin
	-1.0f, -1.0f, -1.0f,
	-1.0f, 1.0f, -1.0f, // triangle 2 : end
	1.0f, -1.0f, 1.0f,
	-1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, 1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, -1.0f,
	1.0f, -1.0f, 1.0f,
	-1.0f, -1.0f, 1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f, -1.0f, 1.0f,
	1.0f, -1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, 1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, -1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, -1.0f,
	-1.0f, 1.0f, -1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, -1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f, -1.0f, 1.0f
};


// color buffer
static const GLfloat g_color_buffer_data[] = {
	0.583f, 0.771f, 0.014f,
	0.609f, 0.115f, 0.436f,
	0.327f, 0.483f, 0.844f,
	0.822f, 0.569f, 0.201f,
	0.435f, 0.602f, 0.223f,
	0.310f, 0.747f, 0.185f,
	0.597f, 0.770f, 0.761f,
	0.559f, 0.436f, 0.730f,
	0.359f, 0.583f, 0.152f,
	0.483f, 0.596f, 0.789f,
	0.559f, 0.861f, 0.639f,
	0.195f, 0.548f, 0.859f,
	0.014f, 0.184f, 0.576f,
	0.771f, 0.328f, 0.970f,
	0.406f, 0.615f, 0.116f,
	0.676f, 0.977f, 0.133f,
	0.971f, 0.572f, 0.833f,
	0.140f, 0.616f, 0.489f,
	0.997f, 0.513f, 0.064f,
	0.945f, 0.719f, 0.592f,
	0.543f, 0.021f, 0.978f,
	0.279f, 0.317f, 0.505f,
	0.167f, 0.620f, 0.077f,
	0.347f, 0.857f, 0.137f,
	0.055f, 0.953f, 0.042f,
	0.714f, 0.505f, 0.345f,
	0.783f, 0.290f, 0.734f,
	0.722f, 0.645f, 0.174f,
	0.302f, 0.455f, 0.848f,
	0.225f, 0.587f, 0.040f,
	0.517f, 0.713f, 0.338f,
	0.053f, 0.959f, 0.120f,
	0.393f, 0.621f, 0.362f,
	0.673f, 0.211f, 0.457f,
	0.820f, 0.883f, 0.371f,
	0.982f, 0.099f, 0.879f
};


static const GLfloat g_uv_buffer_data[] = {
	0.000059f, 1.0f - 0.000004f,
	0.000103f, 1.0f - 0.336048f,
	0.335973f, 1.0f - 0.335903f,
	1.000023f, 1.0f - 0.000013f,
	0.667979f, 1.0f - 0.335851f,
	0.999958f, 1.0f - 0.336064f,
	0.667979f, 1.0f - 0.335851f,
	0.336024f, 1.0f - 0.671877f,
	0.667969f, 1.0f - 0.671889f,
	1.000023f, 1.0f - 0.000013f,
	0.668104f, 1.0f - 0.000013f,
	0.667979f, 1.0f - 0.335851f,
	0.000059f, 1.0f - 0.000004f,
	0.335973f, 1.0f - 0.335903f,
	0.336098f, 1.0f - 0.000071f,
	0.667979f, 1.0f - 0.335851f,
	0.335973f, 1.0f - 0.335903f,
	0.336024f, 1.0f - 0.671877f,
	1.000004f, 1.0f - 0.671847f,
	0.999958f, 1.0f - 0.336064f,
	0.667979f, 1.0f - 0.335851f,
	0.668104f, 1.0f - 0.000013f,
	0.335973f, 1.0f - 0.335903f,
	0.667979f, 1.0f - 0.335851f,
	0.335973f, 1.0f - 0.335903f,
	0.668104f, 1.0f - 0.000013f,
	0.336098f, 1.0f - 0.000071f,
	0.000103f, 1.0f - 0.336048f,
	0.000004f, 1.0f - 0.671870f,
	0.336024f, 1.0f - 0.671877f,
	0.000103f, 1.0f - 0.336048f,
	0.336024f, 1.0f - 0.671877f,
	0.335973f, 1.0f - 0.335903f,
	0.667969f, 1.0f - 0.671889f,
	1.000004f, 1.0f - 0.671847f,
	0.667979f, 1.0f - 0.335851f
};


//先声明几个全局函数

void renderScene(const USShader &shader);
void renderCube();
void renderQuad();




void MOpenglWidget::renderScene(const USShader &shader)
{
// 	glm::mat4 model;
// 	shader.setMat4("model", model);
// 	glBindVertexArray(planeVAO);
// 	glDrawArrays(GL_TRIANGLES, 0, 6);
// 	// cubes
// 	model = glm::mat4();
// 	model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0));
// 	model = glm::scale(model, glm::vec3(0.5f));
// 	shader.setMat4("model", model);
// 	renderCube();
// 	model = glm::mat4();
// 	model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0));
// 	model = glm::scale(model, glm::vec3(0.5f));
// 	shader.setMat4("model", model);
// 	renderCube();
// 	model = glm::mat4();
// 	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 2.0));
// 	model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
// 	model = glm::scale(model, glm::vec3(0.25));
// 	shader.setMat4("model", model);
// 	renderCube();
}
unsigned int cubeVAO = 0;
unsigned int cubeVBO = 0;
void renderCube()
{
	if (cubeVAO == 0)
	{
		float vertices[] = {
			// back face
			-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, // top-right
			1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
			1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, // top-right
			-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			-1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, // top-left
			// front face
			-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
			1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // bottom-right
			1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // top-right
			1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // top-right
			-1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, // top-left
			-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
			// left face
			-1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
			-1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-left
			-1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
			// right face
			1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-left
			1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-right
			1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-right         
			1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-right
			1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-left
			1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom-left     
			// bottom face
			-1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
			1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, // top-left
			1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, // bottom-left
			1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, // bottom-left
			-1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
			// top face
			-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top-left
			1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
			1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // top-right     
			1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
			-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top-left
			-1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f  // bottom-left        
		};
		glGenVertexArrays(1, &cubeVAO);
		glGenBuffers(1, &cubeVBO);
		// fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// link vertex attributes
		glBindVertexArray(cubeVAO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	// render Cube
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}
// unsigned int quadVAO = 0;
// unsigned int quadVBO;
// void renderQuad()
// {
// 	if (quadVAO == 0)
// 	{
// 		float quadVertices[] = {
// 			// positions        // texture Coords
// 			-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
// 			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
// 			1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
// 			1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
// 		};
// 		// setup plane VAO
// 		glGenVertexArrays(1, &quadVAO);
// 		glGenBuffers(1, &quadVBO);
// 		glBindVertexArray(quadVAO);
// 		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
// 		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
// 		glEnableVertexAttribArray(0);
// 		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
// 		glEnableVertexAttribArray(1);
// 		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
// 	}
// 	glBindVertexArray(quadVAO);
// 	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
// 	glBindVertexArray(0);
// }





MOpenglWidget::MOpenglWidget(QWidget* parent)
	:QOpenGLWidget(parent)
{
	setFocusPolicy(Qt::StrongFocus);
	ptr_camera = new MyCamera();
	centerPos[0] = 0.0f;
	centerPos[1] = 0.0f;
	centerPos[2] = 0.0f;
	//renderoperation_ = new MyDeferredShading();
	//centerPos2 = new float{ 0.0f, 0.0f, 0.0f };
};  // 坐标的中心;



MOpenglWidget::~MOpenglWidget()
{
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void  MOpenglWidget::setMat4(const std::string &name, const glm::mat4 &mat)
{
	//glUniformMatrix4fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}


GLuint vertexbuffer;
GLuint vertexColorBuffer;
void MOpenglWidget::initializeGL()
{
    glewInit();
	if (renderoperation_ != nullptr)
	{
		renderoperation_->initOpengl();
	}

	

}

void MOpenglWidget::resizeGL(int w, int h)
{
	glViewport(0, 0, width(),height());
}

GLint orginbuffer;

// model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0));
// model = glm::scale(model, glm::vec3(0.5f));


void MOpenglWidget::paintGL()
{
	glm::mat4 projection = glm::perspective(60.0f, (float)width() / (float)height(), 0.1f, 100.0f);
	glm::mat4 view = ptr_camera->GetViewMatrix();
	glm::vec3 lightPos(0.5f, 1.0f, 0.3f);
	if ( renderoperation_ == nullptr)
	{
		return;
	}
	renderoperation_->shader.use();
	glm::mat4 model;
	model = glm::mat4();
	renderoperation_->shader.setMat4("model", model);
	renderoperation_->shader.setMat4("projection", projection);
	renderoperation_->shader.setMat4("view", view);
	renderoperation_->shader.setVec3("lightPos", lightPos);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	if ( renderoperation_ != nullptr)
	{
		renderoperation_->DoDraw();

	}


// 	glEnable(GL_BLEND);
// 	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
// 	
// 	
// 
// 	glActiveTexture(GL_TEXTURE0);
// 	glBindTexture(GL_TEXTURE_2D, woodTexture);
// 	GLuint TextureID = glGetUniformLocation(shader.ID, "myTextureSampler");
// 	glUniform1i(TextureID, 0);
// 
// 	glEnable(GL_DEPTH_TEST);
// 	glDepthFunc(GL_LESS);
// 	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
// 
// 	
// 
// 	// 配置
// 	glEnableVertexAttribArray(0);
// 	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
// 	glVertexAttribPointer(
// 		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
// 		3,                  // size
// 		GL_FLOAT,           // type
// 		GL_FALSE,           // normalized?
// 		0,                  // stride
// 		(void*)0            // array buffer offset
// 		);
// 
// 
// 
// 	glEnableVertexAttribArray(1);
// 	glBindBuffer(GL_ARRAY_BUFFER, vertexColorBuffer);
// 	glVertexAttribPointer(
// 		1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
// 		2,                                // size
// 		GL_FLOAT,                         // type
// 		GL_FALSE,                         // normalized?
// 		0,                                // stride
// 		(void*)0                          // array buffer offset
// 		);
// 
// 	
// 	
// 	
// 
// 	// Draw the triangle !
// 	glDrawArrays(GL_TRIANGLES, 0, 12 * 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
// 	glDisableVertexAttribArray(0);

}
 static float rotatex = 0.5;
 float lastX;
 float lastY;


void MOpenglWidget::mouseReleaseEvent(QMouseEvent *e)
{
	if (e->button() == Qt::LeftButton)
	{
		move_scene_ = false;
	}
}

void MOpenglWidget::mouseDoubleClickEvent(QMouseEvent *)
{

}



void MOpenglWidget::wheelEvent(QWheelEvent*e)
{
	QPoint pt = e->angleDelta();
	auto power = (e->modifiers() & Qt::ControlModifier) ? 0.1f : 1.0f;
	ptr_camera->ViewScale(pt.y() * MOpenglWidget::wheel_scale_speed_*power);
	update();

}

void MOpenglWidget::focusInEvent(QFocusEvent * event)
{

}





void MOpenglWidget::createGeometry()
{
	m_vertices.clear();
	m_normals.clear();

	qreal x1 = +0.06f;
	qreal y1 = -0.14f;
	qreal x2 = +0.14f;
	qreal y2 = -0.06f;
	qreal x3 = +0.08f;
	qreal y3 = +0.00f;
	qreal x4 = +0.30f;
	qreal y4 = +0.22f;

	quad(x1, y1, x2, y2, y2, x2, y1, x1);
	quad(x3, y3, x4, y4, y4, x4, y3, x3);

	extrude(x1, y1, x2, y2);
	extrude(x2, y2, y2, x2);
	extrude(y2, x2, y1, x1);
	extrude(y1, x1, x1, y1);
	extrude(x3, y3, x4, y4);
	extrude(x4, y4, y4, x4);
	extrude(y4, x4, y3, x3);

	const qreal Pi = 3.14159f;
	const int NumSectors = 100;

	for (int i = 0; i < NumSectors; ++i) {
		qreal angle1 = (i * 2 * Pi) / NumSectors;
		qreal x5 = 0.30 * sin(angle1);
		qreal y5 = 0.30 * cos(angle1);
		qreal x6 = 0.20 * sin(angle1);
		qreal y6 = 0.20 * cos(angle1);

		qreal angle2 = ((i + 1) * 2 * Pi) / NumSectors;
		qreal x7 = 0.20 * sin(angle2);
		qreal y7 = 0.20 * cos(angle2);
		qreal x8 = 0.30 * sin(angle2);
		qreal y8 = 0.30 * cos(angle2);

		quad(x5, y5, x6, y6, x7, y7, x8, y8);

		extrude(x6, y6, x7, y7);
		extrude(x8, y8, x5, y5);
	}

	for (int i = 0; i < m_vertices.size(); i++)
		m_vertices[i] *= 2.0f;
}
void MOpenglWidget::quad(qreal x1, qreal y1, qreal x2, qreal y2, qreal x3, qreal y3, qreal x4, qreal y4)
{
	m_vertices << QVector3D(x1, y1, -0.05f);
	m_vertices << QVector3D(x2, y2, -0.05f);
	m_vertices << QVector3D(x4, y4, -0.05f);

	m_vertices << QVector3D(x3, y3, -0.05f);
	m_vertices << QVector3D(x4, y4, -0.05f);
	m_vertices << QVector3D(x2, y2, -0.05f);

	QVector3D n = QVector3D::normal
		(QVector3D(x2 - x1, y2 - y1, 0.0f), QVector3D(x4 - x1, y4 - y1, 0.0f));

	m_normals << n;
	m_normals << n;
	m_normals << n;

	m_normals << n;
	m_normals << n;
	m_normals << n;

	m_vertices << QVector3D(x4, y4, 0.05f);
	m_vertices << QVector3D(x2, y2, 0.05f);
	m_vertices << QVector3D(x1, y1, 0.05f);

	m_vertices << QVector3D(x2, y2, 0.05f);
	m_vertices << QVector3D(x4, y4, 0.05f);
	m_vertices << QVector3D(x3, y3, 0.05f);

	n = QVector3D::normal
		(QVector3D(x2 - x4, y2 - y4, 0.0f), QVector3D(x1 - x4, y1 - y4, 0.0f));

	m_normals << n;
	m_normals << n;
	m_normals << n;

	m_normals << n;
	m_normals << n;
	m_normals << n;
}

void MOpenglWidget::extrude(qreal x1, qreal y1, qreal x2, qreal y2)
{
	m_vertices << QVector3D(x1, y1, +0.05f);
	m_vertices << QVector3D(x2, y2, +0.05f);
	m_vertices << QVector3D(x1, y1, -0.05f);

	m_vertices << QVector3D(x2, y2, -0.05f);
	m_vertices << QVector3D(x1, y1, -0.05f);
	m_vertices << QVector3D(x2, y2, +0.05f);

	QVector3D n = QVector3D::normal
		(QVector3D(x2 - x1, y2 - y1, 0.0f), QVector3D(0.0f, 0.0f, -0.1f));

	m_normals << n;
	m_normals << n;
	m_normals << n;

	m_normals << n;
	m_normals << n;
	m_normals << n;
}

void MOpenglWidget::createBubbles(int number)
{
	for (int i = 0; i < number; ++i) {
		QPointF position(width()*(0.1 + (0.8*qrand() / (RAND_MAX + 1.0))),
			height()*(0.1 + (0.8*qrand() / (RAND_MAX + 1.0))));
		qreal radius = qMin(width(), height())*(0.0175 + 0.0875*qrand() / (RAND_MAX + 1.0));
		QPointF velocity(width()*0.0175*(-0.5 + qrand() / (RAND_MAX + 1.0)),
			height()*0.0175*(-0.5 + qrand() / (RAND_MAX + 1.0)));

		m_bubbles.append(new Bubble(position, radius, velocity));
	}
}

void MOpenglWidget::sendDataToOpenGL()
{
	/*ShapeData cube = ShapeGenerator::makeCube();
	ShapeData arrow = ShapeGenerator::makeArrow();
	ShapeData plane = ShapeGenerator::makePlane();
	ShapeData teapot = ShapeGenerator::makeTeapot();
	ShapeData sphere = ShapeGenerator::makeSphere();
	ShapeData torus = ShapeGenerator::makeTorus();

	glGenBuffers(1, &theBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, theBufferID);
	glBufferData(GL_ARRAY_BUFFER,
		cube.vertexBufferSize() + cube.indexBufferSize() +
		arrow.vertexBufferSize() + arrow.indexBufferSize() +
		plane.vertexBufferSize() + plane.indexBufferSize() +
		teapot.vertexBufferSize() + teapot.indexBufferSize() +
		sphere.vertexBufferSize() + sphere.indexBufferSize() +
		torus.vertexBufferSize() + torus.indexBufferSize(), 0, GL_STATIC_DRAW);

	GLsizeiptr currentOffset = 0;
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, cube.vertexBufferSize(), cube.vertices);
	currentOffset += cube.vertexBufferSize();
	cubeIndexByteOffset = currentOffset;
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, cube.indexBufferSize(), cube.indices);
	currentOffset += cube.indexBufferSize();
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, arrow.vertexBufferSize(), arrow.vertices);
	currentOffset += arrow.vertexBufferSize();
	arrowIndexByteOffset = currentOffset;
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, arrow.indexBufferSize(), arrow.indices);
	currentOffset += arrow.indexBufferSize();
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, plane.vertexBufferSize(), plane.vertices);
	currentOffset += plane.vertexBufferSize();
	planeIndexByteOffset = currentOffset;
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, plane.indexBufferSize(), plane.indices);
	currentOffset += plane.indexBufferSize();
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, teapot.vertexBufferSize(), teapot.vertices);
	currentOffset += teapot.vertexBufferSize();
	teapotIndexByteOffset = currentOffset;
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, teapot.indexBufferSize(), teapot.indices);
	currentOffset += teapot.indexBufferSize();
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, sphere.vertexBufferSize(), sphere.vertices);
	currentOffset += sphere.vertexBufferSize();
	sphereIndexByteOffset = currentOffset;
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, sphere.indexBufferSize(), sphere.indices);
	currentOffset += sphere.indexBufferSize();
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, torus.vertexBufferSize(), torus.vertices);
	currentOffset += torus.vertexBufferSize();
	torusIndexByteOffset = currentOffset;
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, torus.indexBufferSize(), torus.indices);
	currentOffset += torus.indexBufferSize();

	cubeNumIndices = cube.numIndices;
	arrowNumIndices = arrow.numIndices;
	planeNumIndices = plane.numIndices;
	teapotNumIndices = teapot.numIndices;
	sphereNumIndices = sphere.numIndices;
	torusNumIndices = torus.numIndices;

	glGenVertexArrays(1, &cubeVertexArrayObjectID);
	glGenVertexArrays(1, &arrowVertexArrayObjectID);
	glGenVertexArrays(1, &planeVertexArrayObjectID);
	glGenVertexArrays(1, &teapotVertexArrayObjectID);
	glGenVertexArrays(1, &sphereVertexArrayObjectID);
	glGenVertexArrays(1, &torusVertexArrayObjectID);

	glBindVertexArray(cubeVertexArrayObjectID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, theBufferID);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(sizeof(float) * 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(sizeof(float) * 6));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, theBufferID);

	glBindVertexArray(arrowVertexArrayObjectID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, theBufferID);
	GLuint arrowByteOffset = cube.vertexBufferSize() + cube.indexBufferSize();
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)arrowByteOffset);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(arrowByteOffset + sizeof(float) * 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(arrowByteOffset + sizeof(float) * 6));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, theBufferID);

	glBindVertexArray(planeVertexArrayObjectID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, theBufferID);
	GLuint planeByteOffset = arrowByteOffset + arrow.vertexBufferSize() + arrow.indexBufferSize();
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)planeByteOffset);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(planeByteOffset + sizeof(float) * 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(planeByteOffset + sizeof(float) * 6));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, theBufferID);

	glBindVertexArray(teapotVertexArrayObjectID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, theBufferID);
	GLuint teapotByteOffset = planeByteOffset + plane.vertexBufferSize() + plane.indexBufferSize();
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)teapotByteOffset);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(teapotByteOffset + sizeof(float) * 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(teapotByteOffset + sizeof(float) * 6));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, theBufferID);

	glBindVertexArray(sphereVertexArrayObjectID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, theBufferID);
	GLuint sphereByteOffset = teapotByteOffset + teapot.vertexBufferSize() + teapot.indexBufferSize();
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)sphereByteOffset);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(sphereByteOffset + sizeof(float) * 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(sphereByteOffset + sizeof(float) * 6));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, theBufferID);

	glBindVertexArray(torusVertexArrayObjectID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, theBufferID);
	GLuint torusByteOffset = sphereByteOffset + sphere.vertexBufferSize() + sphere.indexBufferSize();
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)torusByteOffset);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(torusByteOffset + sizeof(float) * 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(torusByteOffset + sizeof(float) * 6));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, theBufferID);

	cube.cleanup();
	arrow.cleanup();
	plane.cleanup();
	teapot.cleanup();
	sphere.cleanup();
	torus.cleanup();*/
}
void MOpenglWidget::installShaders()
{
	/*GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar* adapter[1];
	string temp = readShaderCode("VertexShaderCode.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(vertexShaderID, 1, adapter, 0);
	temp = readShaderCode("FragmentShaderCode.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(fragmentShaderID, 1, adapter, 0);

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	if (!checkShaderStatus(vertexShaderID) || !checkShaderStatus(fragmentShaderID))
		return;

	programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);

	glLinkProgram(programID);

	if (!checkProgramStatus(programID))
		return;

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	temp = readShaderCode("VertexShaderPassThroughCode.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(vertexShaderID, 1, adapter, 0);
	temp = readShaderCode("FragmentShaderPassThroughCode.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(fragmentShaderID, 1, adapter, 0);

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	if (!checkShaderStatus(vertexShaderID) || !checkShaderStatus(fragmentShaderID))
		return;

	passThroughProgramID = glCreateProgram();
	glAttachShader(passThroughProgramID, vertexShaderID);
	glAttachShader(passThroughProgramID, fragmentShaderID);

	glLinkProgram(passThroughProgramID);

	if (!checkProgramStatus(passThroughProgramID))
		return;

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);*/
}

void MOpenglWidget::processShader(std::string file_name)
{
	/*GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar* adapter[1];
	string temp = readShaderCode("VertexShaderCode.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(vertexShaderID, 1, adapter, 0);
	temp = readShaderCode("FragmentShaderCode.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(fragmentShaderID, 1, adapter, 0);

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	if (!checkShaderStatus(vertexShaderID) || !checkShaderStatus(fragmentShaderID))
		return;

	programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);

	glLinkProgram(programID);

	if (!checkProgramStatus(programID))
	{
		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);
	}*/
}


std::string MOpenglWidget::readShaderCode(const char* fileName)
{
	ifstream meInput(fileName);
	if (!meInput.good())
	{
		cout << "File failed to load..." << fileName;
		exit(1);
	}
	return std::string(
		std::istreambuf_iterator<char>(meInput),
		std::istreambuf_iterator<char>());
}

bool MOpenglWidget::checkShaderStatus(GLuint shaderID)
{
	return checkStatus(shaderID, glGetShaderiv, glGetShaderInfoLog, GL_COMPILE_STATUS);
}

bool MOpenglWidget::checkProgramStatus(GLuint programID)
{
	return checkStatus(programID, glGetProgramiv, glGetProgramInfoLog, GL_LINK_STATUS);
}

bool MOpenglWidget::checkStatus(
	GLuint objectID,
	PFNGLGETSHADERIVPROC objectPropertyGetterFunc,
	PFNGLGETSHADERINFOLOGPROC getInfoLogFunc,
	GLenum statusType)
{
	GLint status;
	objectPropertyGetterFunc(objectID, statusType, &status);
	if (status != GL_TRUE)
	{
		GLint infoLogLength;
		objectPropertyGetterFunc(objectID, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* buffer = new GLchar[infoLogLength];

		GLsizei bufferSize;
		getInfoLogFunc(objectID, infoLogLength, &bufferSize, buffer);
		cout << buffer << endl;
		delete[] buffer;
		return false;
	}
	return true;
}

void MOpenglWidget::mousePressEvent(QMouseEvent *e)
{
	if (e->button() == Qt::LeftButton)
	{
		lastX = e->pos().rx();
		lastY = e->pos().ry();
		std::cout << "Press" << std::endl;
		std::cout << "X:" << lastX << "  Y:" << lastY << std::endl;

		move_scene_ = true;
	}
	old_pos_ = e->pos();
}

void MOpenglWidget::mouseMoveEvent(QMouseEvent* e)
{
	if (move_scene_	)
	{
		float xoffset = (e->pos().rx() - lastX)*0.1f;
		float yoffset = (lastY - e->pos().ry())*0.1f; // reversed since y-coordinates go from bottom to top

		lastX = e->pos().rx();
		lastY = e->pos().ry();
		std::cout << "X:" << lastX << "  Y:" << lastY << std::endl;
		//ptr_camera->ProcessMouseMovement(xoffset, yoffset);
		//ptr_camera->cameraViewTranslate(xoffset, yoffset);
	    ptr_camera->ViewTranslate(xoffset*factor_, yoffset*factor_);
		update();
	}
	if (e->buttons() & Qt::MiddleButton)
	{
		const int DeltaX = e->pos().x() - old_pos_.x();
		const int DeltaY = e->pos().y() - old_pos_.y();
		old_pos_ = e->pos();
		float angleH = (float)DeltaX / this->width() * rotate_speed_;
		float angleV = (float)DeltaY / this->height() * rotate_speed_;
		glm::vec3 min, max;
		min = min + max;
		glm::vec3 center = *((glm::vec3*)&min);
		center = glm::vec3(0.0f, 0.0f, 0.0f);
		center *= 0.5f;

		float angleTemp = 0.0f;
		
		ptr_camera->ViewRotate(angleH*20, angleV*20, center);
		update();

	}
	if ( e->buttons()== Qt::RightButton)
	{
		const int DeltaX = e->pos().x() - old_pos_.x();
		int DeltaY = old_pos_.y() - e->pos().y();
		float ratio = 1.0f;
		old_pos_ = e->pos();
		float angleTemp = 0.0f;
		

		int w = width();
		int h = height();
		float angleH = (float)DeltaX / width() * ratio;
		float angleV = (float)DeltaY / height() * ratio;
		ptr_camera->ViewTranslate(angleH*factor_, angleV*factor_);
	}
	update();
}
void MOpenglWidget::keyReleaseEvent(QKeyEvent * event)
{
	
	QOpenGLWidget::keyReleaseEvent(event);
	update();
}
void MOpenglWidget::keyPressEvent(QKeyEvent* e)
{
	
}


GLuint MOpenglWidget::genComputeProg(GLuint texHandle) {
	// Creating the compute shader, and the program object containing the shader
	GLuint progHandle = glCreateProgram();
	GLuint cs = glCreateShader(GL_COMPUTE_SHADER);

	const char *csSrc[] = {
		"#version 430 core\n"
		"uniform float roll;\n"
		"layout (local_size_x = 32, local_size_y = 16) in;\n"
		"layout (rg32f) uniform image2D data;\n"
		"void main(void)\n"
		"{\n"
		"ivec2 storePos = ivec2(gl_GlobalInvocationID.xy); \n"
		"float localCoef = length(vec2(ivec2(gl_LocalInvocationID.xy) - 8) / 8.0); \n"
		"float globalCoef = sin(float(gl_WorkGroupID.x + gl_WorkGroupID.y)*0.1 + roll)*0.5; \n"
		"imageStore(data, storePos, vec4(1.0 - globalCoef*localCoef, 0.0, 0.0, 0.0)); \n"
		"}\n"
	};
	
    glShaderSource(cs, 1, csSrc, NULL);
	glCompileShader(cs);
    int rvalue;
    glGetShaderiv(cs, GL_COMPILE_STATUS, &rvalue);
    if (!rvalue) {
        fprintf(stderr, "Error in compiling the compute shader\n");
        GLchar log[10240];
        GLsizei length;
        glGetShaderInfoLog(cs, 10239, &length, log);
        fprintf(stderr, "Compiler log:\n%s\n", log);
        exit(40);
    }
    glAttachShader(progHandle, cs);

    glLinkProgram(progHandle);
    glGetProgramiv(progHandle, GL_LINK_STATUS, &rvalue);
    if (!rvalue) {
        fprintf(stderr, "Error in linking compute shader program\n");
        GLchar log[10240];
        GLsizei length;
        glGetProgramInfoLog(progHandle, 10239, &length, log);
        fprintf(stderr, "Linker log:\n%s\n", log);
        exit(41);
    }   
	glUseProgram(progHandle);

	
	glGenTextures(1, &output_image);
	glBindTexture(GL_TEXTURE_2D, output_image);
	glTexStorage2D(GL_TEXTURE_2D, 8, GL_RGBA32F, 256, 256);




	return progHandle;
}


GLuint MOpenglWidget::genRenderProg(GLuint texHandle) {
	GLuint progHandle = glCreateProgram();
	GLuint vp = glCreateShader(GL_VERTEX_SHADER);
	GLuint fp = glCreateShader(GL_FRAGMENT_SHADER);

	const char *vpSrc[] = {
		"#version 430\n",
		"in vec2 pos;\
		 out vec2 texCoord;\
		void main() {\
		texCoord = pos*0.5f + 0.5f;\
		gl_Position = vec4(pos.x, pos.y, 0.0, 1.0);\
																																	 												 		 }"
	};

	const char *fpSrc[] = {
		"#version 430\n",
		"uniform sampler2D srcTex;\
		in vec2 texCoord;\
		out vec4 color;\
		 void main() {\
		 float c = texture(srcTex, texCoord).x;\
		color = vec4(c, 1.0, 1.0, 1.0);\
																																													 														 		 }"
	};

	glShaderSource(vp, 2, vpSrc, NULL);
	glShaderSource(fp, 2, fpSrc, NULL);

	glCompileShader(vp);
	int rvalue;
	glGetShaderiv(vp, GL_COMPILE_STATUS, &rvalue);
	if (!rvalue) {
		fprintf(stderr, "Error in compiling vp\n");
		exit(30);
	}
	glAttachShader(progHandle, vp);

	glCompileShader(fp);
	glGetShaderiv(fp, GL_COMPILE_STATUS, &rvalue);
	if (!rvalue) {
		fprintf(stderr, "Error in compiling fp\n");
		exit(31);
	}
	glAttachShader(progHandle, fp);

	glBindFragDataLocation(progHandle, 0, "color");
	glLinkProgram(progHandle);

	glGetProgramiv(progHandle, GL_LINK_STATUS, &rvalue);
	if (!rvalue) {
		fprintf(stderr, "Error in linking sp\n");
		exit(32);
	}

	glUseProgram(progHandle);
	glUniform1i(glGetUniformLocation(progHandle, "srcTex"), 0);

	GLuint vertArray;
	glGenVertexArrays(1, &vertArray);
	glBindVertexArray(vertArray);

	GLuint posBuf;
	glGenBuffers(1, &posBuf);
	glBindBuffer(GL_ARRAY_BUFFER, posBuf);
	float data[] = {
		-1.0f, -1.0f,
		-1.0f, 1.0f,
		1.0f, -1.0f,
		1.0f, 1.0f
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8, data, GL_STREAM_DRAW);
	GLint posPtr = glGetAttribLocation(progHandle, "pos");
	glVertexAttribPointer(posPtr, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(posPtr);

	checkErrors("Render shaders");
	return progHandle;
}


GLuint MOpenglWidget::genTexture() {
	// We create a single float channel 512^2 texture
	GLuint texHandle;
	glGenTextures(1, &texHandle);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texHandle);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, 512, 512, 0, GL_RED, GL_FLOAT, NULL);

	// Because we're also using this tex as an image (in order to write to it),
	// we bind it to an image unit as well
	glBindImageTexture(0, texHandle, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32F);
	checkErrors("Gen texture");
	return texHandle;
}


void MOpenglWidget::checkErrors(std::string desc) {
	GLenum e = glGetError();
	if (e != GL_NO_ERROR) {
		std::cout << "error" << std::endl;
		exit(20);
	}
}

// void MOpenglWidget::paintEvent(QPaintEvent *e)
// {
// 	return QOpenGLWidget::paintEvent(e);
// }

