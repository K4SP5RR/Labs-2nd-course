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

        filename_output = os.path.splitext(filename_input)[0] + ".huff"
        with open(filename_output) as output_file:
            for char in text:
                output_file.write(self._codes[char])
                pass
        
    def decompress(self, input_file, output_file):
        pass

    def _frequency_analysis(self, text):
        dict = {}
        for i in text:
            if i in dict:
                dict[i] += 1
            else:
                dict[i] = 1
        return dict  

    def _build_tree(self, freq_tabl):
        
        tree = [Node(char, freq) for char, freq in freq_tabl]
        
        while len(tree) > 1:
            tree = sorted(tree, key=lambda x: x.freq)
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

    args = pars.parse_args()
    if args.compress:
        print(f"Сжатие файла {args.compress}")
    elif args.decompress:
        print(f"Распаковка файла {args.decompress}")
    else:
        pars.print_help()

if __name__ == "__main__":
    main()