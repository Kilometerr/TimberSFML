#include "stdafx.h"
#include <sstream>
#include "SFML\Graphics.hpp"


using namespace sf;
using namespace std;

//branches setup
void updateBranches(int seed);
const int numBranches = 6;
Sprite branches[numBranches];
enum class side { LEFT, RIGHT, NONE };
side branchPosition[numBranches];

void updateBranches(int seed)
{
	// Move all the branches down one place
	for (int j = numBranches - 1; j > 0; j--)
	{
		branchPosition[j] = branchPosition[j - 1];
	}
	
	srand((int)time(0) + seed);
	int r = (rand() % 5);
	switch (r)
	{
	case 0:
		branchPosition[0] = side::LEFT;
		break;
	case 1:
		branchPosition[0] = side::RIGHT;
		break;
	case 2:
		branchPosition[0] = side::NONE;
		break;
	}
}

int main()
{
	//Create window
	VideoMode vm(1920, 1080);
	RenderWindow window(vm, "TimberSFML", Style::Fullscreen);

	//Create and load Texture
	Texture textureBackground;
	Texture textureTree;
	Texture textureBee;
	Texture textureCloud;
	Texture textureBranch;
	Texture texturePlayer;

	textureCloud.loadFromFile("graphics/cloud.png");
	textureBee.loadFromFile("graphics/bee.png");
	textureTree.loadFromFile("graphics/tree.png");
	textureBackground.loadFromFile("graphics/background.png");
	textureBranch.loadFromFile("graphics/branch.png");
	texturePlayer.loadFromFile("graphics/player.png");

	//Create and attach Sprite to texture
	Sprite spriteBackground;
	Sprite spriteTree;
	Sprite spriteBee;
	Sprite spriteCloud;
	Sprite spriteCloud2;
	Sprite spriteCloud3;
	Sprite spritePlayer;

	//branch sprite func
	for (int i = 0; i < numBranches; i++)
	{
		branches[i].setTexture(textureBranch);
		branches[i].setPosition(-2000, -2000);
		branches[i].setOrigin(220, 20);
	}

	//Assign textures, pos, side etc
	spriteCloud.setTexture(textureCloud);
	spriteCloud2.setTexture(textureCloud);
	spriteCloud3.setTexture(textureCloud);
	
	spriteCloud.setPosition(0, 0);
	spriteCloud2.setPosition(0, 250);
	spriteCloud3.setPosition(0, 500);
	
	spriteTree.setTexture(textureTree);
	spriteTree.setPosition(810, 0);
	
	spriteBackground.setTexture(textureBackground);
	spriteBackground.setPosition(0, 0);
	
	spriteBee.setTexture(textureBee);
	spriteBee.setPosition(0, 810);
	
	spritePlayer.setTexture(texturePlayer);
	spritePlayer.setPosition(580, 720);
	side playerSide = side::LEFT;

	//cloud movement
	bool cloudActive = false;
	bool cloud2Active = false;
	bool cloud3Active = false;
	float cloudSpeed = 0.0f;
	float cloud2Speed = 0.0f;
	float cloud3Speed = 0.0f;
	
	//bee basic movement
	bool beeActive = false;
	float beeSpeed = 0.0f;

	
	//FONTS AND TEXT
	int score = 0;

	Text messageText;
	Text scoreText;

	Font font;
	font.loadFromFile("fonts/KOMIKAP_.ttf");

	messageText.setFont(font);
	messageText.setString("Press Enter to start!");
	messageText.setCharacterSize(75);
	messageText.setFillColor(Color::White);
	scoreText.setFont(font);
	scoreText.setString("Score = 0");
	scoreText.setCharacterSize(100);
	scoreText.setFillColor(Color::White);

	//position of text
	FloatRect textRect = messageText.getLocalBounds();
	messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
	scoreText.setPosition(20, 20);

	//Variables for time
	Clock clock;
	bool paused = true;

	//Time bar
	RectangleShape timeBar;
	float timebarStartWidth = 400;
	float timebarHeight = 80;
	timeBar.setSize(Vector2f(timebarStartWidth, timebarHeight));
	timeBar.setFillColor(Color::Yellow);
	timeBar.setPosition((1920 / 2) - timebarStartWidth / 2, 980);

	Time gameTimeTotal;
	float reaminingTime = 6.0f;
	float timeBarWidthPerSecond = timebarStartWidth / reaminingTime;

	updateBranches(1);
	updateBranches(2);
	updateBranches(3);
	updateBranches(4);
	updateBranches(5);

	while (window.isOpen())
	{
		//Handle Input
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}

		if (Keyboard::isKeyPressed(Keyboard::Return))
		{
			paused = false;
			score = 0;
			reaminingTime = 5;
		}

		if (!paused)
		{


			//Update Scene

			//Measure time		

			Time dt = clock.restart();
			reaminingTime -= dt.asSeconds();

			//resize timebar

			timeBar.setSize(Vector2f(timeBarWidthPerSecond*reaminingTime, timebarHeight));

			if (reaminingTime <= 0.0f)
			{
				paused = true;
				messageText.setString("Out of time!!");
				FloatRect textRect = messageText.getLocalBounds();
				messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
			}

			//Bee main movement
			if (!beeActive)
			{
				//speed
				srand((int)time(0));
				beeSpeed = (rand() % 200 + 200);

				//height
				srand((int)time(0) * 10);
				float height = (rand() % 500) + 500;
				spriteBee.setPosition(2000, height);
				beeActive = true;
			}
			else
			{
				spriteBee.setPosition(spriteBee.getPosition().x - (beeSpeed*dt.asSeconds()), spriteBee.getPosition().y);
				if (spriteBee.getPosition().x < -100)
				{
					beeActive = false;
				}
			}

			//Cloud main movement

			//Cloud1
			if (!cloudActive)
			{
				//speed
				srand((int)time(0 * 10));
				cloudSpeed = (rand() % 200);

				//height
				srand((int)time(0) * 10);
				float height = (rand() % 150);
				spriteCloud.setPosition(-200, height);
				cloudActive = true;

			}
			else
			{
				spriteCloud.setPosition(spriteCloud.getPosition().x + (cloudSpeed*dt.asSeconds()), spriteCloud.getPosition().y);
				if (spriteCloud.getPosition().x > 1920)
				{
					cloudActive = false;
				}
			}

			//Cloud2
			if (!cloud2Active)
			{
				//speed
				srand((int)time(0 * 20));
				cloud2Speed = (rand() % 200);

				//height
				srand((int)time(0) * 20);
				float height = (rand() % 300) - 150;
				spriteCloud2.setPosition(-200, height);
				cloud2Active = true;

			}
			else
			{
				spriteCloud2.setPosition(spriteCloud2.getPosition().x + (cloud2Speed*dt.asSeconds()), spriteCloud2.getPosition().y);
				if (spriteCloud2.getPosition().x > 1920)
				{
					cloud2Active = false;
				}
			}

			//Cloud3
			if (!cloud3Active)
			{
				//speed
				srand((int)time(0 * 30));
				cloud3Speed = (rand() % 200);

				//height
				srand((int)time(0) * 300);
				float height = (rand() % 450) - 150;
				spriteCloud3.setPosition(-200, height);
				cloud3Active = true;

			}
			else
			{
				spriteCloud3.setPosition(spriteCloud3.getPosition().x + (cloud3Speed*dt.asSeconds()), spriteCloud3.getPosition().y);
				if (spriteCloud3.getPosition().x > 1920)
				{
					cloud3Active = false;
				}
			}

			//Update score
			stringstream ss;
			ss << "Score =" << score;
			scoreText.setString(ss.str());
			
			//update branches
			for (int i = 0; i < numBranches; i++)
			{
				float height = i * 150;
				if (branchPosition[i] == side::LEFT)
				{
					branches[i].setPosition(610, height);
					branches[i].setRotation(180);
				}
				else if (branchPosition[i] == side::RIGHT)
				{
					branches[i].setPosition(1330, height);
					branches[i].setRotation(0);
				}
				else
				{
					branches[i].setPosition(3000, height);
				}
			}

		}//end if(!paused)

		 //**DRAW SCENE**
		 //Clear Scene

		window.clear();

		//Draw Game Scene
		window.draw(spriteBackground);
		window.draw(spriteCloud);
		window.draw(spriteCloud2);
		window.draw(spriteCloud3);
		for (int i = 0; i < numBranches; i++)//draw branches
		{
			window.draw(branches[i]);
		}
		window.draw(spriteTree);
		window.draw(spriteBee);
		window.draw(scoreText);

		


		if (paused)
		{
			window.draw(messageText);
		}

		window.draw(timeBar);

		//Display Scene

		window.display();
	}
	return 0;
}
