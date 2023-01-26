#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>

struct Player {
    float x;
    float y = 950.0f;
    float speed = 0.25f;
    float jumpMultiply = 1.3f;
    bool jumping = false; // ruch w górê
    float maxHeight = 350.0; // maksymalna wysokosc skoku
    float jumpStart = 0.0; // gdzie zaczêto poprzedni skok
    bool falling = false; // ruch w dó³
    void Reset(float a) {
        x = a;
        y = 950.0f;
        speed = 0.25f;
        jumpMultiply = 1.3f;
        jumping = false;
        maxHeight = 350.0;
        jumpStart = 0.0;
        falling = false;
    }
};
struct Platform {
    //bool visible = false;
    float x;
    float y;
    int dl;
    sf::Sprite sPlatL;
    sf::Sprite sPlatR;
    sf::Sprite sPlatM[40];
    sf::Texture tPlatformMiddle;
    sf::Texture tPlatformLeft;
    sf::Texture tPlatformRight;
    void Start(float a, float b, int c) {
        x = a;
        y = b;
        dl = c;
        tPlatformLeft.loadFromFile("platformLeft.png");
        tPlatformMiddle.loadFromFile("platformMiddle.png");
        tPlatformRight.loadFromFile("platformRight.png");
        for (int i = 0; i < dl - 2; i++) sPlatM[i].setTexture(tPlatformMiddle);
        sPlatL.setTexture(tPlatformLeft);
        sPlatR.setTexture(tPlatformRight);
    }
    void Update(float a, float b, int c)
    {
        x = a;
        y = b;
        dl = c;
    }
};

void PlayerMove(Player& gracz, Platform plat[], sf::Time& frame, float& score, sf::Sprite& sPlayer, sf::Keyboard::Key lewo, sf::Keyboard::Key prawo, sf::Keyboard::Key skok);
void PlatGen(std::mt19937& generator, Platform plat[], int& platNum, float& lastScore, float& score);