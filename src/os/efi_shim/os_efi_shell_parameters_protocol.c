/*
* Copyright (c) 2018, Intel Corporation.
* SPDX-License-Identifier: BSD-3-Clause
*/
#include <Uefi.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiLib.h>
#include <Library/ShellLib.h>
#include <Library/ShellCommandLib.h>
#include <Guid/FileInfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <s_str.h>
#include <os.h>
#ifdef _MSC_VER
#include <io.h>
#include <conio.h>
#else
#include <unistd.h>
#include <wchar.h>
#include <fcntl.h>
#define _read read
#define _getch getchar
#include <safe_str_lib.h>
#endif

#include <sys/stat.h> 
#include <fcntl.h>
#include "os_efi_shell_parameters_protocol.h"

EFI_SHELL_PARAMETERS_PROTOCOL gOsShellParametersProtocol;
EFI_SHELL_PARAMETERS_PROTOCOL *gEfiShellParametersProtocol = &gOsShellParametersProtocol;

int g_fast_path = 0;
int g_file_io = 0;

#define REC_FILE_NAME_SMBIOS "smbios.rec"
#define REC_FILE_NAME_PASSTHRU "pass_thru.rec"
#define REC_FILE_NAME_ACPI_NFIT "acpi_nfit.rec"
#define REC_FILE_NAME_ACPI_PCAT "acpi_pcat.rec"
#define REC_FILE_NAME_ACPI_PMTT "acpi_pmtt.rec"

int g_record_mode = 0;
int g_playback_mode = 0;
char g_recordings_dir[PATH_MAX];
int g_use_default_recordings_dir = 1;
char g_smbios_rec_path[PATH_MAX];
char g_passthru_rec_path[PATH_MAX];
char g_acpi_nfit_rec_path[PATH_MAX];
char g_acpi_pmtt_rec_path[PATH_MAX];
char g_acpi_pcat_rec_path[PATH_MAX];

#define STR_DASH_OUTPUT_SHORT   "-o"
#define STR_DASH_OUTPUT_LONG    "-output"
#define STR_NVMXML              "nvmxml"
#define STR_ESXXML              "esx"
#define STR_ESXTABLE            "esxtable"
#define STR_TEXT                "text"
#define STR_VERBOSE             "verbose"
#define STR_DASH_FAST_LONG      "-fast"
#ifdef PLAYBACK_RECORD_SUPPORTED
#define STR_RECORD_MODE         "-record"
#define STR_PLAYBACK_MODE       "-playback"
#define STR_REC_DIR             "-recdir"
#endif
#define MAX_INPUT_PARAMS        256
#define MAX_INPUT_PARAM_LEN     4096

