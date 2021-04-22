#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <math.h>

using namespace sf;
using namespace std;

bool start = false;
const int ScreenX = 800;
const int ScreenY = 600;
sf::RenderWindow window(sf::VideoMode(ScreenX, ScreenY), "SFML works!");
const float RAD = 0.017453;
Vector2i pos;
Vector2f Old_Pos;
Vector2f posd;

const int width = 100;
const int height = 100;
int CollMap[width][height];
int FLOOR[width][height];

struct BLOCK_FORM
{
    Texture t;
    Sprite s;
};
vector<BLOCK_FORM> blocks;
void TEXTURES(){
    for(int i = 0;i<10;i++){
        blocks.push_back(BLOCK_FORM());
    }

    blocks[0].t.loadFromFile("res/block2.jpg");
    blocks[0].s.setTexture(blocks[0].t);

    blocks[1].t.loadFromFile("res/block1.jpg");
    blocks[1].s.setTexture(blocks[1].t);

    blocks[2].t.loadFromFile("res/block5.png");
    blocks[2].s.setTexture(blocks[2].t);

    blocks[3].t.loadFromFile("res/cursor.png");
    blocks[3].s.setTexture(blocks[3].t);
    blocks[3].s.setOrigin(blocks[3].s.getGlobalBounds().width / 2,
                          blocks[3].s.getGlobalBounds().height / 2);
    blocks[3].s.setScale(3,3);

        blocks[4].t.loadFromFile("res/grass_side.png");
    blocks[4].s.setTexture(blocks[4].t);

        blocks[5].t.loadFromFile("res/grass_top.png");
    blocks[5].s.setTexture(blocks[5].t);
}
class camera
{
public :
    /// PLAYER MANUAL
    const int rays = 60;
    float fov = 100;
    float start_fov = 0;
    float delta = fov / rays;
    float player_a = 0.0f;
    float speed = 0.05;float cos_a = 0.0f;
    RectangleShape player,player2;float sin_a = 0.0f;
    float rX,rY;Vector2f half_bound;float player_y = 0.1;
    bool isClickLeft = false;float DIST_CENTER_RAY = 0.0f;bool onGround = false;
    float dx = 0.0f,dy = 0.0f,dy3 = 0.0f;

    ///PROJECTION
    float Proj = (rays) * (2 / tan(fov / 2));
    float ProjComp = Proj * (ScreenX / rays);
    float wall_width = (window.getSize().x / rays);
    float WALL_POS = 0;float D = 2;
    RectangleShape wall,wallf,Sky_Fix_artefact;
    ConvexShape FloorYD;

