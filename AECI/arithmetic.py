import argparse
import os
import sys

class Arithmetic:
    def __init__(self):
        self._codes = []
        self._inv_bits = 0

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
        while True:
            if right < half: #1 случай когда интервал в левой части
                for _ in range(self._inv_bits):
                    self._codes.append(1)
                self._inv_bits = 0
                self._codes.append(0)
                left = (left << 1) & mask
                right = ((right << 1) & mask) |1
            elif left >= half: #2 случай когда интервал в правой части 
                for _ in range(self._inv_bits):
                    self._codes.append(0)
                self._inv_bits = 0
                self._codes.append(1)
                left = (left << 1) & mask
                right = ((right << 1) & mask) |1
            elif left > quarter and right < 3*quarter: #3 случай когда интервал близко к середине
                left -= quarter
                right -= quarter
                left *= 2
                right = right*2 + 1
                self._inv_bits += 1
            else:
                return left, right

    def _finished_encode(self,left,bit_size=32):
        if self._inv_bits > 0:
            quarter = 1 << (bit_size-2)
            if left < quarter:
                for _ in range(self._inv_bits):
                    self._codes.append(1)
                self._codes.append(0)
            else:
                for _ in range(self._inv_bits):
                    self._codes.append(0)
                self._codes.append(1)  
            self._inv_bits = 0 
        
        for i in range(bit_size - 1, -1, -1):
            bit = (left >> i) & 1
            self._codes.append(bit)
    
    def _bits_to_bytes(self, bits):
        res = bytearray()
        bit_count = len(bits)
        res.extend(bit_count.to_bytes(4,'big'))
        byte_count = (bit_count + 7) // 8
        for i in range(byte_count):
            byte = 0
            for j in range(8):
                ind = i*8+j
                if(ind < bit_count):
                    byte = byte << 1 | bits[ind]
                else:
                    byte <<= 1
            res.append(byte)
        return bytes(res)

    def _bytes_to_bits(self, bytes):
        if len(bytes) < 4: return []
        bit_count = int.from_bytes(bytes[:4], 'big')
        bytes = bytes[4:]
        bits = []
        for i in range(len(bytes)):
            low = -1
            if bit_count < (i+1)*8:
                low = (i+1)*8 - bit_count - 1
            for j in range(7,low,-1):
                bit = bytes[i] >> j &1
                bits.append(bit)
        return bits

    def _encode_freq_table(self,tabl):
        res = bytearray()
        res.extend(len(tabl).to_bytes(8,'big'))
        for char, freq in tabl.items():
            char_bytes = char.encode('utf-8')
            char_len = len(char_bytes)
            res.extend(char_len.to_bytes(2,'big'))
            res.extend(char_bytes)
            res.extend(freq.to_bytes(4,'big'))
        return bytes(res)

    def _decode_freq_table(self, data):
        len_tabl = int.from_bytes(data[:8],'big')
        data = data[8:]
        tabl = {}
        pos = 0
        for _ in range(len_tabl):
            char_len = int.from_bytes(data[pos:pos+2], 'big')
            pos+=2
            char = data[pos:pos+char_len].decode('utf-8')
            pos += char_len
            freq = int.from_bytes(data[pos:pos+4],'big')
            pos += 4
            tabl[char] = freq
        return tabl
            
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
            left, right = self._normalize(new_left, new_right)
        self._finished_encode(left)


        data = self._encode_freq_table(freq)
        data += self._bits_to_bytes(self._codes)

        filename_output = os.path.splitext(filename_input)[0] + ".arithm"
        with open(filename_output, 'wb') as file_output:
            file_output.write(data)
        return file_output

def main():
    print("Модификация с 3 случаем")
    a = Arithmetic()
    steps = [
        ("K", 19660, 32767),  # Строка 1: после символа K
        ("O", 13104, 28832),  # Строка 2: после O
        ("B", 41937, 48227),  # Строка 3: после B
        (".", 53111, 58143),  # Строка 4: после .
        ("K", 21875, 25901),  # Строка 5: после второго K
        ("O", 21964, 26795),  # Строка 6: после второго O
    ]
    for i in steps:
        l,r = a._normalize(i[1],i[2],16)
        print(f"{i} - {l,r}, code: {a._codes}")
    a._finished_encode(l,16)
    print(f'final: {a._codes}')

if __name__ == "__main__":
    main()
