#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "SFML/Graphics.hpp"
#include "SFML/OpenGL.hpp"

using namespace std;

class Map{
private:
	vector<sf::Sprite> sprites;
	vector<sf::Sprite> collidables;
	vector<sf::Sprite> soundSprites;
	sf::Texture spriteTexture;
	sf::Texture spriteTextureTwo;
public:

	void loadMap(){
		ifstream stream;
		stream.open("Level2.txt");
		string line;
		int y = 0;
		spriteTexture.loadFromFile("Block.png");
		spriteTextureTwo.loadFromFile("Block2.png");
		while (getline(stream, line)){
			for (int x = 0; x < line.size(); x++){
				if (line.at(x) == '0'){
					int in = line.at(x);
					sf::Sprite s;
					s.setPosition(x*64,y*64);
					s.setTexture(spriteTexture);
					sprites.push_back(s);
					collidables.push_back(s);
				}
				else if(line.at(x) == '2'){
					sf::Sprite s;
					s.setPosition(x*64,y*64);
					s.setTexture(spriteTextureTwo);
					soundSprites.push_back(s);
				}
				
			}	
			y++;
		}
	};
	vector<sf::Sprite> getSprites(){
		return sprites;
	};
	vector<sf::Sprite> getCollidables(){
		return collidables;
	};
	vector<sf::Sprite> getSoundSprites(){
		return soundSprites;
	};
};

class TileMap : public sf::Drawable, public sf::Transformable {

private:
	sf::VertexArray m_verts;
	sf::Texture m_tilesheet;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const{
		states.transform *= getTransform();
		states.texture = &m_tilesheet;
		target.draw(m_verts, states);
	}

public:
	bool load(ifstream& stream, std::string tilesheet, sf::Vector2i blockSize){
		if(!m_tilesheet.loadFromFile(tilesheet)){
			return -1;
		};
		m_verts.setPrimitiveType(sf::Quads);

		//ifstream stream;
		stream.open("Level2.txt");
		string line;
		int y = 0;
		while (getline(stream, line)){
			m_verts.resize(m_verts.getVertexCount()+(line.size()*4));
			for (int x = 0; x < line.size(); x++){
				if (line.at(x)){
	
				}

				int tileNumber = line.at(x) - 48;

				int textX = tileNumber % (m_tilesheet.getSize().x / blockSize.x);
				int textY = tileNumber / (m_tilesheet.getSize().x / blockSize.x);

				sf::Vertex* quad = &m_verts[(x+y*line.size())*4];
				quad[0].position = sf::Vector2f(x*blockSize.x, y*blockSize.y);
				quad[1].position = sf::Vector2f((x+1)*blockSize.x, y*blockSize.y);
				quad[2].position = sf::Vector2f((x+1)*blockSize.x, (y+1)*blockSize.y);
				quad[3].position = sf::Vector2f(x*blockSize.x, (y+1)*blockSize.y);

				quad[0].texCoords = sf::Vector2f(textX*blockSize.x, textY*blockSize.y);
				quad[1].texCoords = sf::Vector2f((textX+1)*blockSize.x, textY*blockSize.y);
				quad[2].texCoords = sf::Vector2f((textX+1)*blockSize.x, (textY+1)*blockSize.y);
				quad[3].texCoords = sf::Vector2f(textX*blockSize.x, (textY+1)*blockSize.y);

			}
			y++;
		}
	}

};

#endif