EFI_STATUS init_protocol_shell_parameters_protocol(int argc, char *argv[])
{
  size_t argv_sz_chars = 0;
  char *p_tok_context = NULL;
  int new_argv_index = 1;
  int stripped_args = 0;
  if (argc > MAX_INPUT_PARAMS) {
    return EFI_INVALID_PARAMETER;
  }

  gOsShellParametersProtocol.Argv = AllocateZeroPool(MAX_INPUT_PARAMS * sizeof(CHAR16*));
  if (NULL == gOsShellParametersProtocol.Argv) {
    return EFI_OUT_OF_RESOURCES;
  }
  gOsShellParametersProtocol.StdErr = stderr;
  gOsShellParametersProtocol.StdOut = stdout;
  gOsShellParametersProtocol.StdIn = stdin;
  gOsShellParametersProtocol.Argc = argc;

#ifdef PLAYBACK_RECORD_SUPPORTED
  os_get_cwd(g_recordings_dir, PATH_MAX);
  strcat_s(g_recordings_dir, PATH_MAX, "/recordings/");
#endif

  for (int Index = 1; Index < argc; Index++) {
#ifdef PLAYBACK_RECORD_SUPPORTED
    if(g_use_default_recordings_dir)
    {
      if(0 != s_strncmpi(argv[Index], STR_RECORD_MODE, strlen(STR_RECORD_MODE) + 1) &&
          0 != s_strncmpi(argv[Index], STR_PLAYBACK_MODE, strlen(STR_PLAYBACK_MODE) + 1))
      {
        strcat_s(g_recordings_dir, PATH_MAX, argv[Index]);
      }
    }
#endif
    stripped_args = 0;
    if ((0 == s_strncmpi(argv[Index], STR_DASH_OUTPUT_SHORT, strlen(STR_DASH_OUTPUT_SHORT) + 1) ||
      0 == s_strncmpi(argv[Index], STR_DASH_OUTPUT_LONG, strlen(STR_DASH_OUTPUT_LONG + 1))) &&
      Index + 1 != argc)
    {
      char *tok;
      argv_sz_chars = strnlen_s(argv[Index + 1], MAX_INPUT_PARAM_LEN) + 1;
      tok = s_strtok(argv[Index + 1], &argv_sz_chars, ",", &p_tok_context);

      while (tok)
      {
        if (0 == s_strncmpi(tok, STR_NVMXML, strlen(STR_NVMXML) + 1) ||
          0 == s_strncmpi(tok, STR_ESXXML, strlen(STR_ESXXML) + 1) ||
          0 == s_strncmpi(tok, STR_ESXTABLE, strlen(STR_ESXTABLE) + 1))
        {
          g_file_io = 1;
          gOsShellParametersProtocol.StdOut = fopen("output.tmp", "w+");
        }

        tok = s_strtok(NULL, &argv_sz_chars, ",", &p_tok_context);
      }
    }
    else if (0 == s_strncmpi(argv[Index], STR_DASH_FAST_LONG, strlen(STR_DASH_FAST_LONG) + 1))
    {
      --gOsShellParametersProtocol.Argc;
      g_fast_path = 1;
      stripped_args = 1;
    }
#ifdef PLAYBACK_RECORD_SUPPORTED
    else if (0 == s_strncmpi(argv[Index], STR_RECORD_MODE, strlen(STR_RECORD_MODE) + 1))
    {
      --gOsShellParametersProtocol.Argc;
      g_record_mode = 1;
      stripped_args = 1;
    }
    else if (0 == s_strncmpi(argv[Index], STR_PLAYBACK_MODE, strlen(STR_PLAYBACK_MODE) + 1))
    {
      --gOsShellParametersProtocol.Argc;
      g_playback_mode = 1;
      stripped_args = 1;
    }
    else if (0 == s_strncmpi(argv[Index], STR_REC_DIR, strlen(STR_REC_DIR) + 1) &&
      Index + 1 != argc)
    {
      strcpy_s(g_recordings_dir, PATH_MAX, argv[Index+1]);
      gOsShellParametersProtocol.Argc -= 2;
      stripped_args = 1;
      ++Index;
      g_use_default_recordings_dir = 0;
    }
#endif

    if (!stripped_args)
    {
      int argvSize = strlen(argv[Index]);
      VOID * ptr = AllocateZeroPool((argvSize + 1) * sizeof(wchar_t));
      if (NULL == ptr) {
        FreePool(gOsShellParametersProtocol.Argv);
        return EFI_OUT_OF_RESOURCES;
      }
      gOsShellParametersProtocol.Argv[new_argv_index] = AsciiStrToUnicodeStr(argv[Index], ptr);
      ++new_argv_index;
    }
  }

#ifdef PLAYBACK_RECORD_SUPPORTED
  if (g_playback_mode || g_record_mode)
  {
    strcat_s(g_recordings_dir, PATH_MAX, "/");
    os_mkdir(g_recordings_dir);

    strcpy_s(g_smbios_rec_path, PATH_MAX, g_recordings_dir);
    strcat_s(g_smbios_rec_path, PATH_MAX, REC_FILE_NAME_SMBIOS);

    strcpy_s(g_passthru_rec_path, PATH_MAX, g_recordings_dir);
    strcat_s(g_passthru_rec_path, PATH_MAX, REC_FILE_NAME_PASSTHRU);

    strcpy_s(g_acpi_nfit_rec_path, PATH_MAX, g_recordings_dir);
    strcat_s(g_acpi_nfit_rec_path, PATH_MAX, REC_FILE_NAME_ACPI_NFIT);

    strcpy_s(g_acpi_pmtt_rec_path, PATH_MAX, g_recordings_dir);
    strcat_s(g_acpi_pmtt_rec_path, PATH_MAX, REC_FILE_NAME_ACPI_PMTT);

    strcpy_s(g_acpi_pcat_rec_path, PATH_MAX, g_recordings_dir);
    strcat_s(g_acpi_pcat_rec_path, PATH_MAX, REC_FILE_NAME_ACPI_PCAT);
  }
#endif
  return 0;
}

int uninit_protocol_shell_parameters_protocol()
{
  int Index = 0;
  if (g_file_io)
    fclose(gOsShellParametersProtocol.StdOut);

  for (Index = 0; Index < gOsShellParametersProtocol.Argc; ++Index)
  {
    if (NULL != gOsShellParametersProtocol.Argv[Index])
    {
      FreePool(gOsShellParametersProtocol.Argv[Index]);
    }
  }

  if (NULL != gOsShellParametersProtocol.Argv)
  {
    FreePool(gOsShellParametersProtocol.Argv);
  }
  return EFI_SUCCESS;
}
