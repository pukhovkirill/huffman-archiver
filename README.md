Описание
=====================

Простая реализация архиватора на основе алгоритма Хаффмана.

Сборка
=====================

Для сборки необходимо установленные пакеты CUnit, CUnit-devel, cmake, gcc.\
Приведу на примере сборки Fedora 41.

Последовательность сборки
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

``$ huffer -c foo bar -o archive.hff # create archivee.hff from files foo and bar``

``$ huffer -x archive.hff -o ./ # extract all files from archive.hff``

``$ huffer -l archive.hff # list all files in archive.hff``


Лицензия
=====================
Copyright (c) 2025 Kirill Pukhov\
Email: kirillpuhov526@gmail.com\
Распространяется по лицензии MIT License. Подробнее см. файл LICENSE.
