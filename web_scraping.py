from bs4 import BeautifulSoup
from types import SimpleNamespace
import requests, json
from stockfish import Stockfish
#Ejemplo de lo que pueden hacer los tramposos XD
stockfish = Stockfish('stockfish', parameters={"Threads": 4, "Minimum Thinking Time": 30})
stockfish.set_depth(15)

while True:
    r = requests.get("https://lichess.org/KuXPWqE0bK55")
    soup = BeautifulSoup(r.content, 'html.parser')
    scripts = soup.select('script')
    game = scripts[-1]
    game = game.contents[0][41:-3]
    x = json.loads(game, object_hook=lambda d: SimpleNamespace(**d))
    stockfish.set_fen_position(x.data.game.fen)
    print("Best move: " + stockfish.get_best_move(), end='')
    next_move = input()
