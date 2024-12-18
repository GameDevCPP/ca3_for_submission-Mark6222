#include "LevelSystem.h"
#include <fstream>
#include <filesystem>

using namespace std;
using namespace sf;

std::map<LevelSystem::Tile, sf::Color> LevelSystem::_colours{
    {WALL, Color::White}, {END, Color::Red}
};

sf::Color LevelSystem::getColor(LevelSystem::Tile t) {
    auto it = _colours.find(t);
    if (it == _colours.end()) {
        _colours[t] = Color::Transparent;
    }
    return _colours[t];
}

void LevelSystem::setColor(LevelSystem::Tile t, sf::Color c) {
    _colours[t] = c;
}

std::unique_ptr<LevelSystem::Tile[]> LevelSystem::_tiles;
size_t LevelSystem::_width;
size_t LevelSystem::_height;

float LevelSystem::_tileSize(100.f);
Vector2f LevelSystem::_offset(0.0f, 30.0f);
// Vector2f LevelSystem::_offset(0,0);
vector<std::unique_ptr<sf::RectangleShape> > LevelSystem::_sprites;


Texture LevelSystem::wall;
Texture LevelSystem::crate;

void LevelSystem::loadLevelFile(const std::string &path, float tileSize) {
    _tileSize = tileSize;
    size_t w = 0, h = 0;
    string buffer;

    // Load in file to buffer
    ifstream f(path);
    if (f.good()) {
        f.seekg(0, std::ios::end);
        buffer.resize(f.tellg());
        f.seekg(0);
        f.read(&buffer[0], buffer.size());
        f.close();

        // Clean buffer by removing null terminators
        buffer.erase(remove(buffer.begin(), buffer.end(), '\0'), buffer.end());
        cout << "File loaded and cleaned. Buffer size: " << buffer.size() << endl;
    } else {
        throw string("Couldn't open level file: ") + path;
    }

    std::vector<Tile> temp_tiles;
    int widthCheck = 0;
    for (int i = 0; i < buffer.size(); ++i) {
        const char c = buffer[i];
        if (c == '\0') { break; }
        if (c == '\n') {
            // newline
            if (w == 0) {
                // if we haven't written width yet
                w = i; // set width
            } else if (w != (widthCheck - 1)) {
                throw string("non uniform width:" + to_string(h) + " ") + path;
            }
            widthCheck = 0;
            h++; // increment height
        } else {
            temp_tiles.push_back((Tile) c);
        }
        ++widthCheck;
    }

    if (temp_tiles.size() != (w * h)) {
        throw string("Can't parse level file") + path;
    }
    _tiles = std::make_unique<Tile[]>(w * h);
    _width = w; // set static class vars
    _height = h;
    std::copy(temp_tiles.begin(), temp_tiles.end(), &_tiles[0]);
    cout << "Level " << path << " Loaded. " << w << "x" << h << std::endl;
    buildSprites();
}

void LevelSystem::buildSprites(bool optimise) {
    _sprites.clear();

    struct tp {
        sf::Vector2f p;
        sf::Vector2f s;
        sf::Color c;
    };
    vector<tp> tps;
    const auto tls = Vector2f(_tileSize, _tileSize);
    for (size_t y = 0; y < _height; ++y) {
        for (size_t x = 0; x < _width; ++x) {
            Tile t = getTile({x, y});
            if (t == EMPTY) {
                continue;
            }
            tps.push_back({getTilePosition({x, y}), tls, getColor(t)});
        }
    }

    const auto nonempty = tps.size();

    static bool crateTextureLoaded = false;

    if (!crateTextureLoaded) {
        std::cout << "Current working directory: " << std::filesystem::current_path() << std::endl;

        if (!crate.loadFromFile("res/img/Crate.png")) {
            throw std::runtime_error("Failed to load texture: res/img/Crate.png");
        }
        if (!wall.loadFromFile("res/img/Tile.png")) {
            throw std::runtime_error("Failed to load texture: res/img/Crate.png");
        }
        crateTextureLoaded = true;
    }
    for (auto &t: tps) {
        auto s = make_unique<sf::RectangleShape>();
        s->setPosition(t.p);
        s->setSize(t.s);
        s->setScale(1.f, 1.f);
        std::cout << "Tile is: " << getTileAt(t.p) << std::endl;
        if (getTileAt(t.p) == 'f') {
            s->setTexture(&wall);
            s->setTextureRect(sf::IntRect(0, 0, 40, 40));
        } else if (getTileAt(t.p) == 's') {
            s->setTexture(&wall);
            s->setTextureRect(sf::IntRect(0, 0, 40, 40));
        } else {
            s->setTexture(&crate); // Use preloaded texture
            s->setTextureRect(sf::IntRect(0, 0, 40, 40));
        }
        // s->setFillColor(t.c);
        // s->setFillColor(Color(rand()%128+128,0,0));
        // auto s = make_unique<sf::RectangleShape>();
        // s->setPosition(t.p);
        // s->setSize(t.s);
        // s->setFillColor(Color::Red);
        // s->setFillColor(t.c);
        // s->setFillColor(Color(rand()%255,rand()%255,rand()%255));
        _sprites.push_back(move(s));
    }

    cout << "Level with " << (_width * _height) << " Tiles, With " << nonempty
            << " Not Empty, using: " << _sprites.size() << " Sprites\n";
}

void LevelSystem::render(RenderWindow &window) {
    for (auto &t: _sprites) {
        window.draw(*t);
    }
}

LevelSystem::Tile LevelSystem::getTile(sf::Vector2ul p) {
    if (p.x > _width || p.y > _height) {
        throw string("Tile out of range: ") + to_string(p.x) + "," +
              to_string(p.y) + ")";
    }
    return _tiles[(p.y * _width) + p.x];
}

size_t LevelSystem::getWidth() { return _width; }

size_t LevelSystem::getHeight() { return _height; }

sf::Vector2f LevelSystem::getTilePosition(sf::Vector2ul p) {
    return (Vector2f(p.x, p.y) * _tileSize) + _offset;
}

std::vector<Vector2ul> LevelSystem::findTiles(LevelSystem::Tile tileType) {
    std::vector<Vector2ul> positions;
    size_t width = getWidth();
    size_t height = getHeight();
    for (size_t y = 0; y < height; ++y) {
        for (size_t x = 0; x < width; ++x) {
            size_t index = y * width + x;
            if (_tiles[index] == tileType) {
                positions.push_back(Vector2ul(x, y));
            }
        }
    }
    return positions;
}

LevelSystem::Tile LevelSystem::getTileAt(Vector2f v) {
    auto a = v - _offset;
    if (a.x < 0 || a.y < 0) {
        throw string("Tile out of range ");
    }
    return getTile(Vector2ul((v - _offset) / (_tileSize)));
}

bool LevelSystem::isOnGrid(sf::Vector2f v) {
    auto a = v - _offset;
    if (a.x < 0 || a.y < 0) {
        return false;
    }
    auto p = Vector2ul((v - _offset) / (_tileSize));
    if (p.x > _width || p.y > _height) {
        return false;
    }
    return true;
}

void LevelSystem::setOffset(const Vector2f &_offset) {
    LevelSystem::_offset = _offset;
    buildSprites();
}

void LevelSystem::unload() {
    cout << "LevelSystem unloading\n";
    _sprites.clear();
    _tiles.reset();
    _width = 0;
    _height = 0;
    _offset = {0, 0};
}

const Vector2f &LevelSystem::getOffset() { return _offset; }

float LevelSystem::getTileSize() { return _tileSize; }
