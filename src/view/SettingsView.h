/* Copyright 2016-2017 Jolivet Arthur & Laronze Florian

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

#ifndef SETTINGS_VIEW_H
#define SETTINGS_VIEW_H

#include "AbstractView.h"
#include "../model/Settings.h"
#include "RadioButton.h"
#include "Dialog.h"


/**
 * SettingsView Class
 * @author Arthur
 * @date 20/05/16 - 25/01/17
 */
class SettingsView : public AbstractView
{
public:
    //=== CTORs / DTORs
    SettingsView(sf::RenderWindow *window, TextHandler *textHandler, Settings *settingsModel);
    virtual ~SettingsView();

    //=== METHODS
    virtual void synchronize() override;
    virtual void draw() const override;
    virtual void loadImages() override;
    virtual bool treatEvents(sf::Event event) override;
    void handleMusic();

private:
    //=== ATTRIBUTES
    Settings *m_settings;
    Dialog *m_confirmDialog;

    //Graphic Elements
    Button *m_homeFormButton;
    Button *m_englishLangRadio;
    Button *m_frenchLangRadio;
    Button *m_spanishLangRadio;
    Button *m_easyModeRadio;
    Button *m_hardModeRadio;
    Button *m_defaultBallSkinRadio;
    Button *m_morphBallSkinRadio;
    Button *m_capsuleBallSkinRadio;
    Button *m_resetRectButton;
    Button *m_menuMusicButton;
    Button *m_gameMusicButton;
    GraphicElement *m_logoIUTSprite;
    GraphicElement *m_logoSFMLSprite;

    //Containers
    std::map<int, RadioButton *> m_pageIndicators;
    std::vector<Button*> m_buttonList;
};


#endif
