#pragma once
#include <iostream>
#include <cmath>
#include <random>
#include <algorithm>
#include <SFML/Graphics.hpp>

//Cat-communist
class Mouse
{
public:
    //mouse position
    float x, y;

    //states
    bool left_click;
    bool right_click;
    bool leftPress;
    bool leftRelease;
    bool rightPress;
    bool rightRelease;

public:
    void Update(sf::RenderWindow& window)
    {
        //reset states
        leftPress = false;
        leftRelease = false;
        rightPress = false;
        rightRelease = false;

        x = sf::Mouse::getPosition(window).x;
        y = sf::Mouse::getPosition(window).y;

        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
        {
            if (!left_click)
                leftPress = true;
            left_click = true;
        }
        else
        {
            if (left_click)
                leftRelease = true;
            left_click = false;
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
        {
            if (!right_click)
            {
                rightPress = true;
            }
            else
            {
                rightPress = false;
            }
            right_click = true;
        }
        else
        {
            if (right_click)
            {
                rightRelease = true;
            }
            right_click = false;
        }

    }

    void Reset() {
        x = y = -1000;
        left_click = false;
        right_click = false; 
        leftPress = false;
        leftRelease = false;
        rightPress = false;
        rightRelease = false;
    }
};

class Ship
{
public:
    //position 
    float x = 0.f;
    float y = 0.f;

    //last valid position
    sf::Vector2f last;

    //size
    int decks; //amount of cells
    float cell = 50.f; //one cell size
    float height = cell;
    float width;

    //offsets
    float offset_x = 0.f;
    float offset_y = 0.f;

    //states
    bool isDragged = false;
    bool wasClicked = false;

    //decor
    sf::Color color;
private:
    sf::RectangleShape rect;
public:
    //constructor
    Ship() = default;

    Ship(int decks, sf::Vector2f pos, sf::Color col)
        : decks(decks), x(pos.x), y(pos.y), color(col), last(pos), width(decks * cell)
    {
        rect.setSize({ width, height});
        rect.setFillColor(color);
        rect.setPosition({x, y});
    }

    //Cat_communist
    sf::Vector2f getPosition() const {
        return sf::Vector2f(x, y);
    }

    //Cat_communist
    bool isInside(float x, float y)
    {
        if (x > this->x && x < this->x + width
            && y > this->y && y < this->y + height)
            return true;
        return false;
    }

    //makes shape draggable (Cat_communist)
    void Draggable(Mouse& mouse)
    {
        if (mouse.leftPress && isInside(mouse.x, mouse.y))
        {
            offset_x = mouse.x - x;
            offset_y = mouse.y - y;

            isDragged = true;
        }
        else if (mouse.leftRelease)
            isDragged = false;
        if (isDragged)
        {
            x = mouse.x - offset_x;
            y = mouse.y - offset_y;
        }
        rect.setPosition({ x,y });
    }

    //draws shape and refreshes its position (Cat_communist)
    void Draw(sf::RenderWindow& window)
    {
        rect.setSize({ width, height });
        rect.setFillColor(color);
        rect.setPosition({ x,y });
        window.draw(rect);
    }

    //swaps sides of a shape (Cat_communist)
    void swapSides(float& w, float& h)
    {
        float temp = w;
        w = h;
        h = temp;
    }

    //makes shape rotatable (Cat_communist)
    void Rotatable(Mouse& mouse)
    {
        if (!isDragged) //if not dragged
        {
            if (wasClicked)
            {
                swapSides(width, height);
                wasClicked = false;
            }
            if (mouse.rightRelease && isInside(mouse.x, mouse.y))
            {
                swapSides(width, height);
            }
        }
    }

    //sets position of shape (Cat_communist)
    void setPosition(sf::Vector2f pos)
    {
        x = pos.x;
        y = pos.y;
        rect.setPosition({ x,y });
    }

    //updating last valid position
    void updateLastValidPosition()
    {
        if (color == sf::Color(0, 170, 255))
        {
            last = { rect.getPosition().x, rect.getPosition().y };
        }
    }

    //go back to last (Cat_communist)
    void revertToLastPosition() 
    {
        x = last.x;
        y = last.y;
        rect.setPosition(last); 
        color = sf::Color(0, 170, 255); 
    }

};

class BattleCell {
public:
    //position
    float x = 0.f;
    float y = 0.f;

