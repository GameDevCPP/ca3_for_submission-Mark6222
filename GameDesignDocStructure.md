Mark Hogan
20098168
# Game Design document

Video: https://youtu.be/0v3sHfwczBA 

In the following document, if you used a particular game design pattern in a section, please describe it (a brief description will do) and how you used it. Code snippets may help with this.
In addition to this outline, I have an example of short design document from Richard McKenna titled: "Spiked Acid Game Design Document", which helped inform this document.

## Introduction
This document describes a game called "Warehouse Jump" Which is a 2D jumping game. The game will have sprite-based animation, tiled backgrounds, collison detection, physics with gravity, AI, efficent memeory managment and a audio manager.


## Technology
This game is designed to be played on windows using:
* SFML: graphics, audio, and windowing
* C++ Engine: Built-in game engine powers core functionality
* Resource Manager: Manages loading and reuse of assets efficiently
* Physics: Simulates realistic object interactions and movement
* Renderer: Renders graphics and visuals on-screen
* Entity Manager: Manages game objects and their interactions
* Tile Loader: Loads tiled maps for levels
* Audio Manager: Controls game sounds and music

## Backstory
The story behind WareHouseJump is about a warehouse with robots being crafted by robots, when the crafted robots turn on you and now being a unique jumping robot you have to jump your way out of the warehouse.


## Objective
The player must use its ability to jump and shoot to jump their way out of the warehouse while also shooting down the enemy robots and critters. Once the player reaches the roof then they can escape.


## Gameplay
The player will start off on the first level on the bottom floor. Above the player will be box's which the player will have to jump as high as they can in order to climp out of the warehouse, but be weary as your not alone, there are multiple enemy robots that are flying and shooting above the player along with spiders that follow and damage the flayer opon colliding with the player, which you must take out before its too late. There are 3 levels which the player must converse in order to escape. Keep jumping up to escape
Spiders: Follows the player attacking them if they get too close
Enemy robots: Damage the player opon touch and also firing bullets

## Levels
### level 1
Level 1 contains large box's flying enemy robots.
### level 2 
Level 2 contains medium box's spiders.
### level 3
Level 3 contains small box's spiders.

## Controls
* SPACE -> jump. The longer you hold the button the higher you jump
* A -> moves left
* D -> moves right

## GUI/In-game menu
* Is there a splash screen?
* E.g. New game, pause game etc.
* If you used/implemented a GUI, describe the library you used (whether it was used in tile editor and/or main game e.g. title menu/networking management etc.)
* Is there a countdown or transition to main game (while not necessarily a GUI issue - it's worthwhile mentioning it somewhere)

* Splash Screen - The splash screen GUI has two buttons, one to exit and one to start the game. Opon pressing the start button a box will apear spinning which indicates that the level is loading
* InGame - In the game the player can see their health on the bottom left along with each enemys health beside each enemy

## Artwork
The art work used is basic 2D pixel art with the theme crafty. The player and player animations where created by myself including the box's.

* Player -> jump and idle
* Box's
* Background -> Ai generated
* Enemy Drone -> Changed and edited to animate
* Spider -> multiple srites to animate
* Tile -> background tile
* Fire -> Bullets

There is a resource manager which auto loads all the texures and audio

## Sound (sound effects)
I built a AudioManager library to: 
* Store sounds in unordered maps using SoundBuffer and Sound for playback
* LoadSound() which loads in the sounds
* PlaySound() which plays preloaded sounds



## Music
Sounds:
* The game includes a chill background track
* Player Landing sound for when they hit the ground
* Player jump sound
* Shooting sounds