    camera(){
        player.setSize(Vector2f(16,16));
        player.setFillColor(Color::Red);
        half_bound.x = player.getGlobalBounds().width / 2;
        half_bound.y = player.getGlobalBounds().height / 2;
        player2 = player;player2.setPosition(352,352);
        Sky_Fix_artefact.setSize(Vector2f(800,400));
        Sky_Fix_artefact.setFillColor(Color::Cyan);
        wallf.setScale(1,1);player.setPosition(352,352);
        FloorYD.setPointCount(4);
    }
    void Control(float times){

        if(Keyboard::isKeyPressed(Keyboard::D)){
        dx = cos((player_a + 90) * RAD) * speed;
        dy = sin((player_a + 90) * RAD) * speed;
        }

        if(Keyboard::isKeyPressed(Keyboard::A)){
        dx = cos((player_a - 90) * RAD) * speed;
        dy = sin((player_a - 90) * RAD) * speed;
        }

        if(Keyboard::isKeyPressed(Keyboard::W)){
        dx = cos((player_a) * RAD) * speed;
        dy = sin((player_a) * RAD) * speed;
        }

        if(Keyboard::isKeyPressed(Keyboard::S)){
        dx = -cos((player_a) * RAD) * speed;
        dy = -sin((player_a) * RAD) * speed;
        }

        if(Keyboard::isKeyPressed(Keyboard::Space)){if(onGround){dy3 = 0.01;}}/// + 0.03 because this 3d game
    }
    void Mouse_Control(float times){
        float dx = posd.x - ScreenX / 2;
        float dy = posd.y - ScreenY / 2;

        player_a += dx * 0.5;
        WALL_POS -= dy * 4;
    }
    void RAY(float times){
        start_fov = player_a - fov / 2;
        float rotation = start_fov;VertexArray line(LineStrip,2);

        float Pre_Wall_Height = 0.0f;int M_ax = 0;

        VertexArray line1(LinesStrip,2);

        for(int i = 0;i<rays;i++){

            line[0].position = Vector2f(player.getPosition().x + half_bound.x,
                                        player.getPosition().y + half_bound.y);

            cos_a = cos(rotation * RAD);
            sin_a = sin(rotation * RAD);

            for(int j = 152;j>0;j--){

                rX = player.getPosition().x + half_bound.x + j * cos_a;
                rY = player.getPosition().y + half_bound.x + j * sin_a;
                ///int TEMP = FLOOR[(int)rX / 32][(int)rY / 32];

                if(CollMap[(int)rX / 32][(int)rY / 32] == 1){
                    float ca = cos((player_a - rotation) * RAD);if(ca < 0){ca += 3.14;}if(ca > 0){ca -= 3.14;}
                    int offsetX = (int)rX % (int)blocks[5].s.getLocalBounds().width;
                    int offsetY = (int)rY % (int)blocks[5].s.getLocalBounds().height;

                    float DIST = (ProjComp / j);wallf.setFillColor(Color::Green);
                    wallf.setTexture(&blocks[5].t);
                    wallf.setTextureRect(IntRect(offsetX,offsetY,1,1));
                    wallf.setSize(Vector2f((window.getSize().x / rays),fabs(DIST * ca)));


                    wallf.setPosition(i * (window.getSize().x / rays),WALL_POS + ((wallf.getGlobalBounds().height) * player_y));
                    window.draw(wallf);
                }
            }
            /*


            for(int j = 1;j<125;j++){

                rX = player.getPosition().x + j * cos_a;
                rY = player.getPosition().y + j * sin_a;
                int TEMP = FLOOR[(int)rX / 32][(int)rY / 32];

                if(CollMap[(int)rX / 32][(int)rY / 32] == 1){

                int offsetX = (int)rX % (int)blocks[TEMP].s.getLocalBounds().width;
                int offsetY = (int)rY % (int)blocks[TEMP].s.getLocalBounds().height;

                    float DIST = (ProjComp / j);
                    wallf.setTexture(&blocks[TEMP].t);
                    wallf.setTextureRect(IntRect(offsetX,offsetY,1,1));
                    wallf.setSize(Vector2f((window.getSize().x / rays),DIST));
                    wallf.setPosition(i * (window.getSize().x / rays),WALL_POS);
                    window.draw(wallf);
                    if(j == 124){
                        Sky_Fix_artefact.setPosition(i * (window.getSize().x / rays),WALL_POS);
                        Sky_Fix_artefact.setSize(Vector2f((window.getSize().x / rays),DIST));
                        window.draw(Sky_Fix_artefact);
                    }
                }
            }
            */
            for(int j = 1;j<1200;j++){

                rX = player.getPosition().x + half_bound.x + j * cos_a;
                rY = player.getPosition().y + half_bound.x + j * sin_a;

                if(CollMap[(int)rX / 32][(int)rY / 32] == 1 && (int)rX / 32 > 0 && (int)rX / 32 < width && (int)rY / 32 > 0 && (int)rY / 32 < height){float DIST = (ProjComp / j);
                    float ca = cos((player_a - rotation) * RAD);if(ca < 0){ca += 3.14;}if(ca > 0){ca -= 3.14;}
                    int offsetX = (int)(rX) % (int)blocks[4].s.getLocalBounds().width;
                    int offsetY = (int)(rY) % (int)blocks[4].s.getLocalBounds().height;int offset = 0;
                    if(offsetX == 0 || offsetX == blocks[4].s.getLocalBounds().width - 1){offset = offsetY;}
                    if(offsetY == 0 || offsetY == blocks[4].s.getLocalBounds().height - 1){offset = offsetX;}
                    DIST *= ca;
                    wall.setTexture(&blocks[4].t);
                    wall.setTextureRect(IntRect(offset,0,1,blocks[4].s.getGlobalBounds().height));
                    wall.setSize(Vector2f((window.getSize().x / rays),DIST));
                    wall.setPosition(i * (window.getSize().x / rays),WALL_POS + (wall.getGlobalBounds().height * player_y));
                    window.draw(wall);
                    break;
                }
                line[1].position = Vector2f(rX,rY);
            }


            rotation += delta;
        }
    }
    void CURSOR(){
        int Width = 252;
        if(Mouse::isButtonPressed(Mouse::Left) && !isClickLeft){
            VertexArray line(LinesStrip,2);
            int Pre_X,Pre_y;
            for(int j = 1;j<Width;j++){
            int X = player.getPosition().x + half_bound.x + j * cos(player_a * RAD);
            int Y = player.getPosition().y + half_bound.y + j * sin(player_a * RAD);
            if(CollMap[(int)X / 32][(int)Y / 32] == 1){
                CollMap[(int)Pre_X / 32][(int)Pre_y / 32] = 1;
                break;
            }
            Pre_X = X;
            Pre_y = Y;

            }

            isClickLeft = true;
        }
        if(!Mouse::isButtonPressed(Mouse::Left)){isClickLeft = false;}

        blocks[3].s.setPosition(ScreenX / 2,ScreenY / 2);
        window.draw(blocks[3].s);
    }
    void Collide2d(int dir,float times){
        for(int i = player.getGlobalBounds().left / 32;i<(player.getGlobalBounds().left + player.getGlobalBounds().width) / 32;i++){
            for(int j = player.getGlobalBounds().top / 32;j<(player.getGlobalBounds().top + player.getGlobalBounds().height) / 32;j++){
                if(CollMap[i][j] == 1 && player_y < 1){
                    if(dx > 0 && dir == 1){player.setPosition(i * 32 - player.getGlobalBounds().width,player.getPosition().y);}
                    if(dx < 0 && dir == 1){player.setPosition(i * 32 + player.getGlobalBounds().width + (32 - player.getGlobalBounds().width),player.getPosition().y);}
                    if(dy > 0 && dir == 2){player.setPosition(player.getPosition().x,j * 32 - player.getGlobalBounds().height);}
                    if(dy < 0 && dir == 2){player.setPosition(player.getPosition().x,j * 32 + player.getGlobalBounds().height + (32 - player.getGlobalBounds().height));}
                }
            }
        }
    }
    void Collide3d(float times){
        for(int i = player.getGlobalBounds().left / 32;i<(player.getGlobalBounds().left + player.getGlobalBounds().width) / 32;i++){
            for(int j = player.getGlobalBounds().top / 32;j<(player.getGlobalBounds().top + player.getGlobalBounds().height) / 32;j++){
        if(player_y < 0 || CollMap[i][j] == 1 && player_y <= 1){
            player_y -= dy3 * times;dy3 = 0;
            onGround = true;
        }
            }
        }
    }
    void update(float times){
        /// ^ + not -

        dy3 -= 0.00002 * times;
        onGround = false;
        player_y += dy3* times;

        Collide3d(times);
        player.move(dx * times,0);
        Collide2d(1,times);
        player.move(0,dy * times);
        Collide2d(2,times);

        dx = 0;
        dy = 0;

        RAY(times);CURSOR();
        Control(times);window.draw(player);
    }
};
void gen(){
    for(int i = 0;i<width;i++){
        for(int j = 0;j<height;j++){
        }
    }
    CollMap[13][13] = 1;
}
int main()
{
    window.setMouseCursorVisible(false);

    TEXTURES();
    gen();
    camera camera;
    Clock clock;

    Mouse::setPosition(sf::Vector2i(400, 300), window);

    while (window.isOpen())
    {

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        float times = clock.getElapsedTime().asMicroseconds();
        clock.restart();times = times / 800;

        pos = Mouse::getPosition(window);
        posd = window.mapPixelToCoords(pos);
        if(posd.x != 400 || posd.y != 300){
        camera.Mouse_Control(times);
        Mouse::setPosition(sf::Vector2i(400, 300), window);
        }
        RectangleShape w;w.setSize(Vector2f(32,32));
        window.clear(Color::Cyan);
        camera.update(times);
        window.display();
    }

    return 0;
}
