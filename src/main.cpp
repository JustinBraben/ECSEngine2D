#include <SFML/Graphics.hpp>
#include <iostream>
#include <filesystem>
#include "Student.h"
#include "Course.h"
#include "DynamicArray.h"
#include "../assignment1/main.cpp"

void helloWorld() {
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Hello, World!");

    // Get the path to the current source file
    std::filesystem::path srcPath(__FILE__);

    // Construct the correct path to the font file, goes back another parent path to get to assets folder
    std::filesystem::path fontPath = srcPath.parent_path().parent_path() / "assets" / "arial.ttf";

    std::cout << fontPath << '\n';

    // Load a font
    sf::Font font;
    if (!font.loadFromFile(fontPath.string())) {
        // Error handling (e.g., use the default font or exit)
    }

    // Set up the "Hello, World!" text
    sf::Text text("Hello, World!", font, 24);
    text.setFillColor(sf::Color::White);
    text.setPosition(300, 280);

    // Main loop
    while (window.isOpen()) {
        // Process events
        sf::Event event;
        while (window.pollEvent(event)) {
            // Close window: exit
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Clear screen
        window.clear(sf::Color::Black);

        // Draw the text
        window.draw(text);

        // Update the window
        window.display();
    }
}

void course() 
{
    Course c("COMP 4300");
    c.loadFromFile("../../assets/students.txt");

    c.print();
}

void pp(int& i)
{
    std::cout << &i << " " << i << " " << sizeof(i) << "\n";
}

void dynamicArrayExample()
{
    DynamicArray<float> myArray(10);

    myArray[4] = 7.14;
    myArray[2] = 134.420;

    myArray.print();
}

int main(int argc, char *argv[]) 
{
    printConfig();

    return 0;
}