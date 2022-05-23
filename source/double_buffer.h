#pragma once

#include <SFML/Graphics.hpp>

class DoubleBuffer {
    private:
        sf::Shader         shader;
        sf::RenderTexture  texture1;
        sf::RenderTexture  texture2;
        sf::RectangleShape textureHolder;

        sf::RenderTexture* activeStateTexture;
        sf::RenderTexture* activeRenderTexture;

        bool               firstTextureIsRenderTarget();
    
    public:
                           DoubleBuffer(const uint8_t width, const uint8_t height);

        void               swap();
        void               update();
        void               seed(const bool* initialState);
        void               draw(sf::RenderTarget& target);
        void               configureTargetView(sf::RenderTarget& target);
};
