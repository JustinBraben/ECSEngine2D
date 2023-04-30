#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <SFML/Graphics.hpp>

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

    virtual void draw(sf::RenderWindow& window) const override = 0;

    void print() const override {}
};

class Bounce : public Dynamic {
protected:
    float m_width;
    float m_height;

public:
    Bounce(float startX, float startY, float xSpeed, float ySpeed, float width, float height)
        : Dynamic(startX, startY, xSpeed, ySpeed), m_width(width), m_height(height) {}

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

    void draw(sf::RenderWindow& window) const override {};

    void print() const override {}
};

class TextInShape {
private:
    std::string m_fontPath = "../../include/assignment1/configs/config.txt";
    std::string m_shapeName = "Default shape name";
    float m_textSize = 50;
    sf::Color m_fontColor = sf::Color(255, 255, 255);
    sf::Font m_font;
public:
    TextInShape() {}
    TextInShape(std::string fontPath, std::string shapeName, float textSize, sf::Color fontColor, sf::Font& font)
        : m_fontPath(fontPath), m_shapeName(shapeName), m_textSize(textSize), m_fontColor(fontColor), m_font(font) {
    }

    auto getShapeName() const { return m_shapeName; }
    auto getTextSize() const { return m_textSize; }
    auto getFontColor() const { return m_fontColor; }
    auto& getFont() const { return m_font; }
};

class BouncingShape : public Bounce {
protected:
    sf::Color m_shapeColor;

public:
    BouncingShape(float startX, float startY, float xSpeed, float ySpeed, float width, float height, sf::Color shapeColor)
        : Bounce(startX, startY, xSpeed, ySpeed, width, height), m_shapeColor(shapeColor)
    {}

    virtual void draw(sf::RenderWindow& window) const override = 0;
};

class BouncingRectangle : public BouncingShape {
public:
    BouncingRectangle(float startX, float startY, float xSpeed, float ySpeed, float width, float height, sf::Color shapeColor)
        : BouncingShape(startX, startY, xSpeed, ySpeed, width, height, shapeColor)
    {}

    void update(sf::RenderWindow& window) override {
        BouncingShape::update(window);
    }

    void draw(sf::RenderWindow& window) const override {
        sf::RectangleShape rect(sf::Vector2f(m_width, m_height));
        rect.setFillColor(m_shapeColor);
        rect.setPosition(m_xPos, m_yPos);
        window.draw(rect);
    }
};

class BouncingCircle : public BouncingShape {
public:
    BouncingCircle(float startX, float startY, float xSpeed, float ySpeed, float width, float height, sf::Color shapeColor)
        : BouncingShape(startX, startY, xSpeed, ySpeed, width, height, shapeColor)
    {}

    void update(sf::RenderWindow& window) override {
        float leftBound = 0.f;
        float rightBound = window.getSize().x - 2 * m_width;
        float topBound = 0.f;
        float bottomBound = window.getSize().y - 2 * m_width;

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
        sf::CircleShape circ(m_width);
        circ.setFillColor(m_shapeColor);
        circ.setPosition(m_xPos, m_yPos);
        window.draw(circ);
    }
};

class BouncingText : public Bounce {
    std::string m_shapeName;
    sf::Font m_font;
    sf::Color m_textColor;
    float m_textSize;

public:
    BouncingText(float startX, float startY, float xSpeed, float ySpeed, float width, float height, std::string shapeName, sf::Font& font, sf::Color textColor, float textSize)
        : Bounce(startX, startY, xSpeed, ySpeed, width, height), m_shapeName(shapeName), m_font(font), m_textColor(textColor), m_textSize(textSize) {}

    void update(sf::RenderWindow& window) {
        Bounce::update(window);
    }

    void draw(sf::RenderWindow& window) const override {
        sf::Text text(m_shapeName, m_font, m_textSize);
        text.setFillColor(m_textColor);
        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);

        // Calculate center of circle
        sf::Vector2f circleCenter(m_xPos + m_width / 2.0f, m_yPos + m_height / 2.0f);

        // Set the text's position to the center of the circle
        text.setPosition(circleCenter);

        window.draw(text);
    }
};

void runAssignment1(void)
{
    std::cout << std::filesystem::current_path() << "\n";

    std::string data;
    std::ifstream fin("../../include/assignment1/configs/config.txt");

    float fontSize = 1;
    sf::Font shapeFont;
    std::string fontPath;
    sf::Color fontColor;

    TextInShape textShape;

    sf::RenderWindow window;

    std::vector<std::unique_ptr<Bounce>> myBouncyEntities;

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
            shapeFont.loadFromFile(fontPath);
            fontColor = sf::Color(r, g, b);
            // Use font to set text properties
        }
        else if (type == "Circle") {
            std::string name;
            float x, y, dx, dy, radius;
            int r, g, b;
            ss >> name >> x >> y >> dx >> dy >> r >> g >> b >> radius;
            myBouncyEntities.push_back(std::make_unique<BouncingCircle>(x, y, dx, dy, radius, radius, sf::Color(r, g, b)));
            myBouncyEntities.push_back(std::make_unique<BouncingText>(x, y, dx, dy, radius, radius, name, shapeFont, fontColor, fontSize));
        }
        else if (type == "Rectangle") {
            std::string name;
            float x, y, dx, dy, width, height;
            int r, g, b;
            ss >> name >> x >> y >> dx >> dy >> r >> g >> b >> width >> height;
            myBouncyEntities.push_back(std::make_unique<BouncingRectangle>(x, y, dx, dy, width, height, sf::Color(r, g, b)));
            myBouncyEntities.push_back(std::make_unique<BouncingText>(x, y, dx, dy, width, height, name, shapeFont, fontColor, fontSize));
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