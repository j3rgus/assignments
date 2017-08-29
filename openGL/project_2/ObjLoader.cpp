#include "ObjLoader.h"

vector<Vector3f> & ObjLoader::GetVertices()
{
	return vertices;
}

vector<Vector3f> & ObjLoader::GetNormals()
{
	return normals;
}

vector<Triangle> & ObjLoader::GetTriangles()
{
	return triangles;
}

bool ObjLoader::Load(const char * filename)
{
	vertices.clear();
	normals.clear();
	texcoords.clear();
	triangles.clear();

	ifstream file(filename);
	if (!file.is_open())
	{
		cout << "Error opening file" << endl;
		return false;
	}

	while (!file.fail())
	{
		string prefix;
		file >> prefix;

		if (prefix == "v")
		{
			Vector3f v;
			file >> v.x >> v.y >> v.z;
			vertices.push_back(v);
		}
		else if (prefix == "vt")
		{
			Vector2f tx;
			file >> tx.x >> tx.y;
			texcoords.push_back(tx);
		}
		else if (prefix == "vn")
		{
			Vector3f vn;
			file >> vn.x >> vn.y >> vn.z;
			normals.push_back(vn);
		}
		else if (prefix == "f")
		{
			Triangle t;
			char slash;
			file >> t.v0 >> slash >> t.t0 >> slash >> t.n0;
			file >> t.v1 >> slash >> t.t1 >> slash >> t.n1;
			file >> t.v2 >> slash >> t.t2 >> slash >> t.n2;
			DecrementIndices(t);
			triangles.push_back(t);
		}
		else
		{
			// Other cases
		}
		file.ignore(1000, '\n');
	}

	file.close();
	return true;
}

void ObjLoader::saveObject(Object &obj)
{
	Vector3f vec;
	Vector3f tex;
	Vector3f norm;

	for (vector<Triangle>::iterator it = triangles.begin() ; it != triangles.end(); ++it) {
		vec.x = vertices[it->v0].x; tex.x = texcoords[it->t0].x; norm.x = normals[it->n0].x;
		vec.y = vertices[it->v0].y; tex.y = texcoords[it->t0].y; norm.y = normals[it->n0].y;
		vec.z = vertices[it->v0].z; norm.z = normals[it->n0].z;
		obj.vertices.push_back(vec);
		obj.texcoords.push_back(tex);
		obj.normals.push_back(norm);

		vec.x = vertices[it->v1].x; tex.x = texcoords[it->t1].x; norm.x = normals[it->n1].x;
		vec.y = vertices[it->v1].y; tex.y = texcoords[it->t1].y; norm.y = normals[it->n1].y;
		vec.z = vertices[it->v1].z; norm.z = normals[it->n1].z;
		obj.vertices.push_back(vec);
		obj.texcoords.push_back(tex);
		obj.normals.push_back(norm);

		vec.x = vertices[it->v2].x; tex.x = texcoords[it->t2].x; norm.x = normals[it->n2].x;
		vec.y = vertices[it->v2].y; tex.y = texcoords[it->t2].y; norm.y = normals[it->n2].y;
		vec.z = vertices[it->v2].z; norm.z = normals[it->n2].z;
		obj.vertices.push_back(vec);
		obj.texcoords.push_back(tex);
		obj.normals.push_back(norm);
	}
}

void ObjLoader::DecrementIndices(Triangle & triangle)
{
	triangle.v0--; triangle.t0--; triangle.n0--;
	triangle.v1--; triangle.t1--; triangle.n1--;
	triangle.v2--; triangle.t2--; triangle.n2--;
}

void ObjLoader::PrintLog()
{
	cout << "Vertices: " << vertices.size() << endl;
	cout << "Normals: " << normals.size() << endl;
	cout << "Uvs: " << texcoords.size() << endl;
	cout << "Triangles: " << triangles.size() << endl;
}
