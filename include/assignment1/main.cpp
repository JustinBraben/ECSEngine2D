#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <SFML/Graphics.hpp>

class TextComponent {
protected:
    std::string m_text;
    sf::Color m_color;
    sf::Font m_font;
    sf::Text m_sfText;
    float m_textSize = 30.0f;

public:
    TextComponent(const std::string& textToDisplay, const sf::Color& textColor, sf::Font& textFont, float textSize)
        : m_text(textToDisplay), m_color(textColor), m_font(textFont), m_textSize(textSize) {

        m_sfText.setFont(m_font);

        m_sfText.setCharacterSize(m_textSize);
        m_sfText.setString(m_text);
        m_sfText.setFillColor(m_color);
    }

    void setPosition(float xPos, float yPos) {
        m_sfText.setPosition(xPos, yPos);
    }

    void draw(sf::RenderWindow& window) const {
        window.draw(m_sfText);
    }

    sf::Text getText() {
        return m_sfText;
    }

    sf::Vector2f getCenter() const {
        sf::FloatRect bounds = m_sfText.getGlobalBounds();
        return sf::Vector2f(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
    }
};

class Entity {
protected:
    float m_xPos = 40.0f;
    float m_yPos = 40.0f;

public:
    Entity(float startX, float startY) :
        m_xPos(startX), m_yPos(startY) {}

    float getX() { return m_xPos; }
    float getY() { return m_yPos; }

    virtual void setPosition(float newX, float newY) = 0;

    virtual void update(sf::RenderWindow& window) = 0;

    virtual void draw(sf::RenderWindow& window) const = 0;

    virtual void print() const = 0;
};

class Dynamic : public Entity
{
protected:
    float m_xSpeed = 0.f;
    float m_ySpeed = 0.f;

public:
    Dynamic(float startX, float startY, float xSpeed, float ySpeed)
        : Entity(startX, startY), m_xSpeed(xSpeed), m_ySpeed(ySpeed) {}

    void setPosition(float newX, float newY) {
        m_xPos = newX;
        m_yPos = newY;
    }

    void update(sf::RenderWindow& window) override {
        setPosition(getX() + m_xSpeed, getY() + m_ySpeed);
    }

    void draw(sf::RenderWindow& window) const override = 0;

    void print() const override {}
};

class Shape : public Dynamic {
protected:
    std::string m_shapeType;
    sf::Color m_shapeColor;

public:
    Shape(float startX, float startY, float xSpeed, float ySpeed, const std::string& shapeType, const sf::Color& shapeColor)
        : Dynamic(startX, startY, xSpeed, ySpeed), m_shapeType(shapeType), m_shapeColor(shapeColor) {}

    void draw(sf::RenderWindow& window) const override = 0;
};

class Circle : public Shape {
protected:
    float m_radius = 10.0f;
    std::unique_ptr<TextComponent> m_textComponent;

public:
    Circle(float startX, float startY, float xSpeed, float ySpeed, const std::string& shapeType, sf::Color& shapeColor, float radius, std::unique_ptr<TextComponent> textComponent)
        : Shape(startX, startY, xSpeed, ySpeed, shapeType, shapeColor), m_radius(radius), m_textComponent(std::move(textComponent)) {}

    void update(sf::RenderWindow& window) override {
        float leftBound = 0.f;
        float rightBound = window.getSize().x - 2 * m_radius;
        float topBound = 0.f;
        float bottomBound = window.getSize().y - 2 * m_radius;

        // Update position
        float newX = getX() + m_xSpeed;
        float newY = getY() + m_ySpeed;

        // Check if out of bounds
        if (newX < leftBound || newX > rightBound) {
            m_xSpeed *= -1.f;
        }
        if (newY < topBound || newY > bottomBound) {
            m_ySpeed *= -1.f;
        }

        // Adjust position if out of bounds
        if (newX < leftBound) {
            newX = leftBound;
        }
        else if (newX > rightBound) {
            newX = rightBound;
        }
        if (newY < topBound) {
            newY = topBound;
        }
        else if (newY > bottomBound) {
            newY = bottomBound;
        }

        setPosition(newX, newY);
    }

