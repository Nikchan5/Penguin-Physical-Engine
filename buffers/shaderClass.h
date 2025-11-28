// shaderClass.h

#include <glad/glad.h>
#include <string>
#include <glm/glm.hpp>
#include <fstream> 

class Shader
{
public:
    GLuint ID;
    Shader(const char* vertexFile, const char* fragmentFile);

    void Activate();
    void Delete();
    
    void SetMat4(const char* uniformName, const glm::mat4& matrix);
    // Declaration for setting integer uniforms (used for texture units)
    void SetInt(const char* uniformName, int value); 
};