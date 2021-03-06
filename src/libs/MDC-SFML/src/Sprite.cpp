#include "Sprite.h"

using std::string;
using std::to_string;

namespace Bokoblin
{
namespace MaterialDesignComponentsForSFML
{

//------------------------------------------------
//          CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------

/**
 * Constructs a Sprite with a size
 *
 * @param width the width
 * @param height the height
 * @param image the image used for the texture (optional)
 *
 * @author Arthur
 * @date 21/02/16 - 13/09/18
 */
Sprite::Sprite(float width, float height, const string& image) :
        m_width{width}, m_height{height}, m_isVisible{true},
        m_alpha{255}, m_light{100}, m_textureFilename{image}, m_initialColor{getColor()}
{
    if (!image.empty()) {
        Sprite::loadAndApplyTextureFromImageFile(image);
    }
}


/**
 * Constructs a Sprite with
 * a position, a size and an image
 *
 * @param x the x-axis coordinate
 * @param y the y-axis coordinate
 * @param width the width
 * @param height the height
 * @param image the image used for the texture (optional)
 *
 * @author Arthur, Florian
 * @date 21/02/2016 - 03/05/2020
 */
Sprite::Sprite(float x, float y, float width, float height, const string& image) :
        Sprite(width, height, image)
{
    this->setPosition(x, y);
}


/**
 * Constructs a Sprite with
 * a position, an equal size and an image
 *
 * @param x the x-axis coordinate
 * @param y the y-axis coordinate
 * @param size the size (same width and height)
 * @param image the image used for the texture (optional)
 *
 * @author Arthur
 * @date 11/09/18 - 03/05/2020
 */
Sprite::Sprite(float x, float y, float size, const string& image) :
        Sprite(x, y, size, size, image) {}


/**
 * Constructs a Sprite with a size and image
 *
 * @param size the size (same width and height)
 * @param image the image used for the texture (optional)
 *
 * @author Arthur
 * @date 11/09/18
 */
Sprite::Sprite(float size, const std::string &image) :
        Sprite(size, size, image) {}


/**
 * Copy Constructor
 *
 * @param other another button object to copy
 *
 * @author Arthur, Florian
 * @date 25/02/16 - 16/09/18
 */
Sprite::Sprite(Sprite const& other) :
        sf::Sprite(other), m_width(other.m_width), m_height(other.m_height),
        m_isVisible{other.m_isVisible}, m_alpha{other.m_alpha}, m_light{other.m_light},
        m_textureFilename{other.m_textureFilename}, m_initialColor{other.m_initialColor}
{
    this->setPosition(other.getPosition());
    this->setTexture(*other.getTexture(), true);
    this->setOrigin(other.getOrigin());
}


/**
 * Destructor
 * @author Arthur, Florian
 * @date 21/02/16
 */
Sprite::~Sprite() = default;


//------------------------------------------------
//          GETTERS
//------------------------------------------------

float Sprite::getX() const { return getPosition().x; }

float Sprite::getY() const { return getPosition().y; }

float Sprite::getWidth() const { return m_width; }

float Sprite::getHeight() const { return m_height; }

bool Sprite::isVisible() const { return m_isVisible; }

int Sprite::getAlpha() const { return m_alpha; }

int Sprite::getLight() const { return m_light; }

string Sprite::getTextureFileName() const { return m_textureFilename; }


//------------------------------------------------
//          SETTERS
//------------------------------------------------

void Sprite::setVisible(bool visible) { m_isVisible = visible; }

void Sprite::show() { setVisible(true); }

void Sprite::hide() { setVisible(false); }

void Sprite::setAlpha(int alpha) { m_alpha = static_cast<sf::Uint8>((alpha < 0) ? 0 : (alpha > 255) ? 255 : alpha); }

void Sprite::setLight(int light) { m_light = static_cast<sf::Uint8>((light < 0) ? 0 : (light > 100) ? 100 : light); }

void Sprite::increaseAlpha(int alphaLevel) { setAlpha(getAlpha() + alphaLevel); }

void Sprite::decreaseAlpha(int alphaLevel) { setAlpha(getAlpha() - alphaLevel); }

void Sprite::increaseLight(int lightLevel) { setLight(getLight() + lightLevel); }

void Sprite::decreaseLight(int lightLevel) { setLight(getLight() - lightLevel); }

void Sprite::setFillColor(const sf::Color& color)
{
    m_initialColor = color;
    sf::Sprite::setColor(color);
}


//------------------------------------------------
//          METHODS
//------------------------------------------------

/**
 * Synchronizes sprite by applying color modifiers
 *
 * @author Arthur
 * @date 03/04/16 - 24/12/17
 */
void Sprite::sync()
{
    applyColor();
}


/**
 * Draws the sprite if visible
 *
 * @param window the app's window
 *
 * @author Arthur
 * @date 30/03/16
 */
void Sprite::draw(sf::RenderWindow* window) const
{
    if (isVisible()) {
        window->draw(*this);
    }
}


/**
 * Resizes the sprite with scaling
 *
 * @param width the new width
 * @param height the new height
 *
 * @author Arthur
 * @date 22/02/16 - 25/02/16
 */
void Sprite::resize(float width, float height)
{
    sf::FloatRect bb = this->getLocalBounds();
    float width_factor = width / bb.width;
    float height_factor = height / bb.height;
    this->setScale(width_factor, height_factor);
    this->m_width = width;
    this->m_height = height;
}


/**
 * Equally resizes the sprite with scaling
 *
 * @param size the new width and height
 *
 * @author Arthur
 * @date 11/09/18
 */
void Sprite::resize(float size)
{
    this->resize(size, size);
}


/**
 * Checks if a point of given coordinates is contained
 * inside this element
 *
 * @param x the x-axis of position to check
 * @param y the y-axis of position to check
 *
 * @author Arthur
 * @date 24/01/16
 */
bool Sprite::contains(float x, float y) const
{
    return isVisible() && getGlobalBounds().contains(sf::Vector2f(x, y));
}


/**
 * Applies light and alpha values to color
 *
 * @author Arthur
 * @date 24/12/17 - 13/09/18
 */
void Sprite::applyColor()
{
    setFillColor(sf::Color(static_cast<sf::Uint8>(m_initialColor.r * 0.01f * m_light),
                           static_cast<sf::Uint8>(m_initialColor.g * 0.01f * m_light),
                           static_cast<sf::Uint8>(m_initialColor.b * 0.01f * m_light),
                           m_alpha));
}


/**
 * Loads a texture from an image file
 * and applies it to the sprite on loading success
 *
 * @param imageFile the source file
 *
 * @author Arthur
 * @date 02/01/17 - 04/01/18
 */
void Sprite::loadAndApplyTextureFromImageFile(const std::string& imageFile)
{
    if (m_texture.loadFromFile(imageFile)) {
        processTextureLoading(imageFile);
    } else {
        Config::printError("Image loading failed for \"" + imageFile + "\"");
    }
}


/**
 * Loads a texture from an image file
 * and applies it to the sprite on loading success
 *
 * @param imageFile the source file
 * @param area the image area to use
 *
 * @author Arthur
 * @date 02/01/17 - 13/01/19
 */
void Sprite::loadAndApplyTextureFromImageFile(const string& imageFile, const sf::IntRect& area)
{
    if (m_texture.loadFromFile(imageFile, area)) {
        processTextureLoading(imageFile);
    } else {
        Config::printError("Image loading failed for \"" + imageFile + "\" and the area ("
                + to_string(area.left) + ", " + to_string(area.top) + ", "
                + to_string(area.width) + ", " + to_string(area.height) + ")");
    }
}

/**
 * Applies the texture to the sprite after loading success
 *
 * @param imageFile the source file
 * @param area the image area to use
 *
 * @author Arthur
 * @date 04/01/18
 */
void Sprite::processTextureLoading(const string& imageFile)
{
    m_texture.setSmooth(true);
    setTexture(m_texture);
    m_textureFilename = imageFile;
}

} //namespace MaterialDesignComponentsForSFML
} //namespace Bokoblin
