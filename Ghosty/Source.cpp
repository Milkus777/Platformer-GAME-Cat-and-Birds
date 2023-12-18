#include <SFML/Graphics.hpp>
#include <conio.h>
#include "GameMenu.h"


using namespace sf; //������������ ����

float offsetX = 0, offsetY = 0; //������������ ������

const int H = 25; // ������ ����� 
const int W = 91; // ������ ����� 

const int ts = 50; // ������ ������ 

String TileMap[H] = {    // ������ ����� 
"CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
"C                                                 ABA                                     B",
"C       oooo                                      ABBBA                                   B",
"C                            CCCCCCCCCAB         ABBBBA                                   B",
"C       CCCCCCC    CCCC               ABAAA      BBBBBA                                   B",
"C                     CC              AB         BBBBBBA                                  B",
"C                              CCC    AB     BBBBBBBBBBA                                  B",
"C                     oooooo         ABB         BBBBBBBA                                 B",
"C              CCCC        CC        BBBAAAA     BBBBBBBBA                                B",
"CCC                                  BBB         BBBBBBBBBA                               B",
"CCCC     CCCCCCCCCCCCCCCCCCCCCCCCCCCCBBB     BBBBBBBBBBBBBBA                 W            B",
"A  CC                                BB          BBBBBBBBBBBBA                            B",
"A   CC     oooo                      BB          BBBBBBBBBBBBBBA     W     W W  WW  W     B",
"A                                    BBA         BBBWBBWBBBBBBBA      W W W  W  W W W     B",
"A       CCCCCC                       BBBAAA      BBBWBBWBBBBBBBBA      W W   W  W  WW     B",
"BB                                   BBB         BBWBBBBWBBBBBBBA    CC     CCCCC         B",
"BBBAA                           CCC  BBB      BBBBBBWWWWBBBBBBBBBB                       AB",
"BBAA              B   oooooo         ABB             BBB                                AAB",
"BBBAA          AABBA               ABBBBAAA          BBB                             AAAAAB",
"BBAAAAAAAAAAAAAABBBBAAAAAAAAAAAAAAAAABBBBBBBBBB                AAABBBBBBBBBBAAAAAAAAAAAAAAA",
"BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB       BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",
"BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",
"BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",
"BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",
"BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",
}
;









class Player {
public:
	float dx, dy;           // �������� �� ��� X � Y       
	FloatRect rect;         // �������������, ������������ ������� � ������� ������
	bool onGround, rig;     // �����, ����������� �� ���������� ������ �� ����� � ����������� ��� ��������
	Sprite sprite;          // ������ ������
	float curFrame;         // ������� ���� ��������

	Player(Texture& image) {
		sprite.setTexture(image);
		rect = FloatRect(6 * ts, 15 * ts, 50*1.7, 49);  // ��������� ���������� ��������� ��������� � ���� 
		sprite.setTextureRect(IntRect(0, 0, 50, 50)); // �������� �� ������ ���� �������� ��������� � � ������ � ������ 
		sprite.scale(sf::Vector2f(1.7, 1.7)); // ���������� �������� ��������� �� 1.7

		dx = dy = 0;
		curFrame = 0;
		rig = true;
		//C#
		//Player(Texture image)
		//{
			//sprite.setTexture(image);
			//rect = new FloatRect(6 * ts, 15 * ts, 50 * 1.7f, 49);
			//sprite.setTextureRect(new IntRect(0, 0, 50, 50));
			//sprite.Scale = new Vector2f(1.7f, 1.7f);

			//dx = dy = 0;
			//curFrame = 0;
			//rig = true;
		//}
	}

	void update(float time) {
		rect.left += dx * time;    // ��������� ���������� x ������� �� ������ �������� �������� �� x � �������
		Collision(0);  // ��� x

		if (!onGround)            // ���� ������ �� �� �����, �� ������������� ��� ������������ �������� �� ���� ����������
			dy = dy + 0.0005 * time;

		rect.top += dy * time;     // ��������� ���������� y ������� �� ������ ������������ �������� � �������

		onGround = false;          // ��������� �����, ��� ������ �� �� ����� (������������ ��� �������� �������� �� ��� y)
		Collision(1); // ��� y

		curFrame += 0.005 * time;         // ���������� �������� ����� ��������
		if (curFrame > 8)                   // �������� � ������������� �������� ����� ��������, ����� �� �� �������� ���������� ��������� ������
			curFrame -= 7;
		if (dx > 0) {
			if (dy == 0)
				sprite.setTextureRect(IntRect(50 * int(curFrame), 0, 50, 50)); // ������ �������
			else
				sprite.setTextureRect(IntRect(66, 58, 50, 50)); // ������� ����� ������ ������� 
			rig = true;
		}
		if (dx < 0) {
			if (dy ==0 )
			sprite.setTextureRect(IntRect(50 * int(curFrame) + 50, 0, -50, 50)); // ������ ������ 
			else 
				sprite.setTextureRect(IntRect(50, 58, -50, 50)); // ������� ����� ������ ������ 
			rig = false;
		}

		sprite.setPosition(rect.left - offsetX, rect.top - offsetY);

		dx = 0;
	}

