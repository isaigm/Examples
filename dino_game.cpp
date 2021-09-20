#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <ctime>
#include <cstdlib>
const static char* SPRITESHEET_FILE = "C:\\Users\\isaig\\Desktop\\assets\\sprites.png";
const static int WIDTH = 600;
const static int HEIGHT = 320;
const static float GRAVITY = 900.0f;
const static int N_CLOUDS = 5;
const static float ground = 125 + 47;
const static int player_speed = 130;
const static sf::IntRect dino_frames[] = {
	{936, 2, 44, 47}, 
	{980, 2, 44, 47},
    {848, 2, 44, 47}
};
const static sf::IntRect cactus_frames[] =
{
	{228, 2, 17, 35},
	{245, 2, 34, 35},
	{279, 2, 51, 35},
	{332, 2, 49, 50},
	{382, 2, 25, 50},
	{407, 2, 75, 50}
};
const static sf::IntRect cloud_frame = {86, 1, 46, 17};
const static sf::IntRect die_frame = {1024, 2, 44, 47};
const static sf::IntRect game_over_frame = { 655, 15, 191, 11 };
const static int N_CACTUS = sizeof(cactus_frames) / sizeof(sf::IntRect);

class Player
{
public:
	Player(sf::Texture& texture)
	{
		sp.setTexture(texture);
		sp.setTextureRect(dino_frames[curr_frame_anim]);
		x = 20;
		ground = y = 125;
		y_speed = -500;
		sp.setPosition(x, y);
	}
	void animate()
	{
		if (is_on_ground)
		{
			sp.setTextureRect(dino_frames[curr_frame_anim]);
			if (++curr_frame_anim >= 2)
			{
				curr_frame_anim = 0;
			}
		}
		else
		{
			sp.setTextureRect(dino_frames[2]);
			curr_frame_anim = 1;
		}
		
	}
	void update(float dt)
	{
		x += player_speed * dt;
		if (!is_on_ground)
		{
			y += y_speed * dt;
			y_speed += GRAVITY * dt;
			if (y >= ground)
			{
				y = ground;
				is_on_ground = true;
				y_speed = -500;
			}
		}
		sp.setPosition({ x, y });
	}
	
