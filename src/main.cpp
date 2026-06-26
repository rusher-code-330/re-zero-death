#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include <Geode/modify/PlayLayer.hpp>
#include <fmod.hpp>
#include <Geode/modify/MenuLayer.hpp>

class $modify(TheMenuLayer, MenuLayer) {
    static inline CCSprite* s_happybirthday = nullptr;

public:
    bool init() {
        if (!MenuLayer::init()) {
            return false;
        }
        auto bottomMenu=this ->getChildByID("bottom-menu");
        auto button = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("accountBtn_messages_001.png"),
        this,
        menu_selector(TheMenuLayer::onButtonClicked)
        );
    
    auto menu = CCMenu::create();
    menu->addChild(button);
    menu->setPosition({bottomMenu->getContentWidth() - 50.f, 50.f});
    this->addChild(menu);
    return true;
    }

    void onRobTop(CCObject* sender) {
        FLAlertLayer::create("HAPPY BIRTHDAY", "HAPPY BIRTHDAY DANIEL!!! did you really think I’d let this day pass quietly? I know the kind of power you vibe with, the kind that keeps climbing no matter what. So here’s your gift, a profile picture wrapped in that same aura, use it anywhere, you know the rule, EVEN IF THE STAIRS NEVER END, WE KEEP CLIMBING!!!", "ok")->show();
    }

    void onButtonClicked(CCObject* sender) {
        auto alert = FLAlertLayer::create("Daniel", "Daniel, here's a gift for you", "ok");
        auto spr = CCSprite::create("happybirthday.png"_spr);
        if (spr) {
            spr->setScale(0.5f);
            spr->setPosition({alert->m_mainLayer->getContentWidth() / 2, 160.f});
            alert->m_mainLayer->addChild(spr);
        }
        alert->show();
    }
};

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
