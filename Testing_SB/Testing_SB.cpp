#include <gtest/gtest.h>
#include "Classes.h"

class ShipFixture : public testing::Test 
{
public:
	void SetUp() override;
protected:
	Ship ship;
};

void ShipFixture::SetUp()
{
	ship.x = 5.0f;
	ship.y = 5.0f;
	ship.width = 50.0f;
	ship.height = 50.0f;
}

class MouseShipFixture : public ShipFixture
{
public:
	void SetUp() override;
protected:
	Mouse mouse;
};

void MouseShipFixture::SetUp()
{
	mouse.rightRelease = true;
	mouse.x = 10.0f; mouse.y = 10.0f;

	ship.isDragged = false;
	ship.wasClicked = true;
	ship.x = 5.0f;
	ship.y = 5.0f;
	ship.width = 50.0f;
	ship.height = 100.0f;
}

class BCMSFixture : public MouseShipFixture
{
public:
	void SetUp() override;
protected:
	BattleCell btn;
	BattleCell field[10][10];
};

void BCMSFixture::SetUp()
{
	ship.isDragged = true;
	ship.height = 50.0f;
	ship.width = 100.0f;

	ship.setPosition({ 5.0f, 5.0f });
	ship.x = 5.0f; ship.y = 5.0f;

	mouse.x = 35.0f;
	mouse.y = 25.0f;

	btn.setPosition({ 30.0f, 2.0f });

	for (int i = 0; i < 10; ++i) {
		for (int j = 0; j < 10; ++j) {
			field[i][j] = BattleCell({ 50, 50 }, sf::Color::Black, sf::Color::White, 0);
		}
	}

	float positionX = 225;
	float positionY = 225;

	for (int i = 0; i < 10; ++i) {
		positionX = 225;
		for (int j = 0; j < 10; ++j) {
			field[i][j].setPosition({ positionX, positionY });
			positionX = positionX + 50;
		}
		positionY = positionY + 50;
	}
}

//isInside
TEST_F(ShipFixture, inside)
{
	EXPECT_TRUE(ship.isInside(10.0f, 10.0f));
}

TEST_F(ShipFixture, outside)
{
	EXPECT_FALSE(ship.isInside(100.0f, 100.0f));
}

TEST_F(ShipFixture, blw_zero)
{
	EXPECT_FALSE(ship.isInside(-100.0f, -100.0f));
}

//Rotatable
TEST_F(MouseShipFixture, clicked)
{
	//Act
	ship.Rotatable(mouse);
	EXPECT_FALSE(ship.wasClicked);
}

TEST_F(MouseShipFixture, notclicked)
{
	//Arrange
	ship.wasClicked = false;
	float width = ship.width;

	//Act
	ship.Rotatable(mouse);

	//Expect
	EXPECT_NE(width, ship.width);
}

TEST_F(MouseShipFixture, miss)
{
	//Arrange
	ship.wasClicked = false;
	float width = ship.width;
	mouse.x = 300.0f; mouse.y = 300.0f;

	//Act
	ship.Rotatable(mouse);

	//Expect
	EXPECT_EQ(width, ship.width);
}

TEST_F(MouseShipFixture, notreleased)
{
	//Arrange
	ship.wasClicked = false;
	float width = ship.width;
	mouse.rightRelease = false;

	//Act
	ship.Rotatable(mouse);

	//Expect
	EXPECT_EQ(width, ship.width);
}

TEST_F(MouseShipFixture, isDragged)
{
	//Arrange
	ship.isDragged = true;
	float width = ship.width;

	//Act empty

	//Expect
	EXPECT_EQ(width, ship.width);
}

//updateLastValidPosition
TEST_F(ShipFixture, isRightPos)
{
	//Arrange
	ship.color = sf::Color(0, 170, 255);
	sf::Vector2f validPos = { ship.x, ship.y };

	//Act
	ship.updateLastValidPosition();

	//Expect
	EXPECT_EQ(validPos, ship.last);
}

TEST_F(ShipFixture, notRightPos)
{
	//Arrange
	ship.color = sf::Color(0, 0, 0);
	sf::Vector2f validPos = { ship.x, ship.y };

	//Act
	ship.updateLastValidPosition();

	//Expect
	EXPECT_NE(validPos, ship.last);
}

//Alignbutton
TEST_F(BCMSFixture, horizontal)
{
	//Arrange
	sf::Vector2f btn_coord = { btn.x, btn.y };

	//Act
	btn.Alignbutton(mouse, ship);
	sf::Vector2f ship_coord = { ship.x, ship.y };

	//Expect
	EXPECT_EQ(btn_coord, ship_coord);
}

