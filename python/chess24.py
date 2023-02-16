import requests
import json
import chess
import chess.pgn
import sys
import argparse
from bs4 import BeautifulSoup
from itertools import takewhile


def game_to_pgn(base_url):
    r = requests.get(base_url)
    soup = BeautifulSoup(r.text, 'html.parser')
    all_scripts = soup.find_all('script')
    res = all_scripts[-1].__str__()[146:]
    moves = ''.join(takewhile(lambda x: x != ')', res))
    dic = json.loads(moves)
    moves = dic['chessGame']['moves'][1:]
    result = dic['chessGame']['gameStatus']['result']
    game = chess.pgn.Game()
    game.headers["Event"] = "Example"
    game.headers["White"] = "Anon"
    game.headers["Black"] = "Anon"
    node = game
    for move in moves:
        m = chess.Move.from_uci(move['move'])
        node = node.add_variation(m)
    if result['white'] == 1:
        game.headers["Result"] = "1-0"
    elif result['black'] == 1:
        game.headers["Result"] = "0-1"
    else:
        game.headers["Result"] = "1/2-1/2"
    print(game)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description='Get pgn of chess game played in Chess24.')
    parser.add_argument("url", help="the url of the game")
    args = parser.parse_args()
    if args.url:
        game_to_pgn(args.url)
    else:
        print(args.echo)