	void Collision(float dir) {
		for (int i = rect.top / ts; i < (rect.top + rect.height) / ts; i++)                // �������� �� y
			for (int j = rect.left / ts; j < (rect.left + rect.width) / ts; j++) {         //������� �������� �� �
				if (TileMap[i][j] == 'A') {       // ���� � �� �����������
					if (dx > 0 && dir == 0)
						rect.left = j * ts - rect.width; //���� ���� ������
					if (dx < 0 && dir == 0)
						rect.left = j * ts + ts; // �������� ���� �������� ���� �����
					if (dy > 0 && dir == 1) {
						rect.top = i * ts - rect.height;    // �������� �
						dy = 0;
						onGround = true; //����������� ������ �� ������
					}
					if (dy < 0 && dir == 1) {
						rect.top = i * ts + ts;
						dy = 0;
					}

				}
				if (TileMap[i][j] == 'B') {
					if (dx > 0 && dir == 0)
						rect.left = j * ts - rect.width;
					if (dx < 0 && dir == 0)
						rect.left = j * ts + ts;                        // ����������� �� �������� �������� ��� ����� �����
					if (dy > 0 && dir == 1) {
						rect.top = i * ts - rect.height;
						dy = 0;
						onGround = true;
					}
					if (dy < 0 && dir == 1) {
						rect.top = i * ts + ts;
						dy = 0;
					}

				}
				if (TileMap[i][j] == 'C') {
					if (dx > 0 && dir == 0)
						rect.left = j * ts - rect.width;
					if (dx < 0 && dir == 0)                       
						rect.left = j * ts + ts;                        // ���������� ����������� �������� ��� ������
					if (dy > 0 && dir == 1) {
						rect.top = i * ts - rect.height;
						dy = 0;
						onGround = true;
					}
					if (dy < 0 && dir == 1) {
						rect.top = i * ts + ts;
						dy = 0;
					}

				}
				if (TileMap[i][j] == 'o')                             // ���� � ��� ������������ ��������� � �
					TileMap[i][j] = ' ';                              // �� ������� �� ������� 
			} 
	}
};


// ������� ��������� ������
void InitText(Text& mtext, float xpos, float ypos, String str, int size_font = 60,   // C# void InitText(ref Text mtext, float xpos, float ypos, string str, int size_font = 60,     
	Color menu_text_color = Color::White, int bord = 0, Color border_color = Color::Black);    //Color menu_text_color = Color.White, int bord = 0,Color border_color = Color.Black)           

