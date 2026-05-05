#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"

class Carro {
public:
    glm::vec3 posicion;
    float rotacionLlantas;
    float giroVolante;
    float avance;

    GLuint texturaChasis;
    GLuint texturaLlanta;

    Carro(glm::vec3 posInicial);

    void Inicializar();

    
    void Draw(Shader& shader, GLuint VAO);
};