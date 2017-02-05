#include "SlidingBackground.h"

/**
 * Parameterized Constructor
 * @author Arthur, Florian
 * @date 3/03/16 - 02/01/17
 */
SlidingBackground::SlidingBackground(float w, float h, float speed, std::string image) :
    GraphicElement(w, h), m_speed{speed}, m_alpha{255}
{
    m_left = new GraphicElement(0, 0, w, h);
    m_left->setTextureFromImage(image);
    m_right = new GraphicElement(w, 0, w, h);
    m_right->setTextureFromImage(image);
}


/**
 * Destructor
 * @author Arthur
 * @date 5/03/16 - 15/04/16
 */
SlidingBackground::~SlidingBackground()
{
    delete m_left;
    delete m_right;
}


//=== Getters

int SlidingBackground::getAlpha() const { return m_alpha; }
sf::Vector2f SlidingBackground::getPosition() const
{
    return m_left->getPosition();
}

float SlidingBackground::getSeparationPositionX() const
{
    if ( m_left->getPosition().x >= -1200 && m_left->getPosition().x <= 900) //left is on screen
        return m_left->getPosition().x + m_left->getLocalBounds().width;
    else
        return m_right->getPosition().x + m_right->getLocalBounds().width;
}


//=== Setters

void SlidingBackground::setSpeed(float speed) { m_speed = speed; }
void SlidingBackground::setAlpha(sf::Uint8 alpha) { m_alpha = alpha; }
void SlidingBackground::setPosition(float x, float y)
{
    m_left->setPosition(x, y);
    m_right->setPosition(x + m_width, y);
}


/**
 * Synchronization Function
 * @author Arthur
 * @date 3/03/16 - 7/05/16
 */
void SlidingBackground::sync()
{
    m_left->setPosition(m_left->getPosition().x - m_speed, m_left->getPosition().y);
    m_right->setPosition(m_right->getPosition().x - m_speed, m_right->getPosition().y);

    if ( m_left->getPosition().x + m_width < 0 )
    {
        m_left->setPosition(0, 0);
        m_right->setPosition(m_width, 0);
    }

    m_left->setColor( sf::Color(255,255,255, m_alpha));
    m_right->setColor( sf::Color(255,255,255, m_alpha));
}


/**
 * Drawing Function
 * @author Arthur
 * @date 3/03/16 - 03/04/16
 */
void SlidingBackground::draw(sf::RenderWindow *window) const
{
    window->draw(*m_left);
    window->draw(*m_right);
}


/**
 * Resizing function
 * @author Arthur
 * @date 30/04/16
 */
void SlidingBackground::resize(float w, float h)
{
    m_left->resize(w, h);
    m_right->resize(w, h);
}


/**
 * Decrease alpha function
 * @author Arthur
 * @date 7/05/16
 */
void SlidingBackground::decreaseAlpha(int level)
{
    if ( m_alpha >= 0+ level)
        m_alpha -= level;
    else
        m_alpha = 0;
}

/**
 * Increase alpha function
 * @author Arthur
 * @date 7/05/16
 */
void SlidingBackground::increaseAlpha(int level)
{
    if ( m_alpha <= 255-level)
        m_alpha += level;
    else
        m_alpha = 255;
}


/**
 * Sets the SlidingBackground's texture from an image
 * @author Arthur
 * @date 02/01/17
 */
void SlidingBackground::setTextureFromImage(std::string image)
{
    m_left->setTextureFromImage(image);
    m_right->setTextureFromImage(image);
}

