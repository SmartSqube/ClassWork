#include "ResourceManager.h"
#include "../Renderer/ShaderProgram.h"
#include "../Renderer/Texture2D.h"
#include "../Renderer/Sprite.h"
#include "../Renderer/AnimatedSprite.h"

#include <sstream>
#include <fstream>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "stb_image.h"
using namespace std;
ResourceManager::ResourceManager(const string& executablePath)
{
    size_t found = executablePath.find_last_of("/\\");
    m_path = executablePath.substr(0, found);
}

string ResourceManager::getFileString(const string& relativeFilePath) const
{
    ifstream f;
    f.open(m_path + "/" + relativeFilePath.c_str(), ios::in | ios::binary);
    if (!f.is_open())
    {
        cerr << "Failed to open file: " << relativeFilePath << endl;
        return string{};
    }

    stringstream buffer;
    buffer << f.rdbuf();
    return buffer.str();
}

shared_ptr<Renderer::ShaderProgram> ResourceManager::loadShaders(const string& shaderName, const string& vertexPath, const string& fragmentPath)
{
    string vertexString = getFileString(vertexPath);
    if (vertexString.empty())
    {
        cerr << "No vertex shader!" << endl;
        return nullptr;
    }

    string fragmentxString = getFileString(fragmentPath);
    if (fragmentxString.empty())
    {
        cerr << "No fragment shader!" << endl;
        return nullptr;
    }

    shared_ptr<Renderer::ShaderProgram>& newShader = m_shaderPrograms.emplace(shaderName, make_shared<Renderer::ShaderProgram>(vertexString, fragmentxString)).first->second;
    if (newShader->isCompiled())
    {
        return newShader;
    }

    cerr << "Can't load shader program:\n"
        << "Vertex: " << vertexPath << "\n"
        << "Fragment: " << fragmentPath << endl;

    return nullptr;
}


shared_ptr<Renderer::ShaderProgram> ResourceManager::getShaderProgram(const string& shaderName)
{
    ShaderProgramsMap::const_iterator it = m_shaderPrograms.find(shaderName);
    if (it != m_shaderPrograms.end())
    {
        return it->second;
    }
    cerr << "Can't find the shader program: " << shaderName << endl;
    return nullptr;
}


shared_ptr<Renderer::Texture2D> ResourceManager::loadTexture(const string& textureName, const string& texturePath)
{
    int channels = 0;
    int width = 0;
    int height = 0;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* pixels = stbi_load(string(m_path + "/" + texturePath).c_str(), &width, &height, &channels, 0);

    if (!pixels)
    {
        cerr << "Can't load image: " << texturePath << endl;
        return nullptr;
    }

    shared_ptr<Renderer::Texture2D> newTexture = m_textures.emplace(textureName, make_shared<Renderer::Texture2D>(width,
                                                                                                                            height,
                                                                                                                            pixels,
                                                                                                                            channels,
                                                                                                                            GL_NEAREST,
                                                                                                                            GL_CLAMP_TO_EDGE)).first->second;
    stbi_image_free(pixels);
    return newTexture;
}

shared_ptr<Renderer::Texture2D> ResourceManager::getTexture(const string& textureName)
{
    TexturesMap::const_iterator it = m_textures.find(textureName);
    if (it != m_textures.end())
    {
        return it->second;
    }
    cerr << "Can't find the texture: " << textureName << endl;
    return nullptr;
}

shared_ptr<Renderer::Sprite> ResourceManager::loadSprite(const string& spriteName,
                                                              const string& textureName,
                                                              const string& shaderName,
                                                              const unsigned int spriteWidth,
                                                              const unsigned int spriteHeight,
                                                              const string& subTextureName)
{
    auto pTexture = getTexture(textureName);
    if (!pTexture)
    {
        cerr << "Can't find the texture: " << textureName << " for the sprite: " << spriteName << endl;
    }

    auto pShader = getShaderProgram(shaderName);
    if (!pShader)
    {
        cerr << "Can't find the shader: " << shaderName << " for the sprite: " << spriteName << endl;
    }

    shared_ptr<Renderer::Sprite> newSprite = m_sprites.emplace(textureName, make_shared<Renderer::Sprite>(pTexture,
                                                                                                                    subTextureName,
                                                                                                                    pShader,
                                                                                                                    glm::vec2(0.f, 0.f),
                                                                                                                    glm::vec2(spriteWidth, spriteHeight))).first->second;

    return newSprite;
}

shared_ptr<Renderer::AnimatedSprite> ResourceManager::loadAnimatedSprite(const string& spriteName,
                                                                              const string& textureName,
                                                                              const string& shaderName,
                                                                              const unsigned int spriteWidth,
                                                                              const unsigned int spriteHeight,
                                                                              const string& subTextureName)
{
    auto pTexture = getTexture(textureName);
    if (!pTexture)
    {
        cerr << "Can't find the texture: " << textureName << " for the sprite: " << spriteName << endl;
    }

    auto pShader = getShaderProgram(shaderName);
    if (!pShader)
    {
        cerr << "Can't find the shader: " << shaderName << " for the sprite: " << spriteName << endl;
    }

    shared_ptr<Renderer::AnimatedSprite> newSprite = m_animatedSprites.emplace(textureName, make_shared<Renderer::AnimatedSprite>(pTexture,
        subTextureName,
        pShader,
        glm::vec2(0.f, 0.f),
        glm::vec2(spriteWidth, spriteHeight))).first->second;

    return newSprite;
}

shared_ptr<Renderer::Sprite> ResourceManager::getSprite(const string& spriteName)
{
    SpritesMap::const_iterator it = m_sprites.find(spriteName);
    if (it != m_sprites.end())
    {
        return it->second;
    }
    cerr << "Can't find the sprite: " << spriteName << endl;
    return nullptr;
}

shared_ptr<Renderer::AnimatedSprite> ResourceManager::getAnimatedSprite(const string& spriteName)
{
    auto it = m_animatedSprites.find(spriteName);
    if (it != m_animatedSprites.end())
    {
        return it->second;
    }
    cerr << "Can't find animated sprite: " << spriteName << endl;
    return nullptr;
}

shared_ptr<Renderer::Texture2D> ResourceManager::loatTextureAtlas(string textureName,
                                                                       string texturePath,
                                                                       vector<string> subTextures,
                                                                       const unsigned int subTextureWidth,
                                                                       const unsigned int subTextureHeight)
{
    auto pTexture = loadTexture(move(textureName), move(texturePath));
    if (pTexture)
    {
        const unsigned int textureWidth = pTexture->width();
        const unsigned int textureHeight = pTexture->height();
        unsigned int currentTextureOffsetX = 0;
        unsigned int currentTextureOffsetY = textureHeight;
        for (auto& currentSubTextureName : subTextures)
        {
            glm::vec2 leftBottomUV(static_cast<float>(currentTextureOffsetX) / textureWidth,                 static_cast<float>(currentTextureOffsetY - subTextureHeight) / textureHeight);
            glm::vec2 rightTopUV(static_cast<float>(currentTextureOffsetX + subTextureWidth) / textureWidth, static_cast<float>(currentTextureOffsetY) / textureHeight);
            pTexture->addSubTexture(move(currentSubTextureName), leftBottomUV, rightTopUV);

            currentTextureOffsetX += subTextureWidth;
            if (currentTextureOffsetX >= textureWidth)
            {
                currentTextureOffsetX = 0;
                currentTextureOffsetY -= subTextureHeight;
            }
        }
    }
    return pTexture;
}