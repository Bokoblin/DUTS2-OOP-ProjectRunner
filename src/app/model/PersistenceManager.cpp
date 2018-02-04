#include "PersistenceManager.h"

using std::string;
using std::to_string;
using std::stoi;
using Bokoblin::SimpleLogger::Logger;

//------------------------------------------------
//          STATIC MEMBERS
//------------------------------------------------

bool PersistenceManager::m_isInit = false;
DataBase* PersistenceManager::m_database = nullptr;

//------------------------------------------------
//          PUBLIC METHODS
//------------------------------------------------

/**
 * Inits the app persistence manager context
 * and the persistence system
 *
 * @author Arthur
 * @date 04/02/17
 */
void PersistenceManager::initPersistenceManager(DataBase *database)
{
    m_database = database;
    m_isInit = true;

    initPersistence();
}

/**
 * Closes the persistence manager context
 *
 * @author Arthur
 * @date 04/02/17
 */
void PersistenceManager::closePersistenceManager()
{
    m_isInit = false;
    m_database = nullptr;
}


/**
 * Inits the app persistence system
 * by checking existence and creating otherwise
 *
 * @author Arthur
 * @date 04/02/17
 */
void PersistenceManager::initPersistence()
{
    pugi::xml_document doc; //TODO: Maybe use as member ?

    if (XMLPersistenceHelper::loadXMLFile(doc, CONFIG_FILE))
        Logger::printInfoOnConsole("Persistence loaded successfully");
    else
        createConfigFile();
}


/**
 * Checks if persistence context is valid,
 * takes the necessary measures otherwise
 *
 * @author Arthur
 * @date 04/02/17
 */
void PersistenceManager::checkPersistence()
{
    if (m_isInit && m_database != nullptr && checkConfigFileIntegrity())
        Logger::printInfoOnConsole("Persistence verified");
    else
    {
        Logger::printErrorOnConsole("Persistence check failed, please init persistence first..."); //TODO change
        throw PersistenceException();
    }
}

/**
 * Fetches the activated items
 * from persistence system
 *
 * @author Arthur
 * @date 04/02/17
 */
void PersistenceManager::fetchActivatedBonus()
{
    PersistenceManager::checkPersistence();

    if (fetchActivatedBonusFromConfigFile())
        Logger::printInfoOnConsole("Activated bonus successfully fetched");
    else
        Logger::printErrorOnConsole("Activated bonus fetching failure");
}


/**
 * Fetches the app configuration
 * from persistence system
 *
 * @author Arthur
 * @date 04/02/17
 */
void PersistenceManager::fetchConfiguration()
{
    PersistenceManager::checkPersistence();

    if (fetchConfigurationFromConfigFile())
        Logger::printInfoOnConsole("Configuration successfully fetched");
    else {
        Logger::printErrorOnConsole("Configuration fetching failure");
    }
}


/**
 * Fetches the leaderboard
 * from persistence system
 *
 * @author Arthur
 * @date 04/02/17
 */
void PersistenceManager::fetchLeaderboard()
{
    PersistenceManager::checkPersistence();

    if (fetchLeaderboardFromConfigFile())
        Logger::printInfoOnConsole("Leaderboard successfully fetched");
    else
    {
        Logger::printErrorOnConsole("Leaderboard fetching failure");
    }
}

/**
 * Updates the app configuration
 * of the persistence system
 *
 * @author Arthur
 * @date 04/02/17
 */
void PersistenceManager::updatePersistence()
{
    try
    {
        PersistenceManager::checkPersistence();

        if (persistConfigurationToConfigFile())
            Logger::printInfoOnConsole("Configuration successfully persisted");
        else
            Logger::printErrorOnConsole("Configuration persistence failure");
    }
    catch (const PersistenceException& e)
    {
        Logger::printErrorOnConsole("Persistence failure");
    }

}

