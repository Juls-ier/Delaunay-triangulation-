#include "App.h"

App::App()
    : window(sf::VideoMode(1200, 800), "Geometria")
{

    // Bowyerr_Watson

      mesh.load_points("Delunay2p.txt");

      std::cout << "liczba punktow: " << mesh.points.size() << std::endl;

      auto start1 = std::chrono::high_resolution_clock::now();

      mesh.Bowyer_Watson();

      auto end1 = std::chrono::high_resolution_clock::now();

      std::chrono::duration<double> time1 = end1 - start1;

      mesh.delunay_check();

      std::cout << "liczba elementow: " << mesh.triangles.size() << std::endl;
      std::cout << "czas Bowyer-Watson: " << time1.count() << " s\n";

         mesh.analyze_quality();

      // Lawson

      mesh2.load_points("Delunay2l.txt");

      std::cout << "liczba punktow: " << mesh2.points.size() << std::endl;

      auto start2 = std::chrono::high_resolution_clock::now();

      mesh2.Lawson();

      auto end2 = std::chrono::high_resolution_clock::now();

      std::chrono::duration<double> time2 = end2 - start2;

      mesh2.delunay_check();

      std::cout << "liczba elementow: " << mesh2.triangles.size() << std::endl;
      std::cout << "czas Lawson: " << time2.count() << " s\n";



     
      mesh2.analyze_quality();
      
}

void App::run()
{
    
    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {


            if (event.type == sf::Event::MouseWheelScrolled) {
                renderer.scroll(event.mouseWheelScroll.delta);
            }


            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }


        render();
    }
}



void App::input_reader() {


    bool up = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
    bool down = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
    bool left = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
    bool right = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);

    renderer.move(up, down, left, right);


}



void App::render()
{
  
    input_reader();
    window.clear(sf::Color::White);
    renderer.draw(window, mesh, mesh2);
    window.display();
}

