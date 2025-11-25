import argparse

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