// ������� �������� � ����
void Gam�Start()
{
	RenderWindow Play(VideoMode(1000,500), L"����",Style::Default);
	Play.setFramerateLimit(60); //����������� ������� ������ FPS � 60


	float Frame = 0;

	Texture background_play;
	background_play.loadFromFile("Images/fon.png");  // �������� ����������� ���
	Sprite fon(background_play);

	Texture Picture;
	Picture.loadFromFile("Images/12345.png");  // �������� ����������� ���
	Player p(Picture);

	Texture t2;
	t2.loadFromFile("Images/plat.png"); //�������� ����������� ����� ������ ���� ������
	Sprite plat(t2);



	Clock clock;      // �������� ������� ������ Clock ��� ������������ �������

	while (Play.isOpen()) // ����, ������������� ���� ���� Play �������
	{

		


		float time = clock.getElapsedTime().asMicroseconds();      // ��������� ���������� ������� � �������������
		clock.restart();                          // ����� �������� �������
		time = time / 800;                       // ��������������� �������

		Event event;                           // �������� ������� ������ Event ��� ��������� �������
		while (Play.pollEvent(event))          // ���� ��������� �������
		{
			if (event.type == Event::Closed)    // ��������, �������� �� ������� ��������� ����
				Play.close();                   // �������� ���� Play
		}
		if (!_kbhit())          // ���� ��� ������� ������� 
			if (p.dy == 0) {        // � �������� �� ��������� ( �� ���������� � ������� � �� )
				if (p.rig)
					p.sprite.setTextureRect(IntRect(0, 0, 50, 50));    // ����� �������� ������ �� ����������� , �������          
				else if (!p.rig)
					p.sprite.setTextureRect(IntRect(50, 0, -50, 50)); // ������������� ���������� => ������ 

			}
		if (Keyboard::isKeyPressed(Keyboard::Left)) // ���� ������ ������� ����� 
			p.dx = -0.2;                            // ������� � ����� ������� ��������� 
		if (Keyboard::isKeyPressed(Keyboard::Right)) // ���� ������ ������� ������� 
			p.dx = 0.2;                              // ������� ��������� ������� 

		if (Keyboard::isKeyPressed(Keyboard::Up))
			if (p.onGround) {
				p.dy = -0.4;

				p.onGround = false;

				if (p.rig)
					p.sprite.setTextureRect(IntRect(66, 58, 50, 50)); // ������� ����� ������������ ������ 
				else if (!p.rig)
					p.sprite.setTextureRect(IntRect(50, 58, -50, 50)); // ������� ����� ������������ ������ ������
			}

		if (p.rect.left > 500 && p.rect.left < 4050)
			offsetX = p.rect.left - 500;                        // ����������� ��� �������� ������, ����� ����� �� ������� �� ������� ������� 
		if (p.rect.top > 250 && p.rect.top < 1135)              // ����������� ������ �� y
			offsetY = p.rect.top - 250;

		p.update(time);
		Play.clear(Color::White);
		fon.setPosition(-offsetX, -offsetY); // ��� ��������� ������ � ����������
		Play.draw(fon); // ��������� ����

		for (int i = 0; i < H; i++)          //���� ������ � ����� �������� ����� ����� �������������� �����
			for (int j = 0; j < W; j++) {
				if (TileMap[i][j] == 'A')                                      // �������� �������� �� ����������� �� ����� �, �, ������, ������
					plat.setTextureRect(IntRect(0, 0, ts, ts));                // ts = 50 �� ��� ���� ����� ���������� ������ 50�50 � - �������� �����
				if (TileMap[i][j] == 'B')                                      // ���� �
					plat.setTextureRect(IntRect(100, 0, ts, ts));              // �� ��������� �������� ����� 
				if (TileMap[i][j] == 'C')                                      // ���� � 
					plat.setTextureRect(IntRect(150, 0, ts, ts));              // �� ��������� �������� ������ 
				if (TileMap[i][j] == 'W')                                      // ���� W 
					plat.setTextureRect(IntRect(200, 0, ts, ts));              // �� ��������� �������� ������ 
				if (TileMap[i][j] == 'o')                                      // ���� � 
					plat.setTextureRect(IntRect(ts, 0, ts, ts));               // �� ��������� �������� ������ 
				if (TileMap[i][j] == ' ')                                      // ���� ������ �� ������ �� ���������
					continue;

				plat.setPosition(j * ts - offsetX, i * ts - offsetY);          //������ �� ������
				Play.draw(plat);
			}

		Play.draw(p.sprite);

		Play.display();

		Event event_play;
		while (Play.pollEvent(event_play))
		{
			if (event_play.type == Event::KeyPressed)
			{                                                                           // ������ ������� �������� ���� play  ��� ������� ������� esc
				if (event_play.key.code == Keyboard::Escape) { Play.close();}
			}
		} 
		//C#
		//Event event_play;
		//while (Play.pollEvent(out event_play))
		//{
		//	if (event_play.type == Event.KeyPressed)
		//	{
				
		//		if (event_play.key.code == Keyboard.Escape) { Play.close(); }
		//	}
	    //	}


		Play.clear();                                                                


	}

}

// ������ ���� ��������� ���� � ��� ����������
void Options()
{
	RenderWindow Options(VideoMode::getDesktopMode(), L"���������", Style::Default); // �������� ���� "Options" � ���������, ���������������� ���������� �������� �����
                                                                                    // � ���������� "���������". ���� ��������� � ������ "Default".
	Options.setFramerateLimit(60);                                                  // ����������� ������� ������ FPS � 60

	RectangleShape background_opt(Vector2f(VideoMode::getDesktopMode().width, VideoMode::getDesktopMode().height)); // �������� �������������� ��� ������� ���� � ���������, ���������������� ���������� �������� �����
	Texture texture_opt;                                                         // �������� ������� ������ Texture

	if (!texture_opt.loadFromFile("Images/options.png")) exit(2);      // �������� �������� �� ����� "Images/options.png". 
                                                                     // � ������ ������ �������� ��������, ��������� ����� ��������� � ����� 2.
	 
	background_opt.setTexture(&texture_opt);            // ��������� �������� ��� �������������� ������� ����
	while (Options.isOpen())
	{
		Event event_opt;                             //������� �������
		while (Options.pollEvent(event_opt))         // ��������� ������� � ���� Options
		{
			if (event_opt.type == Event::Closed) Options.close();           // ��������, ���� �� ������� �������� ����
                                                                          // ���� ��, �� ���� Options ����� �������
			if (event_opt.type == Event::KeyPressed)                       // ��������, ���� �� ������� �������
			{
				if (event_opt.key.code == Keyboard::Escape) Options.close();       // ���� ������ ������� Escape, �� ���� Options ����� �������
			}
		} 
		Options.clear();                    // ������� ���� Options
		Options.draw(background_opt); // ��������� �������������� ������� ���� �� ���� Options
		Options.display();             // ����������� ���� Options �� ������h
	}

}

