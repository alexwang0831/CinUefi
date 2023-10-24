# 1.變數型態
## 簡易型態
C語言是強型態語言, 因此變數型態是相當重要的, UEFI的簡單的變數型態可在ProcessorBind.h中找到, 
其中較為特殊的是UINTN, 這個型態是根據Compilier不同而有不同的大小, 一般來說, 它等同於UINT64.
底下為例子<br>
```
UINT16 Index = 0;
```
我們宣告了一個型態為UINT16的變數, 它的名字叫Index, 這種型態的變數使用上只需注意型態的轉換. 
如果要把上述的Index轉成UINT32, 那就直接在前面加上(UINT32)Index就好.

## 陣列(Array)
一個完整功能的程式一定會宣告許多相同型態的變數, 但是過多的變數也會降低程式的可讀性, 也會增
加日後維護的困難. 因此我們可以把相同型態且有相同功能的變數匯集成一個陣列, 如下圖<br>
```
UINT8 TestData[] = {0, 1};
```
宣告陣列時元素數量可加可不加. 使用陣列時要清楚的表示要使用第幾個元素.
```
Print(L"%d", TestData[1]);
```
結果是會顯示1.

## 指標(Pointer)
在UEFI中大量使用指標, 因此了解指標的使用方式就相當重要了.<br>
```
UINT8 Data8 = 0
UINT8 *Data8Ptr = &Data8;
```
上面的例子意思透過取址運算子(&)把Data8這個變數的記憶體位址指定給Data8Ptr.<br>
底下的程式是要透過Data8Ptr改變Data8的值,
```
*Data8Ptr = 2; // Same as Data8 = 2
```

## 陣列與指標
當我們宣告一個陣列時, 事實上是挖出一塊記憶體空間來存放我們的資料, 而指標又是存放記憶體位址,<br>
因此在某些情況下陣列是相當於指標的.
```
UINT8 Data8[5] = {1, 2, 3, 4, 5};
UINT8 *Data8Ptr = Data8;
```
上面程式片段將Data8及Data8Ptr連結在一起, 因此我們可以利用指標的方式來存取陣列
```
UINT8 Data8[5] = {1, 2, 3, 4, 5};
UINT8 *Data8Ptr = Data8;
UINT8 index;

for(index = 0; index < sizeof(Data8)/sizeof(Data8[0]); index++) {
    Print(L"%d %d", Data[index], *(Data8Ptr + index);
}

結果
1 1  
2 2  
3 3  
4 4  
5 5  
```

## 結構(Struct)
## 聯合(Union)
## 結構指標(Struct Pointer)


