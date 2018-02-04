/* Copyright 2016-2018 Jolivet Arthur & Laronze Florian

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

#ifndef MENU_MODEL_H
#define MENU_MODEL_H

#include <chrono>
#include "app/enum/MenuState.h"
#include "app/model/CommandsModel.h"
#include "app/model/LeaderboardModel.h"
#include "app/model/SettingsModel.h"
#include "app/model/ShopModel.h"

/**
 * The MenuModel class inherits AbstractModel.
 * It is the model counterpart of the class MenuView.
 * It handles the menu screen's logic.
 *
 * @author Arthur
 * @date 14/04/16 - 29/12/17
 */
class MenuModel : public AbstractModel
{
public:
    //=== CTORs / DTORs
    explicit MenuModel(DataBase *dataBase);
    ~MenuModel() override;

    //=== GETTERS
    MenuState getMenuState() const;

    //=== SETTERS
    void setMenuState(MenuState state);

    //=== METHODS
    void nextStep() override;
    CommandsModel* launchCommands();
    LeaderboardModel* launchLeaderboard();
    ShopModel* launchShop();
    SettingsModel* launchSettings();

private:
    //=== ATTRIBUTES
    MenuState m_menuState;
    CommandsModel *m_commands;
    LeaderboardModel *m_leaderboard;
    SettingsModel *m_settings;
    ShopModel *m_shop;
    std::chrono::system_clock::time_point m_lastTime;
};

#endif