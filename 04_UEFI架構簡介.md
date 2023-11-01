# UEFI架構簡介
使用越接近人類的語言來開發程式是一種趨勢, 也更容易維護及閱讀. 因此BIOS從早期的功能簡單的<br>
組合語言演變到現在功能繁多的UEF架構, 本章就簡單的說明UEFI的一些概念.

## UEFI開機流程
組合語言時期的BIOS是遵循PostTable的順序來開機的, 但UEFI完全不同. UEFI將開機順序分為四個<br>
階段,分別為SEC, PEI, DXE及BDS, 接下來我們來稍微討論每個階段吧.<br>

### SEC
目前些階段為組合語言編寫, 主要工作是啟動CAR(Cache As RAM)以及讓CPU進入Flat Mode.<br>
底下連結有更仔細的說明, 有興趣的話可以連過去閱讀.<br>

[SEC Phase](https://billy6719.pixnet.net/blog/post/128203544)

### PEI
記憶體這個階段完成初始化, Intel平台的MRC(Memory Reference Code)就是在這階段完成任務的.<br>
由於其階段的程式是未壓縮的, 且跟平台有很大的相依性, 所以這裡的code要越少越好.<br>
此階段我們稱Module而不是Driver, PEIM與PEIM之間溝通的方式為PPI(PEIM to PEIM Interface).<br>
這階段要進入DXE時會把收集到的一些訊息透過HOB傳到DXE.<br>

### DXE
這階段可以使用記憶體了且code是經過壓縮的, 因此使用上的限制比PEI階段少.<br>
各專案相關的code應儘量放在這階段. 這個階段主要的任務是把各種protocol及service建立起來, 
讓各Driver可以快速的使用這些API.<br>

### BDS
BDS是準備開機的階段了, 各Device Driver透過Binding Protocol把自己及適合的protocol及service聯結.<br>
最終BIOS會發出"ReadyToBoot"的事件(Event)把控制權交給OS boot loader, 而boot loader取得控制權之後<br>
會呼叫ExitBootService把所有Boot Service的資料清除.

## Boot service
這些API只有在POST時可以使用, 當ExitBootService被執行之後, 所有的Boot service都會被清掉.<br>
所有可使用的Boot service都可以在UefiSpec.h找到, 我個人最常用的是跟Protocol及記憶體配置相關<br>
的服務.

## runtime service
一樣可在UefiSpec.h中找到可用的API, 最常用的當然是跟Variable相關的API了.<br>

# 本章結語
這裡只是簡單的介紹而已, 後續我們會透過實際的練習來了解到底如何使用Boot Service及Runtime Service.<br>
