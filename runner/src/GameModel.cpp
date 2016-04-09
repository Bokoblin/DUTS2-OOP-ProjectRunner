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

#include "../header/GameModel.h"

using namespace std;
using namespace std::chrono;

/********************************************
    Parameterized Constructor
*********************************************
    Arthur : 26/03 - 01/04
*********************************************/
GameModel::GameModel(unsigned int w, unsigned int h, std::chrono::system_clock::time_point programBeginningTime) :
    Model(w, h, programBeginningTime), m_pauseState{false}, m_endState{false}, m_score{0},
    m_distance{0}, m_gameSpeed{4}, m_lastTime{system_clock::now()},
    m_nbCoinsCollected{0}, m_currentEnemyInterdistance{0}, m_currentCoinInterdistance{0}
{
    srand(time(NULL));
    m_chosenEnemyInterdistance = 10 +rand()%10;
    m_chosenCoinInterdistance = rand()%10;
    addANewMovableElement(PLAYER_DEFAULT_POS_X, GAME_FLOOR, 0);
}


/********************************************
    Destructor
*********************************************
    Arthur : 26/03
*********************************************/
GameModel::~GameModel()
{
    for (set<MovableElement*>::iterator it = m_movableElementsArray.begin(); it!=m_movableElementsArray.end(); ++it)
        delete *it;

    m_movableElementsArray.clear();
}


/********************************************
    Getters
*********************************************
    Arthur : 21/02 - 01/04
    Florian: 21/02 - 25/02
*********************************************/
bool GameModel::getPauseState() const {return m_pauseState;}
bool GameModel::getEndState() const {return m_endState;}
Player* GameModel::getPlayer() const { return m_player; }
int GameModel::getScore() const { return m_score; }
int GameModel::getDistance() const { return m_distance; }
int GameModel::getGameSpeed() const { return m_gameSpeed; }
unsigned int GameModel::getNbCoinsCollected() const { return m_nbCoinsCollected; }
const set<MovableElement*>& GameModel::getMElementsArray() { return m_movableElementsArray; }
const set<MovableElement*>& GameModel::getNewMElementsArray() { return m_newMovableElementsArray; }


/********************************************
    Setters
*********************************************
    Arthur : 8/03 - 01/04
*********************************************/
void GameModel::setPauseState(bool state) {m_pauseState = state;}
void GameModel::setEndState(bool state) {m_endState = state;}
void GameModel::setGameSpeed(int speed) { m_gameSpeed = speed; }
void GameModel::setNbCoinsCollected(unsigned int n) { m_nbCoinsCollected = n;}


/********************************************
    Next Step
*********************************************
    Arthur : 21/02 - 26/03
*********************************************/
void GameModel::nextStep()
{
    system_clock::duration nextStepDelay = system_clock::now() - m_lastTime;

    if (m_pauseState == false && m_endState == false)
    {
        if ( nextStepDelay > milliseconds(400/m_gameSpeed) )
        {
            m_distance ++;

            //=== Add new enemies

            if (m_currentEnemyInterdistance >= m_chosenEnemyInterdistance)
            {
                if (checkIfPositionFree(m_width, GAME_FLOOR) == true)
                {
                    addANewMovableElement(m_width, GAME_FLOOR, 1);
                    m_currentEnemyInterdistance = 0;
                    chooseInterdistance(1);
                }
            }
            else m_currentEnemyInterdistance++;

            //=== Add new Coins

            if (m_currentCoinInterdistance >= m_chosenCoinInterdistance)
            {
                if (checkIfPositionFree(m_width, GAME_FLOOR) == true)
                {
                    addANewMovableElement(m_width, GAME_FLOOR-100, 4);
                    m_currentCoinInterdistance = 0;
                    chooseInterdistance(2);
                }
            }
            else m_currentCoinInterdistance++;

            //=== Delete Movable Elements

            deleteMovableElement();

            //=== Check Collisions

            set<MovableElement*>::const_iterator it;
            for (it = m_movableElementsArray.begin(); it !=m_movableElementsArray.end(); ++it)
            {
                if ( (*it)->getType() != 0 && m_player->collision(**it))
                {
                    (*it)->setCollisionState(true);

                    if ( (*it)->getType() == 1) //standard enemy
                        m_player->setLife(m_player->getLife()-10);

                    else if ( (*it)->getType() == 2) //totem enemy
                        m_player->setLife(m_player->getLife()-15);

                    else if ( (*it)->getType() == 3) //block enemy
                        m_player->setLife(m_player->getLife()-20);

                    else if ( (*it)->getType() == 4) //coin
                        m_nbCoinsCollected += 1;
                }
            }


            if (m_player->getLife() == 0)
            {
                m_endState = true;
            }

            m_lastTime = system_clock::now();
        }
    }
    else if (m_endState == true)
    {
        m_score = m_gameSpeed*m_distance + 20*m_nbCoinsCollected;
    }
}


