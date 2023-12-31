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

## 修改c檔
### include的部份
```
#include <Library/PrintLib.h>
#include <Library/BaseLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/ShellLib.h>
#include <Protocol/ShellParameters.h>
```
<br>這個幾個是我們在寫Shell程式常用的h檔, 建議保留.<br>

### 程式主體
```
EFI_STATUS
EFIAPI
VariableExerciseMain (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
    EFI_STATUS Status = EFI_SUCCESS;
    return Status;
}
```
不管是Application還是Driver,進入點都要宣告<code>ImageHandle</code>及<code>SystemTable</code><br>
兩個參數.<br>

### 宣告Variable長相及歸屬
這邊要運用先前提過的結構來建立我們測試用的Variable,<br>
```
#pragma pack(1)
typedef struct 
{
  /* data */
  UINT8 Var_1;
  UINT8 Var_2;
  UINT8 Var_3;
  UINT8 Var_4;
  UINT8 Var_5;
}EXERCISE_VARIABLE;
#pragma pack()
```
這邊加上<code>pack(1)</code>是告訴compiler, 這個結構裡面的東西要緊緊相依, 不要為了效率而浪費空間.<br>
每個Variable都需要一個住址, 因此我們需要產生一個GUID讓它居住, <br>
```
#define EXERCISE_VARIABLE \
    {0x9472d50e, 0x79ec, 0x11ee, {0x9a, 0x0b, 0x00, 0x15, 0x5d, 0xf8, 0xf3, 0x30}}
```
底下是GetVariable的使用範例<br>
```
EFI_STATUS Status = EFI_SUCCESS;
EFI_GUID VarGuid = EXERCISE_VARIABLE_GUID;
EXERCISE_VARIABLE *ExVar = NULL;
UINTN VarSize = 0;
UINT32 Attribute = EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS;

Status = gRT->GetVariable(
                          L"EXVAR",
                          &VarGuid,
                          &Attribute,
                          &VarSize,
                          ExVar
                         );

if(Status == EFI_NOT_FOUND) {
    ...
} else if(Status == EFI_BUFFER_TOO_SMALL) {
    ...
}
```
關於屬性, 大概說明如下:<br>
EFI_VARIABLE_NON_VOLATILE:表示這個Variable是寫回ROM的, 也就是說關機後再開機它仍然能保持上一次的狀態.<br>
EFI_VARIABLE_BOOTSERVICE_ACCESS:這個Variable是POST時是可讀寫的. 當ExitBootService被呼叫後它就會被刪除.<br>
EFI_VARIABLE_RUNTIME_ACCESS:這個Variable進OS之後仍然可以存取.<br>
<code>Status == EFI_NOT_FOUND</code>表是這個Variable尚未被建立, 因此需要用SetVariable來建立.<br>
<code>Status == EFI_BUFFER_TOO_SMALL</code>則是我們不知道Variable實際大小的情況下, 會先把size<br>
設為0, 若Variable存在的話, 那GetVariable會回傳正確的大小給我們. 完整的code請參考<br>

[VariableExercise](./VariableExercise)

build完成的執行檔會放在底下路徑
<code>
\Build\Shell\DEBUG_GCC5\X64\ShellPkg\Application\VariableExercise\VariableExercise\OUTPUT\VariableExercise.efi
</code>

# 本章結語
這個範例儘是用來說明如何操作Variable, 並不管C的寫作風格. 若與你的習慣不同的話, 可以修改為自己習慣的風格.
