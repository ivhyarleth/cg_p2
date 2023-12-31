#include "include/glad.h"
#include <GLFW/glfw3.h>
#include "include/glm/glm.hpp"
#include "include/glm/gtc/matrix_transform.hpp"

#include "shader_m.h"
#include "camera.h"
#include "Objeto.h"
#include "glut_ply.h"

#include <iostream>
#include <vector>
#include <cmath>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//Camara
Camera camera(glm::vec3(0.0f, 0.0f, 40.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

//Tiempo
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float tiempoInicial = 0.0f, tiempoTranscurrido = 0.0f;

//Luces
glm::vec3 lightPos(1.2f, 30.0f, 2.0f);

//Esferas:
Esfera esfera1(glm::vec3(0.0f, 0.0f, 0.0f), 1.0f, 10, 10, glm::vec3(1.0f, 0.5f, 0.31f));
Esfera esfera2(glm::vec3(0.0f, 0.0f, 0.0f), 2.0f, 20, 20, glm::vec3(0.5f, 0.5f, 1.0f));
Esfera esfera3(glm::vec3(0.0f, 0.0f, 0.0f), 4.0f, 30, 30, glm::vec3(0.2f, 0.8f, 0.3f));

Model_PLY modelo;
std::vector<Objeto*> objetos;
bool boton_presionado = false;

//bool checkSphereCollision(const glm::vec3& center1, float radius1, const glm::vec3& center2, float radius2) {
//    float distance = glm::length(center1 - center2);
//    return distance < (radius1 + radius2);
//}

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

int main() {
    char* archivo = "../models/bunny.ply";
    modelo.Load(archivo);
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glEnable(GL_DEPTH_TEST);
    Shader lightingShader("../2.2.basic_lighting.vs", "../2.2.basic_lighting.fs");

    esfera1.setup();
    esfera2.setup();
    esfera3.setup();
    modelo.setup();

    objetos.emplace_back(&esfera1);
    objetos.emplace_back(&esfera2);
    objetos.emplace_back(&esfera3);

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        tiempoTranscurrido = currentFrame - tiempoInicial;
        // input
        processInput(window);

        // render
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        lightingShader.use();
        lightingShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f); //Color esferaa
        lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        lightingShader.setVec3("lightPos", lightPos);
        lightingShader.setVec3("viewPos", camera.Position);

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);

        glm::mat4 model = glm::mat4(1.0f);
        lightingShader.setMat4("model", model);

        if (boton_presionado) {
            //float radius = 20.0f; // Radio del movimiento circular
            float radius1 = 10.0f; //Radio de movimiento para la esfera 1
            float radius2 = 20.0f; //adio de movimiento para la esfera 2
            float radius3 = 30.0f; //Radio de movimiento para la esfera 3
            float speed = 2.0f; // Velocidad de rotación en la orbita
            glm::vec3 center(0.0f, 0.0f, 0.0f); //Pos del objeto conejo

            float x1 = center.x + radius1 * std::sin(glfwGetTime() * speed);
            float y1 = center.y + radius1 * std::cos(glfwGetTime() * speed);
            float z1 = center.z;

            esfera1.setPosition(glm::vec3(x1, y1, z1));

            float x2 = center.x + radius2 * std::sin(glfwGetTime() * speed + 2.0f * static_cast<float>(M_PI) / 3.0f);
            float y2 = center.y + radius2 * std::cos(glfwGetTime() * speed + 2.0f * static_cast<float>(M_PI) / 3.0f);
            float z2 = center.z;

            esfera2.setPosition(glm::vec3(x2, y2, z2));

            float x3 = center.x + radius3 * std::sin(glfwGetTime() * speed + 4.0f * static_cast<float>(M_PI) / 3.0f);
            float y3 = center.y + radius3 * std::cos(glfwGetTime() * speed + 4.0f * static_cast<float>(M_PI) / 3.0f);
            float z3 = center.z;

            esfera3.setPosition(glm::vec3(x3, y3, z3));
        }

        for (size_t i = 0; i < objetos.size(); i++) {
            for (size_t j = i + 1; j < objetos.size(); j++) {
                if (objetos[i]->collidesWith(objetos[j])) {
                    // Colision entre objetos
                    std::cout << "Colision detectada entre objetos: " << i << " and " << j << std::endl;
                }
            }
        }
        for (auto& obj : objetos) {
            lightingShader.setVec3("objectColor", obj->getColor());
            obj->display(lightingShader);
        }
        modelo.display(lightingShader);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    for (auto& obj : objetos) {
        delete obj;
    }
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        boton_presionado = true;
    }

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_RELEASE) {
        if (boton_presionado) {
            boton_presionado = false;
            tiempoInicial = static_cast<float>(glfwGetTime());
        }
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(yoffset);
}
