#include "GameModel.h"

using namespace std;

/**
 * Constructs a GameModel with the app common model
 * @author Arthur
 * @date 26/03/16 - 26/12/16
 *
 * @param model the app model
 */
GameModel::GameModel(const Model& model) :
    Model(model), m_gameState{RUNNING}, m_inTransition{false},
    m_isTransitionPossible{false}, m_currentZone{HILL},
    m_currentEnemyTimeSpacing{0}, m_currentCoinTimeSpacing{0}, m_currentBonusTimeSpacing{0},
    m_lastTime{chrono::system_clock::now()},  m_bonusTimeout{0}
{
    srand((unsigned int) time(NULL));

    //=== Initialize new game

    m_dataBase->launchNewGame();
    m_dataBase->fetchActivatedShopItems();

    addANewMovableElement(DEFAULT_PLAYER_X, GAME_FLOOR, PLAYER);

    if ( m_dataBase->getDifficulty() == EASY)
        m_gameSpeed = DEFAULT_SPEED;
    else
        m_gameSpeed = 2*DEFAULT_SPEED;

    //=== Initialize elements apparition time-spacing

    m_chosenEnemyTimeSpacing = 10 +rand()%11;  //10 to 20 m
    m_chosenCoinTimeSpacing = rand()%11; //0 to 10m
    m_chosenBonusTimeSpacing = 100 + rand()%51; //100 to 150 m
}


/**
 * Destructor
 * @author Arthur
 * @date 26/03/16 - 24/05/16
 */
GameModel::~GameModel()
{
    for (auto element : m_movableElementsArray)
        delete element;

    m_movableElementsArray.clear();
}


//=== Getters

GameState GameModel::getGameState() const { return m_gameState; }
bool GameModel::getTransitionStatus() const { return m_inTransition; }
bool GameModel::getTransitionPossibleStatus() const { return m_isTransitionPossible; }
float GameModel::getGameSpeed() const { return m_gameSpeed; }
int GameModel::getBonusTimeout() const { return (int) (m_bonusTimeout.count() / 1000); } // return seconds
int GameModel::getCurrentZone() const { return m_currentZone; }
Player* GameModel::getPlayer() const { return m_player; }
const set<MovableElement*>& GameModel::getNewMElementsArray() const { return m_newMovableElementsArray; }

//=== Setters

void GameModel::setGameState(GameState state) { m_gameState = state; }
void GameModel::setTransitionStatus(bool status) { m_inTransition = status; }
void GameModel::setTransitionPossibleStatus(bool status) { m_isTransitionPossible = status; }
void GameModel::setCurrentZone(Zone z) { m_currentZone = z; }


/**
 * Handles game's evolution
 * (elements apparition, behaviours, deletion)
 * and game mode changing
 * @author Arthur
 * @date 21/02/16 - 26/12/16
 */
void GameModel::nextStep()
{
    chrono::system_clock::duration currentNextStepDelay = chrono::system_clock::now() - m_lastTime;

	if (m_gameState == RUNNING || m_gameState == RUNNING_SLOWLY)
	{
	    //=== Handle Movable Elements Collisions

        //outside of delay otherwise some high speed collisions are not triggered
        handleMovableElementsCollisions();

		if ( currentNextStepDelay > chrono::milliseconds( NEXT_STEP_DELAY ) )
		{
            //=== Update distance and gameSpeed

            if (m_gameSpeed < SPEED_LIMIT && m_gameState != RUNNING_SLOWLY)
            {
                m_gameSpeed += SPEED_STEP;
            }

            m_dataBase->increaseCurrentDistance(m_gameSpeed/5);


			//=== Handle Movable Elements Creation & Deletion

			if (!m_isTransitionPossible)
                handleMovableElementsCreation();

            handleMovableElementsDeletion();


            //=== Bonus timeout & ending

            if ( m_bonusTimeout > chrono::milliseconds(0))
                m_bonusTimeout -= chrono::milliseconds( NEXT_STEP_DELAY );

            if ( m_bonusTimeout <= chrono::milliseconds(0)
                 && m_player->getState() != SHIELD)
            {
                m_player->changeState(NORMAL);
            }

            if ( m_bonusTimeout <= chrono::milliseconds(0)
                 && m_gameState == RUNNING_SLOWLY)
            {
                m_gameState = RUNNING;
                if ( m_gameSpeed == m_gameSlowSpeed)
                    m_gameSpeed *= 1.5;
            }


            //=== Handle transition status

            if ( m_dataBase->getCurrentDistance() != 0
                && m_dataBase->getCurrentDistance()%ZONE_CHANGING_DISTANCE == 0)
                m_isTransitionPossible = true;


            //=== Go to Game Over

            if (m_player->getLife() == 0)
                m_gameState = OVER;

            m_lastTime = chrono::system_clock::now();
        }
    }
    else if (m_gameState == OVER)
    {
        m_dataBase->setCurrentScore(m_gameSpeed);
    }
}


