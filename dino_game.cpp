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
const static sf::IntRect dino_frames[] = {
	{936, 1, 44, 49}, 
	{980, 1, 44, 48},
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
const static int N_CACTUS = sizeof(cactus_frames) / sizeof(sf::IntRect);
const static sf::IntRect cloud_frame = { 86, 1, 46, 17 };

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
		x += x_speed * dt;
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
	int x_speed = 125;
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
int main()
{
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "--Google Dino-- por Isaí García Mendoza ");
	window.setVerticalSyncEnabled(true);
	sf::Texture spritesheet;// sf::Texture es excelente, ya que te permite recortar y ocupar ese recorte en un sprite, asi no hace falta tener muchos sf::Texture
							//sino recortar la parte que se necesita
	
	
	if (!spritesheet.loadFromFile(SPRITESHEET_FILE))
	{
		return -1;
	}
	sf::SoundBuffer buffer;
	buffer.loadFromFile("C:\\Users\\isaig\\Desktop\\assets\\jump.wav");
	sf::Sound jump_sfx(buffer);
	std::vector<sf::Sprite> clouds;
	init_clouds(clouds, spritesheet);
	
	sf::Sprite cactus(spritesheet);
	cactus.setTextureRect(cactus_frames[N_CACTUS - 1]);
	cactus.setPosition({ WIDTH + 10, 123});
	Player player(spritesheet);
	Background ground(sf::FloatRect(0, 12, WIDTH, HEIGHT / 2), player.x_speed);
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
					jump_sfx.play();
					player.is_on_ground = false;
				}
				break;
			default:
				break;
			}
		}
		for (auto &cloud: clouds)
		{
			if(!sky.contains(cloud))
			{
				cloud.setPosition({ sky.view.getCenter().x + WIDTH / 2 + 3 * cloud_frame.width, float(get_random_number(0, 30)) });
			}
		}
		if (player.sp.getGlobalBounds().intersects(cactus.getGlobalBounds()))
		{
			
		}
		dist_x += dt * player.x_speed;
		player.update(dt);
		sky.update(dt);
		ground.update(dt);
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
		if (dt_anim >= (1.0f / 10.0f) )
		{
			player.animate();
			dt_anim -= (1.0f / 10.0f);
		}
		window.clear(sf::Color::White);
		window.setView(sky.view);
		for (const auto& cloud : clouds)
		{
			window.draw(cloud);
		}
		window.setView(ground.view);
		window.draw(bg);
		window.draw(next_bg);
		window.draw(cactus);
		window.draw(player.sp);
	
		window.display();
	}

	return 0;
}


