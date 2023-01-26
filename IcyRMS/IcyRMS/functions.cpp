#include "functions.h"

void PlayerMove(Player& gracz, Platform plat[], sf::Time& frame, float& score, sf::Sprite& sPlayer, sf::Keyboard::Key lewo, sf::Keyboard::Key prawo, sf::Keyboard::Key skok) {
    if (sf::Keyboard::isKeyPressed(lewo))
    {
        // Ruch w lewo
        if (gracz.x >= 100) gracz.x -= gracz.speed * frame.asMicroseconds();
        // Sprawdzenie spadania TO W FORA
        for (int i = 0; i < 5; i++)
        {
            //std::cout << gracz1.x << " " << plat[i].x << std::endl;
            if (!gracz.jumping && gracz.y < plat[i].y && gracz.y > plat[i].y - 50 && (gracz.x + 35 < plat[i].x || gracz.x > plat[i].x + (plat[i].dl * 10))) {
                gracz.jumping = true;
                gracz.falling = true;
            }
        }
    }
    if (sf::Keyboard::isKeyPressed(prawo))
    {
        // Ruch w prawo
        if (gracz.x <= 750) gracz.x += gracz.speed * frame.asMicroseconds();
        // Sprawdzanie spadania TO W FORA
        for (int i = 0; i < 5; i++)
        {
            if (!gracz.jumping && gracz.y < plat[i].y && gracz.y > plat[i].y - 50 && (gracz.x + 35 < plat[i].x || gracz.x > plat[i].x + (plat[i].dl * 10))) {
                gracz.jumping = true;
                gracz.falling = true;
            }
        }
    }
    if (sf::Keyboard::isKeyPressed(skok) && !gracz.jumping)
    {
        // Inicjalizacja skoku
        gracz.jumping = true;
    }

    if (gracz.jumping)
    {
        if (sf::Keyboard::isKeyPressed(skok) && gracz.maxHeight > gracz.jumpStart && !gracz.falling)
        {
            gracz.y -= gracz.speed * gracz.jumpMultiply * frame.asMicroseconds();
            gracz.jumpStart += gracz.speed * gracz.jumpMultiply * frame.asMicroseconds();
        }
        else
        {
            gracz.jumpStart = 0;
            gracz.falling = true;
        }

        if (gracz.falling)
        {
            // to w fora
            for (int i = 0; i < 5; i++)
            {
                if (gracz.x > plat[i].x - 40 && gracz.x < plat[i].x + (plat[i].dl * 10) && gracz.y < plat[i].y - 10 && gracz.y > plat[i].y - 30)
                {
                    gracz.y = plat[i].y - 40;
                    gracz.falling = false;
                    gracz.jumping = false;
                }
            }

            if (gracz.falling)
            {
                if (score != 0 || (score == 0 && gracz.y < 950)) gracz.y += gracz.speed * gracz.jumpMultiply * frame.asMicroseconds();
                if (score == 0 && gracz.y >= 950) {
                    gracz.jumping = false;
                    gracz.falling = false;
                }
            }
            else
            {
                gracz.falling = false;
                gracz.jumping = false;
            }
        }
    }
    sPlayer.setPosition(gracz.x, gracz.y);
}

void PlatGen(std::mt19937& generator, Platform plat[], int& platNum, float& lastScore, float& score)
{
    std::uniform_real_distribution<float> dist(150.0f, 750.0f);
    float platX = dist(generator);
    int maxDist = (800 - platX) / 10;
    if (maxDist > 40)
    {
        std::uniform_int_distribution<int> dist2(3, 40);
        plat[platNum].Start(platX, 0, dist2(generator));
    }
    else
    {
        std::uniform_int_distribution<int> dist2(3, maxDist);
        plat[platNum].Start(platX, 0, dist2(generator));
    }

    lastScore = score;

    if (platNum == 4) platNum = 0;
    else platNum++;
}