/**
 * Chooses the time-spacing between elements
 * @author Arthur
 * @date 12/03/16 - 24/05/16
 *
 * @param elementType the type of element that was just created
 */
void GameModel::chooseTimeSpacing(int elementType)
{
    //allows to calculate time-spacing in different situations

    if  ( elementType  == STANDARD_ENEMY ) //any enemy
    {
        if (m_chosenEnemyTimeSpacing > 40)
            m_chosenEnemyTimeSpacing = abs(rand()%31); //0 to 30m
        else if  ( m_chosenEnemyTimeSpacing < 10)
            m_chosenEnemyTimeSpacing = 10 + abs(rand()%41); //10 to 50m
        else
            m_chosenEnemyTimeSpacing = abs(rand()%41); //0 to 40m

        if ( m_dataBase->getDifficulty() != EASY )
            m_chosenEnemyTimeSpacing /= 2;
    }

    else if ( elementType == COIN )
    {
        if (m_chosenCoinTimeSpacing > 10)
            m_chosenCoinTimeSpacing = abs(rand()%21); //0 to 10m
        else if  ( m_chosenCoinTimeSpacing < 10)
            m_chosenCoinTimeSpacing = 10 + abs(rand()%11); //10 to 20m
        else
            m_chosenCoinTimeSpacing = abs(rand()%21); //0 to 20m
    }

    else if ( elementType == PV_PLUS_BONUS ) //any Bonus
    {
        if (m_chosenBonusTimeSpacing > 300)
            m_chosenBonusTimeSpacing = 200 + abs(rand()%101); //200 to 300m
        else if  ( m_chosenBonusTimeSpacing < 275)
            m_chosenBonusTimeSpacing = 300 + abs(rand()%101); //300 to 400m
        else
            m_chosenBonusTimeSpacing = 200 + abs(rand()%201); //200 to 400m
    }
}


/**
 * Checks if a position is free to use
 * @author Arthur
 * @date 08/03/16 - 30/04/16
 *
 * @param x element x-position
 * @param y element y-position
 */
bool GameModel::checkIfPositionFree(float x, float y) const
{
    bool positionIsFree =true;
    set<MovableElement*>::iterator it=m_movableElementsArray.begin();

    while (positionIsFree &&  it != m_movableElementsArray.end() )
    {
        if ( (*it)->contains(x,y) )
            positionIsFree = false;
        else
            ++it;
    }

    return positionIsFree;
}


/**
 * NewMovableElement vector cleaning
 * @author Arthur, Florian
 * @date 02/03/16
 */
void GameModel::clearNewMovableElementList()
{
    m_newMovableElementsArray.clear();
}


/**
 * Elements Moving (enemies, bonus, ...)
 * @author Arthur
 * @date 06/03/16 - 26/03/16
 *
 * @param currentElement the element to move
 */
void GameModel::moveMovableElement(MovableElement *currentElement)
{
    if( currentElement == m_player)
        currentElement->move();
    else if( currentElement != NULL )
    {
        currentElement->setMoveX(-1*m_gameSpeed);
        currentElement->move();
    }
}

/**
 * Handles Elements Creation
 * @author Arthur
 * @date 12/04/16 - 24/05/16
 */
