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
}

class MouseShipFixture : public ShipFixture
{
public:
	void SetUp() override;
protected:
	Mouse mouse;
	Mouse& ref_mouse = mouse;
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

class BattleCellFixture : public testing::Test {
public:
	void SetUp() override;
protected:
	BattleCell bCell;
};

void BattleCellFixture::SetUp() {
	bCell.setBackColor(sf::Color(123, 124, 125));
	bCell.setPosition({ 125.f, 125.f });
}

class ButtonFixture : public testing::Test {
public:
	void SetUp() override;
protected:
	Button button;
};

void ButtonFixture::SetUp() {
	button = Button(sf::Vector2f(100.f, 50.f), sf::Color::Blue, sf::Color::Black);
	button.setPosition({ 200.f, 150.f });
}

// Тесты для BattleCell
TEST_F(BattleCellFixture, setBackColor)
{
	bCell.setBackColor(sf::Color::Red);
	EXPECT_EQ(sf::Color::Red, bCell.getBackColor());
}

TEST_F(BattleCellFixture, getBackColor)
{
	EXPECT_EQ(sf::Color(123, 124, 125), bCell.getBackColor());
}

TEST_F(BattleCellFixture, setPosition)
{
	bCell.setPosition({ 300.f, 400.f });
	sf::Vector2f pos = bCell.getPosition();
	EXPECT_FLOAT_EQ(300.f, pos.x);
	EXPECT_FLOAT_EQ(400.f, pos.y);
}

TEST_F(BattleCellFixture, getPosition)
{
	sf::Vector2f pos = bCell.getPosition();
	EXPECT_FLOAT_EQ(125.f, pos.x);
	EXPECT_FLOAT_EQ(125.f, pos.y);
}

TEST_F(BattleCellFixture, setIndex)
{
	bCell.setIndex(42);
	EXPECT_EQ(42, bCell.getIndex());
}

TEST_F(BattleCellFixture, getIndex)
{
	bCell.setIndex(15);
	EXPECT_EQ(15, bCell.getIndex());
}

// Тесты для Button
TEST_F(ButtonFixture, setBackColor)
{
	button.setBackColor(sf::Color::Green);
	SUCCEED();
}

TEST_F(ButtonFixture, setPosition)
{
	button.setPosition({ 500.f, 600.f });
	sf::Vector2f pos = button.getPosition();
	EXPECT_FLOAT_EQ(500.f, pos.x);
	EXPECT_FLOAT_EQ(600.f, pos.y);
}

TEST_F(ButtonFixture, getPosition)
{
	sf::Vector2f pos = button.getPosition();
	EXPECT_FLOAT_EQ(200.f, pos.x);
	EXPECT_FLOAT_EQ(150.f, pos.y);
}

// Тест для RandomShot
TEST(RandomShotTest, BasicFunctionality)
{
	Mouse mouse;
	BattleCell field[10][10];

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			field[i][j] = BattleCell(sf::Vector2f(50.f, 50.f), sf::Color::Blue, sf::Color::Black, i * 10 + j);
			field[i][j].setPosition(sf::Vector2f(j * 50.f, i * 50.f));
		}
	}

	field[2][3].setBackColor(sf::Color::White);
	field[5][7].setBackColor(sf::Color::Red);

	RandomShot(mouse, field);

	EXPECT_GE(mouse.x, 0.f);
	EXPECT_GE(mouse.y, 0.f);
	EXPECT_LE(mouse.x, 450.f);
	EXPECT_LE(mouse.y, 450.f);

	int cellX = static_cast<int>(mouse.x / 50.f);
	int cellY = static_cast<int>(mouse.y / 50.f);

	EXPECT_NE(sf::Color::White, field[cellY][cellX].getBackColor());
	EXPECT_NE(sf::Color::Red, field[cellY][cellX].getBackColor());

	EXPECT_TRUE(mouse.leftRelease);
}

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

TEST_F(MouseShipFixture, clicked)
{
	//Act
	ship.Rotatable(ref_mouse);
	EXPECT_FALSE(ship.wasClicked);
}

TEST_F(MouseShipFixture, notclicked)
{
	//Arrange
	ship.wasClicked = false;
	float width = ship.width;

	//Act
	ship.Rotatable(ref_mouse);

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
	ship.Rotatable(ref_mouse);

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
	ship.Rotatable(ref_mouse);

	//Expect
	EXPECT_EQ(width, ship.width);
}

TEST_F(MouseShipFixture, isDragged)
{
	//Arrange
	ship.isDragged = true;
	float width = ship.width;

	//Expect
	EXPECT_EQ(width, ship.width);
}

TEST_F(BattleCellFixture, setBack)
{
	EXPECT_EQ(sf::Color(123, 124, 125), bCell.getBackColor());
}

int main()
{
	setlocale(LC_ALL, "rus");
	testing::InitGoogleTest();
	return RUN_ALL_TESTS();
}