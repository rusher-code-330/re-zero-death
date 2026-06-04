#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include <Geode/modify/PlayLayer.hpp>
#include <fmod.hpp>
bool hasDoneThisAttempt = false;

class $modify(SubaruDeath, PlayLayer) {
    void destroyPlayer(PlayerObject* player, GameObject* object) {
        PlayLayer::destroyPlayer(player, object);
        if (object == m_anticheatSpike) return;
        if (m_isPracticeMode) return;
        
        auto audioFile = Mod::get()->getResourcesDir() / "returningbydeath.ogg";
        FMOD::Sound* sound = nullptr;
        FMOD::Channel* channel = nullptr;
        FMOD::System* system = FMODAudioEngine::sharedEngine()->m_system;
        system->createSound(
            geode::utils::string::pathToString(audioFile).c_str(),
            FMOD_DEFAULT, nullptr, &sound
        );
        system->playSound(sound, nullptr, false, &channel);
    }
    
};