// ������� � ��������� ����
void About_Game() {
	RenderWindow About(VideoMode::getDesktopMode(), L"� ����", Style::Default);
	About.setFramerateLimit(60);

	RectangleShape background_ab(Vector2f(VideoMode::getDesktopMode().width, VideoMode::getDesktopMode().height));
	Texture texture_ab;
	if (!texture_ab.loadFromFile("Images/about.png")) exit(3);
	background_ab.setTexture(&texture_ab);

	while (About.isOpen())
	{
		Event event_play;
		while (About.pollEvent(event_play))
		{
			if (event_play.type == Event::Closed) About.close();                // ��������, ���� �� ������� �������� ���� ���� ��, �� ���� About ����� �������
			if (event_play.type == Event::KeyPressed)                           // ��������, ���� �� ������� �������
			{
				if (event_play.key.code == Keyboard::Escape) About.close();             // ���� ������ ������� Escape, �� ���� About ����� �������
			}
			About.clear();
			About.draw(background_ab);
			About.display();
		}
	}
}


	int main(){
    
	

	// ������ ���� windows
	RenderWindow window;
	// ���������: ������ ���� ���������� �������� �������� ���������� ������
	// �������� ��� ����, ���������� ����������� ���� �� ���� ������ ������
	window.create(VideoMode(1920,1080), L"Cat n Birds", Style::Default);
	window.setFramerateLimit(60);
	//��������� ��������� �������
	window.setMouseCursorVisible(false);

	//  ������� ������ ������
	//float width = VideoMode::getDesktopMode().width;
	//float height = VideoMode::getDesktopMode().height;

	// ������������� ��� ��� ������������ ���� 
	// ������ �������������
	RectangleShape background(Vector2f(1920,1080));
	// ��������� � ������������� �������� � ������������ menu9.jpg
	Texture texture_window;
	if (!texture_window.loadFromFile("Images/mav.png")) return 4;
	background.setTexture(&texture_window);


	Font font;
	if (!font.loadFromFile("font/troika.otf")) return 5;               // ������������� ����� ���  ����
	Text Titul;
	Titul.setFont(font);
	
	InitText(Titul, 480, 50, L"Cat and Birds", 150, Color(72, 123, 255), 3);               // ����� � ��������� ����

	
	String name_menu[]{ L"�����",L"���������", L"� ����",L"�����" };                   // �������� ������� ����

	
	game::GameMenu mymenu(window, 950, 350, 4, name_menu, 100, 120);                 // ������ ������� ����
	
	mymenu.setColorTextMenu(Color(72, 123, 255), Color::Red, Color::Black);             // ��������� ����� ��������� ������� ����
	
	mymenu.AlignMenu(2);       // ������������ �� ������ ������� ���� 

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::KeyReleased)
			{
				// ������� ������ ������� ����
				// ������� �� ���������� ������� �����
				if (event.key.code == Keyboard::Up) { mymenu.MoveUp(); }
				// ������� �� ���������� ������� ����
				if (event.key.code == Keyboard::Down) { mymenu.MoveDown(); }
				// ������� �� ���������� ������� Enter
				if (event.key.code == Keyboard::Return)
				{
					// ��������� �� ��������� ����� ����
					switch (mymenu.getSelectedMenuNumber())
					{
					case 0:Gam�Start();   break;
					case 1:Options();     break;
					case 2:About_Game();  break;
					case 3:window.close(); break;

					}

				}
			}
		}

		window.clear();      // ������� ���� �� ����������� �����
		window.draw(background);     // ��������� ������� ����
		window.draw(Titul);         // ��������� ���������
		mymenu.draw();              // ��������� ����
		window.display();           // ����� ������������� ����������� �� �����
	}
	
	
	return 0;
}

// ������� ��������� ������
void InitText(Text& mtext, float xpos, float ypos, String str, int size_font, // ������� InitText, ������� ������������� ��� ��������� ������ � ������� ������ Text
	Color menu_text_color, int bord, Color border_color)
{
	mtext.setCharacterSize(size_font);          // ��������� ������� ������ ������
	mtext.setPosition(xpos, ypos);             // ��������� ������� ������ �� ������ �� ���� X � Y
	mtext.setString(str);                      // ��������� ����������� ������
	mtext.setFillColor(menu_text_color);       // ��������� ����� ������� ������
	mtext.setOutlineThickness(bord);           // ��������� ������� ������� ������
	mtext.setOutlineColor(border_color);      // ��������� ����� ������� ������
}

