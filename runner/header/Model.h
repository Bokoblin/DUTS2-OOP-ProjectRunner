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

#ifndef _MODEL_H
#define _MODEL_H
#include "Ball.h"

class Model
{
public:
    Model(int width, int height);
    ~Model();

    Ball getBall();
    void nextStep();
    void moveBall(bool left);


private:
    int m_modelWidth;
    int m_modelHeight;

    Ball *m_player;
    std::vector<MovableElement*> m_listElements;   //tableau dynamique de MovableElements

};
#endif
