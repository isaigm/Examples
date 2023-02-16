import machine
import time
 
sensor_temp = machine.ADC(4)
conversion_factor = 3.3 / (65535)


def _get_bit(n, k):
    return (n >> k) & 1

def _sleep_ms(ms):
    time.sleep(ms/1000)
    
class lcd16x2:
    def __init__(self, rs_pin, e_pin, rd4, rd5, rd6, rd7):
        self.data_pins = [machine.Pin(rd4, machine.Pin.OUT), machine.Pin(rd5, machine.Pin.OUT),
             machine.Pin(rd6, machine.Pin.OUT), machine.Pin(rd7, machine.Pin.OUT)]
        self.rs = machine.Pin(rs_pin, machine.Pin.OUT) 
        self.e = machine.Pin(e_pin, machine.Pin.OUT) 
        _sleep_ms(15)
        self.send_cmd(0x30)
        self.send_cmd(0x30)
        self.send_cmd(0x32)
        self.send_cmd(0x2C)
        self.send_cmd(0xC)
        self.send_cmd(0x6)
        self.send_cmd(0x1)

    def send_data(self, data):
        temp = data
        data = data >> 4
        for i in range(4):
            self.data_pins[i].value(_get_bit(data, i))
        _sleep_ms(2)
        self.e.value(1)
        _sleep_ms(1)
        self.e.value(0)
        temp = temp & 0xF
        for i in range(4):
            self.data_pins[i].value(_get_bit(temp, i))
        _sleep_ms(2)
        self.e.value(1)
        _sleep_ms(1)
        self.e.value(0)
        
    def send_cmd(self, cmd):
        self.rs.value(0)
        self.send_data(cmd)
        
    def send_character(self, ch, character=True):
        self.rs.value(1)
        if character:
            self.send_data(ord(ch))
        else:
            self.send_data(ch)
            
    def send_string(self, string):
        for ch in string:
            self.send_character(ch)
    
    def set_cursor_pos(self, x, y):
        if 0 <= x < 15:
            if y == 0:
                self.send_cmd(0x80 + x)
            if y == 1:
                self.send_cmd(0xC0 + x)
    
    def create_char(self, data):
        self.send_cmd(0x40)
        for byte in data:
            self.send_character(byte, character=False)
            
def main():      #rs e  rd4 rd5 rd6 rd7
    lcd = lcd16x2(2, 3, 12, 13, 14, 15)
    while True:
        lcd.send_cmd(0x1)
        lcd.send_string("--Temperatura--")
        reading = sensor_temp.read_u16() * conversion_factor 
        temperature = 27 - (reading - 0.706)/0.001721
        lcd.set_cursor_pos(3, 1)
        lcd.send_string(str(temperature))
        lcd.send_character(' ')
        lcd.send_character(0xDF, character=False)
        _sleep_ms(300)

if __name__ == "__main__":
    main()