/********************************************
    choose the interdistance between elements
*********************************************
    Arthur :  12/03 - 26/03
*********************************************/
void GameModel::chooseInterdistance(int elementType)
{
    //allows to calculate interdistance in different situations
    if  ( elementType  == 1 ) //enemy
    {
        if (m_chosenEnemyInterdistance > 40)
            m_chosenEnemyInterdistance = abs(rand()%30);
        else if  ( m_chosenEnemyInterdistance < 10)
            m_chosenEnemyInterdistance = abs(10 + rand()%40);
        else
            m_chosenEnemyInterdistance = abs(rand()%50);
    }

    else if ( elementType == 2 ) //coin
    {
        if (m_chosenCoinInterdistance > 20)
            m_chosenCoinInterdistance = abs(rand()%20);
        else if  ( m_chosenCoinInterdistance < 10)
            m_chosenCoinInterdistance = abs(15 + rand()%20);
        else
            m_chosenCoinInterdistance = abs(rand()%40);
    }
}


/********************************************
    check if a position is free to use
*********************************************
    Arthur :  8/03 - 26/03
*********************************************/
bool GameModel::checkIfPositionFree(const unsigned int posX, const unsigned int posY) const
{
    bool posFree=true;
    set<MovableElement*>::iterator it=m_movableElementsArray.begin();

    while (posFree &&  it != m_movableElementsArray.end() )
    {
        if ( (*it)->contains(posX, posY) )
            posFree = false;
        else
            ++it;
    }

    return posFree;
}


/********************************************
    NewMovableElement vector cleaning
*********************************************
    Arthur : 2/03- 26/03
    Florian: 2/03
*********************************************/
void GameModel::clearNewMovableElementList()
{
    m_newMovableElementsArray.clear();
}


/********************************************
    Elements Moving (enemies, bonus, ...)
*********************************************
    Arthur : 6/03 - 26/03
*********************************************/
void GameModel::moveMovableElement(MovableElement *currentElement)
{
    if( currentElement != NULL )
        currentElement->move();
}


/********************************************
    New MovableElement  Adding
*********************************************
    Arthur : 25/02 - 26/03
    Florian: 2/03
*********************************************/
void GameModel::addANewMovableElement(float posX, float posY, int type)
{
    m_newMElement = nullptr;
    if (type == 0) //Ball
    {
        m_player = new Player(posX, posY, 30, 30, 2.0, 12.0);
        m_newMElement = m_player;
    }
    else if (type == 1) //Enemy
    {
        m_newMElement = new Enemy(posX, posY, 30, 30, getGameSpeed()*(-1), 0);
    }
    else if (type == 4) //Coin
    {
        m_newMElement = new Coin(posX, posY, 30, 30, getGameSpeed()*(-1), 0);
    }
    assert(m_newMElement != nullptr);

    if (m_movableElementsArray.find(m_newMElement) == m_movableElementsArray.end())
    {
        m_newMovableElementsArray.insert( m_newMElement );
        m_movableElementsArray.insert( m_newMElement );
    }
}


/********************************************
    Delete Movable Elements
*********************************************
    Arthur : 12/03 - 31/03
*********************************************/
void GameModel::deleteMovableElement()
{
    set<MovableElement*>::iterator it = m_movableElementsArray.begin();
    bool found=false;
    while( it!=m_movableElementsArray.end() && !found )
    {
        if ( ( (*it)->getPosX() + (*it)->getWidth() ) < 0 || (*it)->getCollisionState() == true )
        {
            m_movableElementsArray.erase(it);
            found = true;
        }
        else
            ++it;
    }
}
