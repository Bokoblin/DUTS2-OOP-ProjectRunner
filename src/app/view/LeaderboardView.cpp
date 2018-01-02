#include "LeaderboardView.h"


/**
 * Parameterized Constructor
 * @author Arthur
 * @date 21/05/16 - 02/01/18
 */
LeaderboardView::LeaderboardView(sf::RenderWindow *window, TextHandler *textHandler, LeaderboardModel *leaderboardModel) :
        AbstractView(window, textHandler), m_leaderboard{leaderboardModel}
{
    //=== Init images and text

    loadImages();

    textHandler->initMenuLeaderboardText();

    //=== Init confirm dialog

    m_confirmDialog = new mdsf::Dialog(m_width/2-140, m_height/2-120, 280, 200, "confirm_leaderboard_delete");
    m_confirmDialog->hide();
    DialogBuilder::retrieveCorrespondingStrings(m_confirmDialog, *m_leaderboard->getDataBase());
}


/**
 * Destructor
 * @author Arthur
 * @date 21/05/16 - 02/01/18
 */
LeaderboardView::~LeaderboardView()
{
    delete m_homeFormButton;
    delete m_clearLbRectButton;
    delete m_confirmDialog;
}


/**
 * Image Loading
 * @author Arthur
 * @date 20/05/16 - 02/01/17
 */
void LeaderboardView::loadImages()
{
    //=== Initialize CLEAR button

    std::vector<sf::IntRect> clipRect_clear;
    clipRect_clear.emplace_back(0, 100, 150, 40);
    clipRect_clear.emplace_back(151, 100, 150, 40);
    m_clearLbRectButton = new mdsf::Button(m_width / 2 - 75, 540, 150, 40, "leaderboard_clear_button",
                                     RECT_BUTTONS_IMAGE, clipRect_clear);
    m_clearLbRectButton->retrieveAndSyncLabel(*m_leaderboard->getDataBase());

    //=== Initialize HOME form button

    std::vector<sf::IntRect> clipRect_home;
    clipRect_home.emplace_back(0, 50, 50, 50);
    clipRect_home.emplace_back(51, 50, 50, 50);
    m_homeFormButton = new mdsf::Button(10, 10, 50, 50, SHAPE_BUTTONS_IMAGE, clipRect_home);
}


/**
 * Synchronization function
 * @author Arthur
 * @date 20/05/16 - 27/12/17
 */
void LeaderboardView::synchronize()
{
    m_homeFormButton->sync();
    m_homeFormButton->resize(FORM_BUTTONS_SIZE);
    m_clearLbRectButton->sync();

    m_textHandler->syncMenuLeaderboardText();
}


/**
 * LeaderboardView Drawing
 * @author Arthur
 * @date 21/05/16 - 02/01/18
 */
void LeaderboardView::draw() const
{
    m_window->clear(mdsf::Color::MaterialMine);

    //=== Graphic Elements drawing

    m_window->draw(*m_homeFormButton);
    m_clearLbRectButton->draw(m_window);
    m_confirmDialog->draw(m_window);

    //=== TextHandler Drawing

    m_textHandler->drawLeaderboardText(m_window);
    m_window->display();
}


/**
 * Handles the user interaction events (mouse, keyboard, title bar buttons)
 * @param event sfml event object
 * @return true if app state is unchanged
 *
 * @author Arthur
 * @date 21/05/16 - 02/01/17
 */
bool LeaderboardView::handleEvents(sf::Event event)
{
    if (MOUSE_LEFT_PRESSED_EVENT)
    {
        if (m_homeFormButton->contains(MOUSE_POSITION))
            m_homeFormButton->setPressed(true);

        if (!m_confirmDialog->isVisible())
        {
            if (m_clearLbRectButton->contains(MOUSE_POSITION))
                m_clearLbRectButton->setPressed(true);
        }
    }

    if (event.type == sf::Event::MouseButtonReleased)
    {
        //=== Reset buttons

        m_homeFormButton->setPressed(false);
        m_clearLbRectButton->setPressed(false);

        //=== handle mouse up on a button

        if (!m_confirmDialog->isVisible())
        {
            if (m_homeFormButton->contains(MOUSE_POSITION))
            {
                m_leaderboard->quit();
                return false;
            }

            if (m_clearLbRectButton->contains(MOUSE_POSITION))
            {
                m_confirmDialog->show();
            }
        }
        else
        {
            if (m_confirmDialog->getOkButtonText().contains(MOUSE_POSITION))
            {
                m_confirmDialog->hide();
                m_leaderboard->getDataBase()->clearLeaderboard();
                m_textHandler->updateWholeText();
                m_textHandler->syncMenuLeaderboardText();
            }
            else if (m_confirmDialog->getCancelButtonText().contains(MOUSE_POSITION)
                    || !m_confirmDialog->contains(MOUSE_POSITION))
            {
                m_confirmDialog->hide();
            }
        }
    }

    if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Escape)
    {
        m_confirmDialog->hide();
    }

    return true;
}
