#pragma once
#include <string>
#include <vector>

using std::string;
using std::vector;

class Torus :
{

public:
    // constructor, etc.
    Torus(float r, float R, int nr, int nR);
    ~Torus();


private:
    void _createTorus();
    void _createGeometry();
    // DATA 

    // torus parameters
    float _r = 0.5f;
    float _R = 1.0f;
    int _nr = 16;
    int _nR = 16;


    // tangent 
    vector<float> _tangents;

    void clear() {
        _vertices.clear();
        _colors.clear();
        _normals.clear();
        _texCoords.clear();
    }

    // vertex data
    vector<float> _vertices;
    vector<float> _colors;
    vector<float> _normals;
    vector<float> _texCoords;

    uint32_t _vertexCount = 0;

};
class Render3D
{
public:

    Render3D() {}
    ~Render3D() {}

    // clear data
    void clear() {
        _vertices.clear();
        _colors.clear();
        _normals.clear();
        _texCoords.clear();
    }

    // vertex data
    vector<float> _vertices;
    vector<float> _colors;
    vector<float> _normals;
    vector<float> _texCoords;

    uint32_t _vertexCount = 0;
};