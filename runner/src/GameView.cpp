/* Copyright (C) 2016 Jolivet Arthur & Laronze Florian

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include "../header/GameView.h"

using namespace std;

/********************************************
    Default Constructor
*********************************************
    Arthur : 26/03 - 27/03
*********************************************/
GameView::GameView(unsigned int w, unsigned int h,
        sf::RenderWindow *mywindow):  View(w, h, mywindow)
{
    loadImages();
}


/********************************************
    Destructor
*********************************************
    Arthur : 26/03 - 02/04
*********************************************/
GameView::~GameView()
{
    if(m_bottomBarGraphic!= NULL)
        delete m_bottomBarGraphic;
    if(m_lifeBoxGraphic!= NULL)
        delete m_lifeBoxGraphic;
    if(m_playerGraphic!= NULL)
        delete m_playerGraphic;
    if(m_standardEnemyGraphic!= NULL)
        delete m_standardEnemyGraphic;
    if(m_totemEnemyGraphic!= NULL)
        delete m_totemEnemyGraphic;
    if(m_blockEnemyGraphic!= NULL)
        delete m_blockEnemyGraphic;
    if(m_explosionGraphic!= NULL)
        delete m_explosionGraphic;
    if(m_coinGraphic!= NULL)
        delete m_coinGraphic;

    if(m_pauseBackgroundGraphic!= NULL)
        delete m_pauseBackgroundGraphic;
    if(m_resumeButtonGraphic!= NULL)
        delete m_resumeButtonGraphic;
    if(m_restartButtonGraphic!= NULL)
        delete m_restartButtonGraphic;
    if(m_homeButtonGraphic!= NULL)
        delete m_homeButtonGraphic;
    if(m_pauseDistanceGraphic!= NULL)
        delete m_pauseDistanceGraphic;
    if(m_endBackgroundGraphic!= NULL)
        delete m_endBackgroundGraphic;
}


/********************************************
   Setters
*********************************************
    Arthur : 27/03
*********************************************/
void GameView::setGameModel(GameModel *model) { m_gameModel = model; }


