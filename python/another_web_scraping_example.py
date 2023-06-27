import requests, json
from bs4 import BeautifulSoup

request = requests.get("https://www.youtube.com/watch?v=cFaf1_P2Y8c&list=PLgeh_RfSoZhK6FbqP33mXtI7gV2zvhGne&ab_channel=CesarAntonioIzquierdoMerlo")

soup = BeautifulSoup(request.content, 'html.parser')

scripts = soup.find_all("script")

def dfs(node):
    for (key, value) in node.items():
        if key == 'playlist':
            return (key, value)
        if isinstance(value, dict):
            res = dfs(value)
            if res != None:
                return res
    return None


for scr in scripts:
    text = str(scr)
    token = "var ytInitialData = {"
    if token in text:
        idx = text.find(token) + len(token)
        start_idx = idx - 1
        curly_brackets_counter = 1
        while curly_brackets_counter > 0 and idx < len(text):
            curr_character = text[idx]
            if curr_character == '{':
                curly_brackets_counter += 1
            elif curr_character == '}':
                curly_brackets_counter -= 1
            idx += 1
        objects = json.loads(text[start_idx: idx])
        
        k, v = dfs(objects)
        hours = 0
        minutes = 0
        secs = 0
        for item in v['playlist']['contents']:
            try:
                duration = item['playlistPanelVideoRenderer']['lengthText']['simpleText']
                duration = list(map(int, duration.split(':')))
                hour = 0
                if len(duration) == 3:
                    hour = duration[0]
                    minute  = duration[1]
                    sec  = duration[2]
                else:
                    minute = duration[0]
                    sec = duration[1]
                secs += sec
                
                minutes += minute
                minutes += secs // 60
                
                hours += hour
                hours += minutes // 60
                
                minutes %= 60
                secs %= 60
            except:
                pass
        print(f'the playlist has a total duration of {hours} hours, {minutes} minutes and {secs} seconds')
        break
