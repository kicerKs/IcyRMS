//#include <SFML/Graphics.hpp>
//#include <iostream>
//#include <random>
#include <ctime>
#include <fstream>
#include <string>
#include "functions.h"

int main()
{
    // generator losowy
    std::mt19937 generator(time(nullptr));

    // Stworzenie okienka
    sf::RenderWindow window(sf::VideoMode(900, 1000), "Icy RMS");

    /*
        Wczytanie grafik i sprite'ów
    */ 

    // t³o menu - rms
    sf::Texture tBackgroundMenu;
    sf::Sprite sBackgroundMenu;
    tBackgroundMenu.loadFromFile("bg.jpg");
    sBackgroundMenu.setTexture(tBackgroundMenu);
    sBackgroundMenu.move(0, 225);
    // t³o menu - tytu³
    sf::Texture tBackgroundMenuHeader;
    sf::Sprite sBackgroundMenuHeader;
    tBackgroundMenuHeader.loadFromFile("bgh.png");
    sBackgroundMenuHeader.setTexture(tBackgroundMenuHeader);
    // t³o menu - stopka
    sf::Texture tBackgroundMenuFooter;
    sf::Sprite sBackgroundMenuFooter;
    tBackgroundMenuFooter.loadFromFile("bgf.png");
    sBackgroundMenuFooter.setTexture(tBackgroundMenuFooter);
    sBackgroundMenuFooter.setPosition(0, 900);
    // menu - przyciski
    sf::Texture tMenuButton1Player;
    sf::Texture tMenuButton2Players;
    sf::Texture tMenuButtonWyniki;
    sf::Texture tMenuButtonExit;
    sf::Sprite sMenuButton1Player;
    sf::Sprite sMenuButton2Players;
    sf::Sprite sMenuButtonWyniki;
    sf::Sprite sMenuButtonExit;
    tMenuButton1Player.loadFromFile("button1player.png");
    tMenuButton2Players.loadFromFile("button2players.png");
    tMenuButtonWyniki.loadFromFile("buttonwyniki.png");
    tMenuButtonExit.loadFromFile("buttonexit.png");
    sMenuButton1Player.setTexture(tMenuButton1Player);
    sMenuButton1Player.setPosition(300, 350);
    sMenuButton2Players.setTexture(tMenuButton2Players);
    sMenuButton2Players.setPosition(300, 450);
    sMenuButtonWyniki.setTexture(tMenuButtonWyniki);
    sMenuButtonWyniki.setPosition(300, 550);
    sMenuButtonExit.setTexture(tMenuButtonExit);
    sMenuButtonExit.setPosition(300, 650);
    // tablica wyników - t³o
    sf::Texture tScoresBg;
    sf::Sprite sScoreBg;
    tScoresBg.loadFromFile("bgscores.png");
    sScoreBg.setTexture(tScoresBg);
    // tablica wyników - przycisk
    sf::Texture tScoresExit;
    sf::Sprite sScoresExit;
    tScoresExit.loadFromFile("buttonHSexit.png");
    sScoresExit.setTexture(tScoresExit);
    sScoresExit.setPosition(575, 875);
    // ekran gry - t³o
    sf::Texture tGameBg;
    sf::Sprite sGameBg;
    tGameBg.loadFromFile("gamebg.png");
    sGameBg.setTexture(tGameBg);
    // ekran gry - œciany
    sf::Texture tGameWall;
    sf::Sprite sGameWallL;
    sf::Sprite sGameWallR;
    tGameWall.loadFromFile("gamewall.png");
    sGameWallL.setTexture(tGameWall);
    sGameWallR.setTexture(tGameWall);
    sGameWallR.move(800, 0);
    // postaæ gracza
    sf::Texture tPlayer;
    sf::Sprite sPlayer1;
    sf::Sprite sPlayer2;
    tPlayer.loadFromFile("player.png");
    sPlayer1.setTexture(tPlayer);
    sPlayer2.setTexture(tPlayer);
    // tekst - wynik
    sf::Font font;
    sf::Text scoreText("SCORE: 0", font);
    font.loadFromFile("font.ttf");
    scoreText.setCharacterSize(50);
    scoreText.setStyle(sf::Text::Bold);
    scoreText.setFillColor(sf::Color::Red);
    scoreText.setPosition(100, 0);

    /*
        Startowe wartoœci
    */
    
    // Gracz(e)
    Player gracz1;
    gracz1.x = 250.0f;
    Player gracz2;
    gracz2.x = 650.0f;
    enum GAMESTATE{MAINMENU, HIGHSCORES, GAME}; // stan gry
    GAMESTATE gamestate = MAINMENU; // startowy
    // Flagi - czy dany gracz gra
    bool p1active = false;
    bool p2active = false;
    float score = 0.0; // wynik
    float lastScore = 0.0; // ostatni wynik - u¿ywany do losowania platform
    int platNum = 0; // numer platformy, która znika

    // Platformy
    Platform plat[5];
    // Pocz¹tkowe pozycje pierwszych 5 platform
    plat[0].Start(300, 800, 42);
    plat[1].Start(120, 600, 12);
    plat[2].Start(600, 400, 4);
    plat[3].Start(460, 200, 15);
    plat[4].Start(170, 0, 23);
    // Ustawienie pozycji sprite'ów platform
    for (int k = 0; k < 5; k++)
    {
        plat[k].sPlatL.setPosition(plat[k].x, plat[k].y);
        plat[k].sPlatR.setPosition(plat[k].x + ((plat[k].dl - 1) * 10), plat[k].y);
        for (int i = 0; i < plat[k].dl - 2; i++) plat[k].sPlatM[i].setPosition(plat[k].x + ((i + 1) * 10), plat[k].y);
    }

    // Czas
    sf::Clock clock;

    // G³ówna pêtla gry
    while (window.isOpen())
    {
        sf::Time frame = clock.getElapsedTime();
        // Zamykanie okna
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Obs³uga zamkniêcia okna (krzy¿yk w rogu)
            if (event.type == sf::Event::Closed)
                window.close();
        }
        //G³ówne Menu
        if (gamestate == MAINMENU)
        {
            //Wybieranie przycisków w menu
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                //Pobierz pozycjê myszy
                sf::Vector2i mousepos = sf::Mouse::getPosition(window);
                // 1 GRACZ
                if (mousepos.x >= 300 && mousepos.x <= 600 && mousepos.y >= 350 && mousepos.y <= 450)
                {
                    gamestate = GAME;
                    // flaga sterowania 1 graczem
                    p1active = true;
                }
                // 2 GRACZY
                if (mousepos.x >= 300 && mousepos.x <= 600 && mousepos.y >= 450 && mousepos.y <= 550)
                {
                    gamestate = GAME;
                    // flagi sterowania 2 graczami
                    p1active = true;
                    p2active = true;
                }
                // WYNIKI
                if (mousepos.x >= 300 && mousepos.x <= 600 && mousepos.y >= 550 && mousepos.y <= 650)
                {
                    gamestate = HIGHSCORES;
                }
                // EXIT
                if (mousepos.x >= 300 && mousepos.x <= 600 && mousepos.y >= 650 && mousepos.y <= 750)
                {
                    window.close();
                }
            }
        }
        //Wyniki
        if (gamestate == HIGHSCORES)
        {
            // Wpisanie wyniku je¿eli by³a rozegrana wczeœniej gra
            if (score != 0)
            {
                sf::Text napis("Wpisz nick i zatwierdz enterem", font);
                napis.setPosition(25, 150);
                napis.setCharacterSize(60);
                napis.setFont(font);
                napis.setFillColor(sf::Color::Green);
                // Napis pokazuj¹cy wpisywany nick przez gracza
                sf::Text nick("wpisz nick",font);
                sf::String sNick;
                nick.setPosition(100, 400);
                nick.setCharacterSize(60);
                nick.setFont(font);
                nick.setFillColor(sf::Color::Green);
                // Obs³uga wpisywania tekstu
                while (window.isOpen())
                {
                    while (window.pollEvent(event))
                    {
                        if (event.type == sf::Event::TextEntered) // Je¿eli coœ wpisano z klawiatury
                        {
                            if (event.text.unicode == '\b' && sNick.getSize()>0) // BACKSPACE
                            {
                                sNick.erase(sNick.getSize()-1, 1);
                                nick.setString(sNick);
                            }
                            else if (event.text.unicode < 0x80 && event.text.unicode != '\b') // Litera alfabetu
                            {
                                sNick += event.text.unicode;
                                nick.setString(sNick);
                            }  
                        }
                        if (event.type == sf::Event::Closed) // Zamkniêcie okienka (krzy¿yk w rogu)
                        {
                            window.close();
                            break;
                        }
                            
                    }
                    // Rysowanie ekranu gry podczas wpisywania nicku
                    window.clear();
                    // Rysowanie t³a i przycisku
                    window.draw(sScoreBg);
                    window.draw(sScoresExit);
                    // Wypisanie tego co gracz wpisuje
                    window.draw(napis);
                    window.draw(nick);
                    window.display();
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) break; // Po enterze wyjscie
                }
                // Odczyt wyników z pliku i ewentualny zapis, je¿eli wynik jest wiêkszy od któregoœ na liœcie
                sf::Text wyniki[10];
                std::string linie[10];
                std::ifstream plik;
                plik.open("scores.txt");
                for (int i = 0; i < 10; i++)
                {
                    score = (int)score;
                    std::string linia;
                    std::getline(plik, linia);
                    std::size_t poz = linia.find(":");
                    std::cout << linia.substr(poz + 1) << std::endl;
                    // Je¿eli wynik uzyskany w poprzedniej grze jest wiêkszy ni¿ odczytany wynik, wpisanie obecnego wyniku i przesuniêcie tablicy o 1
                    if (score > stoi(linia.substr(poz + 1)))
                    {
                        wyniki[i].setString(std::to_string(i+1) + ". " + nick.getString() + " " + std::to_string((int)score));
                        linie[i] = nick.getString() + ":" + std::to_string((int)score);
                        i++;
                        score = 0;
                    }
                    linie[i] = linia;
                    wyniki[i].setString(std::to_string(i + 1) + ". " + linia.substr(poz + 1) + " " + linia.substr(0, poz));
                    wyniki[i].setFont(font);
                    wyniki[i].setFillColor(sf::Color::Black);
                    wyniki[i].setPosition(100, 75 * i + 50);
                    window.draw(wyniki[i]);
                }
                plik.close();
                // Zapis
                std::ofstream plik2;
                plik2.open("scores.txt");
                for (int i = 0; i < 10; i++)
                {
                    plik2 << linie[i] << std::endl;
                }
                plik2.close();
                //Resetowanie graczy i wartoœci w grze
                p2active = false;
                p1active = false;
                gracz1.Reset(250.0f);
                gracz2.Reset(650.0f);
                plat[0].Start(300, 800, 42);
                plat[1].Start(120, 600, 12);
                plat[2].Start(600, 400, 4);
                plat[3].Start(460, 200, 15);
                plat[4].Start(170, 0, 23);
                platNum = 0;
                score = 0;
                lastScore = 0;
                for (int k = 0; k < 5; k++)
                {
                    plat[k].sPlatL.setPosition(plat[k].x, plat[k].y);
                    plat[k].sPlatR.setPosition(plat[k].x + ((plat[k].dl - 1) * 10), plat[k].y);
                    for (int i = 0; i < plat[k].dl - 2; i++) plat[k].sPlatM[i].setPosition(plat[k].x + ((i + 1) * 10), plat[k].y);
                }
            }
            
            // Powrót do mainmenu
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                sf::Vector2i mousepos = sf::Mouse::getPosition(window);

                if (mousepos.x >= 575 && mousepos.x <= 875 && mousepos.y >= 875 && mousepos.y <= 975)
                {
                    gamestate = MAINMENU;
                }
            }
        }
        //Gra
        if (gamestate == GAME)
        {
            // Ruch gracza 1
            if (p1active)
            {
                PlayerMove(gracz1, plat, frame, score, sPlayer1, sf::Keyboard::A, sf::Keyboard::D, sf::Keyboard::W);
            }
            // Ruch gracza 2
            if (p2active)
            {
                PlayerMove(gracz2, plat, frame, score, sPlayer2, sf::Keyboard::Left, sf::Keyboard::Right, sf::Keyboard::Up);
            }
            // Podnoszenie planszy
            if (gracz1.y < 500)
            {
                float diff = ((500 - gracz1.y)*0.0005)+1;
                for(int i=0;i<5;i++) plat[i].y += diff;
                score += diff;
                gracz1.y += diff;
                gracz2.y += diff;
                // Aktualizowanie wyniku
                scoreText.setString("SCORE: " + std::to_string((int)score));
                // Obni¿anie platform
                for (int k = 0; k < 5; k++)
                {
                    plat[k].sPlatL.setPosition(plat[k].x, plat[k].y);
                    plat[k].sPlatR.setPosition(plat[k].x + ((plat[k].dl - 1) * 10), plat[k].y);
                    for (int i = 0; i < plat[k].dl - 2; i++) plat[k].sPlatM[i].setPosition(plat[k].x + ((i + 1) * 10), plat[k].y);
                }
            }
            else if (p2active && gracz2.y < 500)
            {
                float diff = ((500 - gracz2.y) * 0.0005)+1;
                for (int i = 0; i < 5; i++) plat[i].y += diff;
                score += diff;
                gracz1.y += diff;
                gracz2.y += diff;
                // Aktualizowanie wyniku
                scoreText.setString("SCORE: " + std::to_string((int)score));
                // Obni¿anie platform
                for (int k = 0; k < 5; k++)
                {
                    plat[k].sPlatL.setPosition(plat[k].x, plat[k].y);
                    plat[k].sPlatR.setPosition(plat[k].x + ((plat[k].dl - 1) * 10), plat[k].y);
                    for (int i = 0; i < plat[k].dl - 2; i++) plat[k].sPlatM[i].setPosition(plat[k].x + ((i + 1) * 10), plat[k].y);
                }
            }
            // Losowanie nowych platform
            if (score - 200 > lastScore)
            {
                PlatGen(generator, plat, platNum, lastScore, score);
                // zwiêkszanie stopnia trudnoœci - prêdkoœæ gracza i maksymalna wysokoœæ skoku
                gracz1.maxHeight += 10;
                gracz2.maxHeight += 10;
                gracz1.speed += 0.01f;
                gracz2.speed += 0.01f;
            }
            // Koniec gry je¿eli gracz spad³
            if (gracz1.y > 1000 || (p2active && gracz2.y > 1000)) gamestate = HIGHSCORES; 
        }
        // Rysowanie
        window.clear();
        if (gamestate == MAINMENU)
        {
            // Rysowanie g³ównego menu
            window.draw(sBackgroundMenuHeader); // Rysowanie tytu³u
            window.draw(sBackgroundMenu); // Rysowanie t³a
            window.draw(sBackgroundMenuFooter); // Rysowanie stopki
            // Przyciski
            window.draw(sMenuButton1Player);
            window.draw(sMenuButton2Players);
            window.draw(sMenuButtonWyniki);
            window.draw(sMenuButtonExit);
        }
        else if(gamestate == HIGHSCORES)
        {
            // Rysowanie tablicy wyników
            window.draw(sScoreBg);
            window.draw(sScoresExit);
            // Pobieranie z pliku i wyœwietlanie
            sf::Text wyniki[10];
            std::ifstream plik;
            plik.open("scores.txt");
            // Pobranie wyniku z ostatniej gry
            for (int i = 0; i < 10; i++)
            {
                std::string linia;
                std::getline(plik, linia);
                std::size_t poz = linia.find(":");
                wyniki[i].setString(std::to_string(i+1)+". "+linia.substr(poz+1)+" "+linia.substr(0,poz));
                wyniki[i].setFont(font);
                wyniki[i].setFillColor(sf::Color::Black);
                wyniki[i].setPosition(100, 75 * i+150);
                window.draw(wyniki[i]);
            }
            plik.close();
        }
        else if (gamestate == GAME)
        {
            // Rysowanie widoku gry
            window.draw(sGameBg);
            window.draw(sGameWallL);
            window.draw(sGameWallR);
            // Rysowanie platform
            for (int k = 0; k < 5; k++)
            {
                window.draw(plat[k].sPlatL);
                window.draw(plat[k].sPlatR);
                for (int i = 0; i < plat[k].dl - 2; i++) window.draw(plat[k].sPlatM[i]);
            }
            // Rysowanie gracza
            if (p1active) window.draw(sPlayer1);
            if (p2active) window.draw(sPlayer2);
            // Wynik
            window.draw(scoreText);
        }
        // Wyœwietlenie okna
        window.display();
        clock.restart();
    }
}