/********************************************
    Image Loading
*********************************************
    Arthur : 26/03 - 01/04
*********************************************/
void GameView::loadImages()
{
    if (!m_farBackgroundTexture.loadFromFile(BACKGROUND_IMAGE_2))
        cerr << "ERROR when loading image file: " << BACKGROUND_IMAGE_2 << endl;
    else
    {
        m_farBackgroundTexture.setSmooth(true);
        m_farBackground = new SlidingBackground(m_farBackgroundTexture, 1200, m_height, 1);
    }

    if (!m_nearBackgroundTexture.loadFromFile(BACKGROUND_IMAGE_1))
        cerr << "ERROR when loading image file: " << BACKGROUND_IMAGE_1 << endl;
    else
    {
        m_nearBackgroundTexture.setSmooth(true);
        m_nearBackground = new SlidingBackground(m_nearBackgroundTexture, 1200, m_height, 2);
    }

    if (!m_bottomBarTexture.loadFromFile(BOTTOM_BAR))
        cerr << "ERROR when loading image file: " << BOTTOM_BAR << endl;
    else
    {
        m_bottomBarTexture.setSmooth(true);
        m_bottomBarGraphic = new GraphicElement(m_bottomBarTexture, 0, 520, 1200, m_height);
    }

    if (!m_lifeBoxTexture.loadFromFile(LIFE_BOX))
        cerr << "ERROR when loading image file: " << LIFE_BOX << endl;
    else
    {
        m_lifeBoxTexture.setSmooth(true);
        m_lifeBoxGraphic = new GraphicElement(m_lifeBoxTexture, 105, 535, 200, 100);
    }

    if (!m_remainingLifeTexture.loadFromFile(LIFE_BOX))
        cerr << "ERROR when loading image file: " << LIFE_BOX << endl;
    else
    {
        m_remainingLifeTexture.setSmooth(true);
        m_remainingLifeGraphic = new GraphicElement(m_remainingLifeTexture, 105, 535, 200, 100);
    }

    if (!m_playerTexture.loadFromFile(BALL_IMAGE) )
        cerr << "ERROR when loading image file: " << BALL_IMAGE << endl;
    else
    {
        std::vector<sf::IntRect> clip_rects;
        for (int i=0; i<8; i++)
            clip_rects.push_back(sf::IntRect(50*i,0,50,50));

        m_playerTexture.setSmooth(true);
        m_playerGraphic = new AnimatedGraphicElement(clip_rects, m_playerTexture, PLAYER_DEFAULT_POS_X, GAME_FLOOR,30,30);
        m_playerGraphic->setOrigin(0,50);
    }

    if (!m_standardEnemyTexture.loadFromFile(ENEMIES_IMAGE))
        cerr << "ERROR when loading image file: " << ENEMIES_IMAGE << endl;
    else
    {
        std::vector<sf::IntRect> clip_rects;
        for (int i=0; i<2; i++)
            clip_rects.push_back(sf::IntRect(50*i,0,50,50));

        m_standardEnemyTexture.setSmooth(true);
        m_standardEnemyGraphic = new AnimatedGraphicElement(clip_rects, m_standardEnemyTexture, m_width, GAME_FLOOR,30,30);
        m_standardEnemyGraphic->setOrigin(0,50);
    }

    if (!m_totemEnemyTexture.loadFromFile(ENEMIES_IMAGE))
        cerr << "ERROR when loading image file: " << ENEMIES_IMAGE << endl;
    else
    {
        std::vector<sf::IntRect> clip_rects;
        for (int i=0; i<2; i++)
            clip_rects.push_back(sf::IntRect(50*i,0,50,150));

        m_totemEnemyTexture.setSmooth(true);
        m_totemEnemyGraphic = new AnimatedGraphicElement(clip_rects, m_totemEnemyTexture, m_width, GAME_FLOOR,30,90);
        m_totemEnemyGraphic->setOrigin(0,150);
    }

    if (!m_blockEnemyTexture.loadFromFile(BLOCK_ENEMIES_IMAGE))
        cerr << "ERROR when loading image file: " << BLOCK_ENEMIES_IMAGE << endl;
    else
    {
        std::vector<sf::IntRect> clip_rects;
        for (int i=0; i<2; i++)
            clip_rects.push_back(sf::IntRect(50*i,0,50,50));

        m_blockEnemyTexture.setSmooth(true);
        m_blockEnemyGraphic = new AnimatedGraphicElement(clip_rects, m_blockEnemyTexture, m_width, GAME_FLOOR,50,50);
        m_blockEnemyGraphic->setOrigin(0,50);
    }

    if (!m_explosionTexture.loadFromFile(EXPLOSION_IMAGE))
        cerr << "ERROR when loading image file: " << EXPLOSION_IMAGE << endl;
    else
    {
        std::vector<sf::IntRect> clip_rects;
        for (int i=0; i<3; i++)
            clip_rects.push_back(sf::IntRect(200*i,0,200,200));

        m_explosionTexture.setSmooth(true);
        m_explosionGraphic = new AnimatedGraphicElement(clip_rects, m_explosionTexture, m_width, GAME_FLOOR,50,50);
        m_explosionGraphic->setOrigin(0,50);
    }

    if (!m_coinTexture.loadFromFile(BONUS_IMAGE))
        cerr << "ERROR when loading image file: " << BONUS_IMAGE << endl;
    else
    {
        std::vector<sf::IntRect> clip_rects;
        for (int i=0; i<5; i++)
            clip_rects.push_back(sf::IntRect(50*i,0,50,50));

        m_coinTexture.setSmooth(true);
        m_coinGraphic = new AnimatedGraphicElement(clip_rects, m_coinTexture, 30, 95,25,25);
        m_coinGraphic->setOrigin(0,50);
    }
    if (!m_pauseBackgroundTexture.loadFromFile(PAUSE_BACKGROUND_IMAGE))
        cerr << "ERROR when loading image file: " << PAUSE_BACKGROUND_IMAGE << endl;
    else
    {
        m_pauseBackgroundTexture.setSmooth(true);
        m_pauseBackgroundGraphic = new GraphicElement(m_pauseBackgroundTexture, 0, 0, m_width, m_height);
    }

    if (!m_resumeButtonTexture.loadFromFile(PAUSE_BUTTONS_IMAGE, sf::IntRect(0,0,50,50)) )
        cerr << "ERROR when loading image file: " << PAUSE_BUTTONS_IMAGE << endl;
    else
    {
        m_resumeButtonTexture.setSmooth(true);
        m_resumeButtonGraphic = new GraphicElement(m_resumeButtonTexture, 30, 405, 20, 20);
        m_resumeButtonGraphic->resize(20,20);
    }

    if (!m_restartButtonTexture.loadFromFile(PAUSE_BUTTONS_IMAGE, sf::IntRect(0,50,50,50)) )
        cerr << "ERROR when loading image file: " << PAUSE_BUTTONS_IMAGE << endl;
    else
    {
        m_restartButtonTexture.setSmooth(true);
        m_restartButtonGraphic = new GraphicElement(m_restartButtonTexture, 30, 455, 20, 20);
        m_restartButtonGraphic->resize(20,20);
    }

    if (!m_homeButtonTexture.loadFromFile(PAUSE_BUTTONS_IMAGE, sf::IntRect(0,100,50,50)) )
        cerr << "ERROR when loading image file: " << PAUSE_BUTTONS_IMAGE << endl;
    else
    {
        m_homeButtonTexture.setSmooth(true);
        m_homeButtonGraphic = new GraphicElement(m_homeButtonTexture, 30, 505, 20, 20);
        m_homeButtonGraphic->resize(20,20);
    }

        if (!m_pauseDistanceTexture.loadFromFile(PAUSE_BUTTONS_IMAGE, sf::IntRect(0,150,50,50)) )
        cerr << "ERROR when loading image file: " << PAUSE_BUTTONS_IMAGE << endl;
    else
    {
        m_pauseDistanceTexture.setSmooth(true);
        m_pauseDistanceGraphic = new GraphicElement(m_pauseDistanceTexture, 30, 35, 20, 20);
        m_pauseDistanceGraphic->resize(20,20);
    }

    if (!m_endBackgroundTexture.loadFromFile(END_BACKGROUND_IMAGE))
        cerr << "ERROR when loading image file: " << END_BACKGROUND_IMAGE << endl;
    else
    {
        m_endBackgroundTexture.setSmooth(true);
        m_endBackgroundGraphic = new GraphicElement(m_endBackgroundTexture, 0, 0, m_width, m_height);
    }
}