TEST_F(BCMSFixture, vertiacl)
{
	//Arrange
	ship.swapSides(ship.width, ship.height);
	sf::Vector2f btn_coord = { btn.x, btn.y };


	//Act
	btn.Alignbutton(mouse, ship);
	sf::Vector2f ship_coord = { ship.x, ship.y };

	//Expect
	EXPECT_EQ(btn_coord, ship_coord);
}

TEST_F(BCMSFixture, nonDragged)
{
	//Arrange
	ship.isDragged = false;
	sf::Vector2f btn_coord = { btn.x, btn.y };

	//Act
	btn.Alignbutton(mouse, ship);
	sf::Vector2f ship_coord = { ship.x, ship.y };

	//Expect
	EXPECT_NE(btn_coord, ship_coord);
}

//ShipisOn
TEST_F(BCMSFixture, on)
{
	//Arrange 
	btn.setPosition({ 35.0f, 5.0f });
	//Act empty

	//Expect
	EXPECT_TRUE(btn.ShipisOn(ship));
}

TEST_F(BCMSFixture, off)
{
	//Arrange empty
	
	//Act empty

	//Expect
	EXPECT_FALSE(btn.ShipisOn(ship));
}

//checkBoundary
TEST_F(BCMSFixture, inBoundary)
{
	//Arrange
	ship.setPosition({ 40.0f, 40.0f });

	//Act empty

	//Expect
	EXPECT_FALSE(btn.checkBoundary(ship));
}

TEST_F(BCMSFixture, outsideBoundary)
{
	//Arrange
	btn.setPosition({ 100.0f, 100.0f });

	//Act empty

	//Expect
	EXPECT_TRUE(btn.checkBoundary(ship));
}

//checkCollision
TEST_F(BCMSFixture, isOverlapping)
{
	//Arrange
	Ship other_ship = ship;
	other_ship.setPosition({ 35.0f, 5.0f });
	std::vector<Ship*> ships;
	ships.push_back(&ship);
	ships.push_back(&other_ship);

	//Act empty

	//Expect
	EXPECT_TRUE(checkCollision(&ship, ships));
}

TEST_F(BCMSFixture, isTooClose)
{
	//Arrange
	Ship other_ship = ship;
	other_ship.setPosition({ 106.0f, 5.0f });
	std::vector<Ship*> ships;
	ships.push_back(&ship);
	ships.push_back(&other_ship);

	//Act empty

	//Expect
	EXPECT_TRUE(checkCollision(&ship, ships));
}

TEST_F(BCMSFixture, isOkay)
{
	//Arrange
	Ship other_ship = ship;
	other_ship.setPosition({ 155.0f, 5.0f });
	std::vector<Ship*> ships;
	ships.push_back(&ship);
	ships.push_back(&other_ship);
	//Act empty

	//Expect
	EXPECT_FALSE(checkCollision(&ship, ships));
}

//RandomPlacing
TEST_F(BCMSFixture, runtime)
{
	//Arrange
	Ship one_deck1(1, { 10.f, 10.f }, sf::Color(0, 170, 255));
	Ship one_deck2(1, { 10.f, 10.f }, sf::Color(0, 170, 255));
	Ship one_deck3(1, { 10.f, 10.f }, sf::Color(0, 170, 255));
	Ship one_deck4(1, { 10.f, 10.f }, sf::Color(0, 170, 255));
	//two-deck
	Ship two_deck1(2, { 10.f, 10.f }, sf::Color(0, 170, 255));
	Ship two_deck2(2, { 10.f, 10.f }, sf::Color(0, 170, 255));
	Ship two_deck3(2, { 10.f, 10.f }, sf::Color(0, 170, 255));
	//three-deck
	Ship three_deck1(3, { 10.f, 10.f }, sf::Color(0, 170, 255));
	Ship three_deck2(3, { 10.f, 10.f }, sf::Color(0, 170, 255));
	//four-deck
	Ship four_deck(4, { 10.f, 10.f }, sf::Color(0, 170, 255));
	std::vector<Ship*> ships_container = {
		//one-deck
		&one_deck1,
		&one_deck2,
		&one_deck3,
		&one_deck4,
		//two-deck
		&two_deck1,
		&two_deck2,
		&two_deck3,
		//three-deck
		&three_deck1,
		&three_deck2,
		//four-deck
		&four_deck
	};
	bool error = true;

	//Act
	for (Ship* ship : ships_container) {
		for (int attempt = 0; attempt < 1000; ++attempt) {
			if (!RandomPlacing(field, ship, ships_container)) {
				error = false;
			}
		}
	}

	//Expect
	EXPECT_FALSE(error);
}

int main()
{
	setlocale(LC_ALL, "rus");
	testing::InitGoogleTest();
	return RUN_ALL_TESTS();
}