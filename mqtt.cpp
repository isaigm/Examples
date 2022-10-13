#include <SFML/Graphics.hpp>
#include <iostream>
#include <MQTTClient.h>
#include <nlohmann/json.hpp>
#include <map>
#define ADDRESS "tcp://localhost:1883"
#define CLIENTID "test2"
#define TOPIC "test"
#define QOS 1
#define TIMEOUT 10000L

#define HEIGHT 600
#define WIDTH 800
#define MAX_RANGE_LAT 30.0f
#define MAX_RANGE_LON 30.0f
#define KM_PER_DEGREE_LAT 111.111f
#define KM_PER_DEGREE_LON 87.8701f

//g++ gps.cpp  -lsfml-window -lsfml-system -lsfml-graphics -lpaho-mqtt3c -frtti -fexceptions -o gps

using json = nlohmann::json;
std::map<std::string, sf::CircleShape> points;
float centerlat = 19.0441;
float centerlon = -95.9716;

float toplat = centerlat + MAX_RANGE_LAT / KM_PER_DEGREE_LAT;
float bottomlat = centerlat - MAX_RANGE_LAT / KM_PER_DEGREE_LAT;

float leftlon = centerlon - MAX_RANGE_LON / KM_PER_DEGREE_LON;
float rightlon = centerlon + MAX_RANGE_LON / KM_PER_DEGREE_LON;
float map(float n, float x1, float x2, float y1, float y2)
{
    float m = (y2 - y1) / (x2 - x1);
    return y1 + m * (n - x1);
}
int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{

    json packet = json::parse((char *)message->payload);
    float lat = packet["gpsLocation"]["lat"];
    float lon = packet["gpsLocation"]["lon"];
    std::string id_soldier = packet["id_soldier"];
    float ry = map(lat, bottomlat, toplat, HEIGHT, 0);
    float rx = map(lon, leftlon, rightlon, 0, WIDTH);
    if (points.find(id_soldier) == points.end())
    {
        std::cout << id_soldier << "\n";
        sf::CircleShape point(2);
        point.setFillColor(sf::Color::White);
        point.setPosition({rx, ry});
        points[id_soldier] = point;
    }
    else
    {
        points[id_soldier].setPosition({rx, ry});
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

    sf::RenderWindow window(sf::VideoMode(800, 600), "GPS");
    window.setVerticalSyncEnabled(true);
    sf::CircleShape point(2);
    point.setFillColor(sf::Color::Blue);
    point.setPosition({map(centerlon, leftlon, rightlon, 0, WIDTH), map(centerlat, bottomlat, toplat, HEIGHT, 0)});
    points["center"] = point;
    while (window.isOpen())
    {
        sf::Event ev;
        while (window.pollEvent(ev))
        {
            if (ev.type == sf::Event::Closed)
            {
                window.close();
                break;
            }
        }
        window.clear();
        for (auto &p : points)
        {
            window.draw(p.second);
        }
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
