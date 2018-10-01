#include "stdafx.h"
#include "SFML\Graphics.hpp"
using namespace sf;

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

	textureCloud.loadFromFile("graphics/cloud.png");
	textureBee.loadFromFile("graphics/bee.png");
	textureTree.loadFromFile("graphics/tree.png");
	textureBackground.loadFromFile("graphics/background.png");

	//Create and attach Sprite to texture
	Sprite spriteBackground;
	Sprite spriteTree;
	Sprite spriteBee;
	Sprite spriteCloud;
	Sprite spriteCloud2;
	Sprite spriteCloud3;

	spriteCloud.setTexture(textureCloud);
	spriteCloud2.setTexture(textureCloud);
	spriteCloud3.setTexture(textureCloud);
	spriteCloud.setPosition(0, 0);
	spriteCloud2.setPosition(0, 250);
	spriteCloud3.setPosition(0, 500);
		//cloud movement
		bool cloudActive = false;
		bool cloud2Active = false;
		bool cloud3Active = false;
		float cloudSpeed = 0.0f;
		float cloud2Speed = 0.0f;
		float cloud3Speed = 0.0f;
	spriteBee.setTexture(textureBee);
	spriteBee.setPosition(0, 810);
		//bee basic movement
		bool beeActive = false;
		float beeSpeed = 0.0f;
	spriteTree.setTexture(textureTree);
	spriteTree.setPosition(810, 0);
	spriteBackground.setTexture(textureBackground);
	spriteBackground.setPosition(0, 0);

	//Variables for time
	Clock clock;
	
	while (window.isOpen())
	{
		//Handle Input
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}

		//Update Scene

			//Measure time		
			Time dt = clock.restart();
			
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
			
			//Cluoud main movement
				
				//Cloud1
				if (!cloudActive)
				{
					//speed
					srand((int)time(0 * 10));
					float height = 
				}
		//Draw Scene
		//Clear Scene
		window.clear();
		//Draw Game Scene
		window.draw(spriteBackground);
		window.draw(spriteTree);
		window.draw(spriteBee);
		window.draw(spriteCloud);
		window.draw(spriteCloud2);
		window.draw(spriteCloud3);
		//Display Scene
		window.display();
	}
    return 0;
}

