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
    
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    int width, height, nrChannels;
    //Textura del Chasis
    glGenTextures(1, &texturaChasis);
    glBindTexture(GL_TEXTURE_2D, texturaChasis);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    unsigned char* dataChasis = stbi_load("images/cd7.jpg", &width, &height, &nrChannels, 0);
    if (dataChasis) {
        GLenum formato = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, formato, width, height, 0, formato, GL_UNSIGNED_BYTE, dataChasis);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "ERROR: Fallo al cargar textura del chasis." << std::endl;
    }
    stbi_image_free(dataChasis);
   

    //Textura de la Cabina (Cristal con transparencia) 
    glGenTextures(1, &texturaCabina);
    glBindTexture(GL_TEXTURE_2D, texturaCabina);

    // Arreglo RGBA: Azul claro con canal Alfa en 100 (parcialmente transparente)
    unsigned char colorCabina[] = { 150, 200, 255, 100 };
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, colorCabina);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    

    //Textura de las Llantas 
    glGenTextures(1, &texturaLlanta);
    glBindTexture(GL_TEXTURE_2D, texturaLlanta);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    unsigned char* dataLlanta = stbi_load("images/llanta1.jpg", &width, &height, &nrChannels, 0);
    if (dataLlanta)
    {
        GLenum formato = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, formato, width, height, 0, formato, GL_UNSIGNED_BYTE, dataLlanta);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "ERROR: Fallo al cargar la textura de la llanta desde la clase." << std::endl;
    }
    stbi_image_free(dataLlanta);
    
	//llanta lateral (rin)
    glGenTextures(1, &texturaLlantaLateral);
    glBindTexture(GL_TEXTURE_2D, texturaLlantaLateral);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    unsigned char* dataRin = stbi_load("images/rin2.png", &width, &height, &nrChannels, 0);
    if (dataRin)
    {
        GLenum formato = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, formato, width, height, 0, formato, GL_UNSIGNED_BYTE, dataRin);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "ERROR: Fallo al cargar la textura de la llanta desde la clase." << std::endl;
    }
    stbi_image_free(dataRin);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Carro::Draw(Shader& shader, GLuint VAO) {
    GLint modelLoc = glGetUniformLocation(shader.Program, "model");
    GLint matDiffLoc = glGetUniformLocation(shader.Program, "material.diffuse");

    glBindVertexArray(VAO);

    glm::mat4 modelCarro = glm::mat4(1.0f);
    modelCarro = glm::translate(modelCarro, posicion);
    modelCarro = glm::translate(modelCarro, glm::vec3(avance, 0.0f, 0.0f));

    glDisable(GL_BLEND);

    // DIBUJAR CHASIS
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texturaChasis);
    glUniform1i(matDiffLoc, 0);

    glm::mat4 modelChasis = glm::translate(modelCarro, glm::vec3(0.0f, 0.5f, 0.0f));
    modelChasis = glm::scale(modelChasis, glm::vec3(2.5f, 0.5f, 1.2f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelChasis));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // DIBUJAR LLANTAS
    glm::vec3 posLlantas[] = {
        glm::vec3(1.0f, 0.2f, 0.7f),   // DI
        glm::vec3(1.0f, 0.2f, -0.7f),  // DD
        glm::vec3(-1.0f, 0.2f, 0.7f),  // TI
        glm::vec3(-1.0f, 0.2f, -0.7f)  // TD
    };

    for (int i = 0; i < 4; i++) {
        glm::mat4 m = glm::translate(modelCarro, posLlantas[i]);
        if (i < 2) m = glm::rotate(m, glm::radians(giroVolante), glm::vec3(0.0f, 1.0f, 0.0f));

        m = glm::rotate(m, glm::radians(rotacionLlantas), glm::vec3(0.0f, 0.0f, 1.0f));
        m = glm::scale(m, glm::vec3(0.4f, 0.4f, 0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(m));

        //DIBUJAMOS LOS RINES 
        glBindTexture(GL_TEXTURE_2D, texturaLlantaLateral);
        // Dibujamos los primeros 12 vértices
        glDrawArrays(GL_TRIANGLES, 0, 12);

        //DIBUJAMOS LA BANDA DE RODAMIENTO
        glBindTexture(GL_TEXTURE_2D, texturaLlanta);
        // Dibujamos los 24 vértices restantes
        glDrawArrays(GL_TRIANGLES, 12, 24);
    }
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // DIBUJAR CABINA
    glBindTexture(GL_TEXTURE_2D, texturaCabina);
    glm::mat4 modelCabina = glm::translate(modelCarro, glm::vec3(-0.2f, 1.0f, 0.0f));
    modelCabina = glm::scale(modelCabina, glm::vec3(1.2f, 0.5f, 1.0f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCabina));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Limpieza
    glDisable(GL_BLEND);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
}