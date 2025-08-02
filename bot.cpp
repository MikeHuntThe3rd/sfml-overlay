#include "bot.h"
void bot::BotBehaviour() {
	int randnum = RandRange(0, settings["ActionChance"]);
    bool occur = (randnum == settings["ActionChance"]) ? true: false;
    if (occur) {
        while (!Action.first) {
            int minindex = priority;
			Action = { true, RandRange(minindex, 1) };
            if (Action.second >= priority) {
                break;
            }
        }
    }
    if (Action.first) {
        switch (Action.second) {
		case 0: 
			splat = false;
            bot::movement(); 
            break;
		case 1:
			Action = { false, 0 };
			splat = false;
            priority = 0;
        default:
            return;
        }
    }
}
void bot::WalkAnimation(int direction) {
    bool switchdir = false;
    if (BaseDirection != direction) {
        mirrored = -1;
        switchdir = true;
    }
    else
    {
        mirrored = 1;
    }
    sf::Texture newimg(data["WalkImgPath"]);
    CurrentImg = newimg;
    int width = newimg.getSize().x / data["ContainedFrames"];
    int height = newimg.getSize().y;
    sf::Vector2u bounds = CurrentImg.getSize();
    if (switchdir) {
        UniversalOrigin = { (float)width, 0.f};
    }
    else {
        UniversalOrigin = { 0.f, 0.f };
    }
    ObjScale = sf::Vector2f((objsize.first / (bounds.x / data["ContainedFrames"])) * mirrored, objsize.second / bounds.y);
    TextureRect = { {currframe * width, 0}, {width, height} };
    walkframe++;
    if (walkframe % settings["WalkingUpdateSpeed"] == 0 && walkframe != 0) {
        walkframe = 0;
        if (!switchdir) {
			currframe = (currframe < data["ContainedFrames"] - 1) ? currframe + 1 : 0;
        }
        else {
            currframe = (currframe > 0) ? currframe - 1 : data["ContainedFrames"] - 1;
        }
    }
}
void bot::movement() {
    if (BotSpeed == 0.f) return;
    if (!falling && !Mcompleted) {
        float Distance = MPoint - MainObj.getPosition().x;
        float AbsSpeed = BotSpeed * (Distance / std::abs(Distance));
        float Deltax;
        WalkAnimation(Distance / std::abs(Distance));
        if ((std::abs(Distance) >= BotSpeed)) {
            Deltax = MainObj.getPosition().x + AbsSpeed;
        }
        else {
            Deltax = MainObj.getPosition().x + Distance;
            Mcompleted = true;
			Action = { false, 0 };
            TextureRect.reset();
            sf::Texture img;
            img.loadFromFile(data["IdleImgPath"]);
            CurrentImg = img;
            sf::Vector2u bounds = img.getSize();
            ObjScale = { objsize.first / bounds.x, objsize.second / bounds.y };
			UniversalOrigin = { 0.f, 0.f };
            }
        ObjPostion = sf::Vector2f(Deltax, MainObj.getPosition().y);
    }
    else {
        MPoint = RandRange(0, PageSize.first - objsize.first);
        float Distance = MainObj.getPosition().x - MPoint;
        while (MainObj.getPosition().x + std::abs(Distance) <= MainObj.getPosition().x + objsize.first) {
            MPoint = RandRange(0, PageSize.first - objsize.first);
            Distance = MainObj.getPosition().x - MPoint;
        }
        Mcompleted = false;
    }
}
