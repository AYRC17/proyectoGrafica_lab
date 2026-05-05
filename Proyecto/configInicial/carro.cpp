#include "Carro.h"
#include "stb_image.h" // Incluimos la librería para leer la imagen
#include <iostream>

Carro::Carro(glm::vec3 posInicial) {
    posicion = posInicial;
    avance = 0.0f;
    rotacionLlantas = 0.0f;
    giroVolante = 0.0f;
}

void Carro::Inicializar() {
    // --- 1. Textura del Chasis ---
    glGenTextures(1, &texturaChasis);
    glBindTexture(GL_TEXTURE_2D, texturaChasis);

    unsigned char colorAzul[] = { 28, 14, 134 };
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, colorAzul);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, 0);

    // --- 2. Textura de las Llantas ---
    glGenTextures(1, &texturaLlanta);
    glBindTexture(GL_TEXTURE_2D, texturaLlanta);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char* data = stbi_load("images/llanta2.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        GLenum formato = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, formato, width, height, 0, formato, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "ERROR: Fallo al cargar la textura de la llanta desde la clase." << std::endl;
    }
    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Carro::Draw(Shader& shader, GLuint VAO) {
    GLint modelLoc = glGetUniformLocation(shader.Program, "model");
    GLint matDiffLoc = glGetUniformLocation(shader.Program, "material.diffuse");

    glBindVertexArray(VAO);

    glm::mat4 modelCarro = glm::mat4(1.0f);
    modelCarro = glm::translate(modelCarro, posicion);
    modelCarro = glm::translate(modelCarro, glm::vec3(avance, 0.0f, 0.0f));

    // --- 1. CHASIS Y CABINA ---
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texturaChasis);
    glUniform1i(matDiffLoc, 0);

    glm::mat4 modelChasis = glm::translate(modelCarro, glm::vec3(0.0f, 0.5f, 0.0f));
    modelChasis = glm::scale(modelChasis, glm::vec3(2.5f, 0.5f, 1.2f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelChasis));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glm::mat4 modelCabina = glm::translate(modelCarro, glm::vec3(-0.2f, 1.0f, 0.0f));
    modelCabina = glm::scale(modelCabina, glm::vec3(1.2f, 0.5f, 1.0f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCabina));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // --- 2. LLANTAS ---
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texturaLlanta); 
    glUniform1i(matDiffLoc, 0);

    glm::vec3 posLlantas[] = {
        glm::vec3(1.0f, 0.2f, 0.7f),   // Delantera Izquierda
        glm::vec3(1.0f, 0.2f, -0.7f),  // Delantera Derecha
        glm::vec3(-1.0f, 0.2f, 0.7f),  // Trasera Izquierda
        glm::vec3(-1.0f, 0.2f, -0.7f)  // Trasera Derecha
    };

    for (int i = 0; i < 4; i++) {
        glm::mat4 m = glm::translate(modelCarro, posLlantas[i]);
        if (i < 2) m = glm::rotate(m, glm::radians(giroVolante), glm::vec3(0.0f, 1.0f, 0.0f));

        m = glm::rotate(m, glm::radians(rotacionLlantas), glm::vec3(0.0f, 0.0f, 1.0f));
        m = glm::scale(m, glm::vec3(0.4f, 0.4f, 0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(m));
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
}