#include <SPI.h>
#include <TFT_eSPI.h>
#include <vector>
#define GREEN 0x07E0

TFT_eSPI tft = TFT_eSPI();
const int centerx = 320 / 2;
const int centery = 240 / 2;
const int nrays = 20;
const int ntargets = 15;
const int raylen = 100;
int counter = 0;
float deg_to_rad(float deg)
{
    return deg * M_PI / 180.0f;
}
struct point
{
    point() {}
    point(int x, int y)
    {
        this->x = x;
        this->y = y;
    }
    float distance_to(point &other)
    {
        int dx = x - other.x;
        int dy = y - other.y;
        return sqrt(dx * dx + dy * dy);
    }
    int x, y;
};
class ray
{
public:
    ray(int x, int y, int r, float start_angle = 0)
    {
        this->x = x;
        this->y = y;
        this->r = r;
        angle = start_angle;
    }
    point get_end_point()
    {
        return point(x, y);
    }
    void render()
    {
        tft.drawLine(centerx, centery, prevx, prevy, 0);
        x = centerx + r * cos(angle);
        y = centery + r * sin(angle);
        tft.drawLine(centerx, centery, x, y, GREEN);
        prevx = x;
        prevy = y;
        angle += deg_to_rad(4);
    }

private:
    int x, y, prevx = 0, prevy = 0, r;
    float angle = 0;
};
class target
{
public:
    target(int x, int y) : pos(x, y), prevpos(0, 0)
    {
        direction = deg_to_rad(random(1, 360));
    }
    void render(point &p1, point &p2, point &p3)
    {
        int right = centerx + raylen;
        int left = centerx - raylen;
        int up = centery - raylen;
        int down = centery + raylen;
        if (pos.x >= right || pos.x <= left || pos.y <= up || pos.y >= down)
        {
            pos.x = random(centerx - 75, centerx + 75);
            pos.y = random(centery - 75, centery + 75);
            direction = deg_to_rad(random(1, 360));
        }

        uint32_t color = is_visible(p1, p2, p3) ? 0xFFFF : 0;

        tft.fillCircle(prevpos.x, prevpos.y, 2, 0);

        if (counter > 10)
        {

            int steps = random(2);
            pos.x += steps * cos(direction);
            pos.y += steps * sin(direction);
        }

        tft.fillCircle(pos.x, pos.y, 2, color);
        prevpos.x = pos.x;
        prevpos.y = pos.y;
    }

private:
    float sign(point p1, point p2, point p3)
    {
        return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
    }
    bool is_visible(point v1, point v2, point v3)
    {
        float d1, d2, d3;
        bool has_neg, has_pos;

        d1 = sign(pos, v1, v2);
        d2 = sign(pos, v2, v3);
        d3 = sign(pos, v3, v1);

        has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
        has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

        return !(has_neg && has_pos);
    }
    point pos, prevpos;
    float direction;
};
std::vector<ray> rays;
std::vector<target> targets;
void setup()
{
    randomSeed(analogRead(0));

    float angle = 0;
    for (int i = 0; i < nrays / 2; i++)
    {
        rays.emplace_back(centerx, centery, raylen, deg_to_rad(angle));
        angle += 3;
    }
    angle = -3;
    for (int i = 0; i < nrays / 2; i++)
    {
        rays.emplace_back(centerx, centery, raylen, deg_to_rad(angle));
        angle -= 3;
    }
    for (int i = 0; i < ntargets; i++)
    {
        int x = random(centerx - 75, centerx + 75);
        int y = random(centery - 75, centery + 75);
        targets.emplace_back(x, y);
    }

    tft.init();
    tft.fillScreen(TFT_BLACK);
    tft.drawString("PPI Radar Display", 100, 0);
    tft.drawString("CFAR:on", centerx + raylen + 10, centery);
    tft.drawString("MTD:on", centerx + raylen + 10, centery + 15);
    tft.drawString("PD:1", centerx + raylen + 10, centery + 30);
    tft.drawString("PFA:0", centerx + raylen + 10, centery + 45);
    tft.drawString("MAX RANGE: 333 km", 0, 225);
}
void loop()
{
    for (auto &r : rays)
    {
        r.render();
    }
    for (auto &t : targets)
    {
        point p1(centerx, centery);
        point p2 = rays[nrays / 2 - 1].get_end_point();
        point p3 = rays[nrays - 1].get_end_point();
        t.render(p1, p2, p3);
    }
    tft.drawLine(centerx - raylen, centery, centerx + raylen, centery, GREEN);
    tft.drawLine(centerx, centery - raylen, centerx, centery + raylen, GREEN);
    tft.drawCircle(centerx, centery, 100, GREEN);
    tft.drawCircle(centerx, centery, 75, GREEN);
    tft.drawCircle(centerx, centery, 50, GREEN);
    delay(100);
    if (counter > 10)
    {
        counter = 0;
    }
    counter += 1;
}