void GameModel::handleMovableElementsCreation()
{
    if (checkIfPositionFree(m_width, GAME_FLOOR))
    {
        //=== Add new enemies

        if (m_currentEnemyTimeSpacing >= m_chosenEnemyTimeSpacing)
        {
            addANewMovableElement(m_width, GAME_FLOOR, STANDARD_ENEMY);
            m_currentEnemyTimeSpacing = 0;
            chooseTimeSpacing(STANDARD_ENEMY);
            return; //to not add another element if time-spacing valid
        }
        else m_currentEnemyTimeSpacing++;

        //=== Add new Coins

        if (m_currentCoinTimeSpacing >= m_chosenCoinTimeSpacing)
        {
            addANewMovableElement(m_width, GAME_FLOOR-rand()%100, COIN);
            m_currentCoinTimeSpacing = 0;
            chooseTimeSpacing(COIN);
            return;
        }
        else m_currentCoinTimeSpacing++;

        //=== Add new Bonus

        if (m_currentBonusTimeSpacing >= m_chosenBonusTimeSpacing)
        {
            addANewMovableElement(m_width, BONUS_ROW, PV_PLUS_BONUS);
            m_currentBonusTimeSpacing = 0;
            chooseTimeSpacing(PV_PLUS_BONUS);
            return;
        }
        else m_currentBonusTimeSpacing++;
    }
}


/**
 * New MovableElement adding
 * @author Arthur, Florian
 * @date 25/02/16 - 18/05/16
 *
 * @param posX the initial x-position of the new element
 * @param posY the initial y-position of the new element
 * @param type the type of the new element
 */
void GameModel::addANewMovableElement(float posX, float posY, int type)
{
    MovableElement *m_newMElement = nullptr;
    if (type == PLAYER)
    {
        m_player = new Player(posX, posY, 30, 30, 2.0, 18.0);
        m_newMElement = m_player;
    }
    else if (type == STANDARD_ENEMY)//any enemy, transformation in CTOR
    {
        m_newMElement = new Enemy(posX, posY, 30, 30, ELEMENT_MOVE_X, ELEMENT_MOVE_Y);
    }
    else if (type == COIN)
    {
        m_newMElement = new Coin(posX, posY, 25, 25, ELEMENT_MOVE_X, ELEMENT_MOVE_Y);
    }
    else if (type == PV_PLUS_BONUS) //any bonus, transformation in CTOR
    {
        m_newMElement = new Bonus(posX, posY, 25, 25, ELEMENT_MOVE_X, ELEMENT_MOVE_Y);
    }
    assert(m_newMElement != nullptr);

    if (m_movableElementsArray.find(m_newMElement) == m_movableElementsArray.end())
    {
        m_newMovableElementsArray.insert( m_newMElement );
        m_movableElementsArray.insert( m_newMElement );
    }
}


/**
 * Handles Movable Elements Collisions
 * @author Arthur
 * @date 12/03/16 - 20/05/16
 */