/**
 * Deletes the persistence context.
 * Does nothing
 *
 * @author Arthur
 * @date 04/02/17
 */
void PersistenceManager::deletePersistence()
{
    remove(CONFIG_FILE.c_str());
}


//------------------------------------------------
//          PRIVATE METHODS
//------------------------------------------------

/**
 * (Re)creates config file
 *
 * @author Arthur
 * @date 02/05/16 - 22/01/18
 */
void PersistenceManager::createConfigFile()
{
    XMLPersistenceHelper::createXMLFile(CONFIG_FILE, DEFAULT_CONFIG_CONTENT);
    Logger::printInfoOnConsole("Configuration file successfully created");
}


/**
 * TODO : NON-TRIVIAL REWRITE with new structure
 * see: https://pugixml.org/docs/manual.html#loading.errors in XMLHelper
 *
 * Checks if config file has been corrupted
 * by verifying file number of lines, number of each item
 *
 * @return a boolean indicating if file is corrupted
 *
 * @author Arthur
 * @date 02/05/16 - 25/01/17
 */
bool PersistenceManager::checkConfigFileIntegrity()
{
    std::fstream f;
    string line;

    f.open(CONFIG_FILE.c_str(), std::ios::in);

    //=== Opening test
    if (f.fail())
        return false;

    //=== Count lines / elements test
    bool isPresentConfig = false; //should be true
    bool isPresentStats = false; //should be true
    bool isPresentShop = false; //should be true
    bool isPresentScoreEasy = false; //should be true
    bool isPresentScoreHard = false; //should be true
    int nbLines = 0; //should be 52 or 53
    int nbConfigChildren = 0; //should be 6
    int nbStatsChildren = 0; //should be 7
    int nbShopChildren = 0; //should be 6
    int nbScoreChildren = 0; //should be 20

    do {
        getline(f, line, '\n');
        nbLines++;

        unsigned long long int found = line.find("<config>");
        if (found != string::npos)
            isPresentConfig = true;

        found = line.find("<stats>");
        if (found != string::npos)
            isPresentStats = true;

        found = line.find("<shop>");
        if (found != string::npos)
            isPresentShop = true;

        found = line.find("<scoresEasy>");
        if (found != string::npos)
            isPresentScoreEasy = true;

        found = line.find("<scoresHard>");
        if (found != string::npos)
            isPresentScoreHard = true;

        found = line.find("<configItem");
        if (found != string::npos)
            nbConfigChildren++;

        found = line.find("<statItem");
        if (found != string::npos)
            nbStatsChildren++;

        found = line.find("<shopItem");
        if (found != string::npos)
            nbShopChildren++;

        found = line.find("<scoreItem");
        if (found != string::npos)
            nbScoreChildren++;
    } while (!f.eof());

    return isPresentConfig && isPresentStats && isPresentShop && isPresentScoreEasy && isPresentScoreHard
            && nbConfigChildren == 6 && nbStatsChildren == 7 && nbShopChildren == 6 && nbScoreChildren == 20
            && (nbLines == 52 || nbLines == 53);
}


/**
 * Updates variable values from file
 *
 * @author Arthur
 * @date 24/10/16 - 26/01/18
 */
