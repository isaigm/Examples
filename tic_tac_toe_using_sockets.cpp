#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <thread>
#include <iostream>
#define EMPTY 0
#define X_PLAYER 1
#define O_PLAYER 2
#define INIT_X 277
#define INIT_Y 177
#define S 90
#define PORT 1234
sf::TcpSocket socket;
bool canMove = false;
bool finishedGame = false;
class Game
{
public:
  Game()
  {
    if (!font.loadFromFile("C:\\Users\\isaig\\Desktop\\arial.ttf"))
    {
      std::cout << "Cannot load font\n";
      exit(1);
    }
    for (int i = 0; i < 3; i++)
    {
      for (int j = 0; j < 3; j++)
      {
        board[i][j] = EMPTY;
        sf::RectangleShape r;
        sf::Vector2f size(S, S);
        r.setSize(size);
        r.setPosition(INIT_X + S * j, INIT_Y + S * i);
        r.setFillColor(sf::Color(192, 192, 192));
        r.setOutlineColor(sf::Color::Black);
        r.setOutlineThickness(2);
        rects.push_back(std::move(r));
      }
    }
  }
  bool makeMove(int player, int x, int y)
  {
    if (board[y][x] == EMPTY && x >= 0 && x <= 2 && y >= 0 && y <= 2)
    {
      board[y][x] = player;
      sf::Text text;
      text.setCharacterSize(70);
      text.setFillColor(sf::Color::Black);
      text.setFont(font);
      text.setStyle(sf::Text::Regular);
      if (player == X_PLAYER)
      {
        text.setString("X");
        if (won(player, x, y))
        {
          wWon = player;
        }
      }
      else if (player == O_PLAYER)
      {
        text.setString("O");
        if (won(player, x, y))
        {
          wWon = player;
        }
      }
      auto bounds = text.getLocalBounds();
      auto center = sf::Vector2f(INIT_X + S * x + S / 2, INIT_Y + S * y + S / 2);
      text.setPosition(center.x - bounds.width / 2, center.y - bounds.height / 2 - 20);
      moves.push_back(std::move(text));
      sf::Packet packet;
      packet << x;
      packet << y;
      packet << player;
      packet << wWon;
      socket.send(packet);
      return true;
    }
    return false;
  }
  bool chooseCell(int player, const sf::Vector2i &mousePos)
  {
    for (const auto &r : rects)
    {
      if (r.getGlobalBounds().contains(mousePos.x, mousePos.y))
      {
        int x = (r.getPosition().x - INIT_X) / S;
        int y = (r.getPosition().y - INIT_Y) / S;
        return makeMove(player, x, y);
      }
    }
    return false;
  }
  void draw(sf::RenderTarget &target)
  {
    for (const auto &r : rects)
    {
      target.draw(r);
    }
    for (const auto &m : moves)
    {
      target.draw(m);
    }
  }
  int whoWon()
  {
    return wWon;
  }

private:
  uint8_t board[3][3];
  std::vector<sf::RectangleShape> rects;
  std::vector<sf::Text> moves;
  sf::Font font;
  int wWon = 0;
  bool won(int player, int x, int y)
  {
    int count = 0;
    for (int i = 0; i < 3; i++)
    {
      if (board[i][x] == player)
        count++;
    }
    if (count == 3)
      return true;
    count = 0;
    for (int i = 0; i < 3; i++)
    {
      if (board[y][i] == player)
        count++;
    }
    if (count == 3)
      return true;
    if (x == y)
    {
      count = 0;
      for (int i = 0; i < 3; i++)
      {
        if (board[i][i] == player)
          count++;
      }
      if (count == 3)
        return true;
    }
    if ((x == 1 && y == 1) || (x == 2 && y == 0) || (x == 0 && y == 2))
    {
      count = 0;
      for (int i = 2; i >= 0; i--)
      {
        if (board[2 - i][i] == player)
          count++;
      }
      if (count == 3)
        return true;
    }
    return false;
  }
};
int main()
{
  int option;
  std::cin >> option;
  if (option == 1)
  {
    sf::TcpListener listener;
    std::vector<sf::TcpSocket *> clients;
    sf::SocketSelector selector;
    listener.listen(PORT);
    selector.add(listener);
    while (true)
    {
      if (selector.wait())
      {
        if (selector.isReady(listener))
        {
          auto client = new sf::TcpSocket;
          if (listener.accept(*client) == sf::TcpSocket::Done)
          {
            std::cout << "new client\n";
            sf::Packet packet;
            clients.push_back(client);
            selector.add(*client);
            packet << (clients.size() % 2) + 1;
            client->send(packet);
          }
          else
          {
            delete client;
          }
        }
        else
        {
          for (size_t i = 0; i < clients.size(); ++i)
          {
            if (selector.isReady(*clients[i]))
            {
              sf::Packet packet;
              if (clients[i]->receive(packet) == sf::Socket::Done)
              {
                for (size_t j = 0; j < clients.size(); j++)
                {
                  if (i != j)
                  {
                    clients[j]->send(packet);
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  else
  {
    sf::Uint64 player = 0;
    sf::Packet packet;
    sf::RenderWindow window(sf::VideoMode(800, 600), "Tic Tac Toe multiplayer");
    sf::Event ev;
    socket.connect("localhost", PORT);
    if (socket.receive(packet) != sf::TcpSocket::Done)
    {
      std::cout << "cannot get player id\n";
      exit(1);
    }
    packet >> player;
    if (player == O_PLAYER)
    {
      canMove = false;
    }
    else if (player == X_PLAYER)
    {
      canMove = true;
    }
    window.setVerticalSyncEnabled(true);
    Game game; //probable false sharing
    std::thread th([&game, player]() {
      while (true)
      {
        int x = -1;
        int y = -1;
        int splayer = 0;
        int whoWon = 0;
        sf::Packet packet;
        if (socket.receive(packet) == sf::TcpSocket::Done)
        {
          packet >> x;
          packet >> y;
          packet >> splayer;
          packet >> whoWon;
          if (player != splayer)
          {
            if (player == O_PLAYER)
            {
              game.makeMove(X_PLAYER, x, y);
            }
            else if (player == X_PLAYER)
            {
              game.makeMove(O_PLAYER, x, y);
            }
            if (whoWon != 0)
            {
              finishedGame = true;
            }
            canMove = true;
          }
        }
      }
    });
    th.detach();
    while (window.isOpen())
    {
      while (window.pollEvent(ev))
      {
        switch (ev.type)
        {
        case sf::Event::Closed:
          window.close();
          break;
        case sf::Event::MouseButtonPressed:
          if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && canMove && !finishedGame)
          {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            bool flag = game.chooseCell(player, mousePos);
            if (flag)
            {
              canMove = false;
              if (game.whoWon() != 0)
              {
                std::cout << "player: " << game.whoWon() << " has won\n";
                finishedGame = true;
              }
            }
          }
          break;
        default:
          break;
        }
      }
      window.clear(sf::Color::White);
      game.draw(window);
      window.display();
    }
  }
  return 0;
}