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


int main()
{
	setlocale(LC_ALL, "rus");
	testing::InitGoogleTest();
	return RUN_ALL_TESTS();
}