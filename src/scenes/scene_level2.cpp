#include "scene_level2.h"
#include "../components/cmp_enemy_ai.h"

#include "../components/cmp_physics.h"
#include "../components/cmp_player_physics.h"
#include "../game.h"
#include <LevelSystem.h>
#include <iostream>

#include "AudioManager.h"
#include "../components/cmp_collision.h"
#include "../components/cmp_enitity_health.h"
#include "../components/cmp_player_physics.h"
#include "../components/cmp_sprite.h"
#include "../components/cmp_shooting.h"
#include "../components/cmp_spider_ai.h"
#include "../components/cmp_text.h"

using namespace std;
using namespace sf;
extern shared_ptr<Entity> player;
extern vector<shared_ptr<Entity> > spiders;
Texture spritesheet2;
Texture spiderSpritesheet2;

void Level2Scene::Load() {
    Engine::_activeScene->ents.list.clear();
    ls::loadLevelFile("res/level_2.txt", 40.0f);
    auto ho = Engine::getWindowSize().y - (ls::getHeight() * 40.f);
    ls::setOffset(Vector2f(0, ho));
    auto e = level1.getEcm();
    Color green = Color::Green;

    // Create player
    {
        if (!spritesheet2.loadFromFile("res/img/Jump.png")) {
            cerr << "Failed to load spritesheet!" << std::endl;
        }
        player = makeEntity();
        player->addTag("player");
        player->setPosition(ls::getTilePosition(ls::findTiles(ls::START)[0]));
        auto s = player->addComponent<SpriteComponent>();
        s->setTexure(std::make_shared<Texture>(spritesheet2));
        s->setTexureRect(0, 0, 27, 26);
        s->getSprite().setOrigin(27 / 2.0f, 26 / 2.0f);
        player->addComponent<PlayerPhysicsComponent>(Vector2f(26.f, 26.f));
        player->addComponent<ShootingComponent>();
        auto h = player->addComponent<Health>();
        h->setHealth(10, "player");
        auto c = player->addComponent<CollisionComponent>();
        c->setCollision("player", "enemy", e);
    } {
        kills = makeEntity();
        kills->addTag("kills");
        kills->setPosition(sf::Vector2f(60, gameHeight - 80));
        auto pos = ls::findTiles(ls::START)[0];
        auto t = kills->addComponent<TextComponent>("Health: ");
        t->SetText("Health: " + std::to_string(player->GetCompatibleComponent<Health>()[0]->_health), green);
    } {
        // create enmey
        if (!spiderSpritesheet2.loadFromFile("res/img/spider2.png")) {
            cerr << "Failed to load spritesheet!" << std::endl;
        }
        for (int i = 0; i < ls::findTiles(ls::ENEMY).size(); i++) {
            shared_ptr<Entity> spider;
            spider = makeEntity();
            spider->addTag("enemy");
            spider->setPosition(ls::getTilePosition(ls::findTiles(ls::ENEMY)[i]));
            auto s = spider->addComponent<SpriteComponent>();
            s->setTexure(std::make_shared<Texture>(spiderSpritesheet2));
            s->setTexureRect(0, 0, 30, 30);
            s->getSprite().setOrigin(30 / 2.0f, 30 / 2.0f);
            // s->getSprite().setScale(Vector2f(0.4f, 0.4f));
            auto a = spider->addComponent<AnimationComponent>();
            a->setAnimation(180, 30, 6, 0.2f);
            auto ai = spider->addComponent<SpiderAi>();
            ai->setPlayer(player);
            auto h = spider->addComponent<Health>();
            h->setHealth(4, "spider");
            auto c = spider->addComponent<CollisionComponent>();
            c->setCollision("enemy", "player", e);
            spider->addComponent<TextComponent>("0");
            spiders.push_back(spider);
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
    Color red = Color::Red;
    kills->GetCompatibleComponent<TextComponent>()[0]->SetText(
        "Health: " + std::to_string(player->GetCompatibleComponent<Health>()[0]->_health), red);
    for (const auto &spider: spiders) {
        spider->GetCompatibleComponent<TextComponent>()[0]->SetText(
            std::to_string(spider->GetCompatibleComponent<Health>()[0]->_health), red);
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
        AudioManager::playSound("blaze");
    }
}

void Level2Scene::Render() {
    ls::render(Engine::GetWindow());
    Scene::Render();
}
