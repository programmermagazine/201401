## C 語言秘技 (2) – 使用 sprintf 將結構字串化 (作者：陳鍾誠)

在上一期中，我們介紹了如何用 C 語言的 sscanf 模擬正規表達式來剖析字串，以便取出某些欄位，該文連結如下：

* [C 語言秘技 (1) – 使用 sscanf 模仿正規表達式的剖析功能](http://programmermagazine.github.io/201312/htm/article2.html)

在本期中，我們將反過來用 sprintf 將某些結構中的欄位印到字串中，以便將整個結構轉成字串。

### sprintf 的用法

sprintf() 函數是 C  語言用來格式化的主要方法，其函數原形如下所示。由於其中的格式化參數稍微複雜，因此很多學習者並不知道該如何正確的使用這些格式化參數。

```CPP
int sprintf ( char * str, const char * format, ... );

str: 格式化後的輸出字串
format：格式化的規格字串

在 format 字串中，以 % 起頭者為格式段落，其格式語法如下：

格式段落的語法：%[flags][width][.precision][length]specifier

  % 代表變數開始

  [flag] 
    -:靠左
    +:輸出正負號
    (space):當不輸出正負號時，就輸出空白
    #:在 8 或 16 進位 (o,x,X) 時，強制輸出 0x 作為開頭，
      在浮點數 (e, E, f) 時，強制輸出小數點，
      在浮點數 (g, G) 時，強制輸出小數點，但尾端的 0 會被去掉。
    0:在開頭處(左側) 補 0，而非補空白。

  [width] 
    最小輸出寬度 (或 *)

  [.precision]
    精確度，小數點之後的輸出位數

  [length]
    長度符號 h, I, L

  [specifier]
    型態描述元，可以是 c, d, e, E, f, g, G, o, s, u, x, X 等基本型態。
```

sprintf() 函數的用法與 printf() 很類似，只是第一個參數為輸出字串 str 而已，為了說明這些格式的意義，我們寫了以下程式，以示範 format 欄位的各種寫法。

檔案：printf.c

```CPP
#include <stdio.h>

int main()
{
   printf ("Characters: %c %c \n", 'a', 65);
   printf ("Decimals: %d %ld\n", 1977, 650000L);
   printf ("Preceding with blanks: %10d \n", 1977);
   printf ("Preceding with zeros: %010d \n", 1977);
   printf ("Some different radixes: %d %x %o %#x %#o \n", 100, 100, 100, 100, 100);
   printf ("floats: %4.2f %+.0e %E \n", 3.1416, 3.1416, 3.1416);
   printf ("Width trick: %*d \n", 5, 10);
   printf ("%s \n", "A string");
   return 0;
}
```

以上程式的輸出結果如下：

```
D:\cp\code>gcc printf.c -o printf

D:\cp\code>printf
Characters: a A
Decimals: 1977 650000
Preceding with blanks:       1977
Preceding with zeros: 0000001977
Some different radixes: 100 64 144 0x64 0144
floats: 3.14 +3e+000 3.141600E+000
Width trick:    10
A string
```

### 結構的表達

在 C 語言當中，雖然沒有物件概念，但是可以用 struct 表達類似物件的結構，而且也可以輕易的在程式中宣告物件並進行初始化。
舉例而言，如果我們宣告了以下的結構：

```CPP
typedef struct {
  char *name;
  char gender;
  int  age;
  float weight, height;
} Person;
```

那麼我們就可以利用下列語法進行結構的宣告與初始化：

```CPP
  Person list[] = {{ .name="John", .gender='M', .age=45, .weight=59.5, .height=167.5 }, 
                   { .name="Mary", .gender='F', .age=22, .weight=52.3, .height=160.7 }};
```

### 使用 sprintf 將結構字串化

有了以上的基礎之後，我們就可以很輕易的將結構轉為 XML 或 JSON 這樣的字串格式了，以下是一個完整的範例：

檔案：stru2xml.c

```CPP
#include <stdio.h>

typedef struct {
  char *name;
  char gender;
  int  age;
  float weight, height;
} Person;

void person2xml(Person *p, char *xml) {
  sprintf(xml, "<person name=\"%s\" gender=\"%c\">\n <age>%d</age>\n <weight>%.2f</weight>\n <height>%.2f</height>\n</person>\n",  p->name, p->gender, p->age, p->weight, p->height);
}

int main()
{
  Person list[] = {{ .name="John", .gender='M', .age=45, .weight=59.5, .height=167.5 }, 
                   { .name="Mary", .gender='F', .age=22, .weight=52.3, .height=160.7 }};
				
  char listXml[1000];
  int i, tail = 0;				
  for (i=0; i<sizeof(list)/sizeof(Person); i++) {
	person2xml(&list[i], &listXml[tail]);
	tail = strlen(listXml);
  }
  printf("<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n<PersonList>\n%s</PersonList>", listXml);
}
```

執行結果：

```
D:\Dropbox\Public\pmag\201401\code>gcc stru2xml.c -o stru2xml

D:\Dropbox\Public\pmag\201401\code>stru2xml
<?xml version="1.0" encoding="ISO-8859-1"?>
<PersonList>
<person name="John" gender="M">
 <age>45</age>
 <weight>59.50</weight>
 <height>167.50</height>
</person>
<person name="Mary" gender="F">
 <age>22</age>
 <weight>52.30</weight>
 <height>160.70</height>
</person>
</PersonList>
```

### 結語

另外、有朋友提到 sprintf 可能會有 buffer overflow 的問題，因此如果改用 snprintf 會更安全一些，我想這樣是比較好的。

然後，還有網友提醒到，若一個參數不會被寫入，最好加上 const 描述，這樣會比較安全，因此我們將上述程式修改如下：


```CPP
...
void person2xml(const Person *p, char *xml, int size) {
  snprintf(xml, size, "<person name=\"%s\" gender=\"%c\">\n <age>%d</age>\n <weight>%.2f</weight>\n <height>%.2f</height>\n</person>\n",  p->name, p->gender, p->age, p->weight, p->height);
}
...
  char listXml[1000];
  int i, tail = 0;				
  for (i=0; i<sizeof(list)/sizeof(Person); i++) {
	person2xml(&list[i], &listXml[tail], 1000-tail);
	tail = strlen(listXml);
  }
...
```

這樣，就比較嚴謹且不容易有 buffer overflow 的問題了。

雖然 C++ 的使用者通常會用 cout 去輸出物件，但是筆者還是比較喜歡「標準 C」的 sscanf 與 sprintf 函數，
我覺得這兩個函數在進行「結構轉字串」與「字串轉結構」上真的非常好用，只要用習慣了，就可以輕易的用 C 語言
進行簡易的字串處理，而不需要仰賴「regular expression」或「object serialization」的函式庫了。

### 參考文獻
* <http://www.cplusplus.com/reference/clibrary/cstdio/sprintf/>
* <http://www.cplusplus.com/reference/clibrary/cstdio/sscanf/>

