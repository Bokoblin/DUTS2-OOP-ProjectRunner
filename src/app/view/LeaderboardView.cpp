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
    delete m_clearLeaderboardRaisedButton;
    delete m_confirmDialog;
}


/**
 * Image Loading
 * @author Arthur
 * @date 20/05/16 - 04/01/18
 */
void LeaderboardView::loadImages()
{
    //=== Initialize CLEAR button

    std::vector<sf::IntRect> clipRectClear;
    clipRectClear.emplace_back(RAISED_BUTTON_DEFAULT);
    clipRectClear.emplace_back(RAISED_BUTTON_PRESSED);
    m_clearLeaderboardRaisedButton = new mdsf::Button(m_width / 2 - 75, 540, 150, 36, "leaderboard_clear_button",
                                     RECT_BUTTONS_IMAGE, clipRectClear);
    m_clearLeaderboardRaisedButton->retrieveAndSyncLabel(*m_leaderboard->getDataBase());
    m_clearLeaderboardRaisedButton->setColor(mdsf::Color::MaterialRed);

    //=== Initialize HOME form button

    std::vector<sf::IntRect> clipRectHome;
    clipRectHome.emplace_back(0, 50, 50, 50);
    clipRectHome.emplace_back(51, 50, 50, 50);
    m_homeFormButton = new mdsf::Button(10, 10, 50, 50, SHAPE_BUTTONS_IMAGE, clipRectHome);
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
    m_clearLeaderboardRaisedButton->sync();

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
    m_clearLeaderboardRaisedButton->draw(m_window);
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
            if (m_clearLeaderboardRaisedButton->contains(MOUSE_POSITION))
                m_clearLeaderboardRaisedButton->setPressed(true);
        }
    }

    if (event.type == sf::Event::MouseButtonReleased)
    {
        //=== Reset buttons

        m_homeFormButton->setPressed(false);
        m_clearLeaderboardRaisedButton->setPressed(false);

        //=== handle mouse up on a button

        if (!m_confirmDialog->isVisible())
        {
            if (m_homeFormButton->contains(MOUSE_POSITION))
            {
                m_leaderboard->quit();
                return false;
            }

            if (m_clearLeaderboardRaisedButton->contains(MOUSE_POSITION))
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
