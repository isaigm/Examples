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
        self.ch = ch
        self.left = left
        self.right = right
        self.code = None

#esto regresa la serie de bits que representa la codificacion huffman del texto data
def encode(data):
    print(f"Se usaron {len(data)} bytes en la codificacion ASCII")
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


def convert_to_bits(n: int):
    bits = ""
    for _ in range(8):
        bit = n & 1
        if bit:
            bits += '1'
        else:
            bits += '0'
        n >>= 1
    return bits[::-1]

ans = encode("El fisico estadounidense de origen italiano Enrico Fermi logro realizar la fision en 1934, pero la reaccion no se reconocio como tal hasta 1939, cuando los cientificos alemanes Otto Hahn y Fritz Strassmann anunciaron que habian fisionado nucleos de uranio bombardeandolos con neutrones. Esta reaccion libera a su vez neutrones, con lo que se puede causar una reaccion en cadena con otros nucleos.")

output_file = open("encoded.dat", "wb")
offset = 0
if len(ans) % 8 != 0: #dado que la longitud de la serie de bits que se obtiene aplicando la codificacion huffman puede no ser multiplo de 8
    # y ya que no podemos escribir, por ejemplo, 17 bits en un archivo, a fuerza tienen que ser bytes, entonces necesitamos encontrar la cantidad de bits
    #faltantes para que la longitud del bitstream sea multiplo 8 y de esta forma ir tomando 8 bits del bitstream, convertirlo a byte y esto escribirlo en el archivo
    offset = ((len(ans) // 8) + 1) * 8 - len(ans) #con esta formula encontramos lo que nos falta para ser multiplo de 8, la idea es encontrar el multiplo de 8 mas cercano por arriba a len(ans)
    ans = '0' * offset + ans
print(f"se usaron {len(ans)//8 + 1} bytes con la codificacion huffman")
output_file.write(offset.to_bytes(1, byteorder='big'))
j = 0
for _ in range(len(ans) // 8):
    byte = int(ans[j: j + 8], 2)
    output_file.write(byte.to_bytes(1, byteorder='big'))
    j += 8

output_file.close()


input_file = open("encoded.dat", "rb")

offset = int.from_bytes(input_file.read(1), byteorder='big')
data = ""

while True:
    byte = input_file.read(1)
    if byte == b'':
        break
    data += convert_to_bits(int.from_bytes(byte, byteorder='big'))

data = data[offset:] #offset es la cantidad de bits de relleno, de ahi en adelante esta todo el texto codificicado en bits

decode(data)
