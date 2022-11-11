#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include "Cylinder.h"

Cylinder::Cylinder(float radius, float height, int sectors,
	int segments)
{
	this->radius = radius;
	this->height = height;
	this->sectors = sectors;
	if (sectors < 3)
		this->sectors = 3;
	this->segments = segments;
	if (segments < 1)
		this->segments = 1;
	fillSeparateCoordinateArrays();
}

void Cylinder::addCoordinate(float x, float y, float z)
{
	coordinates.push_back(x);
	coordinates.push_back(y);
	coordinates.push_back(z);
}

void Cylinder::addNormal(float x, float y, float z)
{
	normals.push_back(x);
	normals.push_back(y);
	normals.push_back(z);
}


void Cylinder::addIndices(int i1, int i2, int i3)
{
	indices.push_back(i1);
	indices.push_back(i2);
	indices.push_back(i3);
}

void Cylinder::clearArrays()
{
	coordinates.clear();
	normals.clear();
	indices.clear();
}

std::vector<float> Cylinder::getSideNormals()
{
	float sectorStep = 2 * M_PI / sectors;
	float theta;

	// tangent
	float tang = 0.0 / height;
	float x0 = cos(tang);
	float y0 = 0;
	float z0 = sin(tang);

	std::vector<float> normals;
	for (int i = 0; i <= sectors; ++i)
	{
		theta = i * sectorStep;
		float nx = cos(theta) * x0 - sin(theta) * y0;
		float ny = sin(theta) * x0 + cos(theta) * y0;
		normals.push_back(nx);
		normals.push_back(ny);
		normals.push_back(z0);
	}

	return normals;
}


void Cylinder::fillCircleCoordinates()
{
	float sectorStep = 2 * M_PI / sectors;
	float theta;  

	//in a unit circle x coordinate stands for the cos of the angle and 
	//y stands for the sin of the angle
	//angle gets multiplied by the current sector number and the coordinates
	//are added into array respectively
	circleCoordinates.clear();
	for (int i = 0; i <= sectors; ++i)
	{
		theta = i * sectorStep;
		circleCoordinates.push_back(cos(theta));
		circleCoordinates.push_back(sin(theta));
		circleCoordinates.push_back(0);
	}
}
//fills the overall array for the object
//so the vertex attribute pointer will take positions first and then the normals
void Cylinder::insertVertices()
{
	vertices.clear();
	int count = static_cast<int>(coordinates.size());
	for (int i = 0, j = 0; i < count; i += 3, j += 2)
	{
		vertices.push_back(coordinates[i]);
		vertices.push_back(coordinates[i + 1]);
	}
}
//fills arrays for positions(coordinates) and normals as well as their indices

void Cylinder::fillSeparateCoordinateArrays()
{
	clearArrays();
	fillCircleCoordinates();

	float x, y, z;
	float radius;

	std::vector<float> sideNormals = getSideNormals();

	for (int i = 0; i <= segments; ++i)
	{
		z = -(height * 0.5f) + (float)i / segments * height;
		radius = this->radius + (float)i / segments * 0;

		for (int j = 0, k = 0; j <= sectors; ++j, k += 3)
		{
			x = circleCoordinates[k];
			y = circleCoordinates[k + 1];
			addCoordinate(x * this->radius, y * this->radius, z);
			addNormal(sideNormals[k], sideNormals[k + 1], sideNormals[k + 2]);
		}
	}

	unsigned int southVertexIndex = (unsigned int)coordinates.size() / 3;

	z = -height * 0.5f;
	addCoordinate(0, 0, z);
	for (int i = 0, j = 0; i < sectors; ++i, j += 3)
	{
		x = circleCoordinates[j];
		y = circleCoordinates[j + 1];
		addCoordinate(x * this->radius, y * this->radius, z);

	}

	unsigned int northIndexVertex = (unsigned int)coordinates.size() / 3;

	z = height * 0.5f;
	addCoordinate(0, 0, z);
	addNormal(0, 0, 1);
	for (int i = 0, j = 0; i < sectors; ++i, j += 3)
	{
		x = circleCoordinates[j];
		y = circleCoordinates[j + 1];
		addCoordinate(x * this->radius, y * this->radius, z);
		addNormal(0, 0, 1);
	}

	int s1, s2;
	for (int i = 0; i < segments; ++i)
	{
		s1 = i * (sectors + 1);
		s2 = s1 + sectors + 1;

		for (int j = 0; j < sectors; ++j, ++s1, ++s2)
		{
			addIndices(s1, s1 + 1, s2);
			addIndices(s2, s1 + 1, s2 + 1);
		}
	}

	southCapIndex = (int)indices.size();

	for (int i = 0, k = southVertexIndex + 1; i < sectors; ++i, ++k)
	{
		if (i < (sectors - 1))
			addIndices(southVertexIndex, k + 1, k);
		else
			addIndices(southVertexIndex, southVertexIndex + 1, k);
	}

	northCapIndex = (int)indices.size();

	for (int i = 0, k = northIndexVertex + 1; i < sectors; ++i, ++k)
	{
		if (i < (sectors - 1))
			addIndices(northIndexVertex, k, k + 1);
		else
			addIndices(northIndexVertex, k, northIndexVertex + 1);
	}

	insertVertices();
}