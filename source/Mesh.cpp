#include <Mesh.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <GeometryArray.h>

Mesh * Mesh::CreateXYUnitQuad()
{
    Mesh * newMesh = new Mesh;
    GeometryArray * g = new GeometryArray;
    
    newMesh->m_geometry.push_back(g);

    glm::vec3 quad_vertices_array[] = {
        {-1.0f,  -1.f,  0.0f},
        {-1.0f, 1.f,  0.0f},
        { 1.0f, 1.f,  0.0f},
        { 1.0f, -1.f,  0.0f}
    };
    
    glm::vec2 quad_tex_array[] = {
        {0.0f,  0.f},
        {0.0f, 1.f},
        { 1.0f, 1.f},
        { 1.0f, 0.0f}
    };
    GLuint quad_index_array[] = {
        0,1,2,2,3,0
    };
    
    
    
    
    std::vector<glm::vec3> quad_vertices;
    std::vector<glm::vec2> quad_tex;
    
    std::vector<GLuint> quad_indicies;
    
    quad_vertices.assign(quad_vertices_array, quad_vertices_array+ sizeof(quad_vertices_array)/ sizeof(quad_vertices_array[0]));
    quad_tex.assign(quad_tex_array, quad_tex_array+ sizeof(quad_tex_array)/ sizeof(quad_tex_array[0]));
    
    quad_indicies.assign(quad_index_array, quad_index_array+ sizeof(quad_index_array)/ sizeof(quad_index_array[0]));
    
    
    
    g->generateObjects();
    
    g->bind();
    
    g->addIndex(quad_indicies, "quad");
    g->addAttribute(quad_vertices, ATTRIB_POSITION, 3, GL_FLOAT);
    g->addAttribute(quad_tex, ATTRIB_TEXCOORD, 2, GL_FLOAT);
    
    
    g->loadAttributes();
    g->loadIndices();
    
    g->bind();
    return newMesh;
}
Mesh * Mesh::CreateMeshFromFile(std::string path)
{
    Mesh * newMesh = new Mesh;
    
    {
        //    std::vector<MeshRef> meshes;
        
        Assimp::Importer importer;
        const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs );
        
        if(!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
            //        return meshes;
        }
        
        std::cout << scene->mNumMeshes << std::endl;
        int numVertices = 0;
        for (int meshNum = 0; meshNum < scene->mNumMeshes; meshNum++)
        {
            std::cout << "Mesh " << meshNum << ": " << '"' <<(scene->mMeshes[meshNum]->mName.C_Str()) <<'"' <<
            scene->mMeshes[meshNum]->mNumVertices << " verts" << std::endl;
            numVertices += scene->mMeshes[meshNum]->mNumVertices;
            newMesh->m_geometry.push_back(new GeometryArray);
            
        }
        
        
        for (int meshNum = 0; meshNum < scene->mNumMeshes; meshNum++)
        {
            aiMesh *mesh = scene->mMeshes[meshNum];
            
            
            std::vector<glm::vec3> vertices;
            
            std::vector<glm::vec3> normals;
            
            std::vector<glm::vec2> texCoords;
            
            bool hasTexCoords = mesh->mTextureCoords[0];
            
            for (int i = 0; i < mesh->mNumVertices; i++) {
                glm::vec3 vert;
                vert.x = mesh->mVertices[i].x;
                vert.y = mesh->mVertices[i].y;
                vert.z = mesh->mVertices[i].z;
                vertices.push_back(vert);
                
                if (mesh->HasNormals())
                {
                    glm::vec3 norm;
                    norm.x = mesh->mNormals[i].x;
                    norm.y = mesh->mNormals[i].y;
                    norm.z = mesh->mNormals[i].z;
                    normals.push_back(norm);
                }
                
                if (hasTexCoords)
                {
                    texCoords.push_back(glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y));
                }
            }
            
            std::vector<unsigned int> indices;
            for(int i = 0; i < mesh->mNumFaces; i++)
            {
                aiFace face = mesh->mFaces[i];
                // Retrieve all indices of the face and store them in the indices vector
                for(int j = 0; j < face.mNumIndices; j++)
                    indices.push_back(face.mIndices[j]);
            }
            
            std::string directory = "";
            int lastFolderIndex = path.find_last_of('/');
            if (lastFolderIndex < 0)
            {
                lastFolderIndex = path.find_last_of('\\');
            }
            
            if (lastFolderIndex >= 0)
            {
                directory = path.substr(0, lastFolderIndex+1);
            }
            GeometryArray* dest = newMesh->m_geometry.at(meshNum);
            
            dest->addIndex(indices, "shape");
            dest->addAttribute(vertices, ATTRIB_POSITION, 3, GL_FLOAT);
            
            if (mesh->HasNormals())
            {
                dest->addAttribute(normals, ATTRIB_NORMAL, 3, GL_FLOAT);
                
            }
            
            
            if (hasTexCoords)
            {
                dest->addAttribute(texCoords, ATTRIB_TEXCOORD, 2, GL_FLOAT);
            }
            
            dest->generateObjects();
            
            dest->bind();
            
            
            dest->loadAttributes();
            dest->loadIndices();
            
            dest->bind();
            //        if (mesh->mMaterialIndex >= 0)
            //        {
            //            aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
            //            int texCount = material->GetTextureCount(aiTextureType_DIFFUSE);
            //            std::cout << texCount << " count" << std::endl;
            //            if (texCount > 0)
            //            {
            //                aiString str;
            //                material->GetTexture(aiTextureType_DIFFUSE, 0, &str);
            //                std::cout << std::string(str.C_Str()) << std::endl;
            //                finalMesh->setTexture(TextureRef(new SOILTexture(directory + std::string(str.C_Str()))));
            //            }
            //        }
            //        
            //        meshes.push_back(finalMesh);
        }
        
    }
  
    return newMesh;
}

void Mesh::draw()
{
    for (auto &g : m_geometry)
    {
        g->bind();
        g->draw();
    }
}

