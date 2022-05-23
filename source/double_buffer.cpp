#include "./double_buffer.h"

#include <vector>

const std::string get_fragment();

bool DoubleBuffer::firstTextureIsRenderTarget() {
    return &this->texture1 == this->activeRenderTexture;
}

DoubleBuffer::DoubleBuffer(const uint8_t width, const uint8_t height) {
    sf::View view;
    
    view.reset(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
    view.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));

    this->textureHolder.setPosition(0.f, 0.f);
    this->textureHolder.setSize(sf::Vector2f(1.f, 1.f));

    this->texture1.create(width, height);
    this->texture2.create(width, height);

    this->texture1.setView(view);
    this->texture2.setView(view);

    this->texture1.setSmooth(false);
    this->texture2.setSmooth(false);

    this->activeRenderTexture = &this->texture1;
    this->activeStateTexture  = &this->texture2; 
    
    this->shader.loadFromMemory(get_fragment(), sf::Shader::Type::Fragment);
}

void DoubleBuffer::swap() {
    if (firstTextureIsRenderTarget()) {
        this->activeRenderTexture = &this->texture2;
        this->activeStateTexture  = &this->texture1;
    } else {
        this->activeRenderTexture = &this->texture1;
        this->activeStateTexture  = &this->texture2; 
    } 
}

void DoubleBuffer::seed(const bool* initialState) {
    sf::Image   image;
    sf::Texture texture;

    auto size = this->activeStateTexture->getSize();
    
    image.create(size.x, size.y, sf::Color::White);

    for (auto i = 0u; i < size.x; i++) {
        for (auto j = 0u; j < size.y; j++) {
            if (initialState[i + j * size.x]) {
                image.setPixel(i, j, sf::Color::Black);
            }
        }        
    }

    texture.loadFromImage(image);

    this->textureHolder.setTexture(&texture);

    this->activeStateTexture->clear(sf::Color::White);

    this->activeStateTexture->draw(this->textureHolder);

    this->activeStateTexture->display();

    this->textureHolder.setTexture(nullptr);
}

void DoubleBuffer::configureTargetView(sf::RenderTarget& target) {
    sf::View view;

    view.reset(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
    view.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));

    target.setView(view);
}

void DoubleBuffer::update() {
    this->activeRenderTexture->clear(sf::Color::White);

    this->textureHolder.setTexture(&this->activeStateTexture->getTexture());

    auto integerSize = this->activeRenderTexture->getSize();

    this->shader.setUniform("size",  sf::Glsl::Ivec2(integerSize.x, integerSize.y));
    this->shader.setUniform("state", sf::Shader::CurrentTexture);

    sf::Shader::bind(&this->shader);

    this->activeRenderTexture->draw(this->textureHolder);

    sf::Shader::bind(nullptr);

    this->activeRenderTexture->display();
}

void DoubleBuffer::draw(sf::RenderTarget& target) {
    this->textureHolder.setTexture(&this->activeRenderTexture->getTexture());

    target.draw(this->textureHolder);
}
