The program starts with the flags:
```
./hw-02_huffman [flags]
```
Where the flags are:
* `-c`: compression,
* `-u`: uncompress,
* `-f <path>`, `--file <path>`: input file name,
* `-o <path>`, `--output <path>`: name of resulting file. 

Flags can be specified in any order.

The program displays the compression/uncompressed statistics: the size of the original data, the size
the size of the original data and the size that has been used to store auxiliary data in the output
size of the input data and the size of the received data.
All sizes are in bytes.

For example:
```
$ ./huffman -c -f myfile.txt -o result.bin
15678
6172
482
```

Source file size (raw data): 15678 bytes, compressed data size (without additional
information): 6172 bytes, additional data size: 482 bytes. Total compressed file size: 6172 +
482 = 6654 bytes.

```
$ ./huffman -u -f result.bin -o myfile_new.txt
6172
15678
482
```

Unpacked file size (received data): 15678 bytes, compressed data size (without
additional information): 6172 bytes, additional data size: 482 bytes. The size of the total
of the original compressed file: 6172 + 482 = 6654 bytes.