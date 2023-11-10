# Protocol概念
在UEFI中, 很多時候Driver與Driver或硬體之間必須溝通, 因此UEFI把這個溝通橋樑稱為Protocol.<br>
有Protocol, 我們自己寫的Driver可以很方便得到硬體資訊. 本篇會寫一個Driver及一個Application<br>
再加上VariableExercise來說明Protocol如何使用.

## Driver
### Protocol definition
Protocol一樣需要一個GUID, 相關的方式請參考<code>06_UEFI_Variable.md</code>.本文不再探討<br>
Protocol需要定義它的長相, 我們通當會結合結構及函數指標來完成, 底下是本次範例的Protocol定義.<br>
```
typedef struct _EFI_EXERCISE_PROTOCOL EFI_EXERCISE_PROTOCOL;

typedef
UINT8
(*EFI_PROTOCOL_GET_VARIABLE)(
  IN  EFI_EXERCISE_PROTOCOL *This,
  IN  UINT8 Offset
  );

typedef
EFI_STATUS
(*EFI_PROTOCOL_SET_VARIABLE)(
  IN  EFI_EXERCISE_PROTOCOL *This,
  IN  UINT8 Offset,
  IN  UINT8 Value
  );

struct _EFI_EXERCISE_PROTOCOL{
  EFI_PROTOCOL_GET_VARIABLE    ProtocolGetVariable;
  EFI_PROTOCOL_SET_VARIABLE    ProtocolSetVariable;
};
```
<code>_EFI_EXERCISE_PROTOCOL</code>這個結構就是Protocol的長相, 在Protocol中也可以<br>
使用基本型態的變數, 只是本範例沒用到, 因此沒宣告.而<code>EFI_PROTOCOL_GET_VARIABLE</code>及<br>
<code>EFI_PROTOCOL_SET_VARIABLE</code>則是定義了Protocol中函數應該長什麼樣子.<br>
<br>
我們先把函數該做什麼事完成, 本例儘是用Protocol來修改Variable,跟variable相關的的部份請參考上篇.<br>
```
UINT8
PROTOCOL_GET_VARIABLE(
  IN  EFI_EXERCISE_PROTOCOL *This,
  IN  UINT8 Offset
  )
{...}

EFI_STATUS
PROTOCOL_SET_VARIABLE(
  IN  EFI_EXERCISE_PROTOCOL *This,
  IN  UINT8 Offset,
  IN  UINT8 Value
  )
{...}
```
這邊我們把函數該做的事完成.<br>
```
EFIAPI
ProtocolDriExerciseMain (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
...
    EFI_EXERCISE_PROTOCOL *ExerciseProtocol = &gExerciseProtocol;

    ExerciseProtocol->ProtocolGetVariable = PROTOCOL_GET_VARIABLE;
    ExerciseProtocol->ProtocolSetVariable = PROTOCOL_SET_VARIABLE;

    Status = gBS->InstallProtocolInterface(
                                           &ImageHandle,
                                           &gEfiExerciseProtocolGuid,
                                           EFI_NATIVE_INTERFACE,
                                           &gExerciseProtocol
                                          );
}
```
這裡我們直接把函數指定給結構成員<code>ProtocolGetVariable</code>及<code>ProtocolSetVariable</code>, 這做法就是在03_函數<br>時提到的函數指標.當結構成員完成指定或初始化之後, 我們透過<code>InstallProtocolInterface</code>把我們的Protocol<br>
掛在<code>ImageHandle</code>上, 要留意的是我們要把Protocol掛在Shell中, 因此用main函數的參數即可,<br>
若是要掛在特定的driver, 那就要用對應的Handle才可以.<br>
另一種安裝Protocol的方式為<code>InstallMultipleProtocolInterfaces</code>, 若一次需安裝多個Protocol時,<br>
建議用後者較為便利.<br>
此外inf中必須宣告為<code>MODULE_TYPE = UEFI_DRIVER</code>且需把<code>UefiDriverEntryPoint</code>加進來,<br>
否則會build error.

## Application
Application這邊, inf要宣告成<code>UEFI_APPLICATION</code>.良好的程式設計是儘量把重覆的部分寫在h檔裡,<br>
本文不討論這部分, 因此還是在c檔裡定義了Protocol原型.<br>
使用Protocol最簡單的方式是<code>LocateProtocol</code>,這個意思是不管Protocol掛在哪個Handle, 只要找到就拿來用.<br>
進階的使用方式, 我們留到下一章再講解.<br>

# 本章結語
一定要了解C的結構及指標, 要使用或編寫Protocol時才不會出錯.<br>
