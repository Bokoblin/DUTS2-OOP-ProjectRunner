/* Copyright 2016 Jolivet Arthur & Laronze Florian

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifndef MOVABLEELEMENT_H
#define MOVABLEELEMENT_H

#include <string>

/********************************************
    MovableElement Abstract Class
*********************************************
    @author Arthur  @date 23/02 - 12/04
    @author Florian @date 02/03 - 06/04
*********************************************/
class MovableElement
{
public:
    //=== CTORs / DTORs
    MovableElement(float posX, float posY, float w, float h, float mvX, float mvY);
    virtual ~MovableElement();

    //=== METHODS
    virtual void move()=0;
    bool contains(float posX, float posY) const ;
    bool collision( const MovableElement & other) const ;

    //=== GETTERS
    float getPosX() const;
    float getPosY() const;
    float getMoveX() const;
    float getMoveY() const;
    float getWidth() const;
    float getHeight() const;
    bool getCollisionState() const;
    virtual int getType() const;

    //=== SETTERS
    void setMoveX(float mvX);
    void setMoveY(float mvY);
    void setCollisionState(bool collisionState);


protected:
    //=== ATTRIBUTES
    float m_posX;
    float m_posY;
    float m_width;
    float m_height;
    float m_moveX;
    float m_moveY;
    bool m_collisionState;
    int m_elementType;
    /** Element Type :
    0 : Player
    1 : Standard Enemy
    2 : Totem Enemy
    3 : Block Enemy
    4 : Coin
    5 : Bonus - PV+
    6 : Bonus - MEGA
    7 : Bonus - FLY
    8 : Bonus - SLOW SPEED
    */

};

#endif // MOVABLEELEMENT_H
