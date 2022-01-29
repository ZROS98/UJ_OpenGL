//
// Created by pbialas on 25.09.2020.
//

#include "app.h"

#include <iostream>
#include <vector>
#include <tuple>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>

#include "Application/utils.h"

void SimpleShapeApplication::init() {
    // A utility function that reads the shader sources, compiles them and creates the program object
    // As everything in OpenGL we reference program by an integer "handle".
    auto program = xe::utils::create_program(
            {{GL_VERTEX_SHADER,   std::string(PROJECT_DIR) + "/shaders/base_vs.glsl"},
             {GL_FRAGMENT_SHADER, std::string(PROJECT_DIR) + "/shaders/base_fs.glsl"}});

    if (!program) {
        std::cerr << "Invalid program" << std::endl;
        exit(-1);
    }

    std::vector<GLfloat> vertices = {
            -0.5f, 0.0f, -0.5f, 1.0f, 0.0f, 0.0f,   //0
            -0.5f, 0.0f, 0.5f, 1.0f, 0.0f, 0.0f,    //1
            0.5f, 0.0f, -0.5f, 1.0f, 0.0f, 0.0f,    //2
            0.5f, 0.0f, 0.5f, 1.0f, 0.0f, 0.0f,     //3

            -0.5f, 0.f, -0.5f, 0.0f, 1.0f, 0.0f,    //4
            0.f, 1.f, 0.f, 0.0f, 1.0f, 0.0f,        //5
            0.5f, 0.f, -0.5f, 0.0f, 1.0f, 0.0f,     //6

            0.5f, 0.f, -0.5f, 0.0f, 0.0f, 1.0f,     //7
            0.f, 1.f, 0.f, 0.0f, 0.0f, 1.0f,        //8
            0.5f, 0.f, 0.5f, 0.0f, 0.0f, 1.0f,      //9

            -0.5f, 0.f, 0.5f, 1.0f, 1.0f, 0.0f,     //10
            0.f, 1.f, 0.f, 1.0f, 1.0f, 0.0f,        //11
            0.5f, 0.f, 0.5f, 1.0f, 1.0f, 0.0f,      //12

            -0.5f, 0.f, 0.5f, 1.0f, 0.0f, 1.0f,     //13
            0.f, 1.f, 0.f, 1.0f, 0.0f, 1.0f,        //14
            -0.5f, 0.f, -0.5f, 1.0f, 0.0f, 1.0f,    //15
    };

    indices = {
            0, 2, 1,
            1, 2, 3,
            4, 5, 6,
            7, 8, 9,
            11, 10, 12,
            13, 14, 15,
    };

    float strength = 1.0;
    std::vector<GLfloat> color = {0.9, 0.8, 0.7};

    glm::mat4 viewMat = glm::lookAt(
            glm::vec3(-2.0f, -1.f, -5.0f), 
            glm::vec3(0.0f, 0.0f, 0.0f), 
            glm::vec3(0.0f, 1.0f, 0.0f) 
    );
    auto[w, h] = frame_buffer_size();
    glm::mat4 perMat = glm::perspective(glm::radians(45.0f), GLfloat(w) / GLfloat(h), 0.1f, 100.0f);
    glm::mat4 modelMat = glm::mat4(1.0f);
    modelMat = glm::rotate(modelMat, glm::radians(115.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 pvm = perMat * viewMat * modelMat;

    GLuint indicesBuffer = generateIndicesBuffer(indices);
    GLuint verticesBuffer = generateVertexBuffer(vertices);
    GLuint uniformBuffer = generateUniformBuffer(strength, color);
    GLuint transformationBuffer = generateTransformationBuffer(pvm);

    // This setups a Vertex Array Object (VAO) that  encapsulates
    // the state of all vertex buffers needed for rendering
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
    bindDataToUniformBuffer(uniformBuffer, transformationBuffer);
    setDataToVertexBuffer(verticesBuffer);
    setDataToIndicesBuffer(indicesBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    //end of vao "recording"

    // Setting the background color of the rendering window,
    // I suggest not to use white or black for better debuging.
    glClearColor(0.81f, 0.81f, 0.8f, 1.0f);

    glViewport(0, 0, w, h);

    glUseProgram(program);
}

unsigned int SimpleShapeApplication::generateVertexBuffer(std::vector<GLfloat> vertices) {
    GLuint v_buffer_handle;
    glGenBuffers(1, &v_buffer_handle);
    OGL_CALL(glBindBuffer(GL_ARRAY_BUFFER, v_buffer_handle));
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    return v_buffer_handle;
}

unsigned int SimpleShapeApplication::generateIndicesBuffer(std::vector<GLushort> indices) {
    GLuint indicesBuffer;
    glGenBuffers(1, &indicesBuffer);
    OGL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBuffer));
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), indices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    return indicesBuffer;
}

