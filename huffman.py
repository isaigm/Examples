import collections

'''
El algoritmo de Huffman consiste primeramente en asociar a cada caracter
de nuestro texto a codificar/comprimir la frecuencia de este caracter con que aparece
en el mismo texto, asi un caracter mas frecuente tendra una codificacion en bits de menor
longitud que uno menos frecuente, con esto se logra reducir el tamaño en bits del texto con respecto
a la codificacion ASCII.
'''
codes = {}
inv_codes = {}
class node:
    def __init__(self, frq: int, ch: chr, left = None, right = None):
        self.frq = frq
        self.is_char = True
        self.ch = ch
        self.left = left
        self.right = right
        self.code = None

def encode(data):
    print(f"Longitud original: {len(data) * 8}")
    def gen_codes(root, codes, val):
        if root != None:
            if root.right == None and root.left == None:
                codes[root.ch] = val
                inv_codes[val] = root.ch
            gen_codes(root.left, codes, val + '0')
            gen_codes(root.right, codes, val + '1')
    
    frq_table = collections.Counter(data)
    nodes = [node(frq_table[ch], ch) for ch in frq_table]
    nodes.sort(reverse=False, key=lambda n: n.frq)
    while len(nodes) >= 2:
        n1 = nodes[0]
        n2 = nodes[1]
        del nodes[:2]
        nodes.append(node(n1.frq + n2.frq, '\0', n1, n2))
        nodes.sort(reverse=False, key=lambda n: n.frq)
    gen_codes(nodes[0], codes, '')

    return ''.join([codes [ch] for ch in data])

def decode(data):
    code = ''
    for bit in data:
        code += bit
        if code in inv_codes:
            print(inv_codes[code], end='')
            code = ''


ans = encode("hola mundo, esto es un ejemplo de texto")
print(f"Longitud Huffman: {len(ans)}")
decode(ans)
