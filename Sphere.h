#ifndef SPHERE_CLASS_H
#define SPHERE_CLASS_H

#include <GL/glew.h>

class Sphere {
public:
    Sphere();
    ~Sphere();
    void init(GLuint vertexPositionID);
    void cleanup();
    void draw();

private:
    int lats, longs;
    bool isInitialized;
    GLuint m_vao, m_vboVertex, m_vboIndex;
    int numsToDraw;
};

#endif // !SPHERE_CLASS_H
