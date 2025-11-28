#include "EBO.h"

// �����������: ������ ��������� ��������� �� ����������� GLuint.
EBO::EBO(const GLuint* indices, GLsizeiptr size) {
    // ���������� �������� & ��� ��������� ������ ID
    glGenBuffers(1, &ID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
}

// ���������� EBO
void EBO::Bind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

// ����������� EBO
void EBO::Unbind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// �������� EBO
void EBO::Delete() {
    // ���������� �������� & ��� �������� ������
    glDeleteBuffers(1, &ID);
}