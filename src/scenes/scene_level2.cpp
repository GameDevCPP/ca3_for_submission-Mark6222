#include "scene_level2.h"
#include "../components/cmp_enemy_ai.h"

#include "../components/cmp_physics.h"
#include "../components/cmp_player_physics.h"
#include "../game.h"
#include <LevelSystem.h>
#include <iostream>

#include "../components/cmp_collision.h"
#include "../components/cmp_enitity_health.h"
#include "../components/cmp_player_physics.h"
#include "../components/cmp_sprite.h"
#include "../components/cmp_shooting.h"

using namespace std;
using namespace sf;
extern shared_ptr<Entity> player;
extern vector<shared_ptr<Entity> > drones;
Texture spritesheet3;
Texture droneSpritesheet3;

void Level2Scene::Load() {
    soundShoot_buffer = make_shared<SoundBuffer>();
    soundShoot = make_shared<Sound>();
    fireTime = 0.f;
    soundShoot_buffer->loadFromFile("res/sound/blaze.wav");
    if (soundShoot_buffer) {
        soundShoot = make_shared<Sound>(*soundShoot_buffer);
        soundShoot->setVolume(volume);
    }
    ls::loadLevelFile("res/level_2.txt", 40.0f);
    auto ho = Engine::getWindowSize().y - (ls::getHeight() * 40.f);
    ls::setOffset(Vector2f(0, ho));
    // Create player
    auto e = level2.getEcm();
    {
        if (!spritesheet3.loadFromFile("res/img/Jump.png")) {
            cerr << "Failed to load spritesheet!" << std::endl;
        }
        player = makeEntity();
        player->addTag("player");
        player->setPosition(Vector2f(playXStart, ls::getTilePosition(ls::findTiles(ls::START)[0]).y));
        auto s = player->addComponent<SpriteComponent>();
        s->setTexure(std::make_shared<Texture>(spritesheet3));
        s->setTexureRect(0, 0, 27, 26);
        s->getSprite().setOrigin(27 / 2.0f, 26 / 2.0f);
        auto p = player->addComponent<PlayerPhysicsComponent>(Vector2f(26.f, 26.f));
        p->quickJump();
        player->addComponent<ShootingComponent>();
        auto h = player->addComponent<Health>();
        h->setHealth(10);
        auto c = player->addComponent<CollisionComponent>();
        c->setCollision("player", "enemy", e);
    }
    {
        // create enmey
        if (!droneSpritesheet3.loadFromFile("res/img/Drone2.png")) {
            cerr << "Failed to load spritesheet!" << std::endl;
        }
        for (int i = 0; i < ls::findTiles(ls::ENEMY).size(); i++) {
            shared_ptr<Entity> drone;
            drone = makeEntity();
            drone->addTag("enemy");
            drone->setPosition(ls::getTilePosition(ls::findTiles(ls::ENEMY)[i]));
            auto s = drone->addComponent<SpriteComponent>();
            s->setTexure(std::make_shared<Texture>(droneSpritesheet3));
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
        // *********************************
        {
            auto walls = ls::findTiles(ls::WALL);
            for (auto w: walls) {
                auto pos = ls::getTilePosition(w);
                pos += Vector2f(20.f, 20.f); //offset to center
                auto e = makeEntity();
                e->setPosition(pos);
                e->addComponent<PhysicsComponent>(false, Vector2f(40.f, 40.f));
            }
        }
        // *********************************
    }

    cout << " Scene 2 Load Done" << endl;
    setLoaded(true);
}

void Level2Scene::UnLoad() {
    cout << "Scene 2 UnLoad" << endl;
    player.reset();
    ls::unload();
    Scene::UnLoad();
}
void Level2Scene::Update(const double &dt) {
    Scene::Update(dt);

    const auto pp = player->getPosition();
    // cout << pp.x << " PlayerY: " << pp.y << endl;
    if (pp.y > 1260) {
        // player->setPosition(Vector2f(pp.x, pp.y-100));
        // level2.UnLoad();
        // Engine::ChangeScene((Scene *) &level1);
        player->GetCompatibleComponent<PlayerPhysicsComponent>()[0].get()->quickJump();
    }
    if (ls::getTileAt(pp) == ls::END) {
        Engine::ChangeScene((Scene *) &level3);
    } else if (!player->isAlive()) {
        Engine::ChangeScene((Scene *) &level2);
    }


    fireTime -= dt;
    if (fireTime <= 0 && Mouse::isButtonPressed(Mouse::Left)) {
        player->GetCompatibleComponent<ShootingComponent>()[0]->Fire();
        fireTime = 0.5f;
        soundShoot->play();
    }
}

void Level2Scene::Render() {
    ls::render(Engine::GetWindow());
    Scene::Render();
}
