# -------------------------------------------------
# Project created by QtCreator 2009-10-13T16:47:42
# -------------------------------------------------
TARGET = Finaal
TEMPLATE = app
#DEPENDPATH += .\qextserialport
#INCLUDEPATH += .\qextserialport
INCLUDEPATH += ./ \
    src/xlslib \
    src/
include(src/qextserialport/src/qextserialport.pri)
DEPENDPATH += $${INCLUDEPATH} #force rebuild if headers change
SOURCES += src/main.cpp \
    src/qfinaalmain.cpp \
    src/qfinaal.cpp \
    src/tulemuseaken.cpp \
    src/valikkast.cpp \
    src/seaded.cpp \
    src/lehelugeja.cpp \
    src/uhendumiseaken.cpp \
    src/xlslib/xlslib/workbook.cpp \
    src/xlslib/xlslib/unit.cpp \
    src/xlslib/xlslib/summinfo.cpp \
    src/xlslib/xlslib/sheetrec.cpp \
    src/xlslib/xlslib/row.cpp \
    src/xlslib/xlslib/record.cpp \
    src/xlslib/xlslib/recdef.cpp \
    src/xlslib/xlslib/range.cpp \
    src/xlslib/xlslib/number.cpp \
    src/xlslib/xlslib/note.cpp \
    src/xlslib/xlslib/merged.cpp \
    src/xlslib/xlslib/label.cpp \
    src/xlslib/xlslib/index.cpp \
    src/xlslib/xlslib/HPSF.cpp \
    src/xlslib/xlslib/globalrec.cpp \
    src/xlslib/xlslib/formula_expr.cpp \
    src/xlslib/xlslib/formula_estimate.cpp \
    src/xlslib/xlslib/formula_cell.cpp \
    src/xlslib/xlslib/formula.cpp \
    src/xlslib/xlslib/format.cpp \
    src/xlslib/xlslib/font.cpp \
    src/xlslib/xlslib/extformat.cpp \
    src/xlslib/xlslib/err.cpp \
    src/xlslib/xlslib/docsumminfo.cpp \
    src/xlslib/xlslib/datast.cpp \
    src/xlslib/xlslib/continue.cpp \
    src/xlslib/xlslib/colors.cpp \
    src/xlslib/xlslib/colinfo.cpp \
    src/xlslib/xlslib/cell.cpp \
    src/xlslib/xlslib/cbridge.cpp \
    src/xlslib/xlslib/boolean.cpp \
    src/xlslib/xlslib/blank.cpp \
    src/xlslib/xlslib/assert_assist.cpp \
    src/xlslib/common/overnew.cpp \
    src/xlslib/oledoc/oleprop.cpp \
    src/xlslib/oledoc/olefs.cpp \
    src/xlslib/oledoc/oledoc.cpp \
    src/xlslib/oledoc/binfile.cpp \
    src/localcharset.c \
    src/radadeaken.cpp \
    src/rajaspinbox.cpp \
    src/markleht.cpp \
    src/lask.cpp \
    src/lugemismasin.cpp
HEADERS += src/qfinaalmain.h \
    src/qfinaal.h \
    src/tulemuseaken.h \
    src/valikkast.h \
    src/seaded.h \
    src/lehelugeja.h \
    src/uhendumiseaken.h \
    src/xlslib/xlslib/workbook.h \
    src/xlslib/xlslib/unit.h \
    src/xlslib/xlslib/tostr.h \
    src/xlslib/xlslib/summinfo.h \
    src/xlslib/xlslib/sheetrec.h \
    src/xlslib/xlslib/row.h \
    src/xlslib/xlslib/rectypes.h \
    src/xlslib/xlslib/record.h \
    src/xlslib/xlslib/recdef.h \
    src/xlslib/xlslib/range.h \
    src/xlslib/xlslib/number.h \
    src/xlslib/xlslib/note.h \
    src/xlslib/xlslib/merged.h \
    src/xlslib/xlslib/label.h \
    src/xlslib/xlslib/index.h \
    src/xlslib/xlslib/HPSF.h \
    src/xlslib/xlslib/globalrec.h \
    src/xlslib/xlslib/formula_expr.h \
    src/xlslib/xlslib/formula_estimate.h \
    src/xlslib/xlslib/formula_const.h \
    src/xlslib/xlslib/formula_cell.h \
    src/xlslib/xlslib/formula.h \
    src/xlslib/xlslib/formtags.h \
    src/xlslib/xlslib/format.h \
    src/xlslib/xlslib/font.h \
    src/xlslib/xlslib/extformat.h \
    src/xlslib/xlslib/err.h \
    src/xlslib/xlslib/docsumminfo.h \
    src/xlslib/xlslib/datast.h \
    src/xlslib/xlslib/continue.h \
    src/xlslib/xlslib/common.h \
    src/xlslib/xlslib/colors.h \
    src/xlslib/xlslib/colinfo.h \
    src/xlslib/xlslib/cell.h \
    src/xlslib/xlslib/cbridge.h \
    src/xlslib/xlslib/boolean.h \
    src/xlslib/xlslib/blank.h \
    src/xlslib/xlslib/biffsection.h \
    src/xlslib/xlslib.h \
    src/xlslib/xlconfig.h \
    src/xlslib/common/xlsys.h \
    src/xlslib/common/xlstypes.h \
    src/xlslib/common/xls_pshpack4.h \
    src/xlslib/common/xls_pshpack2.h \
    src/xlslib/common/xls_pshpack1.h \
    src/xlslib/common/xls_poppack.h \
    src/xlslib/common/timespan.h \
    src/xlslib/common/systype.h \
    src/xlslib/common/stringtok.h \
    src/xlslib/common/overnew.h \
    src/xlslib/oledoc/oleprop.h \
    src/xlslib/oledoc/olefs.h \
    src/xlslib/oledoc/oledoc.h \
    src/xlslib/oledoc/binfile.h \
    src/localcharset.h \
    src/radadeaken.h \
    src/rajaspinbox.h \
    src/markleht.h \
    src/lask.h \
    src/lugemismasin.h \
    src/version.h
QT += network \
    printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

RESOURCES += finaal.qrc
RC_FILE = src/ikoon.rc
FORMS += src/Seaded.ui \
    src/ValikKast.ui \
    src/lehelugeja.ui \
    src/uhendumiseaken.ui \
    src/radadeaken.ui
#LIBS += -L.\qextserialport\build -lqextserialport
win32{
    isEqual(QT_MAJOR_VERSION, 4): LIBS += -liconv
}

QMAKE_LFLAGS += -static-libstdc++ \
    -static-libgcc \
    -LLIBDIR