/********************************************
    Link mElements with gElements
*********************************************
    Arthur : 18/03 - 26/03
*********************************************/
void GameView::linkElements()
{
    set<MovableElement*>::const_iterator it;
    for ( it = m_gameModel->getNewMElementsArray().begin(); it!=  m_gameModel->getNewMElementsArray().end(); ++it)
    {
        assert((*it) != nullptr);
        if ( (*it)->getType() == 0 ) //player
            m_MovableToGraphicElement[*it] = m_playerGraphic;
        else if ( (*it)->getType() == 1 ) //enemy
        {
            AnimatedGraphicElement *m_newGEnemy;
            if ( (*it)->getEnemyType() == 0 )
                m_newGEnemy = new AnimatedGraphicElement(*m_standardEnemyGraphic);
            else if ( (*it)->getEnemyType() == 1 )
                m_newGEnemy = new AnimatedGraphicElement(*m_totemEnemyGraphic);
            else
                m_newGEnemy = new AnimatedGraphicElement(*m_blockEnemyGraphic);
            m_MovableToGraphicElement[*it] = m_newGEnemy;
        }
        else if ( (*it)->getType() == 2 ) //coin
        {
            AnimatedGraphicElement *m_newcoin = new AnimatedGraphicElement(*m_coinGraphic);
            m_MovableToGraphicElement[*it] = m_newcoin;
        }
    }
    m_gameModel->clearNewMovableElementList();
}