unsigned int SimpleShapeApplication::generateUniformBuffer(float strength, std::vector<GLfloat> color) {
    GLuint uniformBuffer;
    glGenBuffers(1, &uniformBuffer);
    OGL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, uniformBuffer));
    glBufferData(GL_UNIFORM_BUFFER, 8 * sizeof(float), nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(float), & strength);
    glBufferSubData(GL_UNIFORM_BUFFER, 4 * sizeof(float), 3 * sizeof(float), color.data());
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    return uniformBuffer;
}

unsigned int SimpleShapeApplication::generateTransformationBuffer(glm::mat4 pvm) {
    GLuint transformationBuffer;
    glGenBuffers(1, &transformationBuffer);
    OGL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, transformationBuffer));
    glBufferData(GL_UNIFORM_BUFFER, 16 * sizeof(float), nullptr, GL_STATIC_DRAW);

    GLintptr index = 0;
    glBufferSubData(GL_UNIFORM_BUFFER, index, 4 * sizeof(float), &pvm[0]);
    index += 4 * sizeof(float);
    glBufferSubData(GL_UNIFORM_BUFFER, index, 4 * sizeof(float), &pvm[1]);
    index += 4 * sizeof(float);
    glBufferSubData(GL_UNIFORM_BUFFER, index, 4 * sizeof(float), &pvm[2]);
    index += 4 * sizeof(float);
    glBufferSubData(GL_UNIFORM_BUFFER, index, 4 * sizeof(float), &pvm[3]);

    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    return transformationBuffer;
}

void SimpleShapeApplication::setDataToVertexBuffer(GLuint verticesBuffer) {
    glBindBuffer(GL_ARRAY_BUFFER, verticesBuffer);
    setVerticesData();
    setColorToVertices();
}

void SimpleShapeApplication::setVerticesData() {
    int arrayId = 0;
    int dataInArrayOffset = 0;
    // This indicates that the data for attribute 0 should be read from a vertex buffer.
    glEnableVertexAttribArray(arrayId);
    // and this specifies how the data is layout in the buffer.
    glVertexAttribPointer(arrayId, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(dataInArrayOffset));
}

void SimpleShapeApplication::setColorToVertices() {
    int arrayId = 1;
    int dataInArrayOffset = 3 * sizeof(GLfloat);

    glEnableVertexAttribArray(arrayId);
    glVertexAttribPointer(arrayId, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(dataInArrayOffset));
}

void SimpleShapeApplication::setDataToIndicesBuffer(GLuint indicesBuffer) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBuffer);
}

void SimpleShapeApplication::bindDataToUniformBuffer(GLuint uniformBuffer, GLuint transformationBuffer) {
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, uniformBuffer);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, transformationBuffer);
}

//This functions is called every frame and does the actual rendering.
void SimpleShapeApplication::frame() {
    // Binding the VAO will setup all the required vertex buffers.
    glBindVertexArray(vao_);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, reinterpret_cast<GLvoid *>(0));
    glBindVertexArray(0);
}