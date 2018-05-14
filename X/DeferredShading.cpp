#include "DeferredShading.h"



MyDeferredShading::MyDeferredShading()
{
	op_data_ = new MyGlData();
	initOpengl();
}

void MyDeferredShading::initOpengl()
{
	shader = USShader("lesson5.vs", "lesson5.fs");
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, 3*36, op_data_->g_vertex_buffer_data2, GL_STATIC_DRAW);
	glGenBuffers(1, &vertexColorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexColorBuffer);
	glBufferData(GL_ARRAY_BUFFER, 3*36, op_data_->g_uv_buffer_data, GL_STATIC_DRAW);
	woodTexture = MyCommanFuc::loadTexture(".\\texture\\wood.png");
}

void MyDeferredShading::DoDraw()
{
	
	
	shader.use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, woodTexture);
	GLuint TextureID = glGetUniformLocation(shader.ID, "myTextureSampler");
	glUniform1i(TextureID, 0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



	// ÅäÖÃ
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);



	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vertexColorBuffer);
	glVertexAttribPointer(
		1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		2,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
		);





	// Draw the triangle !
	glDrawArrays(GL_TRIANGLES, 0, 12 * 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
	glDisableVertexAttribArray(0);
}