/********************************************
    Update gElements
*********************************************
    Arthur : 6/03 - 26/03
*********************************************/
void GameView::updateElements()
{
    std::map<MovableElement*, GraphicElement*>::iterator it;
    for(it = m_MovableToGraphicElement.begin() ; it != m_MovableToGraphicElement.end() ; ++it)
    {
        //=== Update Position

        m_gameModel->moveMovableElement(it->first);

        unsigned int position_x = (it->first)->getPosX();
        unsigned int position_y = (it->first)->getPosY();
        int move_x = (it->first)->getMoveX();
        int move_y = (it->first)->getMoveY();

        it->second->setPosition(sf::Vector2f( position_x+move_x, position_y+move_y ));

        //=== Update Graphics

        it->second->sync();

        if (it->first->getType() == 0) //player
        {
            it->second->resize(30,30);
        }
        else if (it->first->getType() == 1) //enemy
        {
            if ( m_playerGraphic->getGlobalBounds().intersects(it->second->getGlobalBounds() ) )
            {
                it->second->setTexture(m_explosionTexture);
                (it->first)->setCollisionState(true);
                if (it->first->getEnemyType() == 0)
                    m_gameModel ->getPlayer()->setLife(m_gameModel ->getPlayer()->getLife()-10);
                if (it->first->getEnemyType() == 1)
                    m_gameModel ->getPlayer()->setLife(m_gameModel ->getPlayer()->getLife()-15);
                if (it->first->getEnemyType() == 2)
                    m_gameModel ->getPlayer()->setLife(m_gameModel ->getPlayer()->getLife()-20);
            }

            if (it->first->getEnemyType() == 1)
            {
                it->second->resize(30,90);
            }
            else if (it->first->getEnemyType() )
                it->second->resize(50,50);
            else
                it->second->resize(30,30);
        }
        else if (it->first->getType() == 2) //coins
        {
            it->second->resize(25,25);
            if (m_playerGraphic->getGlobalBounds().intersects(it->second->getGlobalBounds() ) )
            {
                (it->first)->setCollisionState(true);
                m_gameModel->setNbCoinsCollected(m_gameModel->getNbCoinsCollected() + 1);
            }
        }
    }
}


/********************************************
    Delete gElement
*********************************************
    Arthur : 12/03 - 20/03
*********************************************/
void GameView::deleteElements()
{
    std::map<MovableElement *, GraphicElement *>::iterator it = m_MovableToGraphicElement.begin();
    bool found = false;
    while (!found && it!=m_MovableToGraphicElement.end() )
    {
        if ( (it->first)->getCollisionState() == true )
        {
            delete it->second;
            m_MovableToGraphicElement.erase(it);
            found = true;
        }
        else
            ++it;
    }
}


/********************************************
    Synchronization function
*********************************************
    Arthur : 26/03 - 02/04
*********************************************/
void GameView::synchronize()
{
    if (m_gameModel->getPauseState() == false && m_gameModel->getEndState() == false )
    {

        //=== Link new mElements with gElements

        linkElements();

        //=== Elements deleting if not used anymore

        deleteElements();

        //=== Elements update

        m_nearBackground->setSpeed(m_gameModel->getGameSpeed() );
        m_farBackground->sync();
        m_nearBackground->sync();
        m_remainingLifeTexture.loadFromFile( REMAINING_LIFE,
                sf::IntRect( 3*( 100-m_gameModel->getPlayer()->getLife() ), 0, 300, 50 ) );
        updateElements();

        //=== Text update

        m_text.syncGameText(m_gameModel);

    }
    else if (m_gameModel->getPauseState() == true)
    {
        //=== Elements update

        m_coinGraphic->sync();
        m_coinGraphic->resize(20,20);

        //=== Text update

        m_text.syncPauseText(m_gameModel);

    }
    else if (m_gameModel->getEndState() == true )
    {
        //=== Elements update

        m_homeButtonGraphic->resize(30,30);
        m_homeButtonGraphic->setPosition(30, 535);
        m_restartButtonGraphic->resize(30,30);
        m_restartButtonGraphic->setPosition(840, 535);

        //=== Text update

        m_text.syncEndText(m_gameModel);

    }
}


/********************************************
    GameView Drawing
*********************************************
    Arthur : 26/03 - 01/04
*********************************************/
void GameView::draw()
{
    m_window->clear();

    if (m_gameModel->getPauseState() == false && m_gameModel->getEndState() == false )
    {
        //=== Background drawing

        m_farBackground->draw(*m_window);
        m_nearBackground->draw(*m_window);
        m_window->draw(*m_bottomBarGraphic);
        m_window->draw(*m_remainingLifeGraphic);
        m_window->draw(*m_lifeBoxGraphic);

        //=== GraphicElements drawing

        for(auto it = m_MovableToGraphicElement.begin() ; it != m_MovableToGraphicElement.end() ; ++it)
        {
            it->second->draw(m_window);
        }

        //=== Text drawing

        m_text.drawGameText(m_window);

    }
    else if (m_gameModel->getPauseState() == true)
    {
        //=== Background drawing & GraphicElements drawing

        m_window->draw(*m_pauseBackgroundGraphic);
        m_window->draw(*m_pauseDistanceGraphic);
        m_window->draw(*m_coinGraphic);
        m_window->draw(*m_resumeButtonGraphic);
        m_window->draw(*m_restartButtonGraphic);
        m_window->draw(*m_homeButtonGraphic);

        //=== Text drawing

        m_text.drawPauseText(m_window);

    }
    else if (m_gameModel->getEndState() == true )
    {
        //=== Background drawing & GraphicElements drawing

        m_window->draw(*m_endBackgroundGraphic);
        m_window->draw(*m_restartButtonGraphic);
        m_window->draw(*m_homeButtonGraphic);

        //=== Text drawing

        m_text.drawEndText(m_window);

    }

    m_window->display();
}


