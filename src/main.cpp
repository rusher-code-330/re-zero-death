#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include <Geode/modify/PlayLayer.hpp>
#include <fmod.hpp>

class $modify(SubaruDeath, PlayLayer) {
    static inline FMOD::Sound* s_returningbydeath = nullptr;
    static inline CCSprite* s_subarustaire = nullptr;
    
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

        displaytheimage();
    }

    void displaytheimage() {
        if (s_subarustaire) {
            s_subarustaire->removeFromParentAndCleanup(true);
            s_subarustaire = nullptr;
        }
        auto image = CCSprite::create("subarustaire.png"_spr);
        if (!image) {
            log::error("Failed to create sprite for returningbydeath");
            return;
        }
        auto winSize = CCDirector::sharedDirector()->getWinSize();
        image->setPosition(winSize / 2);
        image->setScale(2.0f); // Adjust the scale as needed
        this->addChild(image);
        s_subarustaire = image;
        image->runAction(CCSequence::create(
            CCDelayTime::create(0.2f),
            CCFadeOut::create(0.1f),
            CCCallFunc::create(this, callfunc_selector(SubaruDeath::removeImage)),
            nullptr
        ));
    }

    void removeImage() {
        if (s_subarustaire) {
            s_subarustaire->removeFromParentAndCleanup(true);
            s_subarustaire = nullptr;
        }
    }

    ~SubaruDeath() {
        if (s_returningbydeath) {
            s_returningbydeath->release();
            s_returningbydeath = nullptr;
        }
        removeImage();
    }
};    