    //size of button
    int button_size = 50.f;
public:
    BattleCell() = default;
    //cunstructor of the battlecel, setting initial size, background color and outline color
    BattleCell(sf::Vector2f size, const sf::Color bgColor, sf::Color olColor, int index)
        : index(index)
    {
        button.setSize(size);
        button.setFillColor(bgColor);
        button.setOutlineColor(olColor);
        button.setOutlineThickness(1.f);
    }

    //setting the button background color (Flarishon)
    void setBackColor(sf::Color color)
    {
        button.setFillColor(color);
    }

    //(Flarishon)
    sf::Color getBackColor() const {
        return button.getFillColor();
    }

    //setting the button outline color (Flarishon)
    void setLineColor(sf::Color color)
    {
        button.setOutlineColor(color);
    }

    //setting the position of the button (Flarishon)
    void setPosition(sf::Vector2f pos)
    {
        button.setPosition(pos);
        x = pos.x;
        y = pos.y;
    }

    void setOrigin(sf::Vector2f pos)
    {
        button.setOrigin(pos);
    }

    sf::Vector2f getPosition()
    {
        return button.getPosition();
    }

    //funtion to draw button
    void drawTo(sf::RenderWindow& window)
    {
        window.draw(button);
    }

    //check if mouse is over the button (Flarishon)
    bool isMouseOver(sf::RenderWindow& window)
    {
        int mouseX = sf::Mouse::getPosition(window).x;
        int mouseY = sf::Mouse::getPosition(window).y;

        float btnPosX = button.getPosition().x;
        float btnPosY = button.getPosition().y;

        float btnxPosWidth = button.getPosition().x + button.getSize().x;
        float btnyPosWidth = button.getPosition().y + button.getSize().y;

        if (mouseX < btnxPosWidth && mouseX > btnPosX && mouseY < btnyPosWidth && mouseY > btnPosY)
        {
            return true;
        }
        return false;
    }

    //(Flarishon)
    void setIndex(int newIndex)
    {
        index = newIndex;
    }

    //(Flarishon)
    int getIndex() const
    {
        return index;
    }

    //(Cat-Communist)
    bool MouseisInside(float x, float y)
    {
        if (x > this->x && x < this->x + button_size
            && y > this->y && y < this->y + button_size)
        {
            return true;
        }
        return false;
    }
    
    //(Cat-Communist)
    void Alignbutton(Mouse& mouse, Ship& rect)
    {
        if (rect.isDragged && MouseisInside(mouse.x, mouse.y))
        {
            if (rect.height >= rect.width)  
            {
                rect.setPosition(button.getPosition() - sf::Vector2f{ 0, floor(rect.offset_y / 50) * button_size });
            }
            else
                rect.setPosition(button.getPosition() - sf::Vector2f{ floor(rect.offset_x / 50) * button_size, 0 });
        }
    }

    //use only with battle_cell[0][0] (Cat-Communist)
    bool checkBoundary(Ship& ship)
    {
        float global_border_x0 = button.getPosition().x;
        float global_border_y0 = button.getPosition().y;

        float global_border_x1 = global_border_x0 + 10 * button_size;
        float global_border_y1 = global_border_y0 + 10 * button_size;

        float ship_border_x1 = ship.x + ship.width;
        float ship_border_y1 = ship.y + ship.height;


        if (ship.x < global_border_x0 || ship.y < global_border_y0
            || ship_border_x1 > global_border_x1 || ship_border_y1 > global_border_y1)
            return true;
        return false;
    }

    //(Cat-Communist)
    bool ShipisOn(Ship rect)
    {
        if (button.getPosition().x >= rect.x && button.getPosition().x < rect.x + rect.width
            && button.getPosition().y >= rect.y && button.getPosition().y < rect.y + rect.height)
            return true;
        return false;
    }
private:
    sf::RectangleShape button;
    int index;
};

class Button {
public:
    //cunstructor of the btn setting initial size, background color and outline color
    Button() = default;
    Button(sf::Vector2f size, const sf::Color bgColor, sf::Color olColor)
    {
        button.setSize(size);
        button.setFillColor(bgColor);
        button.setOutlineColor(olColor);
        button.setOutlineThickness(1.f);
    }

