#include "scene_level1.h"
#include "../components/cmp_player_physics.h"
#include "../components/cmp_sprite.h"
#include "../components/cmp_shooting.h"
#include "../game.h"
#include <LevelSystem.h>
#include <iostream>
#include <thread>

#include "../components/cmp_collision.h"
#include "../components/cmp_enemy_ai.h"
#include "../components/cmp_enitity_health.h"

using namespace std;
using namespace sf;

extern shared_ptr<Entity> player;
extern vector<shared_ptr<Entity> > drones;
Texture spritesheet;
Texture droneSpritesheet;

void Level1Scene::Load() {
    soundShoot_buffer = make_shared<SoundBuffer>();
    soundShoot = make_shared<Sound>();
    fireTime = 0.f;
    soundShoot_buffer->loadFromFile("res/sound/blaze.wav");
    if (soundShoot_buffer) {
        soundShoot = make_shared<Sound>(*soundShoot_buffer);
        soundShoot->setVolume(volume);
    } else {
        // Handle error if the sound could not be loaded
        std::cerr << "Failed to load sound: blaze.wav" << std::endl;
    }

    cout << " Scene 1 Load" << endl;
    ls::loadLevelFile("res/level_1.txt", 40.0f);

    auto ho = Engine::getWindowSize().y - (ls::getHeight() * 40.f);
    ls::setOffset(Vector2f(0, ho));
    auto e = level1.getEcm();

    // Create player
    {
        if (!spritesheet.loadFromFile("res/img/Jump.png")) {
            cerr << "Failed to load spritesheet!" << std::endl;
        }
        player = makeEntity();
        player->addTag("player");
        player->setPosition(ls::getTilePosition(ls::findTiles(ls::START)[0]));
        auto s = player->addComponent<SpriteComponent>();
        s->setTexure(std::make_shared<Texture>(spritesheet));
        s->setTexureRect(0, 0, 27, 26);
        s->getSprite().setOrigin(27 / 2.0f, 26 / 2.0f);
        player->addComponent<PlayerPhysicsComponent>(Vector2f(26.f, 26.f));
        player->addComponent<ShootingComponent>();
        auto h = player->addComponent<Health>();
        h->setHealth(10);
        auto c = player->addComponent<CollisionComponent>();
        c->setCollision("player", "enemy", e);
    } {
        // create enmey
        if (!droneSpritesheet.loadFromFile("res/img/Drone2.png")) {
            cerr << "Failed to load spritesheet!" << std::endl;
        }
        for (int i = 0; i < ls::findTiles(ls::ENEMY).size(); i++) {
            shared_ptr<Entity> drone;
            drone = makeEntity();
            drone->addTag("enemy");
            drone->setPosition(ls::getTilePosition(ls::findTiles(ls::ENEMY)[i]));
            auto s = drone->addComponent<SpriteComponent>();
            s->setTexure(std::make_shared<Texture>(droneSpritesheet));
            s->setTexureRect(0, 0, 200, 200);
            s->getSprite().setOrigin(40 / 2.0f, 40 / 2.0f);
            s->getSprite().setScale(Vector2f(0.4f, 0.4f));
            auto a = drone->addComponent<AnimationComponent>();
            a->setAnimation(800, 200, 4, 0.2f);
            auto ai = drone->addComponent<EnemyAIComponent>();
            ai->setPlayer(player);
            drone->addComponent<ShootingComponent>();
            auto h = drone->addComponent<Health>();
            h->setHealth(4);
            auto c = drone->addComponent<CollisionComponent>();
            c->setCollision("enemy", "player", e);
            drones.push_back(drone);
        }
    }

    // Add physics colliders to level tiles.
    {
        auto walls = ls::findTiles(ls::WALL);
        for (auto w: walls) {
            auto pos = ls::getTilePosition(w);
            pos += Vector2f(20.f, 20.f);
            auto e = makeEntity();
            e->setPosition(pos);
            e->addComponent<PhysicsComponent>(false, Vector2f(40.f, 40.f));
        }
    }

    //Simulate long loading times
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    cout << " Scene 1 Load Done" << endl;

    setLoaded(true);
}

void Level1Scene::UnLoad() {
    cout << "Scene 1 Unload" << endl;
    player.reset();
    ls::unload();
    Scene::UnLoad();
}

void Level1Scene::Update(const double &dt) {
    if (ls::getTileAt(player->getPosition()) == ls::END) {
        Engine::ChangeScene((Scene *) &level2);
    }

    fireTime -= dt;

    if (fireTime <= 0 && Mouse::isButtonPressed(Mouse::Left)) {
        player->GetCompatibleComponent<ShootingComponent>()[0]->Fire();
        fireTime = 0.5f;
        soundShoot->play();
    }
    Scene::Update(dt);
}

void Level1Scene::Render() {
    // Engine::setView(gameView);

    ls::render(Engine::GetWindow());
    Scene::Render();
}
