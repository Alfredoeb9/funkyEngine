#pragma once

#include <string>
#include <GL/glew.h>

namespace FunkyEngine {
    // Open GL Shading Language (needed for openGL to run graphics on the GPU)
    // USED TO CREATE VERTEX SHADER AND FRAGMENT SHADER
    class GLSLProgram {
        public: 
            GLSLProgram();
            ~GLSLProgram();

            // read from file and compile 
            void compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);
            void linkShaders();
            void addAttribute(const std::string& attributeName);
            void use();
            void unuse();

            GLint getUniformLocation(const std::string& uniformName);

        private:
            int _numAttributes;
            GLuint _programID;

            GLuint _vertexShaderID;
            GLuint _fragmentShaderID;

            void compileShader(const std::string& filePath, GLuint id);


    };
}