    //setting the button background color
    void setBackColor(sf::Color color)
    {
        button.setFillColor(color);
    }

    //setting the button outline color
    void setLineColor(sf::Color color)
    {
        button.setOutlineColor(color);
    }

    //setting the position of the button
    void setPosition(sf::Vector2f pos)
    {
        button.setPosition(pos);

        float xPos = (pos.x + button.getSize().x / 2); //NOTE: Why? For what purpose?
        float yPos = (pos.y + button.getSize().y / 2);
    }

    sf::Vector2f getPosition()
    {
        return button.getPosition();
    }

    void setOrigin(sf::Vector2f pos)
    {
        button.setOrigin(pos);
    }

    //funtion to draw button
    void drawTo(sf::RenderWindow& window)
    {
        window.draw(button);
    }

    //check if mouse is over the button
    bool isMouseOver(sf::RenderWindow& window)
    {
        int mouseX = sf::Mouse::getPosition(window).x;
        int mouseY = sf::Mouse::getPosition(window).y;

        float btnPosX = button.getPosition().x - button.getSize().x / 2;
        float btnPosY = button.getPosition().y - button.getSize().y / 2;

        float btnxPosWidth = btnPosX + button.getSize().x;
        float btnyPosWidth = btnPosY + button.getSize().y;

        if (mouseX < btnxPosWidth && mouseX > btnPosX && mouseY < btnyPosWidth && mouseY > btnPosY)
        {
            return true;
        }
        return false;
    }
private:
    sf::RectangleShape button;
};

//functions (Cat-Communist)
inline bool checkCollision(Ship* currentShip, const std::vector<Ship*>& otherShips) {
    for (const Ship* otherShip : otherShips) {
        if (currentShip == otherShip)
            continue;

        // Проверка пересечения (прямое наложение)
        bool isOverlapping =
            currentShip->x < otherShip->x + otherShip->width &&
            currentShip->x + currentShip->width > otherShip->x &&
            currentShip->y < otherShip->y + otherShip->height &&
            currentShip->y + currentShip->height > otherShip->y;

        if (isOverlapping) {
            return true;
        }

        // Проверка расстояния 
        // Область вокруг корабля
        float expandedLeft = currentShip->x - currentShip->cell;
        float expandedRight = currentShip->x + currentShip->width + currentShip->cell;
        float expandedTop = currentShip->y - currentShip->cell;
        float expandedBottom = currentShip->y + currentShip->height + currentShip->cell;

        // Проверяем, попадает ли другой корабль в расширенную область
        bool isTooClose =
            otherShip->x < expandedRight &&
            otherShip->x + otherShip->width > expandedLeft &&
            otherShip->y < expandedBottom &&
            otherShip->y + otherShip->height > expandedTop;

        if (isTooClose) {
            return true;
        }
    }
    return false;
}

//(Cat-Communist)
inline bool RandomPlacing(BattleCell field[10][10], Ship* ship, std::vector<Ship*> otherShips)
{
    srand(time(NULL));

    std::mt19937 gen(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<> pos_dist(0, 9);
    std::uniform_int_distribution<> rot_dist(0, 1);

    int random_x = pos_dist(gen);
    int random_y = pos_dist(gen);
    bool random_state = rot_dist(gen);
    ship->setPosition(field[random_y][random_x].getPosition());
    ship->wasClicked = random_state;
    if (random_state)
    {
        ship->swapSides(ship->width, ship->height);
        ship->wasClicked = false;
    }

    if (field[0][0].checkBoundary(*ship) || checkCollision(ship, otherShips))
    {
        return true;
    }

    return false;
}

//Cat-Communist)
inline void RandomShot(Mouse& mouse, BattleCell field[10][10])
{
    std::mt19937 gen(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<> dist(0, 9);
    bool full = true;
    int random_x;
    int random_y;
    while(full)
    {
        random_x = dist(gen);
        random_y = dist(gen);
        if (field[random_x][random_y].getBackColor() != sf::Color::White &&
            field[random_x][random_y].getBackColor() != sf::Color::Red)
        {
            full = false;
        }
    }
    
    mouse.x = field[random_x][random_y].getPosition().x;
    mouse.y = field[random_x][random_y].getPosition().y;

    mouse.leftRelease = true;
}