# Protocol Program
* 重寫部份網路通訊協定範例程式並加上註解   
* 提供少許必要之C語言、Linux知識，以便了解程式運作

## C 語言
### 真正的 main function
```c
int main(void);
int main(int argc, char **argv);
```
* 對於一個走在「正式C語言規格」上的主程式來說，main 理應宣告成上述兩種形式其中一種，才是一個正式的C語言程式
### 終端機呼叫程式
```
gcc -o main main.c
./main 123 para
```
* 以終端機執行指令時，開頭的第一個單詞即是「程式名字」，
如上述 gcc, main 等
    * 在Linux中，如果是系統程式，直接在終端機上打上程式名字，即可執行程式 (gcc 即此類)
    * 若是執行非系統程式，需使用 ./ + 程式名字。以上述 ./main 為例，表示執行終端機「當前目錄」下，程式名字為 main 的程式
* 第一個單詞(程式名字)打完後，若在後面，以空白隔開，再打上其它東西，就都叫做「參數」，參數會傳給第一個單詞所呼叫程式
    * 將參數傳給呼叫的程式是怎麼運作的？ 這之中的過程就是OS會幫我們處理 (~~當然是我懶的講~~)
    * 而附加的參數，通通會被當成「字串」，傳給呼叫的程式
    * 因此，使用 ```int main(int argc, char **argv)```時，就是為了接收傳遞的參數
        * argc為參數的「數量」，意涵是 argument count
        * argv儲存所有的參數(字串形式)，意涵是 argument vector
        * 對```gcc -o main main.c```來說
            * argc = 3
            * argv = { "-o", "main", "main.c" } 

## Linux
