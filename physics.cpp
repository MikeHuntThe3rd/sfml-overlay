#include "physics.h"
physics::physics(sf::RenderWindow& win, sf::Sprite& temp, sf::Texture img)
	:window(win),
    MainObj(temp)
{
}
void physics::SetBaseVariables() {
    sf::Vector2u win = window.getSize();
    #pragma region json
    if (!std::filesystem::exists("data.json")) {
        json myData;
        myData["IdleImgPath"] = (std::string)"imgppath";
        myData["GrabImgPath"] = (std::string)"imgppath";
        myData["splashImgPath"] = (std::string)"imgppath";
        myData["FallImgPath"] = (std::string)"imgppath";
        myData["WalkImgPath"] = (std::string)"imgppath";
        myData["WalkDirection"] = 1;
        myData["ContainedFrames"] = 1;
        std::ofstream outFile("data.json");
        outFile << myData.dump(4);
        outFile.close();
		window.close();
    }
    if (!std::filesystem::exists("settings.json")) {
        json myData;
        myData["SpriteSize"] = win.y / 5.f;
        myData["Acceleration"] = win.y / 125.f;
        myData["HorizontalDeceleration"] = win.x / 240.f;
        myData["Impactthreshold"] = win.x / 96.f;
        myData["Fallthreshold"] = win.y / 12.5f;
        myData["PostImpactAcceleration"] = win.y / 1000.f;
        myData["WiggleMultiplyer"] = 0.3f;
        myData["ActionChance"] = 100;
        myData["BotMovementSpeed"] = win.x / 384.f;
        myData["WalkingUpdateSpeed"] = 8;

        std::ofstream outFile("settings.json");
        outFile << myData.dump(4);
        outFile.close();
        window.close();
    }
    std::ifstream file("data.json");
    file >> data;
    std::ifstream set("settings.json");
    set >> settings;
    #pragma endregion
    #pragma region variables
    if(settings["SpriteSize"] <= win.y * 0.3f){
        objsize = { settings["SpriteSize"], settings["SpriteSize"] };
    }
    else {
		window.close();
    }
    if ((float)settings["HorizontalDeceleration"] < 0 || (float)settings["HorizontalDeceleration"] > settings["SpriteSize"]) settings["HorizontalDeceleration"] = win.x / 240.f;
    if ((float)settings["Impactthreshold"] < 0) window.close();
    if ((float)settings["Fallthreshold"] < 0) window.close();
    if ((float)settings["PostImpactAcceleration"] < 0) window.close();
    if ((float)settings["ActionChance"] < 0) window.close();
    acceleration = ((float)settings["Acceleration"] > 0 && (float)settings["Acceleration"] < win.y) ? (float)settings["Acceleration"] : win.y / 125.f;
    //std::map<std::string, int> test = { (std::string)"left" = -1, (std::string)"right" = 1 };
    BaseDirection = ((float)data["WalkDirection"] == 1 || (float)data["WalkDirection"] == -1) ? (float)data["WalkDirection"] : 1;
    //BaseDirection = test[(std::string)data["WalkDirection"]];
    BotSpeed = ((float)settings["BotMovementSpeed"] >= 0.f) ? (float)settings["BotMovementSpeed"] : 0.f;
#pragma endregion

    sf::FloatRect bounds = MainObj.getLocalBounds();
	sf::Vector2u screenSize = window.getSize();
    ObjScale = sf::Vector2f(objsize.first / bounds.size.x, objsize.second / bounds.size.y);
    ObjPostion = sf::Vector2f(screenSize.x / 2.f, screenSize.y / 2.f);
}
void physics::Fall() {
    float x = MainObj.getPosition().x;
    float y = MainObj.getPosition().y;
    bool Lower = (ObjVirtualLocation.y + objsize.second >= PageSize.second || ObjVirtualLocation.y + objsize.second + speed.second >= PageSize.second);
    bool Upper = (y + speed.second < 0);
    bool Left = (x + speed.first < 0);
    bool Right = (x + objsize.first > PageSize.first);
    #pragma region IfForest
    //top-bot
    if (Lower) {
        falling = false;
        y = PageSize.second - objsize.second;
        acceleration = settings["Acceleration"];
        sf::Texture img;
        if (OffsetSprite) {
            x = ObjPostion.value().x + offset.x;
            y = ObjPostion.value().y + offset.y;
        }
        if (speed.second > settings["Impactthreshold"] && !Action.first || splat) {
			priority = 1;
            splat = true;
            img.loadFromFile(data["splashImgPath"]);
            CurrentImg = img;
            sf::Vector2u bounds = img.getSize();
            ObjScale = {objsize.first / bounds.x, objsize.second / bounds.y};
        }
        else if(!Action.first)
        {
            img.loadFromFile(data["IdleImgPath"]);
            CurrentImg = img;
            sf::Vector2u bounds = img.getSize();
            ObjScale = { objsize.first / bounds.x, objsize.second / bounds.y };
            UniversalOrigin = {0.f, 0.f};
        }
        speed = { 0.f, 0.f };
        Objrotation = sf::degrees(0.f);
        OffsetSprite = false;
    }
    else if (Upper) {
        y = 0.f;
        if (speed.second < -1 * settings["Impactthreshold"]) {
            impact(180.f);
            offset = { -objsize.first, objsize.second };
            ObjVirtualLocation = { x, 0.f };
            x += objsize.first;
            y += objsize.second;
        }
        speed = { 0.f, 0.f };
    }
    else {
        falling = true;
        y += speed.second;
        speed.second += acceleration;
        if (OffsetSprite) {
            ObjVirtualLocation = {ObjVirtualLocation.x, ObjVirtualLocation.y + speed.second};
        }
        if (speed.second > settings["Fallthreshold"]) {
			sf::Texture fall(data["FallImgPath"]);
            CurrentImg = fall;
            ObjScale = {objsize.first / CurrentImg.getSize().x, objsize.second / CurrentImg.getSize().y};
            Objrotation = sf::degrees(0.f);
			x = ObjVirtualLocation.x;
            y = ObjVirtualLocation.y + speed.second;
            speed.second += acceleration;
			OffsetSprite = false;
        }
    }
    //side
    if (Right) {
        x = PageSize.first - objsize.first;
        if (speed.first > settings["Impactthreshold"]) {
            impact(-90.f);
            offset = { 0.f, -objsize.second };
            ObjVirtualLocation = { PageSize.first - objsize.first, y };
            y += objsize.second;
        }
        speed = { 0.f, 0.f };
    }
    else if (Left) {
        x = 0;
        if (speed.first < -1 * settings["Impactthreshold"]) {
            impact(90.f);
            x += objsize.first;
            offset = {-objsize.first, 0.f};
            ObjVirtualLocation = {0.f, y};
        }
        speed = { 0.f, 0.f };
    }
    else {
        float AbsSpeed = (speed.first != 0) ? settings["HorizontalDeceleration"] * (speed.first / std::abs(speed.first)) : 0.f;
        speed.first = (std::abs(speed.first - AbsSpeed) > settings["HorizontalDeceleration"]) ? speed.first - AbsSpeed : 0.f;
        x += speed.first;
    }
    #pragma endregion
    ObjPostion = {x, y};
    UpdatePage();
}
void physics::UpdatePage() {
    //values
    UniversalOrigin = (UniversalOrigin.has_value()) ? UniversalOrigin : sf::Vector2f(0, 0);
    ObjScale = (ObjScale.has_value()) ? ObjScale : sf::Vector2f(1, 1);
    ObjPostion = (ObjPostion.has_value()) ? ObjPostion : sf::Vector2f(1000, 500);
    Objrotation = (Objrotation.has_value()) ? Objrotation: sf::degrees(0.f);
    if (!OffsetSprite) {
        ObjVirtualLocation = MainObj.getPosition();
    }
    sf::RectangleShape rect;
    rect.setSize({ objsize.first, objsize.second });
    rect.setPosition(ObjPostion.value());
    rect.setOutlineThickness(2);
    rect.setOutlineColor(sf::Color::Red);
    
    
    //setting
    MainObj.setTexture(CurrentImg, true);
    MainObj.setScale(ObjScale.value());
    MainObj.setOrigin(UniversalOrigin.value());
    MainObj.setPosition(ObjPostion.value());
    MainObj.setRotation(Objrotation.value());
    if (TextureRect.has_value()) {
        MainObj.setTextureRect(TextureRect.value());
    }
    
    //load
    window.clear(sf::Color::Black);
    window.draw(MainObj);
    //window.draw(rect);
    window.display();
}
bool physics::MouseCollision() {
    float x = sf::Mouse::getPosition().x;
    float y = sf::Mouse::getPosition().y;
    sf::Vector2f pos = { x, y };
    bool xAxis = (ObjVirtualLocation.x <= pos.x && pos.x <= ObjVirtualLocation.x + objsize.first);
    bool yAxis = (ObjVirtualLocation.y <= pos.y && pos.y <= ObjVirtualLocation.y + objsize.second);
    if (xAxis && yAxis) {
        return true;
    }
    return false;
}
int physics::RandRange(int start, int end) {
    static std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<> distrib(start, end);
    return distrib(gen);
}
void physics::pickup() {
    using namespace std::chrono;
    prev = steady_clock::now();
    std::pair<float, float> lastLocation = {MainObj.getPosition().x, MainObj.getPosition().y};
    while (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        if (prev + interval <= steady_clock::now()) {
            Action = {false, 0};
			splat = false;
			priority = 0;
            prev = steady_clock::now();
            acceleration = settings["Acceleration"];
            OffsetSprite = false;
            sf::Texture newimg(data["GrabImgPath"]);
            sf::Vector2u bounds = newimg.getSize();
            sf::Vector2i currPos = sf::Mouse::getPosition();
            float width = objsize.first / 2.f;
            float height = objsize.second / 2.f;
            CurrentImg = newimg;
            UniversalOrigin = { 0.f, 0.f };
            ObjScale = { objsize.first / bounds.x, objsize.second / bounds.y };
            ObjPostion = { currPos.x - width, currPos.y - height };
            TextureRect.reset();
            speed = {currPos.x - lastLocation.first, currPos.y - lastLocation.second};
            lastLocation = {currPos.x, currPos.y};
            wiggle();
        }
        UpdatePage();
    }
}
void physics::wiggle() {
	float wiggle = settings["WiggleMultiplyer"];
    Objrotation = sf::degrees(speed.first * wiggle);
}
void physics::impact(float rotate) {
    acceleration = settings["PostImpactAcceleration"];
    OffsetSprite = true;
    sf::Texture splat(data["splashImgPath"]);
    CurrentImg = splat;
    sf::Vector2u bounds = CurrentImg.getSize();
    ObjScale = sf::Vector2f(objsize.first / bounds.x, objsize.second / bounds.y);
    UniversalOrigin = { 0.f, 0.f };
    Objrotation = sf::degrees(rotate);
}