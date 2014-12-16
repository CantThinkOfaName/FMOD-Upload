#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "SFML/Graphics.hpp"
#include "SFML/OpenGL.hpp"

using namespace std;

class Player {
private:
	sf::Texture m_texture;
	sf::Sprite m_sprite;
	sf::Vector2f m_velocity;

public:
	Player(){
		
	}
	~Player(){

	}
	void update(){
		//m_sprite.getGlobalBounds().intersects(
	}

};

#endif