bool PersistenceManager::fetchConfigurationFromConfigFile()
{
    pugi::xml_document doc;
    if (XMLPersistenceHelper::loadXMLFile(doc, CONFIG_FILE))
    {
        pugi::xml_node config = doc.child("runner").child("config");
        pugi::xml_node stats = doc.child("runner").child("stats");

        for (pugi::xml_node configItem: config.children("configItem"))
        {
            //TODO: improve with multi-map ??
            if (string(configItem.attribute("name").value()) == "language")
            {
                m_database->m_currentLanguage = XMLPersistenceHelper::safeRetrieveXMLValue<string>
                        (configItem.attribute("value"), "en|fr|es", ENGLISH);
            }
            else if (string(configItem.attribute("name").value()) == "difficulty")
            {
                m_database->m_currentDifficulty = XMLPersistenceHelper::safeRetrieveXMLValue<int>
                        (configItem.attribute("value"), "1|2", HARD);
            }
            else if (string(configItem.attribute("name").value()) == "ball_skin")
            {
                m_database->m_currentBallSkin = XMLPersistenceHelper::safeRetrieveXMLValue<string>
                        (configItem.attribute("value"), "default|morphing|capsule", "default");
            }
            else if (string(configItem.attribute("name").value()) == "wallet")
            {
                m_database->m_wallet = XMLPersistenceHelper::safeRetrieveXMLValue<unsigned int>
                        (configItem.attribute("value"), "^(0|[1-9][0-9]*)$", 0);
            }
            else if (string(configItem.attribute("name").value()) == "menu_music")
            {
                m_database->m_isMenuMusicEnabled = XMLPersistenceHelper::safeRetrieveXMLValue<bool>
                        (configItem.attribute("value"), "true|false", false);
            }
            else if (string(configItem.attribute("name").value()) == "game_music")
            {
                m_database->m_isGameMusicEnabled = XMLPersistenceHelper::safeRetrieveXMLValue<bool>
                        (configItem.attribute("value"), "true|false", false);
            }
        }

        for (pugi::xml_node statItem: stats.children("statItem"))
        {
            if (string(statItem.attribute("name").value()) == "total_coins_collected")
            {
                m_database->m_totalCoinsCollected = XMLPersistenceHelper::safeRetrieveXMLValue<unsigned int>
                        (statItem.attribute("value"), "^(0|[1-9][0-9]*)$", 0);
            }
            else if (string(statItem.attribute("name").value()) == "total_distance_travelled")
            {
                m_database->m_totalDistance = XMLPersistenceHelper::safeRetrieveXMLValue<unsigned int>
                        (statItem.attribute("value"), "^(0|[1-9][0-9]*)$", 0);
            }
            else if (string(statItem.attribute("name").value()) == "total_enemies_destroyed")
            {
                m_database->m_totalFlattenedEnemies = XMLPersistenceHelper::safeRetrieveXMLValue<unsigned int>
                        (statItem.attribute("value"), "^(0|[1-9][0-9]*)$", 0);
            }
            else if (string(statItem.attribute("name").value()) == "per_game_coins_collected")
            {
                m_database->m_perGameCoinsCollected = XMLPersistenceHelper::safeRetrieveXMLValue<unsigned int>
                        (statItem.attribute("value"), "^(0|[1-9][0-9]*)$", 0);
            }
            else if (string(statItem.attribute("name").value()) == "per_game_distance_travelled")
            {
                m_database->m_perGameDistance = XMLPersistenceHelper::safeRetrieveXMLValue<unsigned int>
                        (statItem.attribute("value"), "^(0|[1-9][0-9]*)$", 0);
            }
            else if (string(statItem.attribute("name").value()) == "per_game_enemies_destroyed")
            {
                m_database->m_perGameFlattenedEnemies = XMLPersistenceHelper::safeRetrieveXMLValue<unsigned int>
                        (statItem.attribute("value"), "^(0|[1-9][0-9]*)$", 0);
            }
            else if (string(statItem.attribute("name").value()) == "total_games_played")
            {
                m_database->m_totalGamesPlayed = XMLPersistenceHelper::safeRetrieveXMLValue<unsigned int>
                        (statItem.attribute("value"), "^(0|[1-9][0-9]*)$", 0);
            }
        }
        return true;
    }

    return false;
}


/**
 * Updates each score array
 * with values from config file
 *
 * @author Arthur
 * @date 23/10/16 - 26/01/18
 */
