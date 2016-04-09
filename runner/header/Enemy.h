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

#ifndef ENEMY_H
#define ENEMY_H

#include "MovableElement.h"
#include <string>


/********************************************
    Enemy Class
*********************************************
    Arthur : 13/03 - 15/03
*********************************************/
class Enemy : public MovableElement
{
public:
    //=== CTORs / DTORs
    Enemy(unsigned int posX, unsigned int posY, unsigned int w, unsigned int h, int movX, int movY);
    virtual~Enemy();

    //=== METHODS
    void move();
    void chooseEnemyType();

};

#endif // ENEMY_H
