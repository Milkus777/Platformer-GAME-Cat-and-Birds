#include <SFML/Graphics.hpp>
#include <conio.h>
#include "GameMenu.h"


using namespace sf; //пространство имен

float offsetX = 0, offsetY = 0; //коэффициенты камеры

const int H = 25; // ВЫСОТА КАРТЫ 
const int W = 91; // ШИРИНА КАРТЫ 

const int ts = 50; // размер блоков 

String TileMap[H] = {    // задаем карту 
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
	float dx, dy;           // Скорость по оси X и Y       
	FloatRect rect;         // Прямоугольник, определяющий позицию и размеры игрока
	bool onGround, rig;     // Флаги, указывающие на нахождение игрока на земле и направление его движения
	Sprite sprite;          // Спрайт игрока
	float curFrame;         // Текущий кадр анимации

	Player(Texture& image) {
		sprite.setTexture(image);
		rect = FloatRect(6 * ts, 15 * ts, 50*1.7, 49);  // обозначим координаты появления персонажа в мире 
		sprite.setTextureRect(IntRect(0, 0, 50, 50)); // указание на первый кадр анимации персонажа х у ширина и высоты 
		sprite.scale(sf::Vector2f(1.7, 1.7)); // увеличение модельки персонажа на 1.7

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
		rect.left += dx * time;    // Изменение координаты x объекта на основе скорости движения по x и времени
		Collision(0);  // для x

		if (!onGround)            // Если объект не на земле, то увеличивается его вертикальная скорость за счет гравитации
			dy = dy + 0.0005 * time;

		rect.top += dy * time;     // Изменение координаты y объекта на основе вертикальной скорости и времени

		onGround = false;          // Установка флага, что объект не на земле (используется для проверки коллизий по оси y)
		Collision(1); // для y

		curFrame += 0.005 * time;         // Обновление текущего кадра анимации
		if (curFrame > 8)                   // Проверка и корректировка текущего кадра анимации, чтобы он не превышал количество доступных кадров
			curFrame -= 7;
		if (dx > 0) {
			if (dy == 0)
				sprite.setTextureRect(IntRect(50 * int(curFrame), 0, 50, 50)); // ходьба направо
			else
				sprite.setTextureRect(IntRect(66, 58, 50, 50)); // вставка кадра прыжка направо 
			rig = true;
		}
		if (dx < 0) {
			if (dy ==0 )
			sprite.setTextureRect(IntRect(50 * int(curFrame) + 50, 0, -50, 50)); // ходьба налево 
			else 
				sprite.setTextureRect(IntRect(50, 58, -50, 50)); // вставка кадра прыжка налево 
			rig = false;
		}

		sprite.setPosition(rect.left - offsetX, rect.top - offsetY);

		dx = 0;
	}

	void Collision(float dir) {
		for (int i = rect.top / ts; i < (rect.top + rect.height) / ts; i++)                // коллизия по y
			for (int j = rect.left / ts; j < (rect.left + rect.width) / ts; j++) {         //функция коллизии по х
				if (TileMap[i][j] == 'A') {       // если А то отталкивает
					if (dx > 0 && dir == 0)
						rect.left = j * ts - rect.width; //если идет вправо
					if (dx < 0 && dir == 0)
						rect.left = j * ts + ts; // коллизия если персонаж идет влево
					if (dy > 0 && dir == 1) {
						rect.top = i * ts - rect.height;    // коллизия у
						dy = 0;
						onGround = true; //возможность ходить по блокам
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
						rect.left = j * ts + ts;                        // прописываем по аналогии коллизию для блока камня
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
						rect.left = j * ts + ts;                        // аналогично прописываем коллизию для облака
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
				if (TileMap[i][j] == 'o')                             // если у нас столкновение персонажа с о
					TileMap[i][j] = ' ';                              // то заменим на пустоту 
			} 
	}
};


// функция настройки текста
void InitText(Text& mtext, float xpos, float ypos, String str, int size_font = 60,   // C# void InitText(ref Text mtext, float xpos, float ypos, string str, int size_font = 60,     
	Color menu_text_color = Color::White, int bord = 0, Color border_color = Color::Black);    //Color menu_text_color = Color.White, int bord = 0,Color border_color = Color.Black)           

// Функция перехода к игре
void GamеStart()
{
	RenderWindow Play(VideoMode(1000,500), L"Гора",Style::Default);
	Play.setFramerateLimit(60); //ограничение частоты кадров FPS в 60


	float Frame = 0;

	Texture background_play;
	background_play.loadFromFile("Images/fon.png");  // загрузка изображения фон
	Sprite fon(background_play);

	Texture Picture;
	Picture.loadFromFile("Images/12345.png");  // загрузка изображения кот
	Player p(Picture);

	Texture t2;
	t2.loadFromFile("Images/plat.png"); //загрузка изображение земля камень вода птички
	Sprite plat(t2);



	Clock clock;      // Создание объекта класса Clock для отслеживания времени

	while (Play.isOpen()) // Цикл, выполняющийся пока окно Play открыто
	{

		


		float time = clock.getElapsedTime().asMicroseconds();      // Получение прошедшего времени в микросекундах
		clock.restart();                          // Сброс счетчика времени
		time = time / 800;                       // Масштабирование времени

		Event event;                           // Создание объекта класса Event для обработки событий
		while (Play.pollEvent(event))          // Цикл обработки событий
		{
			if (event.type == Event::Closed)    // Проверка, является ли событие закрытием окна
				Play.close();                   // Закрытие окна Play
		}
		if (!_kbhit())          // если нет нажатия клавиши 
			if (p.dy == 0) {        // и персонаж не двигается ( не находиться в падении и тп )
				if (p.rig)
					p.sprite.setTextureRect(IntRect(0, 0, 50, 50));    // тогда выбираем спрайт по координатам , направо          
				else if (!p.rig)
					p.sprite.setTextureRect(IntRect(50, 0, -50, 50)); // отзеркаленные координаты => налево 

			}
		if (Keyboard::isKeyPressed(Keyboard::Left)) // если нажата клавиша влево 
			p.dx = -0.2;                            // смещаем в левую сторону персонажа 
		if (Keyboard::isKeyPressed(Keyboard::Right)) // если нажата клавиша направо 
			p.dx = 0.2;                              // смещаем персонажа направо 

		if (Keyboard::isKeyPressed(Keyboard::Up))
			if (p.onGround) {
				p.dy = -0.4;

				p.onGround = false;

				if (p.rig)
					p.sprite.setTextureRect(IntRect(66, 58, 50, 50)); // вставка кадра статического прыжка 
				else if (!p.rig)
					p.sprite.setTextureRect(IntRect(50, 58, -50, 50)); // вставка кадра статического прыжка налево
			}

		if (p.rect.left > 500 && p.rect.left < 4050)
			offsetX = p.rect.left - 500;                        // ограничения для движения камеры, чтобы карта не уезжала за игровую область 
		if (p.rect.top > 250 && p.rect.top < 1135)              // ограничения камеры по y
			offsetY = p.rect.top - 250;

		p.update(time);
		Play.clear(Color::White);
		fon.setPosition(-offsetX, -offsetY); // фон двигается вместе с персонажем
		Play.draw(fon); // отрисовка фона

		for (int i = 0; i < H; i++)          //цикл задает в каких строчках карты будут отрисовываться блоки
			for (int j = 0; j < W; j++) {
				if (TileMap[i][j] == 'A')                                      // загрузка картинок по координатам из файла х, у, ширина, высота
					plat.setTextureRect(IntRect(0, 0, ts, ts));                // ts = 50 тк все блок имеют одинаковый размер 50х50 А - текстура земли
				if (TileMap[i][j] == 'B')                                      // если В
					plat.setTextureRect(IntRect(100, 0, ts, ts));              // то загружаем текстуру камня 
				if (TileMap[i][j] == 'C')                                      // если С 
					plat.setTextureRect(IntRect(150, 0, ts, ts));              // то загружаем текстуру облака 
				if (TileMap[i][j] == 'W')                                      // если W 
					plat.setTextureRect(IntRect(200, 0, ts, ts));              // то загружаем текстуру золота 
				if (TileMap[i][j] == 'o')                                      // если о 
					plat.setTextureRect(IntRect(ts, 0, ts, ts));               // то загружаем текстуру птички 
				if (TileMap[i][j] == ' ')                                      // если пробел то ничего не загружаем
					continue;

				plat.setPosition(j * ts - offsetX, i * ts - offsetY);          //камера на игроке
				Play.draw(plat);
			}

		Play.draw(p.sprite);

		Play.display();

		Event event_play;
		while (Play.pollEvent(event_play))
		{
			if (event_play.type == Event::KeyPressed)
			{                                                                           // задаем событие закрытие окна play  при нажатии клавиши esc
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

// Раздел меню настройки игры и его содержимое
void Options()
{
	RenderWindow Options(VideoMode::getDesktopMode(), L"Настройки", Style::Default); // Создание окна "Options" с размерами, соответствующими разрешению рабочего стола
                                                                                    // и заголовком "Настройки". Окно создается с стилем "Default".
	Options.setFramerateLimit(60);                                                  // Ограничение частоты кадров FPS в 60

	RectangleShape background_opt(Vector2f(VideoMode::getDesktopMode().width, VideoMode::getDesktopMode().height)); // Создание прямоугольника для заднего фона с размерами, соответствующими разрешению рабочего стола
	Texture texture_opt;                                                         // Создание объекта класса Texture

	if (!texture_opt.loadFromFile("Images/options.png")) exit(2);      // Загрузка текстуры из файла "Images/options.png". 
                                                                     // В случае ошибки загрузки текстуры, программа будет завершена с кодом 2.
	 
	background_opt.setTexture(&texture_opt);            // Установка текстуры для прямоугольника заднего фона
	while (Options.isOpen())
	{
		Event event_opt;                             //создаем событие
		while (Options.pollEvent(event_opt))         // Обработка событий в окне Options
		{
			if (event_opt.type == Event::Closed) Options.close();           // Проверка, было ли событие закрытия окна
                                                                          // Если да, то окно Options будет закрыто
			if (event_opt.type == Event::KeyPressed)                       // Проверка, было ли нажатие клавиши
			{
				if (event_opt.key.code == Keyboard::Escape) Options.close();       // Если нажата клавиша Escape, то окно Options будет закрыто
			}
		} 
		Options.clear();                    // Очистка окна Options
		Options.draw(background_opt); // Отрисовка прямоугольника заднего фона на окне Options
		Options.display();             // Отображение окна Options на экранеh
	}

}

// Функция с описанием игры
void About_Game() {
	RenderWindow About(VideoMode::getDesktopMode(), L"О игре", Style::Default);
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
			if (event_play.type == Event::Closed) About.close();                // Проверка, было ли событие закрытия окна Если да, то окно About будет закрыто
			if (event_play.type == Event::KeyPressed)                           // Проверка, было ли нажатие клавиши
			{
				if (event_play.key.code == Keyboard::Escape) About.close();             // Если нажата клавиша Escape, то окно About будет закрыто
			}
			About.clear();
			About.draw(background_ab);
			About.display();
		}
	}
}


	int main(){
    
	

	// Создаём окно windows
	RenderWindow window;
	// Параметры: размер окна установить согласно текущему разрешению экрана
	// название моя игра, развернуть графическое окно на весь размер экрана
	window.create(VideoMode(1920,1080), L"Cat n Birds", Style::Default);
	window.setFramerateLimit(60);
	//отключаем видимость курсора
	window.setMouseCursorVisible(false);

	//  текущий размер экрана
	//float width = VideoMode::getDesktopMode().width;
	//float height = VideoMode::getDesktopMode().height;

	// Устанавливаем фон для графического окна 
	// Создаём прямоугольник
	RectangleShape background(Vector2f(1920,1080));
	// Загружаем в прямоугольник текстуру с изображением menu9.jpg
	Texture texture_window;
	if (!texture_window.loadFromFile("Images/mav.png")) return 4;
	background.setTexture(&texture_window);


	Font font;
	if (!font.loadFromFile("font/troika.otf")) return 5;               // Устанавливаем шрифт для  игры
	Text Titul;
	Titul.setFont(font);
	
	InitText(Titul, 480, 50, L"Cat and Birds", 150, Color(72, 123, 255), 3);               // Текст с названием игры

	
	String name_menu[]{ L"Старт",L"Настройки", L"О игре",L"Выход" };                   // Название пунктов меню

	
	game::GameMenu mymenu(window, 950, 350, 4, name_menu, 100, 120);                 // Объект игровое меню
	
	mymenu.setColorTextMenu(Color(72, 123, 255), Color::Red, Color::Black);             // Установка цвета элементов пунктов меню
	
	mymenu.AlignMenu(2);       // выравнивание по центру пунктов меню 

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::KeyReleased)
			{
				// События выбора пунктов меню
				// нажатии на клавиатуре стрелки вверх
				if (event.key.code == Keyboard::Up) { mymenu.MoveUp(); }
				// нажатии на клавиатуре стрелки вниз
				if (event.key.code == Keyboard::Down) { mymenu.MoveDown(); }
				// нажатии на клавиатуре клавиши Enter
				if (event.key.code == Keyboard::Return)
				{
					// Переходим на выбранный пункт меню
					switch (mymenu.getSelectedMenuNumber())
					{
					case 0:GamеStart();   break;
					case 1:Options();     break;
					case 2:About_Game();  break;
					case 3:window.close(); break;

					}

				}
			}
		}

		window.clear();      // Очистка окна от предыдущего кадра
		window.draw(background);     // Отрисовка заднего фона
		window.draw(Titul);         // Отрисовка заголовка
		mymenu.draw();              // Отрисовка меню
		window.display();           // Вывод отрисованного содержимого на экран
	}
	
	
	return 0;
}

// функция настройки текста
void InitText(Text& mtext, float xpos, float ypos, String str, int size_font, // функция InitText, которая предназначена для настройки текста в объекте класса Text
	Color menu_text_color, int bord, Color border_color)
{
	mtext.setCharacterSize(size_font);          // Установка размера шрифта текста
	mtext.setPosition(xpos, ypos);             // Установка позиции текста на экране по осям X и Y
	mtext.setString(str);                      // Установка содержимого текста
	mtext.setFillColor(menu_text_color);       // Установка цвета заливки текста
	mtext.setOutlineThickness(bord);           // Установка толщины обводки текста
	mtext.setOutlineColor(border_color);      // Установка цвета обводки текста
}

