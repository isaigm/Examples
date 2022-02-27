class Node:
    def __init__(self, root, rz, cz, mat):
        self.root = root
        self.rz = rz
        self.cz = cz
        self.mat = mat
        
def copy(mat):
    return [r[:] for r in mat]

def chksum(mat):
    value = 0
    hash = 17
    for i in range(3):
        for j in range(3):
            value += (i - j) * 17 +  mat[i][j]
    return value

def nodes(rz, cz, curr, node):
    childs = []
    if rz + 1 < 3:
        mat = copy(curr)
        mat[rz][cz] = mat[rz + 1][cz]
        mat[rz + 1][cz] = 0
        childs.append(Node(node, rz + 1, cz, mat))
    if rz - 1 >= 0:
        mat = copy(curr)
        mat[rz][cz] = mat[rz - 1][cz]
        mat[rz - 1][cz] = 0
        childs.append(Node(node, rz - 1, cz, mat))
    if cz + 1 < 3:
        mat = copy(curr)
        mat[rz][cz] = mat[rz][cz + 1]
        mat[rz][cz + 1] = 0
        childs.append(Node(node, rz, cz + 1, mat))
    if cz - 1 >= 0:
        mat = copy(curr)
        mat[rz][cz] = mat[rz][cz - 1]
        mat[rz][cz - 1] = 0
        childs.append(Node(node, rz, cz - 1, mat))
    return childs

def equal(curr, dest):
    for i in range(3):
        for j in range(3):
            if curr[i][j] != dest[i][j]:
                return False
    return True

dest = [[1, 2, 3], [4, 5, 6], [7, 8, 0]]
curr = [[1, 8, 2], [0, 4, 3], [7, 6, 5]]
rz = 1
cz = 0
node = Node(None, rz, cz, curr)
visited = []

def is_in(mat, arr):
    for m in arr:
        if equal(mat, m): return True
    return False

def to_ascii(mat):
    for i in range(3):
        for j in range(3):
            print(mat[i][j], end=" ")
        print()
    print()
visited = []
MAX_DEPTH = 15
def solve(node, depth):
    if equal(node.mat, dest) and depth == 0: return True, node
    elif depth > 0:
        for child in nodes(node.rz, node.cz, node.mat, node):
            solv, res = solve(child, depth - 1)
            if solv : return True, res
    return False, None

def traverse(node):
    if node != None:
        traverse(node.root)
        to_ascii(node.mat)
        

sol, node = solve(node, MAX_DEPTH)

traverse(node)
