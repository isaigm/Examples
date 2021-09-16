#include <iostream>
#include <SFML/Graphics.hpp>
const static char* SPRISHEET_FILE = "C:\\Users\\isaig\\Desktop\\sprites.png";
const static int WIDTH = 600;
const static int HEIGHT = 320;
const static float gravity = 900.0f;
const static int dino_frames[][4] = { {846, 0, 43, 48},
	{937, 0, 43, 48}, 
	{980, 0, 43, 48} };


sf::IntRect coords_to_rect_int(int idx)
{
	sf::IntRect rect(dino_frames[idx][0], dino_frames[idx][1], dino_frames[idx][2], dino_frames[idx][3]);
	return rect;
}
class player
{
public:
	player(sf::Texture& texture)
	{
		sp.setTexture(texture);
		sp.setTextureRect(coords_to_rect_int(curr_frame_anim));
		x = 20;
		ground = y = 125;
		y_speed = -500;
		sp.setPosition(x, y);
	}
	void animate()
	{
		sp.setTextureRect(coords_to_rect_int(curr_frame_anim));
		if (++curr_frame_anim >= 3)
		{
			curr_frame_anim = 0;
		}
	}
	void update(float dt)
	{
		x += x_speed * dt;
		if (!is_on_ground)
		{
			y += y_speed * dt;
			y_speed += gravity * dt;
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
	int x_speed = 100;
};


int main()
{
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Google Dino :v");
	sf::Texture spritesheet;// sf::Texture es excelente, ya que te permite recortar y ocupar ese recorte en un sprite, asi no hace falta tener muchos sf::Texture
							//sino recortar la parte que se necesita
	
	sf::View view(sf::FloatRect(0, 0, WIDTH, HEIGHT));
	
	if (!spritesheet.loadFromFile(SPRISHEET_FILE))
	{
		return 1;
	}
	
	player dino(spritesheet);
	sf::Sprite bg; //fondo principal
	
	bg.setTexture(spritesheet);
	bg.setTextureRect(sf::IntRect(0, 53, 1233, 15));
	bg.setPosition({ 0, HEIGHT / 2 });
	sf::Sprite next_bg = bg; //fondo auxiliar, aunque igualmente se van a ir intercalando

	window.setVerticalSyncEnabled(true);
	float temp_x = dino.x; //para saber que tanto del fondo actual, que mide 1233 pixeles de ancho, se ha recorrido

	float dt_anim = 0;
	float dt = 0;
	bool main_bg = true;
	int factor = 1;
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
				if (ev.key.code == sf::Keyboard::Space)
				{
				
					dino.is_on_ground = false;
				}
				break;
			default:
				break;
			}
		}
		
		temp_x += dt * dino.x_speed;
		dino.update(dt);
		view.move(dt * dino.x_speed, 0);
		

		if (temp_x >= 1233 - WIDTH) //esto se hace para que antes de recorrer el fondo por completo, se cargue otro en la posicion correcta y asi
		{						    //dar la ilusion de un fondo infinito
			if (main_bg)
			{
				next_bg.setPosition({ float(1200 * factor), HEIGHT / 2 }); 
			}
			else
			{
				bg.setPosition({ float(1200 * factor), HEIGHT / 2 });
			}
			factor++;
			main_bg = !main_bg;
			temp_x = -(1233 - temp_x); //en este punto faltan 1233-temp_x pixeles para recorrer todo, igual aqui el segundo fondo ya se ha cargado, pero aun no es visible
			//es negativo  porque cuando se recorran los 1233-temp_x pixeles, tiene que valer 0, lo que corresponde con que tanto se ha recorrido
			//el segundo fondo
		  
		}
		if (dt_anim >= (1.0f / 15.0f) )
		{
			dino.animate();
			dt_anim -= (1.0f / 15.0f);
		}
		window.clear(sf::Color::White);
		window.setView(view);
		window.draw(dino.sp);
		window.draw(bg);
		window.draw(next_bg);
		window.display();
	}

	return 0;
}
