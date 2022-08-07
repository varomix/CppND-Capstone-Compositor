//
// Created by varomix on 8/6/22.
//

#ifndef CPPNG_MIXTUREPROTOTYPE__SHADER_H_
#define CPPNG_MIXTUREPROTOTYPE__SHADER_H_

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
 public:
    unsigned int ID;
    Shader(const char *vertex_path, const char *fragment_path) {
        std::string vertex_code;
        std::string fragment_code;
        std::ifstream vert_shader_file;
        std::ifstream frag_shader_file;

        vert_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        frag_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try {
            // open shader files
            vert_shader_file.open(vertex_path);
            frag_shader_file.open(fragment_path);

            std::stringstream vert_shader_stream, frag_shader_stream;

            // read file's buffer content into streams
            vert_shader_stream << vert_shader_file.rdbuf();
            frag_shader_stream << frag_shader_file.rdbuf();

            // close file handlers
            vert_shader_file.close();
            frag_shader_file.close();

            // convert stream into string
            vertex_code = vert_shader_stream.str();
            fragment_code = frag_shader_stream.str();
        }
        catch (std::ifstream::failure &e) {
            std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << '\n';
        }

        const char *vert_shader_code = vertex_code.c_str();
        const char *frag_shader_code = fragment_code.c_str();

        // compile shaders
        unsigned int vertex, fragment;

        //vertex shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vert_shader_code, nullptr);
        glCompileShader(vertex);
        check_compile_errors(vertex, "VERTEX");

        // fragment shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &frag_shader_code, nullptr);
        glCompileShader(fragment);
        check_compile_errors(fragment, "FRAGMENT");

        // shader program
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        check_compile_errors(ID, "PROGRAM");

        // delete shaders
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    // activate the shader
    void use() {
        glUseProgram(ID);
    }

    // utility uniform functions
    void set_bool(const std::string &name, bool value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }
    void set_int(const std::string &name, int value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
    void set_float(const std::string &name, float value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }

 private:
    // utility function for checking shader compilation/linking errors.
    void check_compile_errors(unsigned int shader, std::string type) {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM") {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
                std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << '\n' << infoLog
                          << "\n -- --------------------------------------------------- -- \n";
            }
        } else {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
                std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << '\n' << infoLog
                          << "\n -- --------------------------------------------------- -- \n";
            }
        }
    }
};

#endif //CPPNG_MIXTUREPROTOTYPE__SHADER_H_
