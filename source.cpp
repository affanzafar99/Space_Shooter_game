#include<iostream>
#include"SFML\Graphics.hpp"
#include"SFML\Window.hpp"
#include"SFML\System.hpp"
#include<math.h>
#include<cstdlib>
#include<vector>


using namespace sf;
using namespace std;

//class for bullet
class Bullet
{
protected:
	Sprite Bulletshape;
public:
	Sprite getshape() {
		return Bulletshape;
	}

	Bullet(Texture *texture, Vector2f pos)
	{
		Bulletshape.setTexture(*texture);

		Bulletshape.setScale(0.07f, 0.07f);

		Bulletshape.setPosition(pos);
	}
	void Bulletmove(float a, float b) {
		Bulletshape.move(a, b);
	}

	~Bullet() {}
};

//class for user's ship
class Player
{
private:
	Sprite Playershape;
	Texture *texture;
	int HP;
	int HPMax;

public:

	Sprite getshape() {
		return Playershape;
	}
	void HPh() {
		HP--;
	}
	std::vector<Bullet> bullets;
	int getHP() {
		return HP;
	}
	int getHPMax() {
		return HPMax;
	}
	void PlayerMove(float a, float b) {
		Playershape.move(a, b);

	}
	Player(Texture *texture)
	{
		HPMax = 10;
		HP = HPMax;

		texture = texture;
		Playershape.setTexture(*texture);

		Playershape.setScale(0.1f, 0.1f);
	}
	void SetPositionTop(RenderWindow &window) {
		Playershape.setPosition(Playershape.getPosition().x, 0.f);
	}
	void SetPositionBottom(RenderWindow &window) {
		Playershape.setPosition(Playershape.getPosition().x, window.getSize().y - Playershape.getGlobalBounds().height);
	}
	void SetPositionLeft(RenderWindow &window) {
		Playershape.setPosition(0.f, Playershape.getPosition().y);
	}
	void SetPositionRight(RenderWindow &window) {
		Playershape.setPosition(window.getSize().x - Playershape.getGlobalBounds().width, Playershape.getPosition().y);
	}
	~Player() {}

};


//class for enemy ships
class Enemy
{
private:
	Sprite Enemyshape;

	int HP;
	int HPMax;

public:
	Sprite &getshape() {
		return Enemyshape;
	}
	void HPh() {
		HP--;
	}
	int getHP() {
		return HP;
	}
	int getHPMax() {
		return HPMax;
	}
	void Enemymove(float a, float b) {
		Enemyshape.move(a, b);
	}
	Enemy(Texture *texture, Vector2u windowSize)
	{
		HPMax = rand() % 3 + 1;
		HP = HPMax;

		Enemyshape.setTexture(*texture);

		Enemyshape.setScale(0.05f, 0.05f);

		Enemyshape.setPosition(windowSize.x - Enemyshape.getGlobalBounds().width, rand() % (int)(windowSize.y - Enemyshape.getGlobalBounds().height));
	}

	~Enemy() {}
};



