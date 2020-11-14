# Protocol Program
* 重寫部份網路通訊協定範例程式並加上註解   
* 提供少許必要之C語言、Linux知識，以便了解程式運作

## 程式執行環境
* OS : Linux - Ubuntu 20.04
* C Compiler : gcc


## C 語言
### 真正的 main function
```c
int main(void);
int main(int argc, char **argv);
```
* 對於一個走在「正式C語言規格」上的主程式來說，main 理應宣告成上述兩種形式其中一種，才是一個正式的C語言程式

### 終端機呼叫程式
```
$ gcc -o main main.c
$ ./main 123 para
```
* 以終端機執行指令時，開頭的第一個單詞即是「程式名字」，
如上述 gcc, main 等
    * 在Linux中，如果是存在於 /bin 目錄下的程式，直接在終端機上打上程式名字，即可執行程式 (gcc 即此類)
    * 若是執行非系統程式，需使用 ./ + 程式名字。以上述 ./main 為例，表示執行終端機「當前目錄」下，程式名字為 main 的程式
* 第一個單詞(程式名字)打完後，若在後面，以空白隔開，再打上其它東西，就都叫做「參數」，參數會傳給第一個單詞所呼叫程式
    * 將參數傳給呼叫的程式是怎麼運作的？ 這之中的過程就是OS會幫我們處理 (~~當然是我懶的講，也還不會：|~~)
    * 而附加的參數，通通會被當成「字串」，傳給呼叫的程式
    * 因此，使用 ```int main(int argc, char **argv)```時，就是為了接收傳遞的參數
        * argc為參數的「數量」，意涵是 argument count
        * argv儲存所有的參數(字串形式)，意涵是 argument vector
        * 對```gcc -o main main.c```來說
            * argc = 3
            * argv = { "-o", "main", "main.c" } 

### 別再用 system("pause")
* <stdlib.h> 內的 system 函式，其含義是呼叫並執行系統的某個程式
* 我們常在Windows下，執行 ```system("pause")``` ，實際上是，在程式執行到一半時，去呼叫Windows下的 pause 程式，才得以達到「暫停」的效果
* Linux 沒有 pause 這種東西R，若在 Linux 環境下寫C語言，需要有暫停的效果，應採用其它方案，如下列方案其中一種
    * ```getc()```
    * ```fgetc(stdin)```
* 我的做法 ： 以C語言 Macro 模擬 pause
```c
#define PAUSE printf("Press any key to continue") ; \
             fgetc(stdin)
```

### stdin, stdout, stderr

### To be continued...

## Linux
### Unix time
