#pragma once
#include "jsonDependencies/json.hpp"
#include <SFML/Graphics.hpp>
#include <chrono>
#include <fstream>
#include <cmath>
#include <random>
#include <map>
using namespace std::chrono;

class physics
{
public:
	//functions:
	physics(sf::RenderWindow& win, sf::Sprite& temp, sf::Texture img);
	int RandRange(int start, int end);
	void SetBaseVariables();
	bool MouseCollision();
	void UpdatePage();
	void wiggle();
	void pickup();
	void impact(float rotate = 0);
	void Fall();
	//variables:
	//library
	using json = nlohmann::ordered_json;
	json data;
	json settings;
	sf::RenderWindow& window;
	sf::Texture CurrentImg;
	sf::Vector2f ObjVirtualLocation, offset;
	std::optional<sf::Vector2f> UniversalOrigin, ObjPostion, ObjScale;
	std::optional<sf::Angle> Objrotation;
	std::optional<sf::IntRect> TextureRect;
	//native
	float acceleration = 8.f, MPoint = 0.f, BotSpeed = 5.f, idk = 90.f;
	int currframe = 0, walkframe = 0, BaseDirection = 1, mirrored = 1, priority = 0;
	bool falling = false, Mcompleted = true, OffsetSprite = false, splat = false;
	std::pair<float, float> PageSize;
	std::pair<float, float> objsize;
	std::pair<float, float> speed;
	std::pair<bool, int> Action = { false, 0 };
	steady_clock::time_point prev = steady_clock::now();
	milliseconds interval = milliseconds(17);
	//sprite:
	sf::Sprite& MainObj;
};

