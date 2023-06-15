//
// Created by hgallegos on 5/10/2022.
//

#ifndef LEARNOPENGL_OBJETO_H
#define LEARNOPENGL_OBJETO_H
#include "include/glad.h"
#include "include/glm/glm.hpp"
#include "include/glm/gtx/string_cast.hpp"
#include "include/glm/gtc/matrix_transform.hpp"
#include "include/glm/gtc/type_ptr.hpp"
#include <vector>
#include "shader_m.h"
using namespace std;
using namespace glm;

class Objeto {
public:
    vector<vec3> positions;
    vector<vec3> normals;
    vector<vec2> textureCoords;
    vector<GLuint> indices;
    GLuint indices_size;
    float v0, x0, y0, a0, xt, yt;
    GLuint vao;
    mat4 model;
    bool visible=true;
    GLint POSITION_ATTRIBUTE=0, NORMAL_ATTRIBUTE=1, TEXCOORD0_ATTRIBUTE=8;
    virtual GLuint setup()=0;
    virtual void display(Shader &sh)=0;
    //Svirtual void actualizarPosicion(float tiempo)=0;
    virtual glm::vec3 getColor() const { return glm::vec3(1.0f); }
    virtual bool collidesWith(const Objeto* other) const {
        // Al inicio no hay colisión entre objetos
        return false;
    }
};

class Esfera : public Objeto {
private:
    glm::vec3 color; // Color de la esfera
public:
    vec3 centro;
    float radius;
    int slices, stacks;
    //float tiempo_inicial;


    Esfera() {
        centro = vec3(0.0);
    }
    Esfera(vec3 _centro) {
        centro = _centro;
    }
    Esfera(vec3 _centro, float _radius, int _slices, int _stacks, glm::vec3 _color) {
        centro = _centro;
        radius = _radius;
        slices = _slices;
        stacks = _stacks;
        color = _color;
    }

    GLuint setup();
    void display(Shader& sh) override;
    void actualizarPosicion(float tiempo);

    void setPosition(const vec3& position) {
        centro = position;
    }
    glm::vec3 getColor() const override {
        return color;
    }

    bool collidesWith(const Objeto* other) const override {
        // Verificar si tmb es una esfera
        const Esfera* otherEsfera = dynamic_cast<const Esfera*>(other);
        if (otherEsfera) {
            // Colision de esferas
            float distance = glm::distance(centro, otherEsfera->centro);
            float sumOfRadii = radius + otherEsfera->radius;
            return distance <= sumOfRadii;
        }

        //No hay colision
        return false;
    }
};

#endif //LEARNOPENGL_OBJETO_H
