
#include "QuickCutShared/QuickCutPCH.h"
#include "ProfileManager.h"

ProfileManager::ProfileManager() noexcept
    : BaseManager()
    , m_Parser("Config/profiles.json")
    , m_LoadSucceed(false)
{
}

ProfileManager::ProfileManager(const QString & filePath) noexcept
    : BaseManager()
    , m_Parser(filePath)
    , m_LoadSucceed(false)
{
}

ProfileManager::ProfileManager(QString && filePath) noexcept
    : BaseManager()
    , m_Parser(std::move(filePath))
    , m_LoadSucceed(false)
{
}

ProfileManager::~ProfileManager() = default;

bool ProfileManager::load()
{
    m_LoadSucceed = m_Parser.parse(&m_Data);
    return m_LoadSucceed;
}

bool ProfileManager::save()
{
    return m_Parser.save(m_Data);
}

bool ProfileManager::isFileExists()
{
    return QFile::exists(m_Parser.getFilePath());
}

bool ProfileManager::isLoadSucceed()
{
    return m_LoadSucceed;
}

const QString & ProfileManager::getConfigFilePath()
{
    return m_Parser.getFilePath();
}

std::shared_ptr<Profile> ProfileManager::getActiveProfile()
{
    auto activeProfile = std::find_if(m_Data.begin(), m_Data.end(),
                                      [](auto profile) { return profile->isActive(); });
    if (activeProfile == m_Data.end()) return nullptr;

    return *activeProfile;
}

const std::shared_ptr<Profile> ProfileManager::getActiveProfile() const
{
    auto activeProfile = std::find_if(m_Data.begin(), m_Data.end(),
                                      [](auto profile) { return profile->isActive(); });
    if (activeProfile == m_Data.end()) return nullptr;

    return *activeProfile;
}

void ProfileManager::setActiveProfile(int index)
{
    if (index < 0 || index >= count()) return;

    auto profile = m_Data[index];
    if (!profile) return;

    for (auto && p : m_Data) p->setActive(false);
    profile->setActive(true);
}

void ProfileManager::setActiveProfile(std::shared_ptr<Profile> profile)
{
    if (!profile) return;

    auto itr = std::find(m_Data.begin(), m_Data.end(), profile);
    if (itr == m_Data.end()) return;

    for (auto && p : m_Data) p->setActive(false);
    (*itr)->setActive(true);
}
