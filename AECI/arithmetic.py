import argparse
import os
import time
from collections import Counter

class Arithmetic:
    def __init__(self):
        self._codes = []
        self._inv_bits = 0
        self.MAX_VALUE = 65535          
        self.FIRST_QTR = (self.MAX_VALUE + 1) // 4
        self.HALF = self.FIRST_QTR * 2
        self.THIRD_QTR = self.FIRST_QTR * 3

    def _frequency_analysis(self, data):
        freq = Counter(data)
        res = []
        for i in range(256):
            res.append(freq.get(i, 0) or 1)
        return res
    
    def _get_cumulative(self, freq):
        total = 0
        tabl = []
        for count in freq:
            tabl.append(total)
            total += count
        tabl.append(total)
        return tabl, total
    
    def _normalize(self, left, right):
        while True:
            if right < self.HALF:
                self._codes.append(0)
                for _ in range(self._inv_bits):
                    self._codes.append(1)
                self._inv_bits = 0
            elif left >= self.HALF:
                self._codes.append(1)
                for _ in range(self._inv_bits):
                    self._codes.append(0)
                self._inv_bits = 0
                left -= self.HALF
                right -= self.HALF
            elif left >= self.FIRST_QTR and right < self.THIRD_QTR:
                self._inv_bits += 1
                left -= self.FIRST_QTR
                right -= self.FIRST_QTR
            else:
                break
            
            left = (left << 1) & self.MAX_VALUE
            right = ((right << 1) | 1) & self.MAX_VALUE
        return left, right

    def _finished_encode(self, left):
        self._inv_bits += 1
        if left < self.FIRST_QTR:
            self._codes.append(0)
            for _ in range(self._inv_bits):
                self._codes.append(1)
        else:
            self._codes.append(1)
            for _ in range(self._inv_bits):
                self._codes.append(0)

    def _bits_to_bytes(self, bits):
        res = bytearray()
        bit_count = len(bits)
        res.extend(bit_count.to_bytes(4, 'big'))
        
        byte = 0
        for i, bit in enumerate(bits):
            byte = (byte << 1) | bit
            if (i + 1) % 8 == 0:
                res.append(byte)
                byte = 0
        if len(bits) % 8 != 0:
            res.append(byte << (8 - (len(bits) % 8)))
        return res

    def _bytes_to_bits(self, data):
        if len(data) < 4: return []
        bit_count = int.from_bytes(data[:4], 'big')
        data = data[4:]
        bits = []
        for byte in data:
            for i in range(7, -1, -1):
                bits.append((byte >> i) & 1)
        return bits[:bit_count]

    def compress(self, filename_input):
        start_time = time.time()
        
        with open(filename_input, 'rb') as f:
            data = f.read()
        
        input_size = len(data)
        freq = self._frequency_analysis(data)
        tabl, total = self._get_cumulative(freq)
        
        left, right = 0, self.MAX_VALUE
        
        for byte in data:
            range_len = right - left + 1
            new_left = left + (range_len * tabl[byte]) // total
            right = left + (range_len * tabl[byte + 1]) // total - 1
            left = new_left
            left, right = self._normalize(left, right)
        
        self._finished_encode(left)
        
        result = bytearray()
        result.extend(input_size.to_bytes(4, 'big'))
        for f in freq:
            result.extend(f.to_bytes(4, 'big'))
        result.extend(self._bits_to_bytes(self._codes))
        
        output_name = os.path.splitext(filename_input)[0] + ".arithm"
        with open(output_name, 'wb') as f:
            f.write(result)
        
        end_time = time.time()
        output_size = len(result)
        
        duration = end_time - start_time
        ratio_percent = (output_size / input_size * 100) if input_size > 0 else 0
        ratio_factor = (input_size / output_size) if output_size > 0 else 0
        
        print(f"\nСтатистика сжатия")
        print(f"Время: {duration:.4f} сек")
        print(f"Входной файл: {input_size} байт")
        print(f"Сжатый файл:  {output_size} байт")
        print(f"Эффективность: {ratio_percent:.2f}% от оригинала ({ratio_factor:.2f}x)")
        
        return output_name
    
    def decompress(self, filename_input):
        start_time = time.time()
        
        with open(filename_input, 'rb') as f:
            data = f.read()
        
        text_length = int.from_bytes(data[:4], 'big')
        freq = []
        for i in range(256):
            freq.append(int.from_bytes(data[4 + i*4 : 4 + (i+1)*4], 'big'))
        
        tabl, total = self._get_cumulative(freq)
        bits = self._bytes_to_bits(data[4 + 256*4:])
        
        bit_pos = 0
        def read_bit():
            nonlocal bit_pos
            if bit_pos < len(bits):
                bit = bits[bit_pos]
                bit_pos += 1
                return bit
            return 0

        left, right, value = 0, self.MAX_VALUE, 0
        for _ in range(16):
            value = (value << 1) | read_bit()

        result = bytearray()
        for _ in range(text_length):
            range_len = right - left + 1
            freq_val = ((value - left + 1) * total - 1) // range_len
            
            c = 0
            while tabl[c + 1] <= freq_val:
                c += 1
            
            result.append(c)
            
            new_left = left + (range_len * tabl[c]) // total
            right = left + (range_len * tabl[c + 1]) // total - 1
            left = new_left
            
            while True:
                if right < self.HALF:
                    pass
                elif left >= self.HALF:
                    left -= self.HALF
                    right -= self.HALF
                    value -= self.HALF
                elif left >= self.FIRST_QTR and right < self.THIRD_QTR:
                    left -= self.FIRST_QTR
                    right -= self.FIRST_QTR
                    value -= self.FIRST_QTR
                else:
                    break
                left = (left << 1) & self.MAX_VALUE
                right = ((right << 1) | 1) & self.MAX_VALUE
                value = ((value << 1) | read_bit()) & self.MAX_VALUE
        
        output_name = os.path.splitext(filename_input)[0] + "_decompressed.txt"
        with open(output_name, 'wb') as f:
            f.write(result)
            
        end_time = time.time()
        print(f"Время: {end_time - start_time:.4f} сек")
        print(f"Восстановлено байт: {len(result)}")
        
        return output_name

def main():
    parser = argparse.ArgumentParser(prog="arithmetic.py")
    parser.add_argument("-c", "--compress", help="Сжать файл")
    parser.add_argument("-d", "--decompress", help="Распаковать файл")
    parser.add_argument("-v", "--verify", help="Проверить целостность")
    args = parser.parse_args()

    ar = Arithmetic()

    if args.compress:
        print(f"Сжатие {args.compress}")
        ar.compress(args.compress)
    elif args.decompress:
        print(f"Распаковка {args.decompress}")
        ar.decompress(args.decompress)
    elif args.verify:
        print(f"Запуск полной проверки для {args.verify}")
        comp = ar.compress(args.verify)
        decomp = ar.decompress(comp)
        with open(args.verify, "rb") as f1, open(decomp, "rb") as f2:
            if f1.read() == f2.read():
                print("\nФайлы идентичны")
            else:
                print("\nФайлы различаются.")
    else:
        parser.print_help()

if __name__ == "__main__":
    main()