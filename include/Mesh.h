#ifndef MESH_H
#define MESH_H

#include <vector>

class GeometryArray;
class Mesh;
typedef std::shared_ptr<Mesh> MeshRef;

class Mesh {
public:
    static Mesh * CreateMeshFromFile(std::string path);
    static Mesh * CreateXYUnitQuad();

    std::vector<GeometryArray*> m_geometry;
    void draw();
};

#endif