bool PersistenceManager::fetchLeaderboardFromConfigFile()
{
    pugi::xml_document doc;
    if (XMLPersistenceHelper::loadXMLFile(doc, CONFIG_FILE))
    {
        pugi::xml_node scoresEasy = doc.child("runner").child("scoresEasy");
        pugi::xml_node scoresHard = doc.child("runner").child("scoresHard");

        m_database->m_scoresEasyArray.clear();
        m_database->m_scoresHardArray.clear();

        for (pugi::xml_node scoreItem: scoresEasy.children("scoreItem"))
        {
            if (string(scoreItem.attribute("value").value()) != "0")
            {
                m_database->m_scoresEasyArray.insert(XMLPersistenceHelper::safeRetrieveXMLValue<unsigned int>
                                                             (scoreItem.attribute("value"), "^(0|[1-9][0-9]*)$", 0));
            }
        }
        for (pugi::xml_node scoreItem: scoresHard.children("scoreItem"))
        {
            if (string(scoreItem.attribute("value").value()) != "0")
            {
                m_database->m_scoresHardArray.insert(XMLPersistenceHelper::safeRetrieveXMLValue<unsigned int>
                                                             (scoreItem.attribute("value"), "^(0|[1-9][0-9]*)$", 0));
            }
        }
        return true;
    }

    return false;
}


/**
 * Updates activated items array
 *
 * @author Arthur
 * @date 14/05/16 - 26/01/18
 */
bool PersistenceManager::fetchActivatedBonusFromConfigFile()
{
    pugi::xml_document doc;
    if (XMLPersistenceHelper::loadXMLFile(doc, CONFIG_FILE))
    {
        pugi::xml_node shop = doc.child("runner").child("shop");

        for (pugi::xml_node shopItem: shop.children("shopItem"))
        {
            if (string(shopItem.attribute("bought").value()) == "true")
            {
                m_database->m_activatedItemsArray.insert(XMLPersistenceHelper::safeRetrieveXMLValue<string>
                                                                 (shopItem.attribute("id"), "shop_[a-z|A-Z]+", ""));
            }
        }
        return true;
    }

    return false;
}



/**
 * Pushes Configuration data to file
 *
 * @author Arthur
 * @date 02/05/16 - 26/01/18
 */
