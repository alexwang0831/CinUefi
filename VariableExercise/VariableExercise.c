#include <Library/PrintLib.h>
#include <Library/BaseLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/ShellLib.h>
#include <Protocol/ShellParameters.h>

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

EFI_STATUS
EFIAPI
VariableExerciseMain (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
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
        Print(L"EFI_NOT_FOUND, Create Variable\n");
        VarSize = sizeof(EXERCISE_VARIABLE);
        Status = gBS->AllocatePool(EfiBootServicesData, VarSize, (VOID **)&ExVar);
        //gBS->SetMem(ExVar, VarSize, 0);
        ExVar->Var_1 = 1;
        ExVar->Var_2 = 2;
        ExVar->Var_3 = 3;
        ExVar->Var_4 = 4;
        ExVar->Var_5 = 5;

        Status = gRT->SetVariable(
                                  L"EXVAR",
                                  &VarGuid,
                                  Attribute,
                                  VarSize,
                                  ExVar
                                 );

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
        Print(L"Var_1 = %x\n", ExVar->Var_1);
        Print(L"Var_2 = %x\n", ExVar->Var_2);
        Print(L"Var_3 = %x\n", ExVar->Var_3);
        Print(L"Var_4 = %x\n", ExVar->Var_4);
        Print(L"Var_5 = %x\n", ExVar->Var_5);
    }

    return Status;
}
