#ifndef VBO_H
#define VBO_H

//Include
#include <glad/glad.h>

class VBO {
public:
	GLuint ID;
	VBO(GLfloat* vertices, GLsizeiptr size);

	void Bind();
	void Unbind();
	void Delete();

};

#endif // !VBO_H