int main()
{
	srand(time(NULL));

	RenderWindow window(VideoMode(800, 600), "Space Shooter", Style::Default);
	window.setFramerateLimit(60);

	//Init text
	Font write;
	write.loadFromFile("Fonts/Dosis-Light.otf");


	//Init textures
	Texture playerTexture;
	playerTexture.loadFromFile("Textures/ship.png");

	Texture enemyTexture;
	enemyTexture.loadFromFile("Textures/enemy.png");

	Texture bulletTexture;
	bulletTexture.loadFromFile("Textures/missileTex01.png");
	Sprite backg;
	backg.setPosition(Vector2f(0.f, 0.f));
	backg.setScale(Vector2f(3.f, 3.f));
	Texture bg;
	bg.loadFromFile("Textures/space.png");
	backg.setTexture(bg);
	//UI init
	Text scoreText;
	scoreText.setFont(write);
	scoreText.setCharacterSize(20);
	scoreText.setFillColor(Color::White);
	scoreText.setPosition(10.f, 10.f);

	Text gameOverText;
	gameOverText.setFont(write);
	gameOverText.setCharacterSize(30);
	gameOverText.setFillColor(Color::Red);
	gameOverText.setPosition(300.f, window.getSize().y / 2);
	gameOverText.setString("GAME OVER!\nPress ESC to end game or Q to Play Again");

	//Player init
	int score = 0;
	Player player(&playerTexture);
	int shootTimer = 15;
	Text hpText;
	hpText.setFont(write);
	hpText.setCharacterSize(12);
	hpText.setFillColor(Color::White);

	//Enemy init
	int enemySpawnTimer = 0;
	vector<Enemy> enemies;

	Text eHpText;
	eHpText.setFont(write);
	eHpText.setCharacterSize(12);
	eHpText.setFillColor(Color::White);

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
			if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
				player.bullets.push_back(Bullet(&bulletTexture, player.getshape().getPosition()));
				shootTimer = 0; //reset timer
			}

			//Bullets
			for (size_t i = 0; i < player.bullets.size(); i++)
			{
				//Move
				player.bullets[i].Bulletmove(20.f, 0.f);

				//Out of window bounds
				if (player.bullets[i].getshape().getPosition().x > window.getSize().x)
				{
					player.bullets.erase(player.bullets.begin() + i);
					break;
				}

				//Enemy collision
				for (size_t k = 0; k < enemies.size(); k++)
				{
					if (player.bullets[i].getshape().getGlobalBounds().intersects(enemies[k].getshape().getGlobalBounds()))
					{
						if (enemies[k].getHP() <= 1)
						{
							score += enemies[k].getHPMax();


							enemies.erase(enemies.begin() + k);
						}
						else
							enemies[k].HPh(); //ENEMY TAKE DAMAGE

						player.bullets.erase(player.bullets.begin() + i);
						break;
					}
				}
			}

			//Enemy
			if (enemySpawnTimer < 25)
				enemySpawnTimer++;

			//enemy spawn
			if (enemySpawnTimer >= 25)
			{
				enemies.push_back(Enemy(&enemyTexture, window.getSize()));
				enemySpawnTimer = 0; //reset timer

			}

			for (size_t i = 0; i < enemies.size(); i++)
			{

				enemies[i].Enemymove(-5, 0.f);

				if (enemies[i].getshape().getPosition().x <= 0 - enemies[i].getshape().getGlobalBounds().width)
				{
					enemies.erase(enemies.begin() + i);
					break;
				}

				if (enemies[i].getshape().getGlobalBounds().intersects(player.getshape().getGlobalBounds()))
				{
					enemies.erase(enemies.begin() + i);

					player.HPh(); // PLAYER TAKE DAMAGE
					break;
				}

			}

			//UI Update

			scoreText.setString("Score: " + std::to_string(score));

		}


		// Draw == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == = DRAW
		window.clear();

		//player
		window.draw(backg);
		window.draw(player.getshape());

		//Bullets
		for (size_t i = 0; i < player.bullets.size(); i++)
		{
			window.draw(player.bullets[i].getshape());
		}

		//enemy
		for (size_t i = 0; i < enemies.size(); i++)
		{
			eHpText.setString(std::to_string(enemies[i].getHP()) + "/" + std::to_string(enemies[i].getHPMax()));
			eHpText.setPosition(enemies[i].getshape().getPosition().x, enemies[i].getshape().getPosition().y - eHpText.getGlobalBounds().height);
			window.draw(eHpText);
			window.draw(enemies[i].getshape());
		}

		//UI
		window.draw(scoreText);
		window.draw(hpText);

		if (player.getHP() <= 0)

		{

			window.draw(gameOverText);
			if (Keyboard::isKeyPressed(Keyboard::Q))
			{
				window.close();
				main();
			}

		}
		window.display();



	}

	return 0;
	system("pause");
}
