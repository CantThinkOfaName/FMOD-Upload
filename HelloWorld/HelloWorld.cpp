////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include "stdafx.h"
#ifdef _DEBUG
#pragma comment(lib,"sfml-graphics-d.lib")
#pragma comment(lib,"sfml-audio-d.lib")
#pragma comment(lib,"sfml-system-d.lib")
#pragma comment(lib,"sfml-window-d.lib")
#pragma comment(lib,"sfml-network-d.lib")
#else
#pragma comment(lib,"sfml-graphics.lib")
#pragma comment(lib,"sfml-audio.lib")
#pragma comment(lib,"sfml-system.lib")
#pragma comment(lib,"sfml-window.lib")
#pragma comment(lib,"sfml-network.lib")
#endif
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")
#include "SFML/Graphics.hpp"
#include "SFML/OpenGL.hpp"

//FMOD includes
#pragma comment(lib,"fmodex_vc.lib")

//#include 
#include "fmod.hpp"
#include "fmod_errors.h"
#include "Map.h"

#include <iostream>


 
 
////////////////////////////////////////////////////////////
///Entrypoint of application
////////////////////////////////////////////////////////////



int main()
{
	// Create the main window
	sf::RenderWindow window(sf::VideoMode(800, 600, 32), "Well here's Some FMOD stuff (c00153187)");
	sf::View view(sf::FloatRect(0,0,800, 600));
	//load a font
	sf::Font font;
	font.loadFromFile("C:\\Windows\\Fonts\\GARA.TTF");
 
	//setup FMOD
	FMOD::System *FMODsys; //will point to the FMOD system
	FMOD_RESULT result;
	result = FMOD::System_Create(&FMODsys);         // Create the main system object.
	if (result != FMOD_OK)
	{
		std::cout << "FMOD error!" <<result << FMOD_ErrorString(result);
		exit(-1);
	} 
	result = FMODsys->init(100, FMOD_INIT_NORMAL, 0);   // Initialize FMOD.   
	if (result != FMOD_OK)	{
		std::cout << "FMOD error!" << result << FMOD_ErrorString(result);
		exit(-1);
	}
	FMOD::Sound *sound;
	result = FMODsys->createSound("../media/wave.mp3", FMOD_DEFAULT, 0, &sound);
	if (result != FMOD_OK)	{
		std::cout << "FMOD error! (%d) %s\n" << result;
		exit(-1);
	}
	FMOD::Sound *step;
	result = FMODsys->createSound("stomp.flac", FMOD_DEFAULT, 0, &step);
	if (result != FMOD_OK)	{
		std::cout << "FMOD error! (%d) %s\n" << result;
		exit(-1);
	}
	FMOD::Sound *fire;
	result = FMODsys->createSound("fire-1.wav", FMOD_DEFAULT, 0, &fire);
	if (result != FMOD_OK)	{
		std::cout << "FMOD error! (%d) %s\n" << result;
		exit(-1);
	}
	FMOD::Sound *land;
	result = FMODsys->createSound("jump.wav", FMOD_DEFAULT, 0, &land);
	if (result != FMOD_OK)	{
		std::cout << "FMOD error! (%d) %s\n" << result;
		exit(-1);
	}
	FMOD::Reverb *reverb;
	result = FMODsys->createReverb(&reverb);
	FMOD_REVERB_PROPERTIES prop = FMOD_PRESET_UNDERWATER;
	reverb->setProperties(&prop);
	FMOD_VECTOR pos = {2200, 200.0,0 };
	float mindist = 100.0f;
	float maxdist = 300.0f;
	reverb->set3DAttributes(&pos, mindist, maxdist);
	FMOD::Channel *BGM;

	FMODsys->playSound(
		FMOD_CHANNEL_FREE, //finda free channel
		sound,              // sound to play
		true,          //start paused
		&BGM);   
	//create a circle
	sf::Texture spriteTexture;
	sf::Texture blockTexture;
	if(!spriteTexture.loadFromFile("texture.png")){
		return -1;
	};
	if(!blockTexture.loadFromFile("Block.png")){
		return -1;
	};
	ifstream stream;
	Map map;
	//ifstream stream;
	map.loadMap();
	TileMap tMap;
	tMap.load(stream, "Tiles.png", sf::Vector2i(64,64));
	sf::Sprite sprite;

	sprite.setTexture(spriteTexture);

	sprite.setPosition(90,90);	

	sf::Vector2f spriteVelocity(0,0);
	//sf::Event::KeyEvent prevKeyState;
	window.setFramerateLimit(120);
	// Start game loop
	//bool upPress, downPress, leftPress, rightPress = false;
	bool upPress = false;
	bool downPress = false;
	bool leftPress = false;
	bool rightPress = false;
	bool spacePress = false;
	bool onGround = false;
	bool onePress = false;
	bool twoPress = false;
	bool threePress = false;
	bool fourPress = false;
	
	bool soundsOn = false;
	bool musicOn = false;
	bool reverbOn = false;
	bool threeDSounds = false;
	sf::Text soundsText;
	soundsText.setFont(font);
	soundsText.setString("Sounds: ");
	soundsText.setPosition(20,40);
	soundsText.setCharacterSize(40);
	sf::Text musicText;
	musicText.setFont(font);
	musicText.setString("Music: ");
	musicText.setPosition(20,80);
	musicText.setCharacterSize(40);
	sf::Text reverbText;
	reverbText.setFont(font);
	reverbText.setString("Reverb: ");
	reverbText.setPosition(20,120);
	reverbText.setCharacterSize(40);
	sf::Text threeText;
	threeText.setFont(font);
	threeText.setString("3D Sounds: ");
	threeText.setPosition(20,160);
	threeText.setCharacterSize(40);

	sf::Clock clock;
	while (window.isOpen())
	{

		sf::Event Event;


		while (window.pollEvent(Event))
		{

			if (Event.type == sf::Event::Closed)
				window.close();

			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Escape))
				window.close();
			spriteVelocity = sf::Vector2f(0,0);
			if(Event.type == sf::Event::KeyPressed){
				if(Event.key.code == sf::Keyboard::Up){
					upPress = true;
				}

				if(Event.key.code == sf::Keyboard::Down){
					downPress = true;
				}

				if(Event.key.code == sf::Keyboard::Left){
					leftPress = true;
				}

				if(Event.key.code == sf::Keyboard::Right){
					rightPress = true;
				}
				if(Event.key.code == sf::Keyboard::Space){
					spacePress = true;
				}
				if(Event.key.code == sf::Keyboard::Num1){
					onePress = true;
				}
				if(Event.key.code == sf::Keyboard::Num2){
					twoPress = true;
				}
				if(Event.key.code == sf::Keyboard::Num3){
					threePress = true;
				}
				if(Event.key.code == sf::Keyboard::Num4){
					fourPress = true;
				}
			}
			if (Event.type == sf::Event::KeyReleased){
				if(Event.key.code == sf::Keyboard::Up){
					upPress = false;
				}
				if(Event.key.code == sf::Keyboard::Down){
					downPress = false;
				}
				if(Event.key.code == sf::Keyboard::Left){
					leftPress = false;
				}
				if(Event.key.code == sf::Keyboard::Right){
					rightPress = false;
				}
				if(Event.key.code == sf::Keyboard::Space){
					spacePress = false;
				}
				if(Event.key.code == sf::Keyboard::Num1){
					onePress = false;
				}
				if(Event.key.code == sf::Keyboard::Num2){
					twoPress = false;
				}
				if(Event.key.code == sf::Keyboard::Num3){
					threePress = false;
				}
				if(Event.key.code == sf::Keyboard::Num4){
					fourPress = false;
				}
			}
			
		}
		if (upPress){

		}
		else if (downPress){

		}
		if (leftPress){
			spriteVelocity.x = -4;
		}
		else if (rightPress){
			spriteVelocity.x = 4;
		}
		if (spacePress){
			if (onGround){
				spriteVelocity.y = -10;
				onGround = false;
			}
		}

		if (onePress){
			if (soundsOn){
				soundsOn=false;
			}
			else{
				soundsOn=true;
			}
		}
		if(twoPress){
			if (musicOn){
				musicOn = false;
				BGM->setPaused(true);
			}
			else{
				musicOn = true;
				BGM->setPaused(false);
			}
		}
		if (threePress){
			if (threeDSounds){
				threeDSounds = false;
			}
			else{
				threeDSounds = true;
			}
		}
		if (fourPress){
			if (reverbOn){
				reverbOn = false;
			}
			else{
				reverbOn = true;
			}
		}


		sf::Time elapsed = clock.getElapsedTime();
		clock.restart();
		if (!onGround){
			float gravity = 9.81*elapsed.asSeconds();
			if (spriteVelocity.y < 10){
				spriteVelocity.y +=gravity;
			}
		}
		sprite.move(spriteVelocity);
		
		vector<sf::Sprite> collidables = map.getCollidables();
		sf::FloatRect rect = sprite.getGlobalBounds();
		for (int i =0; i < collidables.size(); i++){
			sf::FloatRect rectB = collidables.at(i).getGlobalBounds();
			sf::FloatRect rectC;
			if (sprite.getGlobalBounds().intersects(collidables.at(i).getGlobalBounds(), rectC)){
				cout << "";
				if (rectC.height < rectC.width){
					if (sprite.getPosition().y > collidables.at(i).getPosition().y){
					  sprite.move(0, rectC.height);
					}
					else {
						sprite.move(0, -rectC.height);
						if (soundsOn && onGround == false){
							FMODsys->playSound(FMOD_CHANNEL_FREE, land, false, 0);
						}
						onGround = true;
					}
				}
				else if (rectC.height > rectC.width){
					if (sprite.getPosition().x > collidables.at(i).getPosition().x){
						sprite.move(rectC.width, 0);
					}
					else{
						sprite.move(-rectC.width, 0);
					}
				}
			}
		}


		

		view.setCenter(sprite.getPosition().x+sprite.getGlobalBounds().width/2,300);
		window.clear();
		vector<sf::Sprite> blocks = map.getSprites();

		window.setView(view);
		window.draw(tMap);
		FMOD_VECTOR  listenerPos  = { sprite.getPosition().x, sprite.getPosition().y, 0.0f };
		FMOD_VECTOR listenerVel = {spriteVelocity.x, spriteVelocity.y, 0.0f };
		FMODsys->set3DListenerAttributes(0, &listenerPos, &listenerVel, 0,0);
		if (threeDSounds){
			for (int i =0; i < 1; i++){
				FMOD::Channel *channel=0;
				float volume=0.9f;
				result = channel->setVolume(volume);   
				//need this for sound fall off
				channel->set3DMinMaxDistance(50,100);
				FMOD_VECTOR  sourcePos  = { map.getSoundSprites().at(i).getPosition().x, map.getSoundSprites().at(i).getPosition().y , 0.0f};
				//source is fixed so velocity is zero
				channel->set3DAttributes(&sourcePos,0);
				FMODsys->playSound(FMOD_CHANNEL_FREE, fire, false, &channel);
			}
		}
		if (reverbOn){
			reverb->setActive(true);
			sf::CircleShape c(maxdist);
			c.setPosition(2200, 200);
			c.setOutlineColor(sf::Color(0,0,255,255));
			c.setOutlineThickness(4);
			window.draw(c);
		}
		else{
			reverb->setActive(false);
		}

		FMODsys->update();
		window.draw(sprite);

		soundsText.setString("Sounds: "+std::to_string(soundsOn));
		musicText.setString("Music: "+std::to_string(musicOn));
		reverbText.setString("Reverb: "+std::to_string(reverbOn));
		threeText.setString("3D Sounds: "+std::to_string(threeDSounds));
		window.setView(window.getDefaultView());
		window.draw(soundsText);
		window.draw(musicText);
		window.draw(reverbText);
		window.draw(threeText);

		sf::Transform rot;

		window.display();
	} //loop back for next frame
	return EXIT_SUCCESS;
}

