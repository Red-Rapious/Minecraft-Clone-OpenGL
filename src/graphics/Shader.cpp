#include "Shader.h"
#include "Renderer.h"
#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader(const std::string& filepath)
	: m_FilePath(filepath), m_RendererID(0)
{
    ShaderProgramSource source = ParseShader(filepath); // load shader file
    m_RendererID = CreateShader(source.VertexSource, source.FragmentSource); // compile, attach, link shaders
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_RendererID));
}



void Shader::Bind() const
{
    GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
    GLCall(glUseProgram(0));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

void Shader::SetUniform1i(const std::string& name, int value)
{
    GLCall(glUniform1i(GetUniformLocation(name), value));
}


int Shader::GetUniformLocation(const std::string& name)
{
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];

    GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1)
        std::cout << "[WARNING] Uniform " << name << " doesn't exist." << std::endl;
    m_UniformLocationCache[name] = location;
    return location;
}

ShaderProgramSource Shader::ParseShader(const std::string& filePath)
{
    /*
    Function which open a file and return a ShaderProgramSource containing the source code
    of the Fragment and Vertex shaders in this file
    */

    std::ifstream stream(filePath); // file

    enum class ShaderType // The type of shader of the present line
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2]; // the strings containing the source code ; for indexes, see ShaderType
    ShaderType type = ShaderType::NONE;

    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos) // check if the line contains our instruction to delimitate shaders
        {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX; // set mode to VERTEX
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT; // set mode to SHADER
        }
        else // otherwise, save the line into the proper stringstream
        {
            if ((int)type != -1)
                ss[(int)type] << line << "\n"; // index is determined by the current shader type
            else
                std::cout << "[Shader Error] Undefinied shader type" << std::endl;
        }
    }

    return { ss[0].str(), ss[1].str() }; // return a ShaderProgramSource containing the two stringstreams, converted to strings
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    GLCall(unsigned int id = glCreateShader(type));
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id); // compile shader

    // Error tracking
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)malloc(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader..." << std::endl;
        std::cout << message << std::endl;

        glDeleteShader(id);
        return 0;
    }

    return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    /*
    Create a shader from the two source codes
    Compile, attach, link and validate the shaders, and return the program id
    */

    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}