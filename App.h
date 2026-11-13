#pragma once

#include <SFML/Graphics.hpp>
#include "Core.h"
#include "Renderer.h"

class App
{
private:
    sf::RenderWindow window;
    Renderer renderer;
    Mesh mesh;
    Mesh mesh2;
    void input_reader();
    void render();
    

public:
    
    App();
    void run();
};