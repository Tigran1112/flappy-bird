#include <SFML/Graphics.hpp>
#include <random>
#include <cmath>

sf::RenderWindow window(sf::VideoMode({600, 800}), "Flappy Bird");
sf::Clock Fallclock;
float interval = 0.1f;

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution selectPos(100, 400);

sf::RectangleShape bird({50, 50});
sf::RectangleShape pipe({100, 1000});
sf::RectangleShape gap({100, 150});

sf::Font font;
std::string mainFont;

bool IsComplete = false;
int score = 0;

void ResetPipes()
{
    pipe.setFillColor(sf::Color(20, 120, 0));
    pipe.setPosition({(float)window.getSize().x, 0});

    int yOffset = selectPos(gen);
    gap.setFillColor(sf::Color::Black);
    gap.setPosition({(float)window.getSize().x, (window.getSize().y / 2.f) - (gap.getSize().y / 2) + yOffset});
}
void ShapeSettings()
{
    bird.setFillColor(sf::Color(219, 163, 39));
    bird.setPosition({200, 500});
    ResetPipes();
}
void Bird()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
    {
        bird.move({0, -5});
        bird.setRotation(sf::degrees(-10.f));
    }
    else
    {
        bird.move({0, 5});
        bird.setRotation(sf::degrees(10.f));
    }
}
void Collision()
{
    if (bird.getPosition().y < 30)
    {
        bird.setPosition({bird.getPosition().x, 30});
        bird.setRotation(sf::degrees(0.f));
    }
    else if (bird.getPosition().y > window.getSize().y - 100)
    {
        window.close();
    }

    float tolerance = 25.f;

    float birdTop = bird.getPosition().y;
    float birdBottom = bird.getPosition().y + bird.getSize().y;
    float birdRight = bird.getPosition().x + bird.getSize().x;

    float gapTop = gap.getPosition().y - tolerance;
    float gapBottom = gap.getPosition().y + gap.getSize().y + tolerance;
    float gapLeft = gap.getPosition().x;

    if (birdBottom >= gapTop && birdTop <= gapBottom) IsComplete = true; 
    else IsComplete = false;

    if (!IsComplete)
    {
        if (bird.getPosition().x + 50 >= pipe.getPosition().x)
        {
            window.close();
        }
    }

    if (birdRight == gapLeft) score++;
    
}
void Pipes()
{
    pipe.move({-10, 0});
    gap.move({-10, 0});
    if(pipe.getPosition().x < -100)
    {
        ResetPipes();
    }
}
int main()
{
    ShapeSettings();


    #ifdef _WIN32 
        mainFont = "C:\\\\Windows\\Fonts\\arial.ttf";
    #elif __linux__
        mainFont = "/usr/share/fonts/gsfonts/NimbusMonoPS-Regular.otf";
    #else
        return -1;
    #endif
    if(!font.openFromFile(mainFont)) return 1;

    sf::Text scoreCounter(font);
    scoreCounter.setCharacterSize(24);
    scoreCounter.setFillColor(sf::Color::White);
    scoreCounter.setPosition({10.f, 10.f});

    while (window.isOpen())
    {

        while (auto eventOpt = window.pollEvent()) if (eventOpt->is<sf::Event::Closed>()) window.close();

        if (Fallclock.getElapsedTime().asSeconds() >= interval)
        {
            Bird();
            Pipes();
            Collision();
               
            Fallclock.restart();
        }
        
        window.clear();
        window.draw(pipe);
        window.draw(gap);
        window.draw(bird);

        scoreCounter.setString(std::to_string(score));
        window.draw(scoreCounter);

        window.display();
    }
    
}