#include "stdafx.h"
#include <sstream>
#include "SFML\Graphics.hpp"
#include "SFML\Audio.hpp"


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
	default:
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
	Texture textureGravestone;
	Texture textureAxe;
	Texture textureLog;

	textureCloud.loadFromFile("graphics/cloud.png");
	textureBee.loadFromFile("graphics/bee.png");
	textureTree.loadFromFile("graphics/tree.png");
	textureBackground.loadFromFile("graphics/background.png");
	textureBranch.loadFromFile("graphics/branch.png");
	texturePlayer.loadFromFile("graphics/player.png");
	textureGravestone.loadFromFile("graphics/rip.png");
	textureAxe.loadFromFile("graphics/axe.png");
	textureLog.loadFromFile("graphics/log.png");

	//Create and attach Sprite to texture
	Sprite spriteBackground;
	Sprite spriteTree;
	Sprite spriteBee;
	Sprite spriteCloud;
	Sprite spriteCloud2;
	Sprite spriteCloud3;
	Sprite spritePlayer;
	Sprite spriteGravestone;
	Sprite spriteAxe;
	Sprite spriteLog;

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
	spritePlayer.setPosition(1200, 720);
	side playerSide = side::RIGHT;
	
	spriteGravestone.setTexture(textureGravestone);
	spriteGravestone.setPosition(600, 2000);
	
	spriteAxe.setTexture(textureAxe);
	spriteAxe.setPosition(1075, 830);
	const float axePositionLeft = 1075;
	const float axePositionRight = 700;

	spriteLog.setTexture(textureLog);
	spriteLog.setPosition(810, 720);
	bool logActive = false;
	float logSpeedX = 1000;
	float logSpeedY = -1500;
	

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
	float remainingTime = 6.0f;
	float timeBarWidthPerSecond = timebarStartWidth / remainingTime;

	updateBranches(1);
	updateBranches(2);
	updateBranches(3);
	updateBranches(4);
	updateBranches(5);

	bool acceptInput = false;

	//sound handling
	SoundBuffer chopBuffer;
	SoundBuffer deathBuffer;
	SoundBuffer ootBuffer;

	chopBuffer.loadFromFile("sound/chop.wav");
	deathBuffer.loadFromFile("sound/death.wav");
	ootBuffer.loadFromFile("sound/out_of_time.wav");
	
	Sound chop;
	chop.setBuffer(chopBuffer);
	chop.setVolume(20);
	Sound death;
	death.setBuffer(deathBuffer);
	death.setVolume(5);
	Sound outOfTime;
	outOfTime.setBuffer(ootBuffer);
	outOfTime.setVolume(20);

	while (window.isOpen())
	{
		//Handle Input

		Event event;
		
		while (window.pollEvent(event))
		{
			if (event.type == Event::KeyReleased &&!paused)
			{
				acceptInput = true;

				//hide the axe
				spriteAxe.setPosition(2000, spriteAxe.getPosition().y);
			}
		}

		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}

		if (Keyboard::isKeyPressed(Keyboard::Return))
		{
			paused = false;
			score = 1;
			remainingTime = 6;
		}

		//Wrap player Input
		if (acceptInput)
		{
			if (Keyboard::isKeyPressed(Keyboard::Right))
			{
				playerSide = side::RIGHT;
				score++;
				remainingTime += (2 / score) + .15;

				spriteAxe.setPosition(axePositionLeft,spriteAxe.getPosition().y);

				spritePlayer.setPosition(1200, 720);

				//update branches
				updateBranches(score);

				//animate logs
				spriteLog.setPosition(810, 720);
				logSpeedX = -5000;
				logActive = true;

				acceptInput = false;

				// Play a chop sound
				chop.play();
			}
			if (Keyboard::isKeyPressed(Keyboard::Left))
			{
				
				playerSide = side::LEFT;
				score++;
				
				remainingTime += (2 / score) + .15;
				spriteAxe.setPosition(axePositionRight,spriteAxe.getPosition().y);
				spritePlayer.setPosition(580, 720);
				//update branches
				updateBranches(score);
				//animate logs
				spriteLog.setPosition(810, 720);
				logSpeedX = 5000;
				logActive = true;
				acceptInput = false;

				// Play a chop sound
				chop.play();
			}
		}
		
		if (!paused)
		{


			//Update Scene

			//Measure time		

			Time dt = clock.restart();
			remainingTime -= dt.asSeconds();

			//resize timebar

			timeBar.setSize(Vector2f(timeBarWidthPerSecond*remainingTime, timebarHeight));

			if (remainingTime <= 0.0f)
			{
				paused = true;
				messageText.setString("Out of time!!");
				FloatRect textRect = messageText.getLocalBounds();
				messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);

				// Play the out of time sound
				outOfTime.play();
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

			//Handle log animation
			if (logActive)
			{
				spriteLog.setPosition(spriteLog.getPosition().x + (logSpeedX*dt.asSeconds()), spriteLog.getPosition().y + (logSpeedY*dt.asSeconds()));
				if (spriteLog.getPosition().x < -100 || spriteLog.getPosition().x>2000)
				{
					logActive = false;
					spriteLog.setPosition(800, 600);
				}
			}

			//Death handling
			if (branchPosition[5] == playerSide)
			{
				paused = true;
				acceptInput = false;


				spriteGravestone.setPosition(525, 760);
				spritePlayer.setPosition(2000, 2000);
				messageText.setString("SQUISHED!! LUL GIT GUT");
				FloatRect textRect = messageText.getLocalBounds();
				messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
				messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

				// Play the death sound
				death.play();
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
		window.draw(spritePlayer);
		window.draw(spriteGravestone);
		window.draw(spriteAxe);
		window.draw(spriteLog);

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
