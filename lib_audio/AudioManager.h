//
// Created by marky on 01/01/2025.
//

#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H
#include <SFML/Audio.hpp>
#include <unordered_map>
#include <string>
#include <memory>

using namespace std;
using namespace sf;

class AudioManager {
protected:
    static std::unordered_map<std::string, shared_ptr<SoundBuffer>> soundBuffers;
    static std::unordered_map<std::string, Sound> sounds;

    static shared_ptr<Music> backgroundMusic;
public:
    AudioManager();

    virtual void update(double dt);

    virtual void render();

    static bool loadSound(const std::string& name, const std::string& filename);
    static void playSound(const std::string& name);
    static bool playMusic(const std::string& filename, bool loop = true);
    static void stopMusic();
    static void setSoundVolume(const std::string& name, float volume);

    static void setMusicVolume(float volume);
};



#endif //AUDIOMANAGER_H
