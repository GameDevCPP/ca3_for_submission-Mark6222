#include <iostream>

#include "AudioManager.h"

#include "system_resources.h"

std::unordered_map<std::string, std::shared_ptr<sf::SoundBuffer>> AudioManager::soundBuffers;
std::unordered_map<std::string, sf::Sound> AudioManager::sounds;
std::shared_ptr<sf::Music> AudioManager::backgroundMusic = nullptr;

AudioManager::AudioManager() {
    backgroundMusic = std::make_shared<sf::Music>();
}
void AudioManager::render() {

}
void AudioManager::update(double dt) {

}
bool AudioManager::loadSound(const string &name, const string &filename) {
    shared_ptr<SoundBuffer> buffer = Resources::get<SoundBuffer>(filename);
    soundBuffers[name] = buffer;
    Sound sound;
    sound.setBuffer(*soundBuffers[name]);
    sounds[name] = sound;
    return true;
}


void AudioManager::playSound(const string &name) {
    if (sounds.find(name) != sounds.end()) {
        sounds[name].play();
    }
}

void AudioManager::setSoundVolume(const string &name, float volume) {
    if (sounds.find(name) != sounds.end()) {
        sounds[name].setVolume(volume);
    }
}

bool AudioManager::playMusic(const std::string &filename, bool loop) {
    std::cout << "Attempting to load music: " << filename << std::endl;

    backgroundMusic = Resources::get<sf::Music>(filename);
    if (backgroundMusic) {
        std::cout << "Music loaded successfully: " << filename << std::endl;
    } else {
        std::cout << "Failed to load music: " << filename << std::endl;
        return false;
    }

    backgroundMusic->setLoop(loop);
    std::cout << "Loop set to: " << (loop ? "true" : "false") << std::endl;

    backgroundMusic->play();
    std::cout << "Playing music: " << filename << std::endl;

    return true;
}


void AudioManager::stopMusic() {
    backgroundMusic->stop();
}

void AudioManager::setMusicVolume(float volume) {
    backgroundMusic->setVolume(volume);
}

