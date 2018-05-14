#ifndef MY_MESH_H
#define MY_MESH_H
#include <string>
#include <iostream>
#include <fstream>

class MyMesh
{
public:
	MyMesh(const std::string &_filename);
	~MyMesh();
private:
	int vsize;


};

#endif