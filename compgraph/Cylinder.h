#ifndef CYLINDER_H
#define CYLINDER_H

#include <vector>
class Cylinder {
private:
	void clearArrays();
	void addCoordinate(float x, float y, float z);
	void addNormal(float x, float y, float z);
	void addIndices(int i1, int i2, int i3);
	std::vector<float> getSideNormals();
	float radius;
	float height;
	int sectors;
	int segments;

	std::vector<float> coordinates;
	std::vector<float> normals;
	std::vector< int> indices;
	std::vector<float> circleCoordinates;
	std::vector<float> vertices;
public:
	Cylinder(float radius = 1.0f, float height = 2.0f,
		int sectors = 4, int segments = 1);
	~Cylinder() {}

	// for vertex data
	std::vector<float> getUnitCirleVertices() { return circleCoordinates; }
	int getCoordinatesCount() const { return (int)coordinates.size() / 3; }
	int getNormalCount() const { return (int)normals.size() / 3; }
	int getIndexCount() const { return (int)indices.size(); }
	int getCoordinatesSize() const { return (int)coordinates.size() * sizeof(float); }
	int getNormalSize() const { return (int)normals.size() * sizeof(float); }
	int getIndexSize() const { return (int)indices.size() * sizeof(int); }
	const float* getCoordinates() const { return coordinates.data(); }
	const float* getNormals() const { return normals.data(); }
	const  int* getIndices() const { return indices.data(); }

	int getVertexCount() const { return getCoordinatesCount(); }
	int getVertexSize() const { return (int)vertices.size() * sizeof(int); } //bytes   
	const float* getInterleavedVertices() const { return vertices.data(); }

	int getSouthCapIndexCount() const { return ((int)indices.size() - southCapIndex) / 2; }
	int getNorthCapIndexCount() const { return ((int)indices.size() - southCapIndex) / 2; }
	int getSideIndexCount() const { return southCapIndex; }

	void fillCircleCoordinates();
	void insertVertices();
	void fillSeparateCoordinateArrays();
	int southCapIndex;
	int northCapIndex;
};

#endif