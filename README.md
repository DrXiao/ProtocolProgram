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
int main(int argc, char **argv, char **envp);
```
* 對於一個走在「正式C語言規格」上的主程式來說，main 理應宣告成上述前兩種形式其中一種，才是一個正式的C語言程式
* 第三種形式先不討論 (我查到是不具有「可攜性」，也有其它方案可以得到 envp)

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
    * **包含開頭的程式名字，也會傳進參數裡頭**
    * 因此，使用 ```int main(int argc, char **argv)```時，就是為了接收傳遞的參數
        * argc為參數的「數量」，意涵是 argument count
        * argv儲存所有的參數(字串形式)，意涵是 argument vector
        * 對```gcc -o main main.c```來說
            * argc = 4
            * argv = { "gcc", "-o", "main", "main.c" } 
    * 不需要傳遞的參數時，則可使用```int main(void)```

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

### Standard Stream - stdin, stdout, stderr
* Standard Stream 扮演的角色
    * Computer Program <———> **Stream** <———> Computer Enviornment
    * 換言之，執行程式時，有三大層面的事情，「程式」、「環境」，第三層面則是「Stream」了
* Standard Stream 其實就是一個溝通的「頻道」，當有資料的「流動」時，由 Standard Stream 在「程式」與「環境」兩層面之間，負責資料的「傳遞」
* 三大 Input/Output Stream
    * Standard Input - **stdin**
        * 意旨是：輸入至程式的資料
        * 而輸入可以從「檔案」或者「鍵盤」而來，預設會是「**鍵盤**」
    * Standard Output - **stdout**
        * 意含是：從程式輸出的資料
        * 輸出可以寫入到「檔案」內，或者顯示在「終端機」上，預設會是「**終端機**」
    * Standard Error - **stderr**
        * 也是一種輸出的Stream，但有別於 stdout ，是為了輸出「Error message」而獨立出來的
        * 同stdout，預設是輸出錯誤訊息置「**終端機**」上
    * C 語言的 stdin, stdout, stderr，就是指以上三者
* Linux 上，由於一切事物皆為「檔案」，因此 Linux 上以C語言，使用I/O Stream 時，就如同你使用```FILE *FilePtr = fopen("file.txt", "r")```的道理，是很像的
    * 使用 I/O Stream，其實原理都是以一個**指標**，指向 Stream，並做 輸入/輸出 的動作
    * 輸入就「像」是```fopen("stdin", "w")```，對 stdin 寫入資料
    * 輸出就「像」是```fopen("stdout", "r")```，從 stdout 讀取資料

* Reference
    * Standard Stream 被 Linux 視為檔案
        * https://www.howtogeek.com/435903/what-are-stdin-stdout-and-stderr-on-linux/
    * Wiki 對 Standard Stream 的解釋
        * https://en.wikipedia.org/wiki/Standard_streams

### To be continued...

## Linux
### Unix time

### Every thing is a file
