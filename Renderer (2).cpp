#include "Renderer.h"


float offset_x = 0;
float offset_y = 0;

float X0 = 600;
float Y0 = 400;


float moveset = 1;


int spacing = 20;


float zoom = 1.0f;

void Renderer::move(bool up, bool down, bool left, bool right) {


    if (up) {
        offset_y += moveset; 
    }
    if (down) {
        offset_y -= moveset; 
    }
    if (left) {
        offset_x += moveset; 
    }
    if (right) {
        offset_x -= moveset;
    }

   

}

void Renderer::scroll(float delta) {

    spacing +=delta;

}



void Renderer::draw_grid_line(sf::Color color, sf::VertexArray& lines, float x1, float y1, float x2, float y2) {

    lines.append(sf::Vertex(sf::Vector2f(x1 + offset_x, y1 + offset_y), color));
    lines.append(sf::Vertex(sf::Vector2f(x2 + offset_x, y2 + offset_y), color));


}


void Renderer::draw_point(sf::RenderWindow& window, float x, float y){


    sf::CircleShape point(5);
    point.setOrigin(5, 5);
    point.setFillColor(sf::Color::Blue);
    point.setPosition(X0+x*spacing + offset_x, Y0 -y*spacing + offset_y);
    window.draw(point);


}




void Renderer::draw_line(sf::Color color, sf::VertexArray& lines, float x1, float y1, float x2, float y2) {

    lines.append(sf::Vertex(sf::Vector2f(X0+ (x1*spacing) +offset_x, Y0- (y1* spacing) +offset_y), color));
    lines.append(sf::Vertex(sf::Vector2f(X0+ (x2*spacing) +offset_x, Y0 - (y2* spacing) +offset_y), color));


}

void Renderer::draw_circle(sf::RenderWindow& window, const Circle& c) {

    if (c.r <= 0) return; // pomijamy zdegenerowane

    float screen_x = X0 + c.S.x * spacing + offset_x;
    float screen_y = Y0 - c.S.y * spacing + offset_y;

    float radius = c.r * spacing;

    sf::CircleShape circle(radius);
    circle.setOrigin(radius, radius);
    circle.setPosition(screen_x, screen_y);

    circle.setFillColor(sf::Color::Transparent);
    circle.setOutlineColor(sf::Color::Red);
    circle.setOutlineThickness(1);

    window.draw(circle);
}



void Renderer::draw(sf::RenderWindow& window, Mesh& mesh, Mesh& mesh2)
{

    // siatka


    sf::VertexArray grid(sf::Lines);
    sf::Color color_grid(135, 206, 235);
    sf::Color color_oxy(85, 156, 185);

    
   

    for (int i = X0; i <= 3000; i += spacing) {
        draw_grid_line(color_grid, grid, i, Y0 +1500, i, Y0  - 1500);
        draw_grid_line(color_grid, grid, 2*X0-i, Y0  + 1500, 2*X0-i, Y0  - 1500);
    } 


    for (int i = Y0; i <= 3000; i += spacing) {
        draw_grid_line(color_grid, grid, X0+1500, i, X0-1500, i);
        draw_grid_line(color_grid, grid, X0 + 1500, 2*Y0 - i, X0 - 1500, 2 * Y0 - i);
    }

    
    


    draw_grid_line(color_oxy, grid, 0, 400, 1200, 400);   ///osie
    draw_grid_line(color_oxy, grid, 600, 0, 600, 800);

  

    window.draw(grid);

    // punkty
    
   /* for (int i = 0; i < mesh.points.size(); i++) {

        draw_point(window, mesh.points[i].x, mesh.points[i].y);

    }*/

    // trójk¹ty


    sf::VertexArray triLines(sf::Lines);
    sf::Color triColor = sf::Color::Blue;

    for (int i = 0; i < mesh.triangles.size(); i++) {

        Triangle& t = mesh.triangles[i];

        Point& A = mesh.points[t.a];
        Point& B = mesh.points[t.b];
        Point& C = mesh.points[t.c];

        // AB
        draw_line(triColor, triLines, A.x, A.y, B.x, B.y);

        // BC
        draw_line(triColor, triLines, B.x, B.y, C.x, C.y);

        // CA
        draw_line(triColor, triLines, C.x, C.y, A.x, A.y);
    }



    window.draw(triLines);

    // punkty
    for (int i = 0; i < mesh.points.size(); i++) {
        draw_point(window, mesh.points[i].x, mesh.points[i].y);
    }
    

    //for (int i = 0; i < mesh.triangles.size(); i++) {
    //    draw_circle(window, mesh.triangles[i].circle);
    //}



     //mesh2



    

    for (int i = 0; i < mesh2.triangles.size(); i++) {

        Triangle& t = mesh2.triangles[i];

        Point& A = mesh2.points[t.a];
        Point& B = mesh2.points[t.b];
        Point& C = mesh2.points[t.c];

        // AB
        draw_line(triColor, triLines, A.x, A.y, B.x, B.y);

        // BC
        draw_line(triColor, triLines, B.x, B.y, C.x, C.y);

        // CA
        draw_line(triColor, triLines, C.x, C.y, A.x, A.y);
    }


    window.draw(triLines);

    // punkty 
    for (int i = 0; i < mesh2.points.size(); i++) {
        draw_point(window, mesh2.points[i].x, mesh2.points[i].y);
    }

    //for (int i = 0; i < mesh2.triangles.size(); i++) {
    //    draw_circle(window, mesh2.triangles[i].circle);
    //}
    //

   
}


