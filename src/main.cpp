#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include <Geode/modify/PlayLayer.hpp>
#include <fmod.hpp>
#include <Geode/modify/MenuLayer.hpp>

class $modify(MyMenuLayer, MenuLayer) {
public:
    void onRobTop(CCObject* sender) {
        FLAlertLayer::create("HAPPY BIRTHDAY", "HAPPY BIRTHDAY DANIEL!!! did you really think I’d let this day pass quietly? I know the kind of power you vibe with, the kind that keeps climbing no matter what. So here’s your gift, a profile picture wrapped in that same aura, use it anywhere, you know the rule, EVEN IF THE STAIRS NEVER END, WE KEEP CLIMBING!!!", "ok")->show();
    }
};

class $modify(SubaruDeath, PlayLayer) {
    static inline FMOD::Sound* s_returningbydeath = nullptr;
    
    bool init(GJGameLevel* level, bool useReplays, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplays, dontCreateObjects)) return false;
        auto audioFile = Mod::get()->getResourcesDir() / "returningbydeath.ogg";
        if (std::filesystem::exists(audioFile) && !s_returningbydeath) {
            FMOD::System* system = FMODAudioEngine::sharedEngine()->m_system;
            if (system) {
                system->createSound(
                    geode::utils::string::pathToString(audioFile).c_str(),
                    FMOD_CREATESAMPLE,
                    nullptr,
                    &s_returningbydeath
                );
            }
        }
        return true;
    }
    void resetLevel() {
        PlayLayer::resetLevel();
        if (s_returningbydeath) {
            FMOD::System* system = FMODAudioEngine::sharedEngine()->m_system;
            FMOD::Channel* channel = nullptr;
            system->playSound(s_returningbydeath, nullptr, false, &channel);
            if (channel) {
                channel->setVolume(Mod::get()->getSettingValue<float>("volume"));
            }
        }
    }
    ~SubaruDeath() {
        if (s_returningbydeath) {
            s_returningbydeath->release();
            s_returningbydeath = nullptr;
        }
    }
};    
