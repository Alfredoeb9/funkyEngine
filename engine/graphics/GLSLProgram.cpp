#include "GLSLProgram.h"
#include <vector>
#include <core/Errors.h>
#include <fstream>

GLSLProgram::GLSLProgram() : _programID(0), _vertexShaderID(0), _fragmentShaderID(0), _numAttributes(0) {

}

GLSLProgram::~GLSLProgram() {

}

// https://wikis.khronos.org/opengl/Shader_Compilation        <---- Offical Docs
// https://learnopengl.com/getting-started/shaders
void GLSLProgram::compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath) {
    // Vertex and fragment shaders are successfully compiled.
    // Now time to link them together into a program.
    // Get a program object.
    _programID = glCreateProgram();
    
    _vertexShaderID = glCreateShader(GL_VERTEX_SHADER);

    if (_vertexShaderID == 0) {
        EngineErrors::fatalError("Vertex shader failed to be created!");
    }

    _fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    if (_fragmentShaderID == 0) {
        EngineErrors::fatalError("Fragment shader failed to be created!");
    }

    compileShader(vertexShaderFilePath, _vertexShaderID);
    compileShader(fragmentShaderFilePath, _fragmentShaderID);

    
}

// Combine shaders into one program
void GLSLProgram::linkShaders() {

    // Attach our shaders to our program
    glAttachShader(_programID, _vertexShaderID);
    glAttachShader(_programID, _fragmentShaderID);

    // Link our program
    glLinkProgram(_programID);

    // Note the different functions here: glGetProgram* instead of glGetShader*.
    GLint isLinked = 0;
    glGetProgramiv(_programID, GL_LINK_STATUS, (int *)&isLinked);

    if (isLinked == GL_FALSE) {
        GLint maxLength = 0;
        glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> errorLog(maxLength);
        glGetProgramInfoLog(_programID, maxLength, &maxLength, &errorLog[0]);
        
        // We don't need the program anymore.
        glDeleteProgram(_programID);
        // Don't leak shaders either.
        glDeleteShader(_vertexShaderID);
        glDeleteShader(_fragmentShaderID);

        // Use the infoLog as you see fit.
        std::printf("%s\n", &(errorLog[0]));
        EngineErrors::fatalError("Shader failed to link!");
        
    }

    // Always detach shaders after a successful link.
    glDetachShader(_programID, _vertexShaderID);
    glDetachShader(_programID, _fragmentShaderID);
    glDeleteShader(_vertexShaderID);
    glDeleteShader(_fragmentShaderID);
}


// Binds 
void GLSLProgram::addAttribute(const std::string& attributeName) {
    glBindAttribLocation(_programID, _numAttributes, attributeName.c_str());
    _numAttributes++;
}

GLint GLSLProgram::getUniformLocation(const std::string& uniformName) {
    GLint location = glGetUniformLocation(_programID, uniformName.c_str());

    if (location == -1) {
        EngineErrors::fatalError("Uniform " + uniformName + " not found in shader!");
    }

    return location;
}

void GLSLProgram::use() {
    // Tell Open GL this is the shader program we want to use
    glUseProgram(_programID);

    // enable each vertex attribute and bind the index
    for (int i = 0; i < _numAttributes; i++) {
        glEnableVertexAttribArray(i);
    }
}

void GLSLProgram::unuse() {
    // Use no program
    glUseProgram(0);

    for (int i = 0; i < _numAttributes; i++) {
        glDisableVertexAttribArray(i);
    }
}


void GLSLProgram::compileShader(const std::string& filePath, GLuint id) {
    // Load code from files 
    // Fix: not the fastest but works
    std::ifstream vertexFile(filePath);

    if (vertexFile.fail()) {
        perror(filePath.c_str());
        EngineErrors::fatalError("Failed to open "  + filePath);
    } 

    // copy over vertex files into one file string
    std::string fileContents = "";
    std::string line;

    while(std::getline(vertexFile, line)) {
        fileContents += line + "\n";
    }

    vertexFile.close();

    const char* contentsPtr = fileContents.c_str();
    glShaderSource(id, 1, &contentsPtr, nullptr);

    glCompileShader(id);

    // Check if our compiled code worked
    GLint success = 0;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);

    if (success == GL_FALSE) {
        GLint maxLength = 0;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxlength includes the NULL character
        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);

        // Provide the infolog in whatever manor you deem best
        // exit with failure
        glDeleteShader(id);    // Don't leak shader

        std::printf("%s\n", &(errorLog[0]));
        EngineErrors::fatalError("Shader " + filePath + " failed to compile");
        return;
    }
}