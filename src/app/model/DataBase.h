﻿/* Copyright 2016-2018 Jolivet Arthur & Laronze Florian

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

#ifndef DATABASE_H
#define DATABASE_H

#include <set>
#include "libs/Logger/Logger.h"
#include "app/enum/AppState.h"
#include "app/enum/Difficulty.h"
#include "app/model/PersistenceManager.h"
#include "app/utils/constants.h"

/**
 * The DataBase class concentrates
 * all the app's data for configuration,
 * statistics, buyable items, scores,
 * current game's values, etc.
 * @author Arthur
 * @date 02/05/16 - 27/12/17
 */
class DataBase
{
public:
    //=== CTORs / DTORs
    DataBase();
    DataBase(const DataBase& d)=delete;
    ~DataBase();

    //=== GETTERS
    AppState getAppState() const;
    int getTotalCoinsNumber() const;
    int getTotalDistance() const;
    int getTotalFlattenedEnemies() const;
    int getPerGameCoinsNumber() const;
    int getPerGameDistance() const;
    int getPerGameFlattenedEnemies() const;
    int getTotalGamesPlayed() const;
    int getCurrentCoinsNumber() const;
    int getCurrentDistance() const;
    int getCurrentFlattenedEnemies() const;
    int getCurrentScore() const;
    int getWallet() const;
    int getDifficulty() const;
    bool isMenuMusicEnabled() const;
    bool isGameMusicEnabled() const;
    std::string getLanguage() const;
    std::string getBallSkin() const;
    std::string getLanguageFile() const;

    //=== SETTERS
    void setAppState(AppState state);
    void decreaseWallet(int amount);
    void increaseCurrentCoinsCollected(int amount);
    void increaseCurrentDistance(float amount);
    void increaseCurrentFlattenedEnemies(int amount);
    void setCurrentScore(float speed);
    void setDifficulty(int difficulty);
    void setLanguage(const std::string &language);
    void setBallSkin(const std::string &skin);
    void setMenuMusic(bool on);
    void setGameMusic(bool on);

    //=== METHODS
    void addNewScore(int score);
    void saveCurrentGame();
    void launchNewGame();
    void clearLeaderboard();
    void clearAppData();
    bool findActivatedItem(const std::string &itemLabel);
    void addNewActivatedBonus(const std::string &itemLabel);
    bool isScoreEasyArrayEmpty() const;
    bool isScoreHardArrayEmpty() const;
    std::string stringifyLeaderboard(Difficulty difficulty) const;

private:
    friend class PersistenceManager;

    //=== ATTRIBUTES

    //Global App
    AppState m_appState;
    int m_totalCoinsCollected;
    int m_totalDistance;
    int m_totalFlattenedEnemies;
    int m_totalGamesPlayed;
    int m_perGameCoinsCollected;
    int m_perGameDistance;
    int m_perGameFlattenedEnemies;
    int m_wallet;
    int m_currentDifficulty;
    bool m_isMenuMusicEnabled;
    bool m_isGameMusicEnabled;
    int m_scoreBonusFlattenedEnemies;
    std::string m_currentLanguage;
    std::string m_currentBallSkin;
    const int COIN_MULTIPLIER = 20;
    const int MAX_SCORES = 10;

    //Current Game //TODO: can't be gameModel members only ?
    int m_currentCoinsNumber;
    float m_currentDistance;
    int m_currentFlattenedEnemies;
    int m_currentScore;

    //Containers
    std::set<int> m_scoresEasyArray;
    std::set<int> m_scoresHardArray;
    std::set<std::string> m_activatedItemsArray;

    //=== METHODS
    void initWithDefaultValues();
};

#endif