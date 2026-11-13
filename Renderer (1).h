#pragma once
#include <SFML/Graphics.hpp>
#include "Core.h"

class Renderer
{
public:


    void move(bool up, bool down, bool left, bool right);
    void draw(sf::RenderWindow& window, Mesh& mesh, Mesh& mesh2);  /// <- ref zeby nie kopiowac vector 
    void scroll(float delta);
    void draw_grid_line(sf::Color color, sf::VertexArray& lines, float x1, float y1, float x2, float y2);
    void draw_point(sf::RenderWindow& window, float x, float y);
    void draw_line(sf::Color color, sf::VertexArray& lines, float x1, float y1, float x2, float y2);

    void draw_circle(sf::RenderWindow& window, const Circle& c);

   

};