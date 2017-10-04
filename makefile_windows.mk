# makefile
# date 2017-09-05
# Copyright 2017 Mamoru kaminaga
CC = C:\"Program Files (x86)"\"Microsoft Visual Studio"\2017\Community\VC\Tools\MSVC\14.11.25503\bin\HostX86\x86\cl.exe
LINK = C:\"Program Files (x86)"\"Microsoft Visual Studio"\2017\Community\VC\Tools\MSVC\14.11.25503\bin\HostX86\x86\link.exe
OUTDIR = build
TARGET = satpass.exe
PDB = satpass.pdb
MAP = satpass.map
RES =
SRC =\
		 export.cc\
		 file.cc\
		 input.cc\
		 main.cc\
		 output.cc\
		 solution.cc
OBJ =\
		 $(OUTDIR)/export.obj\
		 $(OUTDIR)/file.obj\
		 $(OUTDIR)/main.obj\
		 $(OUTDIR)/input.obj\
		 $(OUTDIR)/output.obj\
		 $(OUTDIR)/solution.obj
LIBS = "kernel32.lib" "user32.lib" "gdi32.lib" "winspool.lib" "comdlg32.lib"\
"advapi32.lib" "shell32.lib" "ole32.lib" "oleaut32.lib" "uuid.lib"\
"odbc32.lib" "odbccp32.lib" "sat/v1.0.2/sat.lib" "tle/v2.1.0/tle.lib"

# Debug build
#CPPFLAGS = /nologo /W4 /Zi /O2 /MT /D"UNICODE" /D"_UNICODE" /D"DEBUG" /TP\
#	/EHsc /Fd"$(OUTDIR)/" /D"NOMINMAX"
#LFLAGS = $(LIBS) /NOLOGO /SUBSYSTEM:CONSOLE /PDB:"$(PDB)" /MAP:"$(MAP)"\
#	/DEBUG

# Release build
CPPFLAGS = /nologo /W4 /Zi /O2 /MT /D"UNICODE" /D"_UNICODE"\
					 /EHsc /Fd"$(OUTDIR)/" /D"NOMINMAX"
LFLAGS = $(LIBS) /NOLOGO /SUBSYSTEM:CONSOLE /PDB:"$(PDB)" /MAP:"$(MAP)"

ALL: $(TARGET)

$(TARGET): $(OBJ) $(RES)
	$(LINK) $(LFLAGS) /OUT:$(TARGET) $(OBJ) $(RES)

.cc{$(OUTDIR)}.obj:
	$(CC) $(CPPFLAGS) /Fo"$(OUTDIR)\\" /c $<
