#include "Earth.h"

namespace basicgraphics{

using namespace std;
using namespace glm;

Earth::Earth() {

    // Note: TEXTURE_PATH is set in config.h
    shared_ptr<Texture> tex = Texture::create2DTextureFromFile(TEXTURE_PATH);
    tex->setTexParameteri(GL_TEXTURE_WRAP_S, GL_REPEAT);
    tex->setTexParameteri(GL_TEXTURE_WRAP_T, GL_REPEAT);
    tex->setTexParameteri(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    tex->setTexParameteri(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    textures.push_back(tex);
	setupGeometry();
}

void Earth::setupGeometry() {
    // Tesselate and create texture coordinates for the Earth based on STACKS, SLICES, and SPHERE_RADIUS params.
    const int STACKS = 20;
    const int SLICES = 40;
    const float SPHERE_RADIUS = 1.0f;
    
    std::vector<Mesh::Vertex> vertexArray;
    std::vector<int> indexArray;
    
    const float stackHeight = 2.0 / STACKS;
    const float sliceAngle = 2 * glm::pi<float>() / SLICES;
    const float texStackHeight = -1.0 / STACKS;
    const float texSliceWidth = 1.0 / SLICES;
    
    std::vector<vec3> topVerts;
    std::vector<vec3> bottomVerts;
    float stackTopY;
    float stackBottomY;
    float stackTopRadius;
    float stackBottomRadius;
    
    vec3 prevTop;
    vec3 prevBottom;
    vec3 nextTop;
    vec3 nextBottom;
    
    vec2 prevTexTop;
    vec2 prevTexBottom;
    vec2 nextTexTop;
    vec2 nextTexBottom;
    
    // CREATE NORTH POLE (EARTH TOP)
    stackTopY = SPHERE_RADIUS;
    stackBottomY = SPHERE_RADIUS - stackHeight;
    stackBottomRadius = sqrt(pow(SPHERE_RADIUS, 2) - pow(stackBottomY, 2));
    nextTop = vec3(0, stackTopY, 0);
    nextBottom = vec3(0, stackBottomY, stackBottomRadius);
    
    for (int slIndex = 0; slIndex <= SLICES; slIndex++) {
        // Set the triangle points
        prevBottom = nextBottom;
        bottomVerts.push_back(prevBottom);
        nextBottom = vec3(stackBottomRadius * sin(sliceAngle * (slIndex + 1)), stackBottomY, stackBottomRadius * cos(sliceAngle * (slIndex + 1)));
        
        Mesh::Vertex vert1;
        vert1.position = nextTop;
        vert1.normal = normalize(nextTop);
        vert1.texCoord0 = vec2(slIndex * texSliceWidth, 1);
        vertexArray.push_back(vert1);
        indexArray.push_back(slIndex * 3);
        
        Mesh::Vertex vert2;
        vert2.position = prevBottom;
        vert2.normal = normalize(prevBottom);
        vert2.texCoord0 = vec2(slIndex * texSliceWidth, SPHERE_RADIUS - texStackHeight);
        vertexArray.push_back(vert2);
        indexArray.push_back(slIndex * 3 + 1);
        
        Mesh::Vertex vert3;
        vert3.position = nextBottom;
        vert3.normal = normalize(nextBottom);
        vert3.texCoord0 = vec2((slIndex + 1) * texSliceWidth, SPHERE_RADIUS - texStackHeight);
        vertexArray.push_back(vert3);
        indexArray.push_back(slIndex * 3 + 2);
    }
    
    // CREATE EARTH BODY
    for (int stIndex = 0; stIndex < STACKS - 2; stIndex++) {
        stackTopY = stackBottomY;
        stackBottomY = SPHERE_RADIUS - (stIndex + 2) * stackHeight;
        stackTopRadius = stackBottomRadius;
        stackBottomRadius = sqrt(1 - pow(stackBottomY, 2));
        
        topVerts = bottomVerts;
        bottomVerts.clear();

        nextTop = topVerts[0];
        nextBottom = vec3(0, stackBottomY, stackBottomRadius);
        
        int VBOIndex = vertexArray.size();

        float tex = 0;

        // Create a layer
        for (int slIndex = 0; slIndex <= SLICES; slIndex++) {
            tex = slIndex * texSliceWidth;
            
            // Set the triangle points
            prevTop = nextTop;
            nextTop = topVerts[slIndex + 1];
            prevBottom = nextBottom;
            nextBottom = vec3(stackBottomRadius * sin(sliceAngle * (slIndex + 1)), stackBottomY, stackBottomRadius * cos(sliceAngle * (slIndex + 1)));
            bottomVerts.push_back(prevBottom);
            
            Mesh::Vertex vert1;
            vert1.position = prevTop;
            vert1.normal = normalize(prevTop);
            vert1.texCoord0 = vec2(slIndex * texSliceWidth, 1 - (stIndex + 1) * texStackHeight);
            vertexArray.push_back(vert1);
            indexArray.push_back(slIndex * 6 + VBOIndex);
            
            Mesh::Vertex vert2;
            vert2.position = prevBottom;
            vert2.normal = normalize(prevBottom);
            vert2.texCoord0 = vec2(slIndex * texSliceWidth, 1 - (stIndex + 2) * texStackHeight);
            vertexArray.push_back(vert2);
            indexArray.push_back(slIndex * 6 + 1 + VBOIndex);
            
            Mesh::Vertex vert3;
            vert3.position = nextBottom;
            vert3.normal = normalize(nextBottom);
            vert3.texCoord0 = vec2((slIndex + 1) * texSliceWidth, 1 - (stIndex + 2) * texStackHeight);
            vertexArray.push_back(vert3);
            indexArray.push_back(slIndex * 6 + 2 + VBOIndex);

            Mesh::Vertex vert4;
            vert4.position = prevTop;
            vert4.normal = normalize(prevTop);
            vert4.texCoord0 = vec2(slIndex * texSliceWidth, 1 - (stIndex + 1) * texStackHeight);
            vertexArray.push_back(vert4);
            indexArray.push_back(slIndex * 6 + 3 + VBOIndex);
            
            Mesh::Vertex vert5;
            vert5.position = nextBottom;
            vert5.normal = normalize(nextBottom);
            vert5.texCoord0 = vec2((slIndex + 1) * texSliceWidth, 1 - (stIndex + 2) * texStackHeight);
            vertexArray.push_back(vert5);
            indexArray.push_back(slIndex * 6 + 4 + VBOIndex);
            
            Mesh::Vertex vert6;
            vert6.position = nextTop;
            vert6.normal = normalize(nextTop);
            vert6.texCoord0 = vec2((slIndex + 1) * texSliceWidth, 1 - (stIndex + 1) * texStackHeight);
            vertexArray.push_back(vert6);
            indexArray.push_back(slIndex * 6 + 5 + VBOIndex);
            
            
        }
    }

    // CREATE SOUTH POLE (EARTH BOTTOM)
    stackTopY = stackBottomY;
    stackBottomY = -SPHERE_RADIUS;
    stackTopRadius = stackBottomRadius;
    topVerts = bottomVerts;

    nextTop = topVerts[0];
    nextBottom = vec3(0,stackBottomY,0);;
    int VBOIndex = vertexArray.size();

    for (int slIndex = 0; slIndex <= SLICES; slIndex++) {
        // Set the triangle points
        prevTop = nextTop;
        nextTop = topVerts[slIndex + 1];
        
        Mesh::Vertex vert1;
        vert1.position = prevTop;
        vert1.normal = normalize(prevTop);
        vert1.texCoord0 = vec2(slIndex * texSliceWidth, texStackHeight);
        vertexArray.push_back(vert1);
        indexArray.push_back(slIndex * 3 + VBOIndex);
        
        Mesh::Vertex vert2;
        vert2.position = nextBottom;
        vert2.normal = normalize(nextBottom);
        vert2.texCoord0 = vec2(slIndex * texSliceWidth, 0);
        vertexArray.push_back(vert2);
        indexArray.push_back(slIndex * 3 + 1 + VBOIndex);
        
        Mesh::Vertex vert3;
        vert3.position = nextTop;
        vert3.normal = normalize(nextTop);
        vert3.texCoord0 = vec2((slIndex + 1) * texSliceWidth, texStackHeight);
        vertexArray.push_back(vert3);
        indexArray.push_back(slIndex * 3 + 2 + VBOIndex);
    }
    
    
    const int numVertices = vertexArray.size();
    const int vertexByteSize = sizeof(Mesh::Vertex) * numVertices;
    const int indexByteSize = sizeof(int) * indexArray.size();
    _mesh.reset(new Mesh(textures, GL_TRIANGLES, GL_STATIC_DRAW, vertexByteSize, indexByteSize, 0,vertexArray, indexArray.size(), indexByteSize, &indexArray[0]));
}


glm::vec3 Earth::getPosition(double latitude, double longitude) {
    // Given a latitude and longitude as input, returns the corresponding 3D x,y,z position
    double z = -cos(radians(longitude)) * cos(radians(latitude));//sin(theda);
    double y = sin(radians(latitude));
    double x = -sin(radians(longitude)) * cos(radians(latitude));//cos(theda);
    return vec3(x, y, z);
}


void Earth::draw(GLSLProgram &shader) {
    _mesh->draw(shader);
}
}
