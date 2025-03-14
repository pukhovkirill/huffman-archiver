Описание
=====================

Простая реализация архиватора на основе алгоритма Хаффмана.

Архитектура проекта построена вокруг реализации оптимального
префиксного кодирования Хаффмана.

Алгоритм построения кодов следующий:
1. Построить таблицу частотности символов загруженных файлов.\
   Time complexity: O(n);

2. Составить приоритетную очередь { частотность : символ }.\
   Приоритетная очередь представляет собой двоичную кучу (heap),
   на вершине которой всегда хранится символ с наименьшей
   частотностью.\
   Бинарная куча построена на основе массива.\
   Time complexity: O(n);\
   https://ru.algorithmica.org/cs/basic-structures/heap/

3. Составить дерево Хаффмана.\
   Дерево строится по следующему методу:
   1. Упорядочиваем символы исходного алфавита в порядке невозрастания их частотности. (Выполнено во втором пункте).
   2. Объединяем два символа с наименьшими вероятностями.\
      Символу с большей вероятностью приписываем "1", символу с
      меньшей — "0" в качестве элементов их кодов. Если вероятности совпадают то первому символу приписывается "0", a второму "1".
   3. Считаем объединение символов за один символ с вероятностью, равной сумме вероятностей объединённых символов.
   4. Возвращаемся на Шаг 2 до тех пор, пока все символы не будут объединены в один.
   
4. Составить таблицу кодов.\
   Для этого применяется итеративный обход в глубину (с помощью стека)

Кодирование файла с помощью таблицы кодов.\
Файл считывается побайтно и каждый считанный символ заменяется 
соответствующей строкой битового кода.\
Битовые представления записываются следующим образом:
```
char code[] = codes[ch];             <- Получаем битовое представление символа ch;
for(int i = 0; i < len(code); i++){  <- Перебираем каждый бит кодировки символа;
   if(code[i] == '1')                <- Если текущий бит равен '1',
       sum |= bit;                   <- добавляем его в переменную sum
   if(bit < 128) {
       bit <<= 1;                    <- Смещаем битовый маркер влево: bit = 1, 2, 4, ..., 128;
   }else if(next_byte < BLK_SIZE) {  <- Если достигли 8 бит (1 байт) и в блоке есть место;
       data[next_byte++] = sum;      <- Записываем накопленный байт в текущий блок;
       sum = 0;                      <- Сбрасываем sum для следующего набора бит;
       bit = 1;                      <- Возвращаем bit в начальное положение;
   }
}
```

Декодирование файла с помощью дерева хаффмана.\
Файл считывается побайтно, и каждый считанный байт декодируется в соответствии с деревом хаффмана.
Обход дерева начинается с корня. В байте читается каждый бит, и в зависимости от значения бита выбирается левая или правая ветка.\
Т.о. алгоритм доходит до узла в котором нет потомков (т.е. до листа), считывает символ и записывает в буфер.
```
uint8_t byte = data[j];                   <- Получаем текущий байт;
for(int bit = 1; bit <= 128; bit <<= 1) { <- Перебираем каждый бит в текущем байте;
    if((byte & bit) == 0) {               <- Если значение бита равно нулю, то
        tree = tree->bit0;                <- идём в правую ветку;
    }else {                               <- В противном случае,
        tree = tree->bit1;                <- идём в левую ветку;
    }

    if(tree->bit0 == NULL) {              <- Если достигнут листовой узел дерева (символ найден),
        bytes[offst++] = tree->character; <- записываем декодированный символ в буфер;
        tree = root;                      <- Возвращаемся в корень дерева для следующего символа;
    }
}
```

Структура файла:

```
0         1         2         3         4         5         6         7         8
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|       Magic       |  Version number   |  Flags  | Frq Len |       CRC-16      |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                                                                               |
~                             Frequency Table (1 Kb)                            ~
|                                                                               |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                                                                               |
~                                File Name (100 b)                              ~
|                                                                               |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|              Mode (4 b)               |           Owner ID (4 b)              |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|              Group ID (4 b)           |      Modification Time (4 b)          |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                         +  Modification Time (8 b)                            |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|       +  Modification Time (4 b)      |           File Size (4 b)             |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|           +  File Size (8 b)          |           Block Count(4 b)            |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|          +  Block Count (4 b)         | Tail Length (2 b) |   CRC-16 (2 b)    |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                                                                               |
~                                 BLOCK 0 (512 b)                               ~
|                                                                               |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                                                                               |
~                                 BLOCK 1 (512 b)                               ~
|                                                                               |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

                                      * * *
                                      
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+                                   
|                                                                               |
~                                 BLOCK N (512 b)                               ~
|                                                                               |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                                                                               |
~                                  Next File                                    ~
|                                                                               |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

                                      * * *
```

Флаги:
```
F_FILE_ATTRIBUTES_STORED <- сохранить атрибуты файлов
F_CRC_FILES_EXISTS       <- посчитать checksum для каждого файла
F_CRC_HEADER_EXISTS      <- посчитать checksum для заголовка архива
```

По умолчанию все флаги включены.

Ключи:
```
Ключи:
  -c, --create=<file1> <file2> ...  Создать новый архив из файлов <file1>,<file2>,...
  -h, --help                        Показать справку
  -l, --list=<file>                 Вывести содержимое архива <file>
  -o, --output=<file>               Сохранить вывод в <file>.
                                    Может быть файлом или каталогом  
  -v, --version                     Показать информацию о версии архиватора
  -x, --extract=<achv1> <achv2> ... Извлечь файлы из архивов <achv1>,<achv2>,...

Модификаторы:
  --no-crc-files-check              Не проверять контрольную сумму файлов
  --no-crc-header-check             Не проверять контрольную сумму архива
  --no-preserve-file-attributes     Не сохранять атрибуты файлов
```

Сборка
=====================

Для сборки необходимо установить пакеты CUnit, CUnit-devel, cmake, gcc.\
Приведу на примере сборки Fedora 41.

Процесс сборки
=====================
```
$ dnf install CUnit CUnit-devel cmake gcc git

$ git clone https://github.com/pukhovkirill/huffman-archiver.git

$ cd huffman-archiver/build

$ cmake ..

$ sudo make install
```

Примеры использования
=====================

``$ huffer -c foo bar -o archive.hff # create archive.hff from files foo and bar``

``$ huffer -x archive.hff -o ./      # extract all files from archive.hff``

``$ huffer -l archive.hff            # list all files in archive.hff``


Лицензия
=====================
Copyright (c) 2025 Kirill Pukhov\
Email: kirillpuhov526@gmail.com\
Распространяется по лицензии MIT License. Подробнее см. файл LICENSE.
