#include <Library/PrintLib.h>
#include <Library/BaseLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/ShellLib.h>
#include <Protocol/ShellParameters.h>

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
//<<

UINT8 StrLen8(
  IN CHAR16 *String
)
{
  UINT8  Length;
  for (Length = 0; *String != '\0'; String++, Length++){}
  return Length;  
}

UINT8 StrToI8 (
  IN CHAR16 *StrType
)
{
    CHAR16 *HexTemplate = L"0123456789ABCDEF";
    UINT8 Data8;
    UINT8 offset = 0;

    Print(L"StrLen8: %d\n", StrLen8(StrType));
    for(offset = 0; offset < 0x10; offset++){
        if(StrType[0] == HexTemplate[offset])
            break;
    }
    Data8 = offset;
    Print(L"%d\n", Data8);    
    if(StrLen8(StrType) == 1) { return Data8; }

    Data8 = offset * 0x10;
    for(offset = 0; offset < 0x10; offset++){
        if(StrType[1] == HexTemplate[offset])
            break;
    }
    Data8 = Data8 + offset;
    Print(L"%d\n", Data8);    
    return Data8;
}

EFI_STATUS
EFIAPI
ProtocolAppExerciseMain (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
    EFI_STATUS Status = EFI_SUCCESS;
    EFI_EXERCISE_PROTOCOL *ExProtocol = NULL;
    EFI_SHELL_PARAMETERS_PROTOCOL *gEfiShellParametersProtocol = NULL;

    UINT8 Data8 = 0;

    Status = gBS->AllocatePool(EfiBootServicesData, sizeof(EFI_EXERCISE_PROTOCOL), (VOID **)&ExProtocol);
    //    Status = gBS->OpenProtocol (
    //                    ImageHandle,
    //                    &gEfiExerciseProtocolGuid,
    //                    (VOID **)&ExProtocol,
    //                    ImageHandle,
    //                    NULL,
    //                    EFI_OPEN_PROTOCOL_GET_PROTOCOL
    //                    );
    Status = gBS->LocateProtocol(
                                 &gEfiExerciseProtocolGuid,
                                 NULL,
                                 (VOID **)&ExProtocol

    );
    if(EFI_ERROR(Status)){
        Print(L"Locate EFI_EXERCISE_PROTOCOL fail %r\n", Status);
        return Status;
    }

    if (gEfiShellParametersProtocol == NULL) {
        Status = gBS->OpenProtocol (
                        ImageHandle,
                        &gEfiShellParametersProtocolGuid,
                        (VOID **)&gEfiShellParametersProtocol,
                        ImageHandle,
                        NULL,
                        EFI_OPEN_PROTOCOL_GET_PROTOCOL
                        );
  
        if(EFI_ERROR(Status)){
            Print(L"Locate EFI_SHELL_PARAMETERS_PROTOCOL fail %r\n", Status);
            return Status;
        }
    }

    if(StrCmp(gEfiShellParametersProtocol->Argv[1], L"-Get") == 0){
        Print(L"Execute ProtocolGetVariable\n");
        Data8 = ExProtocol->ProtocolGetVariable(ExProtocol, StrToI8(gEfiShellParametersProtocol->Argv[2]));
        Print(L"Variable is %d\n", Data8);
    } else {
        Print(L"Execute ProtocolSetVariable\n");      
        Status = ExProtocol->ProtocolSetVariable(ExProtocol, StrToI8(gEfiShellParametersProtocol->Argv[2]), StrToI8(gEfiShellParametersProtocol->Argv[3]));
        Print(L"SetVariable Status %r\n", Status);
    }

    return Status;
}
