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
    EFI_EXERCISE_PROTOCOL *ExerciseProtocol = &gExerciseProtocol;

    ExerciseProtocol->ProtocolGetVariable = PROTOCOL_GET_VARIABLE;
    ExerciseProtocol->ProtocolSetVariable = PROTOCOL_SET_VARIABLE;

    Status = gBS->InstallProtocolInterface(
                                           &ImageHandle,
                                           &gEfiExerciseProtocolGuid,
                                           EFI_NATIVE_INTERFACE,
                                           &gExerciseProtocol
                                          );
```
這裡我們直接把函數指定給結構成員<code>ProtocolGetVariable</code>及<code>ProtocolSetVariable</code>, 這卜火

## Applicateon

# 本章結語
