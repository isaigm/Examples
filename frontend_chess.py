import pygame as py
import sys
import os
from pygame.locals import *
from stockfish import Stockfish
import chess

py.init()
py.mixer.quit()


class Cell(py.Rect):
    def __init__(self, x, y, square, name):
        py.Rect.__init__(self, x, y, 50, 50)
        self.square = square
        self.color = (0, 0, 0)
        self.name = name


def convert_ascii_to_cells(ascii_board: str, cells: [Cell]):
    idx = 0
    for row in ascii_board:
        for cell in row:
            cells[idx].square = conversions[cell]
            idx += 1


def select_cell(mouse_pos, cells: [Cell]):
    for c in cells:
        if c.collidepoint(mouse_pos):
            return c.name
    return ''


BLACK_PAWN = 0
WHITE_PAWN = 1
BLACK_BISHOP = 2
WHITE_BISHOP = 3
BLACK_KNIGHT = 4
WHITE_KNIGHT = 5
BLACK_ROOK = 6
WHITE_ROOK = 7
BLACK_QUEEN = 8
WHITE_QUEEN = 10
BLACK_KING = 11
WHITE_KING = 12
EMPTY = -1

WIDTH = 800
HEIGHT = 600
conversions = {'r': BLACK_ROOK, 'R': WHITE_ROOK, 'n': BLACK_KNIGHT, 'N': WHITE_KNIGHT,
               'b': BLACK_BISHOP, 'B': WHITE_BISHOP, 'q': BLACK_QUEEN, 'Q': WHITE_QUEEN,
               'k': BLACK_KING, 'K': WHITE_KING, 'p': BLACK_PAWN, 'P': WHITE_PAWN, '.': EMPTY}
idx = {BLACK_ROOK: (1, 4), WHITE_ROOK: (0, 4), BLACK_KNIGHT: (1, 3), WHITE_KNIGHT: (0, 3),
       BLACK_BISHOP: (1, 2), WHITE_BISHOP: (0, 2), BLACK_QUEEN: (1, 1), WHITE_QUEEN: (0, 1),
       BLACK_KING: (1, 0), WHITE_KING: (0, 0), BLACK_PAWN: (1, 5), WHITE_PAWN: (0, 5)}
board = chess.Board()
ascii_board = board.__str__().split('\n')
ascii_board = [row.replace(' ', '') for row in ascii_board]
window = py.display.set_mode([WIDTH, HEIGHT])
py.display.set_caption("Frontend chess")
clock = py.time.Clock()
pieces = py.image.load(os.path.join("pieces.svg"))
surf = py.Surface((45, 45), py.SRCALPHA)
cells = []
valid_move = False
start_pos = ''
end_pos = ''
stockfish = Stockfish()
r = 7
c = 0
milis = 0
white_turn = True
black_turn = False
for i in range(8):
    for j in range(8):
        cell = Cell((WIDTH - (50*8))/2 + j*50,
                    (HEIGHT - (50*8))/2 + i*50, EMPTY, chess.SQUARE_NAMES[r * 8 + c])
        if i % 2 == 0:
            if j % 2 == 0:
                cell.color = (89, 147, 93)
            else:
                cell.color = (241, 246, 178)
        else:
            if j % 2 == 0:
                cell.color = (241, 246, 178)
            else:
                cell.color = (89, 147, 93)
        c += 1
        if c > 7:
            c = 0
            r -= 1
        cells.append(cell)
convert_ascii_to_cells(ascii_board, cells)
while True:
    if milis >= 2000:
        stockfish.set_fen_position(board.board_fen())
        board.push(chess.Move.from_uci(stockfish.get_best_move()))
        ascii_board = board.__str__().split('\n')
        ascii_board = [row.replace(' ', '') for row in ascii_board]
        convert_ascii_to_cells(ascii_board, cells)
        black_turn = False
        white_turn = True
        milis = 0
    for e in py.event.get():
        if e.type == QUIT:
            py.quit()
            sys.exit(0)
        elif e.type == py.MOUSEBUTTONDOWN:
            if py.mouse.get_pressed()[0]:
                pos = select_cell(py.mouse.get_pos(), cells)
                if pos != '':
                    if start_pos == '':
                        start_pos = pos
                    else:
                        end_pos = pos
                        move = chess.Move(chess.parse_square(start_pos), chess.parse_square(end_pos))
                        valid_move = board.is_legal(move)
                        if valid_move and white_turn:
                            print('Outstanding move')
                            board.push(move)
                            ascii_board = board.__str__().split('\n')
                            ascii_board = [row.replace(' ', '') for row in ascii_board]
                            convert_ascii_to_cells(ascii_board, cells)
                            white_turn = False
                            black_turn = True
                            milis = 0
                        end_pos = ''
                        start_pos = ''
    window.fill((255, 255, 255))
    for c in cells:
        py.draw.rect(window, c.color, (c.x, c.y, c.w, c.h), 0)
        if c.square != EMPTY:
            row, col = idx[c.square]
            surf.fill((0, 0, 0, 0))
            surf.blit(pieces, (0, 0), (45 * col, 45 * row, 45, 45))
            window.blit(surf, (c.x + 2, c.y + 2))
    py.display.flip()
    dt = clock.tick(60)
    if black_turn:
        milis += dt
