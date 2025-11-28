// shaderClass.cpp

#include "shaderClass.h"
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <string>
#include <fstream>   
#include <sstream>   
#include <iostream>  
#include <stdexcept> 

// *** HELPER FUNCTION TO READ SHADER FILE ***
std::string read_shader_file(const char* filename) {
    std::ifstream file;
    // Enable exceptions for file stream errors
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit); 
    try {
        file.open(filename);
        std::stringstream shaderStream;
        shaderStream << file.rdbuf();
        file.close();
        return shaderStream.str();
    } catch (std::ifstream::failure& e) {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << filename << std::endl;
        // Throw runtime error on failure
        throw std::runtime_error("Failed to read shader file.");
    }
}

// *** CONSTRUCTOR ***
Shader::Shader(const char* vertexFile, const char* fragmentFile) {
    // 1. Read file content
    std::string vertexCode = read_shader_file(vertexFile);
    const char* vertexSrc = vertexCode.c_str();

    std::string fragmentCode = read_shader_file(fragmentFile);
    const char* fragmentSrc = fragmentCode.c_str();

    // 2. Compile Vertex Shader
    GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShaderID, 1, &vertexSrc, NULL);
    glCompileShader(vertexShaderID);

    // 3. Compile Fragment Shader
    GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderID, 1, &fragmentSrc, NULL);
    glCompileShader(fragmentShaderID);

    // 4. Link Program
    ID = glCreateProgram();
    glAttachShader(ID, vertexShaderID);
    glAttachShader(ID, fragmentShaderID);
    glLinkProgram(ID);
    
    // 5. Cleanup
    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);
    // NOTE: In a production environment, you must add error checking (glGetShaderiv, glGetProgramiv) here.
}

// *** METHOD: ACTIVATE SHADER ***
void Shader::Activate() {
    glUseProgram(ID);
}

// *** METHOD: DELETE SHADER PROGRAM ***
void Shader::Delete() {
    glDeleteProgram(ID);
}

// *** METHOD: SET MAT4 UNIFORM ***
void Shader::SetMat4(const char* uniformName, const glm::mat4& matrix)
{
    GLint uniformLocation = glGetUniformLocation(ID, uniformName);
    if (uniformLocation != -1) {
        // Send the matrix to the shader
        glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(matrix));
    }
}

// *** METHOD: SET INT UNIFORM (For texture units) ***
void Shader::SetInt(const char* uniformName, int value)
{
    GLint uniformLocation = glGetUniformLocation(ID, uniformName);
    if (uniformLocation != -1) {
        // Send the integer (texture unit index) to the shader
        glUniform1i(uniformLocation, value);
    } 
}