bool PersistenceManager::persistConfigurationToConfigFile()
{
    pugi::xml_document doc;
    if (XMLPersistenceHelper::loadXMLFile(doc, CONFIG_FILE))
    {
        pugi::xml_node config = doc.child("runner").child("config");
        pugi::xml_node stats = doc.child("runner").child("stats");
        pugi::xml_node shop = doc.child("runner").child("shop");
        pugi::xml_node scoresEasy = doc.child("runner").child("scoresEasy");
        pugi::xml_node scoresHard = doc.child("runner").child("scoresHard");

        //=== Save configuration

        for (pugi::xml_node configItem: config.children("configItem"))
        {
            if (string(configItem.attribute("name").value()) == "language")
            {
                pugi::xml_attribute nodeValue = configItem.attribute("value");
                nodeValue.set_value(m_database->m_currentLanguage.c_str());
            }
            else if (string(configItem.attribute("name").value()) == "difficulty")
            {
                pugi::xml_attribute nodeValue = configItem.attribute("value");
                nodeValue.set_value((to_string(m_database->m_currentDifficulty)).c_str());
            }
            else if (string(configItem.attribute("name").value()) == "ball_skin")
            {
                pugi::xml_attribute nodeValue = configItem.attribute("value");
                nodeValue.set_value(m_database->m_currentBallSkin.c_str());
            }
            else if (string(configItem.attribute("name").value()) == "wallet")
            {
                pugi::xml_attribute nodeValue = configItem.attribute("value");
                nodeValue.set_value((to_string(m_database->m_wallet)).c_str());
            }
            else if (string(configItem.attribute("name").value()) == "menu_music")
            {
                pugi::xml_attribute nodeValue = configItem.attribute("value");
                nodeValue.set_value(m_database->m_isMenuMusicEnabled);
            }
            else if (string(configItem.attribute("name").value()) == "game_music")
            {
                pugi::xml_attribute nodeValue = configItem.attribute("value");
                nodeValue.set_value(m_database->m_isGameMusicEnabled);
            }
        }

        //=== Save stats

        for (pugi::xml_node statItem: stats.children("statItem"))
        {
            if (string(statItem.attribute("name").value()) == "total_coins_collected")
            {
                pugi::xml_attribute nodeValue = statItem.attribute("value");
                nodeValue.set_value((to_string(m_database->m_totalCoinsCollected)).c_str());
            }
            else if (string(statItem.attribute("name").value()) == "total_distance_travelled")
            {
                pugi::xml_attribute nodeValue = statItem.attribute("value");
                nodeValue.set_value((to_string(m_database->m_totalDistance)).c_str());
            }
            else if (string(statItem.attribute("name").value()) == "total_enemies_destroyed")
            {
                pugi::xml_attribute nodeValue = statItem.attribute("value");
                nodeValue.set_value((to_string(m_database->m_totalFlattenedEnemies)).c_str());
            }
            else if (string(statItem.attribute("name").value()) == "per_game_coins_collected")
            {
                pugi::xml_attribute nodeValue = statItem.attribute("value");
                nodeValue.set_value((to_string(m_database->m_perGameCoinsCollected)).c_str());
            }
            else if (string(statItem.attribute("name").value()) == "per_game_distance_travelled")
            {
                pugi::xml_attribute nodeValue = statItem.attribute("value");
                nodeValue.set_value((to_string(m_database->m_perGameDistance)).c_str());
            }
            else if (string(statItem.attribute("name").value()) == "per_game_enemies_destroyed")
            {
                pugi::xml_attribute nodeValue = statItem.attribute("value");
                nodeValue.set_value((to_string(m_database->m_perGameFlattenedEnemies)).c_str());
            }
            else if (string(statItem.attribute("name").value()) == "total_games_played")
            {
                pugi::xml_attribute nodeValue = statItem.attribute("value");
                nodeValue.set_value((to_string(m_database->m_totalGamesPlayed)).c_str());
            }
        }

        //=== Save shop activated items

        for (pugi::xml_node shopItem: shop.children("shopItem"))
        {
            if (m_database->findActivatedItem(string(shopItem.attribute("id").value())))
            {
                shopItem.attribute("bought").set_value(true);
                doc.save_file(CONFIG_FILE.c_str());
            }
        }

        //=== Save leaderboard

        auto it = m_database->m_scoresEasyArray.begin();
        for (pugi::xml_node scoreItem: scoresEasy.children("scoreItem"))
        {
            pugi::xml_attribute nodeValue = scoreItem.attribute("value");
            if (it != m_database->m_scoresEasyArray.end())
            {
                nodeValue.set_value((to_string(*it)).c_str());
                ++it;
            }
            else
                nodeValue.set_value("0");
        }

        it = m_database->m_scoresHardArray.begin();
        for (pugi::xml_node scoreItem: scoresHard.children("scoreItem"))
        {
            pugi::xml_attribute nodeValue = scoreItem.attribute("value");
            if (it != m_database->m_scoresHardArray.end())
            {
                nodeValue.set_value((to_string(*it)).c_str());
                ++it;
            }
            else
                nodeValue.set_value("0");
        }

        return doc.save_file(CONFIG_FILE.c_str());
    }

    return false;
}


/**
 * Retrieves a localized string for a given label.
 * Allows abstraction of the internationalization storage system
 *
 * @param label the label to describe the string
 * @return the localized string
 *
 * @author Arthur
 * @date 04/01/17 - 24/01/18
 */
string PersistenceManager::fetchLocalizedString(const string &label)
{
    //TODO: recover xml once instead
    string currentLocaleFile = m_database->getLanguageFile();
    return XMLPersistenceHelper::loadLabeledString(currentLocaleFile, label);
}