    void draw(sf::RenderWindow& window) const override {
        sf::CircleShape circ(m_radius);
        circ.setFillColor(m_shapeColor);
        circ.setPosition(m_xPos, m_yPos);
        window.draw(circ);

        if (m_textComponent) {
            m_textComponent->setPosition(m_xPos, m_yPos);

            // Get the global bounds of the shape
            sf::FloatRect shapeBounds = circ.getGlobalBounds();

            // Get the global bounds of the text
            sf::FloatRect textBounds = m_textComponent->getText().getGlobalBounds();

            // Calculate the position of the text
            float xText = shapeBounds.left + (shapeBounds.width - textBounds.width) / 2.0f;
            float yText = shapeBounds.top + (shapeBounds.height - textBounds.height) / 2.0f;

            // Set the position of the text
            m_textComponent->setPosition(xText, yText);

            m_textComponent->draw(window);
        }
    }
};

class Rectangle : public Shape {
protected:
    float m_width = 100.0f;
    float m_height = 100.0f;
    std::unique_ptr<TextComponent> m_textComponent;

public:
    Rectangle(float startX, float startY, float xSpeed, float ySpeed, const std::string& shapeType, sf::Color& shapeColor, float width, float height, std::unique_ptr<TextComponent> textComponent)
        : Shape(startX, startY, xSpeed, ySpeed, shapeType, shapeColor), m_width(width), m_height(height), m_textComponent(std::move(textComponent)) {}

    void update(sf::RenderWindow& window) override {
        float leftBound = 0.f;
        float rightBound = window.getSize().x - m_width;
        float topBound = 0.f;
        float bottomBound = window.getSize().y - m_height;

        // Update position
        float newX = getX() + m_xSpeed;
        float newY = getY() + m_ySpeed;

        // Check if out of bounds
        if (newX < leftBound || newX > rightBound) {
            m_xSpeed *= -1.f;
        }
        if (newY < topBound || newY > bottomBound) {
            m_ySpeed *= -1.f;
        }

        setPosition(newX, newY);
    }

    void draw(sf::RenderWindow& window) const override {
        sf::RectangleShape rect(sf::Vector2f(m_width, m_height));
        rect.setFillColor(m_shapeColor);
        rect.setPosition(m_xPos, m_yPos);
        window.draw(rect);

        if (m_textComponent) {
            m_textComponent->setPosition(m_xPos, m_yPos);

            // Get the global bounds of the shape
            sf::FloatRect shapeBounds = rect.getGlobalBounds();

            // Get the global bounds of the text
            sf::FloatRect textBounds = m_textComponent->getText().getGlobalBounds();

            // Calculate the position of the text
            float xText = shapeBounds.left + (shapeBounds.width - textBounds.width) / 2.0f;
            float yText = shapeBounds.top + (shapeBounds.height - textBounds.height) / 2.0f;

            // Set the position of the text
            m_textComponent->setPosition(xText, yText);

            m_textComponent->draw(window);
        }
    }

    sf::Vector2f getCenter(sf::RectangleShape& rect) const {
        sf::FloatRect bounds = rect.getGlobalBounds();
        return sf::Vector2f(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
    }
};

void runAssignment1(void)
{
    std::cout << std::filesystem::current_path() << "\n";

    std::string data;
    std::ifstream fin("../../include/assignment1/configs/config.txt");

    float fontSize = 1;
    sf::Font font;
    std::string fontPath;
    sf::Color fontColor;

    sf::RenderWindow window;

    std::vector<std::unique_ptr<Shape>> myBouncyEntities;

    std::string line;
    while (std::getline(fin, line)) {
        std::stringstream ss(line);
        std::string type;
        ss >> type;
        if (type == "Window") {
            int width, height;
            ss >> width >> height;
            window.create(sf::VideoMode(width, height), "SFML Window");
            //window.setFramerateLimit(60);
        }
        else if (type == "Font") {
            int r, g, b;
            ss >> fontPath >> fontSize >> r >> g >> b;
            font.loadFromFile(fontPath);
            fontColor = sf::Color(r, g, b);
            // Use font to set text properties
        }
        else if (type == "Circle") {
            std::string name;
            float x, y, dx, dy, radius;
            int r, g, b;
            ss >> name >> x >> y >> dx >> dy >> r >> g >> b >> radius;
            std::cout << name << "\n";
            auto shapeTextComponent = std::make_unique<TextComponent>(name, fontColor, font, fontSize);
            auto circle = std::make_unique<Circle>(x, y, dx, dy, name, sf::Color(r, g, b), radius, std::move(shapeTextComponent));
            myBouncyEntities.push_back(std::move(circle));
        }
        else if (type == "Rectangle") {
            std::string name;
            float x, y, dx, dy, width, height;
            int r, g, b;
            ss >> name >> x >> y >> dx >> dy >> r >> g >> b >> width >> height;
            auto shapeTextComponent = std::make_unique<TextComponent>(name, fontColor, font, fontSize);
            auto rect = std::make_unique<Rectangle>(x, y, dx, dy, name, sf::Color(r, g, b), width, height, std::move(shapeTextComponent));
            myBouncyEntities.push_back(std::move(rect));
        }
    }

    fin.close();

    while (window.isOpen()) {
        sf::Event evnt;

        while (window.pollEvent(evnt)) {
            if (evnt.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        for (auto& bouncyEntity : myBouncyEntities) {
            bouncyEntity->update(window);
            bouncyEntity->draw(window);
        }

        window.display();
    }
}

int assignment1(void)
{
    runAssignment1();
    return 0;
}