#include <SFML/Graphics.hpp>
#include <ctime>
#include <iostream>
#include <vector>

const int main_field_width = 10;//10;
const int main_field_height = 21;//21;
const sf::Vector2f dot_size(15, 15);
sf::Color colors[4]{ sf::Color::Red,sf::Color::Blue,sf::Color::Green,sf::Color::Yellow };
sf::Color baseColor = sf::Color(100, 100, 100, 255);


//описание точки основного поля
class dot
{
public:
    sf::RectangleShape pixel;
    bool isColorful;
    bool isWillBeDelet;
    dot()
    {
        isWillBeDelet = 0;
        pixel.setFillColor(baseColor);
        isColorful = false;
        pixel = sf::RectangleShape(dot_size);
        pixel.setFillColor(baseColor);
        pixel.setOutlineColor(sf::Color(50, 50, 50, 255));
        pixel.setOutlineThickness(1);
    }
    //опустошение точки
    void basefication()
    {
        isColorful = 0;
        pixel.setFillColor(baseColor);
    }
};

//описани появляющейся фигуры
class figure
{
public:
    dot palka[3];
    figure()
    {
        for (int i = 0; i < 3; i++)
        {
            palka[i].isColorful = 1;
            int color = rand() % 4;
            palka[i].pixel.setFillColor(colors[color]);
        }
    }
    ~figure() = default;

};

//функция падения точки
void fall(dot& a, dot& b)
{
    b.isColorful = a.isColorful;
    b.pixel.setFillColor(a.pixel.getFillColor());
    a.basefication();
}

//функция появления цветной точки
void dotSpawn(dot& empty, dot colorful)
{
    colorful.pixel.setPosition(empty.pixel.getPosition());
    empty = colorful;
}