/********************************************
    Events treating
*********************************************
    Arthur : 21/02 - 01/04
    Florian: 21/02 - 2/03
*********************************************/
bool GameView::treatEvents()
{
    bool result = false;

    if(m_window->isOpen())
    {
        result = true;

        if (m_gameModel->getPauseState() == false)
        {
            if ( (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left ) )
                    && m_gameModel ->getPlayer()->getPosX()  > 0 )
            {
                m_gameModel->moveBallAccordingEvent(true);
            }
            if ( (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right ) )
                    && (m_gameModel ->getPlayer()->getPosX() + m_gameModel ->getPlayer()->getWidth()) < m_width )
            {
                m_gameModel->moveBallAccordingEvent(false);
            }
        }

        sf::Event event;
        while (m_window->pollEvent(event))
        {
            if  (event.type == sf::Event::Closed)
            {
                m_model->setGameState(false);
                m_window->close();
                result = false;
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
            {
                m_gameModel->setPauseState(!m_gameModel->getPauseState() );
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Add)
            {
                m_gameModel->setEndState(true );
            }

            if (m_gameModel->getPauseState() == true)
            {
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
                {
                    if ( m_resumeButtonGraphic->getGlobalBounds().contains(sf::Vector2f(event.mouseButton.x, event.mouseButton.y)) ||
                                m_text.getPauseResumeText().getGlobalBounds().contains( sf::Vector2f(event.mouseButton.x, event.mouseButton.y) ) )
                    {
                        m_gameModel->setPauseState(false);
                    }
                    else if ( m_restartButtonGraphic->getGlobalBounds().contains(sf::Vector2f(event.mouseButton.x, event.mouseButton.y)) ||
                                m_text.getRestartText().getGlobalBounds().contains( sf::Vector2f(event.mouseButton.x, event.mouseButton.y) ) )
                    {
                        m_gameModel->setPauseState(false);
                        m_model->setGameState(false);
                        m_model->setResetGameState(true);
                        result = false;
                    }
                    else if ( m_homeButtonGraphic->getGlobalBounds().contains(sf::Vector2f(event.mouseButton.x, event.mouseButton.y)) ||
                                m_text.getHomeText().getGlobalBounds().contains( sf::Vector2f(event.mouseButton.x, event.mouseButton.y) ) )
                    {
                        m_gameModel->setPauseState(false);
                        m_model->setGameState(false);
                        m_model->setMenuState(true);
                        result = false;
                    }
                }
            }
            if (m_gameModel->getEndState() == true)
            {
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
                {
                    if ( m_restartButtonGraphic->getGlobalBounds().contains(sf::Vector2f(event.mouseButton.x, event.mouseButton.y)) ||
                                m_text.getRestartText().getGlobalBounds().contains( sf::Vector2f(event.mouseButton.x, event.mouseButton.y) ) )
                    {
                        m_gameModel->setEndState(false);
                        m_model->setGameState(false);
                        m_model->setResetGameState(true);
                        result = false;
                    }
                    else if ( m_homeButtonGraphic->getGlobalBounds().contains(sf::Vector2f(event.mouseButton.x, event.mouseButton.y)) ||
                                m_text.getHomeText().getGlobalBounds().contains( sf::Vector2f(event.mouseButton.x, event.mouseButton.y) ) )
                    {
                        m_gameModel->setEndState(false);
                        m_model->setGameState(false);
                        m_model->setMenuState(true);
                        result = false;
                    }
                }
            }
        }
    }
    return result;
}
