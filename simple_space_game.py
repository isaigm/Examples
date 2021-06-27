import machine, utime, framebuf, random
from ssd1306 import SSD1306_I2C

ROWS = 63
COLS = 127
SPRITE_WIDTH = 16
SPRITE_HEIGHT = 16
i2c = machine.I2C(1, scl=machine.Pin(15), sda=machine.Pin(14))
oled = SSD1306_I2C(128, 64, i2c)
pot = machine.ADC(28)
shoot = machine.Pin(16, machine.Pin.IN, machine.Pin.PULL_DOWN)
game_over = False
who_won = False
class spaceship:
    def __init__(self, x: int, y: int, sprite: list):
        self.x = x
        self.y = y
        self.fb = framebuf.FrameBuffer(bytearray(sprite), SPRITE_WIDTH, SPRITE_HEIGHT, framebuf.MONO_HLSB)
        self.speed = 1
        self.health = 20
        self.bullets = []
        self.can_shoot = False
    
    def draw(self, oled):
        for b in self.bullets:
            oled.pixel(b.x, b.y, 1)
        oled.blit(self.fb, self.x, self.y)
        
class bullet: 
    def __init__(self, x: int, y: int, speed: int):
        self.x = x
        self.y = y
        self.speed = speed
    
    def collide(self, _spaceship):
        colliding = _spaceship.x <= self.x <= _spaceship.x + SPRITE_WIDTH and _spaceship.y <= self.y <= _spaceship.y + SPRITE_HEIGHT
        if colliding:
            _spaceship.health -= 1
        return colliding

def try_to_avoid_bullets(cpu, bullets):
    if len(bullets) > 0:
        temp = [bullet(cpu.x, 0, 1)] + bullets + [bullet(cpu.x, ROWS, 1)]
        for i in range(len(temp) - 1):
            dist_y = temp[i + 1].y - 1 - (temp[i].y + 1)
            if dist_y >= SPRITE_HEIGHT:
                return temp[i].y + 1
    return None

player = spaceship(5, random.randint(0, ROWS), [0x00, 0x00, 0xfc, 0x00, 0xfc, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0xe0, 0xff, 0xf0, 
                0xff, 0xf0, 0xff, 0xe0, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xfc, 0x00, 0xfc, 0x00, 0x00, 0x00])

cpu =  spaceship(COLS - 16 - 5, random.randint(0, ROWS), [0x00, 0x00, 0x00, 0x3f, 0x00, 0x3f, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x07, 0xff, 0x0f, 0xff, 
                0x0f, 0xff, 0x07, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0x3f, 0x00, 0x3f, 0x00, 0x00])

def enable_shoot(timer):
    global player
    global cpu
    cpu.can_shoot = True
    player.can_shoot = True
    
def update_pos(cpu, target):
    if cpu.y < target:
        cpu.y += 1
    elif cpu.y > target:
        cpu.y -= 1
        
def game_over_screen(win):
    oled.vline(0, 0, ROWS, 1)
    oled.vline(COLS, 0, ROWS, 1)
    oled.hline(0, 0, COLS, 1)
    oled.hline(0, ROWS, COLS, 1)
    if win:
        oled.text("GANASTE!!", (COLS - 81) // 2 , ROWS // 2)
    else:
        oled.text("PERDISTE", (COLS - 64)// 2 , ROWS // 2)
        

shoot_timer = machine.Timer()
shoot_timer.init(period=500, mode=machine.Timer.PERIODIC, callback=enable_shoot)

while True:
    if not game_over:
        player.y = (pot.read_u16() * 47) // 65535
        if shoot.value() and player.can_shoot:
            player.can_shoot = False
            player.bullets.append(bullet(player.x, player.y + 7, 1))
        if cpu.can_shoot:
            cpu.bullets.append(bullet(cpu.x, cpu.y + 7, 1))
            cpu.can_shoot = False
        for b in player.bullets:
            b.x += 1
        for b in cpu.bullets:
            b.x -= 1
        player.bullets = list(filter(lambda b: b.x <= COLS and not b.collide(cpu), player.bullets))
        cpu.bullets = list(filter(lambda b: b.x >= 0 and not b.collide(player), cpu.bullets))
        if cpu.health <= 0:
            game_over = True
            who_won = True
        elif player.health <= 0:
            game_over = True
            who_won = False
        safe_place = try_to_avoid_bullets(cpu, player.bullets)
        if safe_place:
            update_pos(cpu, safe_place)
        else:
            update_pos(cpu, player.y)
        oled.fill(0)
        oled.text(str(cpu.health), COLS // 2 + 1, 0)
        oled.text(str(player.health), COLS // 2 - 16, 0)
        player.draw(oled)
        cpu.draw(oled)
        oled.vline(COLS // 2, 0, ROWS, 1)
        oled.show()
    else:
        oled.fill(0)
        game_over_screen(who_won)
        oled.show()
        utime.sleep(2)
        if shoot.value():
            game_over = False
            player.y = random.randint(0, ROWS)
            cpu.y = random.randint(0, ROWS)
            cpu.health = 20
            player.health = 20
            
