#pragma once

#include <glad/glad.h>
#include <glm/vec2.hpp>

#include <memory>
using namespace std;
using namespace glm;

namespace Renderer {

    class Texture2D;
    class ShaderProgram;

    class Sprite {
	public:
        Sprite(const shared_ptr<Texture2D> pTexture,
               const shared_ptr<ShaderProgram> pShaderProgram,
               const vec2& position = vec2(0.f),
               const vec2& size = vec2(1.f),
               const float rotation = 0.f);

        ~Sprite();

        Sprite(const Sprite&) = delete;
        Sprite& operator=(const Sprite&) = delete;

        void render() const;
        void setPosition(const vec2& position);
        void setSize(const vec2& size);
        void setRotation(const float rotation);

    private:
        shared_ptr<Texture2D> m_pTexture;
        shared_ptr<ShaderProgram> m_pShaderProgram;
        vec2 m_position;
        vec2 m_size;
        float m_rotation;

};

}