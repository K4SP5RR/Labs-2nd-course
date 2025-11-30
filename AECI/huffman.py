import argparse
import os

class Node:
    def __init__(self, char, freq, left=None, right=None):
        self.char = char
        self.freq = freq
        self.left = left
        self.right = right

class Huffman:
    def __init__(self):
        self._codes = {}
    
    def compress(self, filename_input):
        with open(filename_input, 'r', encoding='utf-8') as input_file:
            text = input_file.read()
        
        freq_tabl = self._frequency_analysis(text)
        tree = self._build_tree(freq_tabl)
        self._generate_codes(tree, "")

        encode_bits = ''.join(self._codes[char] for char in text)
        ser_tree = self._serialize_tree(tree)

        data = self._bits_to_bytes(ser_tree+encode_bits)
        filename_output = os.path.splitext(filename_input)[0] + ".huff"

        with open(filename_output, 'wb') as output_file:
            output_file.write(data)
        return filename_output
            
        
    def decompress(self, filename_input):
        with open(filename_input, 'rb') as input_file:
            bytes = input_file.read()
        bits = self._bytes_to_bits(bytes)
        root, text = self._deserialize_tree(bits)
        result = ''
        tree = root
        for i in range(len(text)):
            if text[i] == '0':
                tree = tree.left
            else:
                tree = tree.right
            if tree.char is not None:
                result += tree.char
                tree = root
        filename_output = os.path.splitext(filename_input)[0] + "_decompressed.txt"
        with open(filename_output, 'w', encoding='utf-8') as output_file:
            output_file.write(result)
        return filename_output

    def _bits_to_bytes(self, bits):
        bytes = bytearray()
        pad = 8 - (len(bits)%8)
        if pad == 8:
            pad = 0
        
        bits += '0'*pad
        bytes.append(pad)

        for bit in range(0, len(bits), 8):
            b = bits[bit:bit+8]
            bytes.append(int(b,2))
        return bytes

    def _bytes_to_bits(self, bytes):
        bits=''
        pad = bytes[0]
        for byte in bytes[1:]:
            bits += format(byte, '08b')
        if pad > 0: bits = bits[:-pad]
        return bits
    
    def _deserialize_tree(self, ser_tree):
        if ser_tree == "":
            return None, ""
        
        if ser_tree[0] == '1':
            char_bits = ser_tree[1:17]
            char = chr(int(char_bits,2))
            return Node(char, 0), ser_tree[17:]
        
        elif ser_tree[0] == '0':
            left_node, ost = self._deserialize_tree(ser_tree[1:])
            right_node, fost = self._deserialize_tree(ost)
            return Node(None, 0, left_node, right_node), fost
    
    def _serialize_tree(self, node):
        if node is None:
            return ""
        
        if node.char is not None:
            char_code = ord(node.char)
            char_bits = format(char_code, '016b')
            return '1'+char_bits
        
        return '0' + self._serialize_tree(node.left) + self._serialize_tree(node.right)


    def _frequency_analysis(self, text):
        dict = {}
        for i in text:
            if i in dict:
                dict[i] += 1
            else:
                dict[i] = 1
        return dict  

    def _build_tree(self, freq_tabl):
        
        tree = [Node(char, freq_tabl[char]) for char in freq_tabl]
        
        while len(tree) > 1:
            tree.sort(key=lambda x: x.freq)
            node1 = tree.pop(0)
            node2 = tree.pop(0)
            parents = Node(None, node1.freq+node2.freq, node1, node2)
            
            tree.append(parents)
        
        return tree[0]
    
    def _generate_codes(self, node, code):
        
        if node is None:
            return

        if node.char is not None:
            self._codes[node.char] = code
            return 
        
        self._generate_codes(node.left, code + '0')
        self._generate_codes(node.right, code + '1')


def main():
    pars = argparse.ArgumentParser(prog='huffman.py',
                                   description='Программа для сжатия и файлов методом Хаффмана',
                                   epilog='Пример: python huffman.py -c file.txt')
    pars.add_argument('-c', '--compress', help='Сжать файл')
    pars.add_argument('-d', '--decompress', help='Распаковать файл')
    pars.add_argument('-v', '--verify', help='Проверить целостность файлов после сжатия и распаковки')

    args = pars.parse_args()
    if args.compress:
        print(f"Сжатие файла {args.compress}")
        ans = Huffman()
        try:
            ans.compress(args.compress)
        except FileNotFoundError:
            print("Файл не найден")
        print("Файл успешно сжат")
    elif args.decompress:
        print(f"Распаковка файла {args.decompress}")
        ans = Huffman()
        try:
            ans.decompress(args.decompress)
        except FileNotFoundError:
            print("Файл не найден")
        print("Файл успешно распакован")
    elif args.verify:
        print(f"Проверка целостности файла {args.verify}")
        ans = Huffman()
        try:
            compressed_file = ans.compress(args.verify)
            decompressed_file = ans.decompress(compressed_file)
            with open(args.verify, 'r', encoding='utf-8') as original, open(decompressed_file, 'r', encoding='utf-8') as decompressed:
                if original.read() == decompressed.read():
                    print("Файлы идентичны")
                else:
                    print("Файлы не идентичны")
        except FileNotFoundError:
            print("Файл не найден")
    else:
        pars.print_help()

if __name__ == "__main__":
    main()