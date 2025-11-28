#ifndef EBO_H
#define EBO_H

//Include
#include <glad/glad.h>

class EBO {
public:
    EBO(const GLuint* indices, GLsizeiptr size);

    void Bind();
    void Unbind();
    void Delete();

private:
    GLuint ID;
};

#endif // EBO_H