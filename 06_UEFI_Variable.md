# Variable概念
在UEFI的世界中, Variable與C的Variable相同, 都是用來存儲資料的.<br>
但是在BIOS中有些設定是必須被儲存起來, 下次開機時可以直接拿來用, 因此我們把這些設定值<br>
稱為variable, 透過runtime service中GetVariable及SetVariable來使用.<br>

現在我們開始練習吧, 請注意, 為避免文件變得冗長, 本文件不會做仔細的講解.<br>
## 建立資料夾
在底下圖片的位置建立VariableExercise資料夾.<br>
![Alt text](Image/VariableExercise_01.png)<br>
接著在ShellPkg.dsc中的<code>[Components]</code>底下加入VariableExercise.inf的位置, 如下:<br>
<code> ShellPkg/Application/VariableExercise/VariableExercise.inf</code><br>

## 修改inf
```
[Defines]
  INF_VERSION                    = 0x0001001B
  BASE_NAME                      = VariableExercise
  FILE_GUID                      = 46d5a9c2-7959-11ee-9126-00155d4bd306
  MODULE_TYPE                    = UEFI_APPLICATION
  VERSION_STRING                 = 1.0
  ENTRY_POINT                    = VariableExerciseMain
```
每個inf的<code>[Defines]</code>都要做適當的修改, 我們此次是要寫應用程式,<br>
因此要設定為<code>UEFI_APPLICATION</code>, 而ENTRY_POINT是這個應用程式的進入點, <br>
習慣上會以Main或EntryPoint結尾. 每個inf的GUID都是不同的, 可以uuid產生器來產生新的GUID.<br>

## 修改c及h檔