void GameModel::handleMovableElementsCollisions()
{
    for ( MovableElement* element : m_movableElementsArray )
    {
        if ( !element->isColliding() && element->getType() != PLAYER && m_player->collision(*element))
        {
            element->setColliding(true);

            //=== Different behaviours following element type

            switch ( element->getType() )
            {
            case STANDARD_ENEMY:
                if ( m_player->getState() == MEGA ) {
                    //add 100 to number of flattened enemies
                    m_dataBase->increaseCurrentFlattenedEnemies(100);
                }
                else if (m_player->getState() == SHIELD
                         && m_bonusTimeout != chrono::milliseconds(SHIELD_TIMEOUT))
                {
                    if ( m_dataBase->getActivatedItemsArray().find("shieldPlus")
                            == m_dataBase->getActivatedItemsArray().end() )
                        m_player->changeState(NORMAL);
                    else
                        m_bonusTimeout = chrono::milliseconds(SHIELD_TIMEOUT);
                }
                else if (m_player->getState() == SHIELD)
                    m_player->changeState(NORMAL);
                else {
                    if (m_dataBase->getDifficulty() == EASY)
                        m_player->setLife(m_player->getLife()-10);
                    else
                        m_player->setLife(m_player->getLife()-20);
                }
                break;

            case TOTEM_ENEMY:
                 if ( m_player->getState() == MEGA ) {
                    //add 100 to number of flattened enemies
                     m_dataBase->increaseCurrentFlattenedEnemies(300);
                }
                else if (m_player->getState() == SHIELD
                         && m_bonusTimeout != chrono::milliseconds(SHIELD_TIMEOUT))
                {
                    if ( m_dataBase->getActivatedItemsArray().find("shieldPlus")
                            == m_dataBase->getActivatedItemsArray().end() )
                        m_player->changeState(NORMAL);
                    else
                        m_bonusTimeout = chrono::milliseconds(SHIELD_TIMEOUT);
                }
                else if (m_player->getState() == SHIELD)
                    m_player->changeState(NORMAL);
                else {
                    if (m_dataBase->getDifficulty() == EASY)
                        m_player->setLife(m_player->getLife()-15);
                    else
                        m_player->setLife(m_player->getLife()-30);
                }
                break;

            case BLOCK_ENEMY:
                 if ( m_player->getState() == MEGA ) {
                    //add 100 to number of flattened enemies
                     m_dataBase->increaseCurrentFlattenedEnemies(500);
                }
                else if (m_player->getState() == SHIELD
                         && m_bonusTimeout != chrono::milliseconds(SHIELD_TIMEOUT))
                {
                    if ( m_dataBase->getActivatedItemsArray().find("shieldPlus")
                            == m_dataBase->getActivatedItemsArray().end() )
                        m_player->changeState(NORMAL);
                    else
                        m_bonusTimeout = chrono::milliseconds(SHIELD_TIMEOUT);
                }
                else if (m_player->getState() == SHIELD)
                    m_player->changeState(NORMAL);
                else {
                    if (m_dataBase->getDifficulty() == EASY)
                        m_player->setLife(m_player->getLife()-25);
                    else
                        m_player->setLife(m_player->getLife()-50);
                }
                break;

            case COIN:
                if ( m_dataBase->getActivatedItemsArray().find("doubler")
                     == m_dataBase->getActivatedItemsArray().end() )
                    m_dataBase->increaseCurrentCoinsCollected(1); //1-increment coins number
                else
                    m_dataBase->increaseCurrentCoinsCollected(2);
                break;

            case PV_PLUS_BONUS:
                m_player->setLife(m_player->getLife()+10);
                break;

            case MEGA_BONUS:
            {
                int mega_bonus_timeout = 10000; //stop Mega bonus effect in 10s
                if ( m_dataBase->getActivatedItemsArray().find("megaPlus")
                     != m_dataBase->getActivatedItemsArray().end() )
                    mega_bonus_timeout += 5000;

                m_player->changeState(MEGA);
                m_bonusTimeout = chrono::milliseconds(mega_bonus_timeout);
            }
                break;

            case FLY_BONUS:
            {
                int fly_bonus_timeout = 15000; //stop Fly bonus effect in 15s
                if ( m_dataBase->getActivatedItemsArray().find("flyPlus")
                     != m_dataBase->getActivatedItemsArray().end() )
                    fly_bonus_timeout += 5000;

                m_player->changeState(FLY);
                m_bonusTimeout = chrono::milliseconds(fly_bonus_timeout);
            }
                break;

            case SLOW_SPEED_BONUS:
            {
                m_gameState = RUNNING_SLOWLY;
                int slowSpeed_bonus_timeout = 20000;
                m_gameSpeed = m_gameSpeed/2;
                m_gameSlowSpeed = m_gameSpeed;
                m_bonusTimeout = chrono::milliseconds(slowSpeed_bonus_timeout);
            }
                break;

            case SHIELD_BONUS:
            {
                m_player->changeState(SHIELD);
            }
                break;

            default:
                break;
            }
        }
    }
}

/**
 * Handles Movable Elements Deletion \n
 * Note : Pointers will only be deleted in Dtor
 *
 * @author Arthur
 * @date 12/03/16 - 11/04/16
 */
void GameModel::handleMovableElementsDeletion()
{
    set<MovableElement*>::iterator it = m_movableElementsArray.begin();
    while( it!=m_movableElementsArray.end() )
    {
        if ( ( (*it)->getPosX() + (*it)->getWidth() ) < 0
             || (*it)->isColliding())
        {
            m_movableElementsArray.erase(it);
            it = m_movableElementsArray.end();
        }
        else
            ++it;
    }
}
