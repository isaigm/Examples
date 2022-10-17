#include <SFML/Graphics.hpp>
#include <iostream>
#include <MQTTClient.h>
#include <nlohmann/json.hpp>
#include <map>
#include <cmath>

#define ADDRESS "tcp://192.168.10.133:1883"
#define CLIENTID "clientid"
#define TOPIC "test"
#define QOS 1
#define TIMEOUT 10000L
#define HEIGHT 480
#define WIDTH 800
#define MAX_RANGE_LAT 30.0f
#define MAX_RANGE_LON 30.0f
#define KM_PER_DEGREE_LAT 111.111f
#define KM_PER_DEGREE_LON 87.8701f
#define POINT_RADIUS 5
#define BOUNDS_SIZE 25

using json = nlohmann::json;
const float centerlat = 19.0441;
const float centerlon = -95.9716;
const float toplat = centerlat + MAX_RANGE_LAT / KM_PER_DEGREE_LAT;
const float bottomlat = centerlat - MAX_RANGE_LAT / KM_PER_DEGREE_LAT;
const float leftlon = centerlon - MAX_RANGE_LON / KM_PER_DEGREE_LON;
const float rightlon = centerlon + MAX_RANGE_LON / KM_PER_DEGREE_LON;

float map(float n, float x1, float x2, float y1, float y2)
{
    float m = (y2 - y1) / (x2 - x1);
    return y1 + m * (n - x1);
}
float deg_to_rad(float deg)
{
    return deg * M_PI / 180;
}
struct gps_point
{
    gps_point(float _lat, float _lon) : lat(_lat), lon(_lon) {}
    gps_point() {}
    sf::Vector2f to_screen_coords()
    {
        float x = map(lon, leftlon, rightlon, 0, WIDTH);
        float y = map(lat, bottomlat, toplat, HEIGHT, 0);
        return {x, y};
    }
    float distance_to(gps_point other)
    {
        float earth_radius = 6371;
        float dlat = deg_to_rad(other.lat - lat);
        float dlon = deg_to_rad(other.lon - lon);
        float a = sin(dlat / 2) * sin(dlat / 2) +
                  sin(dlon / 2) * sin(dlon / 2) * cos(lat) * cos(other.lat);
        float c = 2 * atan2(sqrt(a), sqrt(1 - a));
        return earth_radius * c;
    }
    float lat, lon;
};

class gps_ui
{
public:
    gps_ui() {}
    gps_ui(float lat, float lon, float radius = POINT_RADIUS, sf::Color color = sf::Color::Green) : loc(lat, lon)
    {
        circle.setFillColor(color);
        circle.setRadius(radius);
        circle.setOrigin({radius, radius});
        circle.setPosition(loc.to_screen_coords());
        bounds.width = BOUNDS_SIZE;
        bounds.height = BOUNDS_SIZE;
        center_bounds(circle.getPosition().x, circle.getPosition().y);
    }
    void update_pos(float lat, float lon)
    {
        loc.lat = lat;
        loc.lon = lon;
    }
    void render(sf::RenderTarget &rt)
    {
        
        circle.setPosition(loc.to_screen_coords());
        center_bounds(circle.getPosition().x, circle.getPosition().y);
        rt.draw(circle);
    }
    bool is_selected(sf::Vector2f mouse_pos)
    {
        return bounds.contains(mouse_pos);
    }
    gps_point get_location()
    {
        return loc;
    }

private:
    void center_bounds(float x, float y)
    {
        bounds.left = x - BOUNDS_SIZE / 2;
        bounds.top = y - BOUNDS_SIZE / 2;
    }
    sf::FloatRect bounds;
    sf::CircleShape circle;
    gps_point loc;
};
std::map<std::string, gps_ui> points;
int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
    try
    {
        json packet = json::parse((char *)message->payload);
        float lat = packet["gpsLocation"]["lat"];
        float lon = packet["gpsLocation"]["lon"];
        std::string id_soldier = packet["id_soldier"];

        if (points.find(id_soldier) == points.end())
        {
            gps_ui point(lat, lon);
            points[id_soldier] = point;
        }
        else
        {
            points[id_soldier].update_pos(lat, lon);
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}
void connlost(void *context, char *cause)
{
    printf("\nConnection lost\n");
    printf("     cause: %s\n", cause);
}
void drawAxis(sf::RenderTarget &rt)
{
    sf::Vertex xaxis[2];
    xaxis[0].position = {0, HEIGHT / 2};
    xaxis[0].color = sf::Color::White;
    xaxis[1].position = {WIDTH, HEIGHT / 2};
    xaxis[1].color = xaxis[0].color;
    sf::Vertex yaxis[2];
    yaxis[0].position = {WIDTH / 2, 0};
    yaxis[0].color = sf::Color::White;
    yaxis[1].position = {WIDTH / 2, HEIGHT};
    yaxis[1].color = yaxis[0].color;
    rt.draw(xaxis, 2, sf::Lines);
    rt.draw(yaxis, 2, sf::Lines);
}
int main()
{
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    int rc;

    if ((rc = MQTTClient_create(&client, ADDRESS, CLIENTID,
                                MQTTCLIENT_PERSISTENCE_NONE, NULL)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to create client, return code %d\n", rc);
        return rc;
    }

    if ((rc = MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, NULL)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to set callbacks, return code %d\n", rc);
        return rc;
    }

    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect, return code %d\n", rc);
        return rc;
    }

    if ((rc = MQTTClient_subscribe(client, TOPIC, QOS)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to subscribe, return code %d\n", rc);
        return rc;
    }

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "GPS");
    window.setVerticalSyncEnabled(true);
    points["-1"] = gps_ui(centerlat, centerlon, 10, sf::Color::Blue);
    sf::Font font;
    sf::Text text;
    font.loadFromFile("fuente.ttf");
    text.setFont(font);
    text.scale({0.5, 0.5});
    text.setPosition(10, 10);
    
    while (window.isOpen())
    {
        sf::Event ev;
        sf::Vector2f mouse_pos;
        while (window.pollEvent(ev))
        {
            switch (ev.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::MouseButtonPressed:
                mouse_pos = sf::Vector2f(sf::Mouse::getPosition(window));
                printf("%f %f\n", mouse_pos.x, mouse_pos.y);
                for (auto &p : points)
                {
                    if (p.second.is_selected(mouse_pos))
                    {
                        char msg[256];
                        float distance = points["-1"].get_location().distance_to(p.second.get_location());
                        snprintf(msg, sizeof(msg), "Objetivo seleccionado: %s\nDistancia al gateway: %.2f km", p.first.c_str(), distance);
                        text.setString(std::string(msg));
                        break;
                    }
                }
                break;
            default:
                break;
            }
        }
        window.clear();
        drawAxis(window);
        for (auto &p : points)
        {
            p.second.render(window);
        }
        window.draw(text);
        window.display();
    }
    if ((rc = MQTTClient_unsubscribe(client, TOPIC)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to unsubscribe, return code %d\n", rc);
        rc = EXIT_FAILURE;
    }
    if ((rc = MQTTClient_disconnect(client, 10000)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to disconnect, return code %d\n", rc);
        rc = EXIT_FAILURE;
    }
    return 0;
}