	bool is_on_ground = true;
	sf::Sprite sp;
	int curr_frame_anim = 0;
	float x,  y, ground, y_speed;
};
class Background
{
public:
	Background(sf::FloatRect rect, float speed) : view(rect)
	{
		this->speed = speed;

	}
	void update(float dt)
	{
		view.move(speed * dt, 0);
	}
	bool contains(sf::Sprite& sp)
	{
		return !(sp.getPosition().x + sp.getGlobalBounds().width + 5 - (view.getCenter().x - WIDTH / 2) <= 0);
	}
	float speed;
	sf::View view;
};
class Sfx
{
public:
	Sfx(std::string file)
	{
		if (!buffer.loadFromFile(file))
		{
			std::cerr << "no se pudo cargar el archivo de audio\n";
			exit(-1);
		}
		sound.setBuffer(buffer);
	}
	void play()
	{
		sound.play();
	}
private:
	sf::SoundBuffer buffer;
	sf::Sound sound;
};
int get_random_number(int x, int y)
{
	return rand() % (y + 1 - x) + x;
}
void init_clouds(std::vector<sf::Sprite>& clouds, sf::Texture &spritesheet)
{
	float last_coords = WIDTH + 20;
	for (int i = 0; i < N_CLOUDS; i++)
	{
		sf::Sprite cloud(spritesheet);
		cloud.setTextureRect(cloud_frame);
		cloud.setPosition({ last_coords, float(get_random_number(0, 20))});
		last_coords += get_random_number(2, 4) * cloud_frame.width;
		clouds.push_back(std::move(cloud));
	}
}
void set_cactus_coords(sf::Sprite& cactus, float x)
{
	cactus.setPosition({ x, ground - cactus.getGlobalBounds().height });
}
int main()
{
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "--Google Dino-- por Isaí García Mendoza ");
	window.setVerticalSyncEnabled(true);
	sf::Texture spritesheet;// sf::Texture es excelente, ya que te permite recortar y ocupar ese recorte en un sprite, asi no hace falta tener muchos sf::Texture
							//sino recortar la parte que se necesita
	
	srand(time(nullptr));
	if (!spritesheet.loadFromFile(SPRITESHEET_FILE))
	{
		std::cerr << "no se pudo cargar el archivo de sprites\n";
		exit(-1);
	}
	Sfx jump("C:\\Users\\isaig\\Desktop\\assets\\jump.wav");
	Sfx die("C:\\Users\\isaig\\Desktop\\assets\\die.wav");
	std::vector<sf::Sprite> clouds;
	init_clouds(clouds, spritesheet);
	sf::Sprite game_over_screen(spritesheet), cactus(spritesheet);
	cactus.setTextureRect(cactus_frames[get_random_number(0, N_CACTUS - 1)]);
	set_cactus_coords(cactus, 500);
	game_over_screen.setTextureRect(game_over_frame);
	game_over_screen.setPosition({ float(WIDTH - game_over_frame.width) / 2, 50});
	Player player(spritesheet);
	Background ground(sf::FloatRect(0, 12, WIDTH, HEIGHT / 2), player_speed);
	Background sky(sf::FloatRect(0, 0, WIDTH, HEIGHT / 2), 25);
	ground.view.setViewport(sf::FloatRect(0, 0, 1, 0.5f));
	sky.view.setViewport(sf::FloatRect(0, 0, 1, 0.5f));
	sf::Sprite bg(spritesheet); //fondo principal
	bg.setTextureRect(sf::IntRect(2, 53, 1200, 15));
	bg.setPosition({ 0, HEIGHT / 2 });
	sf::Sprite next_bg = bg; //fondo auxiliar, aunque igualmente se van a ir intercalando
	float dist_x = player.x; //para saber que tanto del fondo actual, que mide 1200 pixeles de ancho, se ha recorrido
	float dt_anim = 0;
	float dt = 0;
	bool main_bg = true;
	bool game_over = false;
	float next_coords = 1200;
	sf::Clock clock;
	
	while (window.isOpen())
	{
		dt = clock.restart().asSeconds();
		dt_anim += dt;
		sf::Event ev;
		while (window.pollEvent(ev))
		{
			switch (ev.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::KeyPressed:
				if (ev.key.code == sf::Keyboard::Space && player.is_on_ground)
				{
					jump.play();
					player.is_on_ground = false;
				}
				break;
			default:
				break;
			}
		}
		if (game_over && sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			bg.setPosition({ 0, HEIGHT / 2 });
			game_over = false;
			dt = 0;
			dt_anim = 0;
			next_coords = 1200;
			main_bg = true;
			dist_x = 20;
			player.x = 20;
			player.y = 125;
			clouds.erase(clouds.begin(), clouds.end());
			init_clouds(clouds, spritesheet);
			cactus.setTextureRect(cactus_frames[get_random_number(0, N_CACTUS - 1)]);
			sky.view.setCenter({ WIDTH / 2, 80 });
			ground.view.setCenter({ WIDTH / 2, 92 });
			set_cactus_coords(cactus, 500);
			continue;
		}
		if (!game_over)
		{
			dist_x += dt * player_speed;
			player.update(dt);
			sky.update(dt);
			ground.update(dt);
			if (player.sp.getGlobalBounds().intersects(cactus.getGlobalBounds()))
			{
				player.sp.setTextureRect(die_frame);
				die.play();
				game_over = true;
				continue;
			}
			if (!ground.contains(cactus))
			{
				cactus.setTextureRect(cactus_frames[get_random_number(0, N_CACTUS - 1)]);
				set_cactus_coords(cactus, ground.view.getCenter().x + WIDTH / 2 + 30);
			}
			for (auto& cloud : clouds)
			{
				if (!sky.contains(cloud))
				{
					cloud.setPosition({ sky.view.getCenter().x + WIDTH / 2 + 3 * cloud_frame.width, float(get_random_number(0, 30)) });
				}
			}
			if (dist_x >= 1200 - WIDTH) //esto se hace para que antes de recorrer el fondo por completo, se cargue otro en la posicion correcta y asi
			{						    //dar la ilusion de un fondo infinito

				if (main_bg)
				{
					next_bg.setPosition({ next_coords, HEIGHT / 2 });
				}
				else
				{
					bg.setPosition({ next_coords, HEIGHT / 2 });
				}
				next_coords += 1200;
				main_bg = !main_bg;
				dist_x = -(1200 - dist_x); //en este punto faltan 1200-temp_x pixeles para recorrer todo, igual aqui el segundo fondo ya se ha cargado, pero aun no es visible
				//es negativo  porque cuando se recorran los 1200-temp_x pixeles, tiene que valer 0, lo que corresponde con que tanto se ha recorrido
				//el segundo fondo
			}
			if (dt_anim >= (1.0f / 10.0f))
			{
				player.animate();
				dt_anim -= (1.0f / 10.0f);
			}
		}
		window.clear(sf::Color::White);
		if (game_over)
		{
			window.setView(window.getDefaultView());
			window.draw(game_over_screen);
		}
		window.setView(sky.view);
		for (const auto& cloud : clouds)
		{
			window.draw(cloud);
		}
		window.setView(ground.view);
		window.draw(bg);
		window.draw(player.sp);
		window.draw(next_bg);
		window.draw(cactus);
		window.display();
	}
	return 0;
}
