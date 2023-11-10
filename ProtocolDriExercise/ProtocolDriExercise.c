#include <Library/PrintLib.h>
#include <Library/BaseLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/ShellLib.h>
#include <Protocol/ShellParameters.h>

//>> Variable
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

#define EXERCISE_VARIABLE_GUID \
    {0x9472d50e, 0x79ec, 0x11ee, {0x9a, 0x0b, 0x00, 0x15, 0x5d, 0xf8, 0xf3, 0x30}}
//<<

//>> Protocol
#define EXERCISE_PROTOCOL_GUID \
  { \
    0xbdd3f37e, 0x7c7b, 0x11ee, {0xac, 0xf4, 0x00, 0x15, 0x5d, 0x67, 0x8c, 0xda } \
  }

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

//EFI_GUID  gEfiExerciseProtocolGuid = EXERCISE_PROTOCOL_GUID;
EFI_EXERCISE_PROTOCOL gExerciseProtocol;

UINT8
PROTOCOL_GET_VARIABLE(
  IN  EFI_EXERCISE_PROTOCOL *This,
  IN  UINT8 Offset
  )
{
    EFI_STATUS Status = EFI_SUCCESS;
    EFI_GUID VarGuid = EXERCISE_VARIABLE_GUID;
    EXERCISE_VARIABLE ExVar;
    EXERCISE_VARIABLE *ExVarPtr = &ExVar;
    UINTN VarSize = sizeof(EXERCISE_VARIABLE);
    UINT32 Attribute = EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS;

    Print(L"PROTOCOL_GET_VARIABLE\n");
    Status = gRT->GetVariable(
                              L"EXVAR",
                              &VarGuid,
                              &Attribute,
                              &VarSize,
                              ExVarPtr
                             );
    if(!EFI_ERROR(Status)) {
        Print(L"ExVar->Var_[%d] = %d\n", Offset, *((UINT8 *)ExVarPtr + Offset));
        return *((UINT8 *)ExVarPtr + Offset);
    } else {
        Print(L"GetVariable Fail, Status = %r\n", Status);
        return -1;
    }

}  

EFI_STATUS
PROTOCOL_SET_VARIABLE(
  IN  EFI_EXERCISE_PROTOCOL *This,
  IN  UINT8 Offset,
  IN  UINT8 Value
  )
{
    EFI_STATUS Status = EFI_SUCCESS;
    EFI_GUID VarGuid = EXERCISE_VARIABLE_GUID;
    EXERCISE_VARIABLE *ExVar = NULL;
    UINTN VarSize = 0;
    UINT32 Attribute = EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS;

    Print(L"PROTOCOL_SET_VARIABLE\n");
    Status = gRT->GetVariable(
                              L"EXVAR",
                              &VarGuid,
                              &Attribute,
                              &VarSize,
                              ExVar
                             );
    if(Status == EFI_NOT_FOUND) {
        Print(L"EFI_NOT_FOUND, Create Variable\n");
        VarSize = sizeof(EXERCISE_VARIABLE);
        Status = gBS->AllocatePool(EfiBootServicesData, VarSize, (VOID **)&ExVar);
        gBS->SetMem(ExVar, VarSize, 0);
    } else if(Status == EFI_BUFFER_TOO_SMALL) {
        Print(L"Buffer too small, allocate pool\n");
        //Print(L"GetVariable will return correct size if BUFFER_TOO_SMALL\n");
        Status = gBS->AllocatePool(EfiBootServicesData, VarSize, (VOID **)&ExVar);
        Status = gRT->GetVariable(
                                  L"EXVAR",
                                  &VarGuid,
                                  &Attribute,
                                  &VarSize,
                                  ExVar
                                 );
    }

    *((UINT8 *)ExVar + Offset) = Value;
    Status = gRT->SetVariable(
                              L"EXVAR",
                              &VarGuid,
                              Attribute,
                              VarSize,
                              ExVar
                             );

    return EFI_SUCCESS;
}  

//<<
EFI_STATUS
EFIAPI
ProtocolDriExerciseMain (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
    EFI_STATUS Status = EFI_SUCCESS;
    
    EFI_EXERCISE_PROTOCOL *ExerciseProtocol = &gExerciseProtocol;

    ExerciseProtocol->ProtocolGetVariable = PROTOCOL_GET_VARIABLE;
    ExerciseProtocol->ProtocolSetVariable = PROTOCOL_SET_VARIABLE;

    Status = gBS->InstallProtocolInterface(
                                           &ImageHandle,
                                           &gEfiExerciseProtocolGuid,
                                           EFI_NATIVE_INTERFACE,
                                           &gExerciseProtocol
                                          );
    Print(L"Install Status: %r\n", Status);
    return Status;
}
