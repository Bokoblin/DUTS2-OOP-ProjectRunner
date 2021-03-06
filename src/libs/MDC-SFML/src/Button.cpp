#include "Button.h"

using std::string;

namespace Bokoblin
{
namespace MaterialDesignComponentsForSFML
{

//------------------------------------------------
//          CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------
//TODO: add clip rect parameter for allowing non-states clips to regroup images (to introduce with image DB (filename + origin + original size))

/**
 * @brief Constructs a button with coordinates,
 * a size, a texture and an image rectangle clip
 *
 * @param x the x-axis coordinate
 * @param y the y-axis coordinate
 * @param width the width
 * @param height the height
 * @param description the label's description (optional)
 * @param textureImage the button texture image (optional)
 *
 * @author Arthur
 * @date 06/04/16 - 03/05/2020
 */
Button::Button(float x, float y, float width, float height, const string& description, const string& textureImage) :
        Sprite(x, y, width, height, textureImage),
        m_isPressed{false}, m_isEnabled{true}, m_label{description}, m_labelPosition{CENTER}
{
    m_font.loadFromFile(Config::DEFAULT_CONDENSED_FONT);
    m_label.setFont(m_font);
    m_label.setFillColor(sf::Color::White);
    m_label.setCharacterSize(DEFAULT_CHAR_SIZE);
    Button::syncLabelPosition();
}

/**
 * @brief Copy Constructor
 *
 * @param other another button object to copy
 *
 * @author Arthur
 * @date 06/04/16 - 04/01/18
 */
Button::Button(Button const& other) :
        Sprite(other), m_isPressed{other.m_isPressed}, m_isEnabled{other.m_isEnabled},
        m_font{other.m_font}, m_label{other.m_label}, m_labelPosition{other.m_labelPosition}
{}

//------------------------------------------------
//          GETTERS
//------------------------------------------------

bool Button::isPressed() const { return m_isPressed; }
bool Button::isEnabled() const { return m_isEnabled; }
LabelPosition Button::getLabelPosition() const { return m_labelPosition; }

//------------------------------------------------
//          SETTERS
//------------------------------------------------

void Button::setPressed(bool pressed) { m_isPressed = pressed; }
void Button::setEnabled(bool enabled) { m_isEnabled = enabled; }
void Button::setPositionSelfCentered(float x, float y) { setPosition(x - m_width / 2, y); }
void Button::setLabelPosition(const LabelPosition& labelPosition) { m_labelPosition = labelPosition; }
void Button::setLabelDescription(const std::string &description) { m_label.setDescription(description); }

//------------------------------------------------
//          METHODS
//------------------------------------------------

/**
 * @brief Synchronizes button sprite
 *
 * @author Arthur
 * @date 06/04/16 - 13/09/18
 */
void Button::sync()
{
    Sprite::applyColor();
}

/**
 * @brief Retrieves the button's label text
 * @details Also syncs the label position as it's content as changed
 *
 * @param func the function used to retrieve the string given a label
 *
 * @author Arthur
 * @date 23/12/16 - 14/01/19
 */
void Button::retrieveLabel(const label_retrieval_func_t& func)
{
    if (!m_label.getDescription().empty()) {
        string utf8_string = func(m_label.getDescription());
        m_label.setUtf8String(utf8_string);
    }

    syncLabelPosition();
}

/**
 * @brief Sync the button's label position
 *
 * @author Arthur
 * @date 23/12/16 - 04/01/18
 */
void Button::syncLabelPosition()
{
    if (!m_label.getDescription().empty()) {
        switch (m_labelPosition) {
            case TOP:
                m_label.setPositionSelfCentered(getX() + getWidth() / 2, getY() - getHeight() / 2);
                m_label.setOrigin(0, m_label.getHeight() / 2);
                break;
            case RIGHT:
                m_label.setPosition(getX() + getWidth() + HORIZONTAL_LABEL_MARGIN, getY() + getHeight() / 2 - 7);
                m_label.setOrigin(0, m_label.getHeight() / 2);
                break;
            case BOTTOM:
                m_label.setPositionSelfCentered(getX() + getWidth() / 2, getY() + getHeight() + 50);
                m_label.setOrigin(0, m_label.getHeight() / 2);
                break;
            case LEFT:
                m_label.setPosition((getX() - HORIZONTAL_LABEL_MARGIN), getY() + getHeight() / 2 - 7);
                m_label.setOrigin(m_label.getWidth(), m_label.getHeight() / 2);
                break;
            case CENTER:
                m_label.setPositionSelfCentered(getX() + getWidth() / 2, getY() + getHeight() / 2 + 2);
                m_label.setOrigin(0, m_label.getHeight() / 2);
                break;
        }
    }
}

/**
 * @brief Draws the button and its label if visible
 *
 * @param window the app's window
 *
 * @author Arthur
 * @date 23/12/16 - 02/01/18
 */
void Button::draw(sf::RenderWindow* window) const
{
    if (isVisible()) {
        window->draw(*this);
        m_label.draw(window);
    }
}

/**
 * @brief Checks if a point of given coordinates is contained
 * in the button or its label
 *
 * @param x the x-axis coordinate
 * @param y the y-axis coordinate
 *
 * @author Arthur
 * @date 23/12/16 - 21/07/18
 */
bool Button::contains(float x, float y) const
{
    return isEnabled() && isVisible()
            && (getGlobalBounds().contains(sf::Vector2f(x, y)) || m_label.contains(x, y));
}

} //namespace MaterialDesignComponentsForSFML
} //namespace Bokoblin
