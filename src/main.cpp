#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include <Geode/modify/PlayLayer.hpp>
#include <fmod.hpp>

class $modify(SubaruDeath, PlayLayer) {
    static inline FMOD::Sound* s_lastSound = nullptr;
    void resetLevel() {
        PlayLayer::resetLevel();
        if (s_lastSound) {
            s_lastSound->release();
            s_lastSound = nullptr;
        }
        auto audioFile = Mod::get()->getResourcesDir() / "returningbydeath.ogg";
        if (!std::filesystem::exists(audioFile)) return;
        FMOD::Sound* sound = nullptr;
        FMOD::Channel* channel = nullptr;
        FMOD::System* system = FMODAudioEngine::sharedEngine()->m_system;
        if (system){
            system->createSound(
                geode::utils::string::pathToString(audioFile).c_str(),
                FMOD_DEFAULT, nullptr, &sound
            );
        }
        if (!sound) return;
        s_lastSound = sound;
        system->playSound(sound, nullptr, false, &channel);
        if (channel) {
            channel->setVolume(Mod::get()->getSettingValue<float>("volume"));
        }
    }
    
};
