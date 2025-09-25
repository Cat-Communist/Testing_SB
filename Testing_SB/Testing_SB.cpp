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

class BattleCellFixture : public testing::Test {
public: 
	void SetUp() override;
protected:
	BattleCell btn;
};

void BattleCellFixture::SetUp() {
	btn.setBackColor(sf::Color(123, 124, 125));
	btn.setPosition({ 127.f, 140.f });
}

TEST_F(BattleCellFixture, setBack)
{
	EXPECT_EQ(sf::Color(123, 124, 125), btn.getBackColor());
}

TEST(suiteName, setBackColor) {
	BattleCell button;
	button.setBackColor(sf::Color(127, 127, 127));
	EXPECT_EQ(sf::Color(127, 127, 127), button.getBackColor());
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

int main()
{
	setlocale(LC_ALL, "rus");
	testing::InitGoogleTest();
	return RUN_ALL_TESTS();
}