int main()
{
    setlocale(LC_ALL, "Russian");
    //задний фон
    sf::RenderWindow window(sf::VideoMode(500, 400), "MainWindow");
    sf::Vector2f main_field_position(200, 30);
    sf::RectangleShape background(sf::Vector2f(500, 400));

    //разметка основного поля
    dot main_field[main_field_width][main_field_height];

    for (int i = 0; i < main_field_width; i++) {
        for (int j = 0; j < main_field_height; j++) 
        {
            main_field[i][j].pixel.setPosition(main_field_position + sf::Vector2f(i * (dot_size.x + 1), j * (dot_size.y + 1)));
        }
    }


    int x;
    std::srand(std::time(nullptr));

    bool isCalm=1;
    bool isFull=0;

    bool isGameOver = 0;
    sf::Clock clock;
    const double dropInterval = 1.0f;
    figure tipledot;
    int frames=0;

    //блок счёта
    int score = 0;
    int best_score = 0;
    sf::Font font;
    font.loadFromFile("Roboto-Regular.ttf");
    sf::Text scoreboard;
    sf::Text showfps;
    sf::Text randchek;
    scoreboard.setFont(font);
    scoreboard.setString("Счёт: "+std::to_string(score)+"\nЛучший счёт: "+ std::to_string(best_score));
    scoreboard.setCharacterSize(20);
    scoreboard.setFillColor(sf::Color::Black);
    scoreboard.setPosition(30,30);

    showfps.setFont(font);
    showfps.setCharacterSize(20);
    showfps.setPosition(0, 0);
    showfps.setFillColor(sf::Color::Black);
    showfps.setString(std::to_string(frames));
    
    int v[main_field_width]{};
    randchek.setFont(font);
    randchek.setCharacterSize(9);
    randchek.setPosition(10, 380);
    randchek.setFillColor(sf::Color::Black);
    randchek.setString(std::to_string(v[0])+ " " + std::to_string(v[1]) + " " + std::to_string(v[2]) + " " + std::to_string(v[3]) + " " + std::to_string(v[4]) + " " +std::to_string(v[5]) + " " + std::to_string(v[6])+ " "+ std::to_string(v[7])+" "+ std::to_string(v[8])+" "+ std::to_string(v[9]));
    

    while (window.isOpen())
    {
        

            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    window.close();
            }
            window.draw(background);
            
            //с интервалом проссчитываем логику                      
            if (!isGameOver)
            {
                //if (clock.getElapsedTime().asSeconds() >= dropInterval)
                {
                   // clock.restart(); // Сбрасываем таймер
                    isCalm = 1; // Сбрасываем показатель спокойствия

                    //разбор полётов
                    for (int i = main_field_width - 1; i >= 0; i--)
                    {
                        for (int j = main_field_height - 2; j >= 0; j--)
                        {
                            if (main_field[i][j].isColorful && !main_field[i][j + 1].isColorful)
                            {
                                fall(main_field[i][j], main_field[i][j + 1]);
                                //если что-то падает, то на поле неспокойно
                                isCalm = 0;
                            }
                        }
                    }


                    //удаление 3 в ряд, при условии если на поле спокойно
                    if (isCalm)
                    {
                        //поиск попадающих под удаление
                        for (int j = 0; j < main_field_height; j++)
                        {

                            for (int i = 0; i < main_field_width; i++)
                            {
                                if (main_field[i][j].isColorful) if (j > 0 && j < main_field_height - 1) if (main_field[i][j].pixel.getFillColor() == main_field[i][j - 1].pixel.getFillColor() && main_field[i][j].pixel.getFillColor() == main_field[i][j + 1].pixel.getFillColor())
                                {
                                    main_field[i][j].isWillBeDelet = 1;
                                    main_field[i][j - 1].isWillBeDelet = 1;
                                    main_field[i][j + 1].isWillBeDelet = 1;
                                }
                                if (main_field[i][j].isColorful) if (i > 0 && i < main_field_width - 1) if (main_field[i][j].pixel.getFillColor() == main_field[i - 1][j].pixel.getFillColor() && main_field[i][j].pixel.getFillColor() == main_field[i + 1][j].pixel.getFillColor())
                                {
                                    main_field[i][j].isWillBeDelet = 1;
                                    main_field[i + 1][j].isWillBeDelet = 1;
                                    main_field[i - 1][j].isWillBeDelet = 1;
                                }
                            }
                        }
                        //удаление
                        for (int j = 0; j < main_field_height; j++)
                        {
                            for (int i = 0; i < main_field_width; i++)
                            {
                                if (main_field[i][j].isWillBeDelet)
                                {
                                    score++;
                                    scoreboard.setString("Score: " + std::to_string(score) + "\nBest score: " + std::to_string(best_score));
                                    main_field[i][j].basefication();
                                    main_field[i][j].isWillBeDelet = 0;
                                }
                            }  
                        }
                        //разбор полётов после удаления
                        for (int i = main_field_width - 1; i >= 0; i--)
                        {
                            for (int j = main_field_height - 2; j >= 0; j--)
                            {
                                if (main_field[i][j].isColorful && !main_field[i][j + 1].isColorful)
                                {
                                    fall(main_field[i][j], main_field[i][j + 1]);
                                    isCalm = 0;
                                }
                            }
                        }
                    }
                    

                    //если РЕАЛЬНО ничего не падает, т.е. всё спокойно, то спавним ещё фигуру
                    if (isCalm)
                    {
                        tipledot.~figure();
                        figure tripledot;
                        x = rand() % main_field_width;
                        v[x]++;
                        randchek.setString(std::to_string(v[0]) + " " + std::to_string(v[1]) + " " + std::to_string(v[2]) + " " + std::to_string(v[3]) + " " + std::to_string(v[4]) + " " + std::to_string(v[5]) + " " + std::to_string(v[6]) + " " + std::to_string(v[7]) + " " + std::to_string(v[8]) + " " + std::to_string(v[9]));
                        for (int i = 2; i >= 0; i--)
                        {
                            //если пустого места нет, то ггвп, но иначе всё-таки спавним
                            if (main_field[x][i].isColorful)
                            {
                                isGameOver = 1;
                                break;
                            }
                            else
                                dotSpawn(main_field[x][i], tripledot.palka[i]);
                        }

                    }
                }
            }
            else
            {
                if(score>best_score)best_score = score;
                score = 0;
                for (int i = 0; i < main_field_width; i++)
                {
                    for (int j = 0; j < main_field_height; j++)
                    {
                        main_field[i][j].basefication();
                        isGameOver = 0;
                    }
                }  
            }
            
            //отрисовка
           
            
            for (int i = 0; i < main_field_width; i++)
            {
                for (int j = 0; j < main_field_height; j++)
                {
                    window.draw(main_field[i][j].pixel);
                }
            }

            //fps
            if (clock.getElapsedTime().asSeconds() >= dropInterval)
            {
                clock.restart();
                showfps.setString(std::to_string(frames));
                frames = 1;
            }

            frames++;
            window.draw(showfps);
            window.draw(scoreboard);
            window.draw(randchek);

            window.display();
        }
        

        return 0;
    }
