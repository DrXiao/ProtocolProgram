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

### 真正的 scanf, printf ——> sscanf, fprintf


### To be continued...

## Linux
### Linux 的誕生
* **Unix** 作業系統
    * 在 Linux 誕生之前，就已經有一套廣受喜愛的作業系統 - **Unix** 作業系統，它是屬於**商業用**的作業系統，其強大的地方在於「**伺服器的應用**」
    * 從 **貝爾實驗室 (Bell Lab)** 與 **AT&T 公司** 創造出來的產物
    * 有一群人想要使用 Unix 作業系統，可是在當時候是要付費才能取得，於是群雄並起，讓這一群人想要創造免費的作業系統，並且讓其使用上很接近於使用 Unix。
        * 這樣的理想導致了許多著名的作業系統誕生了，並且這些作業系統可大致統稱為**類 Unix 作業系統 (Unix-like OS)**
        * 主打 **開源**、**免費** 等使用特色

* **GNU 計畫 (GNU Project)** —— 一位想實現 Unix-like OS 的大神
    * 一位全名叫 **Richard Matthew Stallman** 的大神，創立了 GNU 計畫，目的就是要創造一個名為 **GNU** 的作業系統
    * 這位大神在發展期間，便創造一堆神奇工具，著名的工具，舉例而言便是
        * **G**NU **C**ompiler **C**ollection - **GCC**
        * **G**NU **D**e**b**ugger - **GDB**
    * 創立了 **GNU General and Public Licenses** -- **GPL**
        * 明文規定說，使用此條例下的軟體，使用者可以擁有什麼權力
        * 基本權力
            * 執行 - **Run**
            * 研究 - **Study**
            * 分享 - **Share**
            * 修改 - **Modify**
* **Linux** 誕生
    * 另一位大神 **Linus Torvalds** 在大學期間，自創了一套作業系統，就是後來的 Linux 作業系統，於是也就有了所謂的 **Linux Kernel**
    * 慢慢發展作業系統的期間，後來用了很多 **GNU** 的軟體，也就導致後來有了「**GNU / Linux**」的說法出現，但現今還是多以「**Linux**」稱呼這套作業系統
    * **Linux Distribution (Linux 發行版)** ＝ 軟體 + Linux Kernel + Documentation 
### Unix time
* Unix time 的定義是「自**格林威治時間** 1970 年 1 月 1 日 0 時 0 分 0 秒 開始，累積至現今當下的**秒數**」，就叫做 Unix time
* 在 C 語言 ```<time.h>``` 中，有名為 ```timeval```的結構(```struct```)，就可以拿它來紀錄 Unix time
```c
struct timeval {
    time_t tv_sec;                  // 秒
    time_t susecond_t tv_usec;      // 微秒
}
```
* 透過 ```localtime_r```，可以把```timeval```的```tv_sec```，轉換成另一種結構
```c
struct tm
{
  int tm_sec;			/* Seconds.	[0-60] (1 leap second) */
  int tm_min;			/* Minutes.	[0-59] */
  int tm_hour;			/* Hours.	[0-23] */
  int tm_mday;			/* Day.		[1-31] */
  int tm_mon;			/* Month.	[0-11] */
  int tm_year;			/* Year	- 1900.  */
  int tm_wday;			/* Day of week.	[0-6] */
  int tm_yday;			/* Days in year.[0-365]	*/
  int tm_isdst;			/* DST.		[-1/0/1]*/

# ifdef	__USE_MISC
  long int tm_gmtoff;		/* Seconds east of UTC.  */
  const char *tm_zone;		/* Timezone abbreviation.  */
# else
  long int __tm_gmtoff;		/* Seconds east of UTC.  */
  const char *__tm_zone;	/* Timezone abbreviation.  */
# endif
};
```
* 那為什麼要這麼囉哩囉唆這些事情呢？ 因為在 ex1-PktGet 中，就有使用到這些傢伙，詳細的再自己看了
```c
// ex1-PktGet - main.c
while ((responseValue = pcap_next_ex(devAdapterHandler, &packetHeader,
                                         &packetData) >= 0)) {
        /* 
         * If responseValue is zero, it comes across timeout and continues to
         * capture next packet
         * */
        if (responseValue == 0) continue;

        /*
         * Getting Unix time from header, converting Unix time 
         * to local time and showing the relative message about header.
         * 
         * locoltime_r :    Let the information of time_t type be 
         *                  converted to a representation of tm type.
         * strftime :   Converting the information of tm type to a string representataion
         *              by passing a buffer, buffer size, and a pointer to tm type.
         * 
         * */
        localUnixTimevalSec = packetHeader->ts.tv_sec;
        localtime_r(&localUnixTimevalSec, &localTime);
        strftime(timeStr, sizeof(timeStr), "%H:%M:%S", &localTime);
        printf("%s, %.6ld len:%d\n", timeStr, packetHeader->ts.tv_usec,
               packetHeader->len);
    }
```

### Every thing is a file
