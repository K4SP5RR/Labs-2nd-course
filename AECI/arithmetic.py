import argparse
import os
import sys

class Arithmetic:
    def __init__(self):
        self._codes = []

    def _frequency_analysis(self, text):
        dict = {}
        for i in text:
            if i in dict:
                dict[i] += 1
            else:
                dict[i] = 1
        return dict
    
    def _get_cumulative(self, dict):
        total = 0
        b = {}
        for i in dict:
            b[i] = [total, total + dict[i]]
            total += dict[i]
        return b, total
    
    def _normalize(self, left, right, bit_size=32):
        half = 1 << (bit_size - 1)
        quarter = 1 << (bit_size - 2)
        mask = (1 << bit_size) - 1
        inv = 0
        while True:
            if right < half:
                for _ in range(inv):
                    self._codes.append(1)
                inv = 0
                self._codes.append(0)
                left = (left << 1) & mask
                right = ((right << 1) & mask) +1
            elif left >= half:
                for _ in range(inv):
                    self._codes.append(0)
                inv = 0
                self._codes.append(1)
                left = (left << 1) & mask
                right = ((right << 1) & mask) |1
            elif left > quarter and right < 3*quarter:
                left -= quarter
                right -= quarter
                left *= 2
                right = right*2 + 1
                inv += 1
            else:
                return left, right

    def compress(self, filename_input):
        with open(filename_input, 'r', encoding='utf-8') as file_input:
            text = file_input.read()
        
        freq = self._frequency_analysis(text)
        tabl, total = self._get_cumulative(freq)
        left = 0
        right = (1 << 32) - 1
        for i in text:
            m, n = tabl[i]
            len = right - left + 1
            new_left = left + len * m // total
            new_right = left + len * n // total - 1
            left, right = self._normalize(new_left, new_right, 32)            
        
        filename_output = os.path.splitext(filename_input)[0] + ".arithm"
        with open(filename_output, 'wb') as file_output:
            file_output.write()
        return file_output

def main():
    pars = argparse.ArgumentParser(prog="arithmetic.py",
                                   description="Программа для арифметического сжатия файлов",
                                   epilog='Пример: python arithmetic.py -c file.txt')
    pars.add_argument('-c', '--compress', help='Сжать файл')
    pars.add_argument('-d', '--decompress', help='Распаковать файл')
    pars.add_argument('-v', '--verify', help='Проверить целостность файлов после сжатия и распаковки')

    args = pars.parse_args()

    if args.compress:
        print(f"Сжатие файла {args.compress}")
        ans = Arithmetic()
        try:
            ans.compress(args.compress)
        except FileNotFoundError:
            print("Файл не найден")
        print("Файл успешно сжат")
    elif args.decompress:
        print(f"Распаковка файла {args.decompress}")
        ans = Arithmetic()
        try:
            ans.decompress(args.decompress)
        except FileNotFoundError:
            print("Файл не найден")
        print("Файл успешно распакован")
    elif args.verify:
        print(f"Проверка целостности файла {args.verify}")
        ans = Arithmetic()
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
