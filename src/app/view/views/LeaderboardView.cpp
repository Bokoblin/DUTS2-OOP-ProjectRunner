#include "LeaderboardView.h"

namespace ViewResources = Bokoblin::BokoRunner::Resources::View;

//------------------------------------------------
//          CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------

/**
 * Constructs the leaderboard view
 * with the window, the text manager and its model counterpart
 *
 * @param window the app window
 * @param textManager the text manager
 * @param leaderboardModel the leaderboard model counterpart
 *
 * @author Arthur
 * @date 21/05/16 - 02/01/18
 */
LeaderboardView::LeaderboardView(sf::RenderWindow* window, AppTextManager* textManager,
                                 LeaderboardModel* leaderboardModel) :
        AbstractView(window, textManager), m_leaderboard{leaderboardModel}
{
    //=== Init images and text

    LeaderboardView::loadSprites();

    textManager->initMenuLeaderboardText();

    //=== Init confirm dialog

    //TODO [MDC-CPP] No dialog init at startup
    m_confirmDialog = new mdsf::Dialog(getDialogXPosition(DIALOG_WIDTH), getDialogYPosition(DIALOG_HEIGHT),
                                       DIALOG_WIDTH, DIALOG_HEIGHT, "confirm_leaderboard_delete");
    m_confirmDialog->hide();
    DialogBuilder::retrieveCorrespondingStrings(m_confirmDialog);
}


/**
 * Destructor
 * @author Arthur
 * @date 21/05/16 - 02/01/18
 */
LeaderboardView::~LeaderboardView()
{
    delete m_homeButton;
    delete m_clearLeaderboardButton;
    delete m_confirmDialog;
}


//------------------------------------------------
//          METHODS
//------------------------------------------------

/**
 * Loads all sprites used by the leaderboard screen
 *
 * @author Arthur
 * @date 20/05/16 - 17/09/18
 */
void LeaderboardView::loadSprites()
{
    //=== Initialize CLEAR button

    m_clearLeaderboardButton = new mdsf::RaisedButton(getHalfXPosition() - (0.5f * RESET_BTN_WIDTH),
                                                      0.9f * m_height, RESET_BTN_WIDTH, RESET_BTN_HEIGHT,
                                                      "leaderboard_clear_button");
    m_clearLeaderboardButton->retrieveLabel(LocalizationManager::fetchLocalizedString);
    m_clearLeaderboardButton->setFillColor(mdsf::Color::MaterialRed);

    //=== Initialize HOME button

    m_homeButton = new mdsf::RaisedButton(10, 10, DEFAULT_HOME_SIZE, DEFAULT_HOME_SIZE, "", ViewResources::HOME_IMAGE);
    m_homeButton->resize(HOME_BTN_SIZE, HOME_BTN_SIZE);
}


/**
 * Synchronizes leaderboard elements
 *
 * @author Arthur
 * @date 20/05/16 - 22/09/18
 */
void LeaderboardView::synchronize()
{
    m_homeButton->sync();
    m_clearLeaderboardButton->sync();

    m_textManager->syncMenuLeaderboardText();
}


/**
 * Draws leaderboard elements on the window
 *
 * @author Arthur
 * @date 21/05/16 - 02/01/18
 */
void LeaderboardView::draw() const
{
    m_window->clear(AppColor::CoalMine);

    //=== Graphic Elements drawing

    m_homeButton->draw(m_window);
    m_clearLeaderboardButton->draw(m_window);
    m_confirmDialog->draw(m_window);

    //=== Text Drawing

    m_textManager->drawMenuLeaderboardText(m_window);

    m_window->display();
}


/**
 * Handles the user interaction events (mouse, keyboard, title bar buttons)
 *
 * @param event sfml event object
 * @return true if app state is unchanged
 *
 * @author Arthur
 * @date 21/05/16 - 13/01/19
 */
bool LeaderboardView::handleEvents(sf::Event& event)
{
    if (EventUtils::wasMouseLeftPressed(event)) {
        m_homeButton->setPressed(EventUtils::isMouseInside(*m_homeButton, event));

        if (!m_confirmDialog->isVisible()) {
            m_clearLeaderboardButton->setPressed(EventUtils::isMouseInside(*m_clearLeaderboardButton, event));
        }
    }

    if (EventUtils::wasMouseReleased(event)) {
        //=== Reset buttons

        m_homeButton->setPressed(false);
        m_clearLeaderboardButton->setPressed(false);

        //=== handle mouse up on a button

        if (!m_confirmDialog->isVisible()) {
            if (EventUtils::isMouseInside(*m_homeButton, event)) {
                m_leaderboard->quit();
                return false;
            }

            if (EventUtils::isMouseInside(*m_clearLeaderboardButton, event)) {
                m_confirmDialog->show();
            }
        } else {
            if (EventUtils::isMouseInside(m_confirmDialog->getOkButtonText(), event)) {
                m_confirmDialog->hide();
                m_leaderboard->clearLeaderboardData();
                m_textManager->updateWholeStandaloneTextContent();
                m_textManager->syncMenuLeaderboardText();
            } else if (EventUtils::isMouseInside(m_confirmDialog->getCancelButtonText(), event)
                    || !EventUtils::isMouseInside(*m_confirmDialog, event)) {
                m_confirmDialog->hide();
            }
        }
    }

    if (EventUtils::wasKeyboardEscapePressed(event)) {
        m_confirmDialog->hide();
    }

    return true;
}

/**
 * Get dialog X position from the dialog width
 *
 * @param width the dialog width
 * @return the X position
 *
 * @author Arthur
 * @date 10/09/18 - 16/09/18
 */
float LeaderboardView::getDialogXPosition(int width) const
{
    return getHalfXPosition() - 0.5f * width;
}


/**
 * Get dialog Y position from the dialog height
 *
 * @param height the dialog height
 * @return the Y position
 *
 * @author Arthur
 * @date 10/09/18 - 16/09/18
 */
float LeaderboardView::getDialogYPosition(int height) const
{
    return getHalfYPosition() - 0.5f * height;
}
