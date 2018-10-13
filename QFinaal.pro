# -------------------------------------------------
# Project created by QtCreator 2009-10-13T16:47:42
# -------------------------------------------------
TARGET = Finaal
TEMPLATE = app
#DEPENDPATH += .\qextserialport
#INCLUDEPATH += .\qextserialport
INCLUDEPATH += ./src/xlslib \
	src/
include(src/qextserialport/src/qextserialport.pri)
DEPENDPATH += $${INCLUDEPATH} #force rebuild if headers change
SOURCES += main.cpp \
    qfinaalmain.cpp \
    qfinaal.cpp \
    tulemuseaken.cpp \
    valikkast.cpp \
    seaded.cpp \
    lehelugeja.cpp \
    uhendumiseaken.cpp \
    xlslib/workbook.cpp \
    xlslib/unit.cpp \
    xlslib/summinfo.cpp \
    xlslib/sheetrec.cpp \
    xlslib/row.cpp \
    xlslib/record.cpp \
    xlslib/recdef.cpp \
    xlslib/range.cpp \
    xlslib/number.cpp \
    xlslib/note.cpp \
    xlslib/merged.cpp \
    xlslib/label.cpp \
    xlslib/index.cpp \
    xlslib/HPSF.cpp \
    xlslib/globalrec.cpp \
    xlslib/formula_expr.cpp \
    xlslib/formula_estimate.cpp \
    xlslib/formula_cell.cpp \
    xlslib/formula.cpp \
    xlslib/format.cpp \
    xlslib/font.cpp \
    xlslib/extformat.cpp \
    xlslib/err.cpp \
    xlslib/docsumminfo.cpp \
    xlslib/datast.cpp \
    xlslib/continue.cpp \
    xlslib/colors.cpp \
    xlslib/colinfo.cpp \
    xlslib/cell.cpp \
    xlslib/cbridge.cpp \
    xlslib/boolean.cpp \
    xlslib/blank.cpp \
    xlslib/assert_assist.cpp \
    common/overnew.cpp \
    oledoc/oleprop.cpp \
    oledoc/olefs.cpp \
    oledoc/oledoc.cpp \
    oledoc/binfile.cpp \
    localcharset.c \
    radadeaken.cpp \
    rajaspinbox.cpp \
    markleht.cpp \
    ../Protokollitaja/lask.cpp \
    lugemismasin.cpp
HEADERS += qfinaalmain.h \
    qfinaal.h \
    tulemuseaken.h \
    valikkast.h \
    seaded.h \
    lehelugeja.h \
    uhendumiseaken.h \
    xlslib/workbook.h \
    xlslib/unit.h \
    xlslib/tostr.h \
    xlslib/summinfo.h \
    xlslib/sheetrec.h \
    xlslib/row.h \
    xlslib/rectypes.h \
    xlslib/record.h \
    xlslib/recdef.h \
    xlslib/range.h \
    xlslib/number.h \
    xlslib/note.h \
    xlslib/merged.h \
    xlslib/label.h \
    xlslib/index.h \
    xlslib/HPSF.h \
    xlslib/globalrec.h \
    xlslib/formula_expr.h \
    xlslib/formula_estimate.h \
    xlslib/formula_const.h \
    xlslib/formula_cell.h \
    xlslib/formula.h \
    xlslib/formtags.h \
    xlslib/format.h \
    xlslib/font.h \
    xlslib/extformat.h \
    xlslib/err.h \
    xlslib/docsumminfo.h \
    xlslib/datast.h \
    xlslib/continue.h \
    xlslib/common.h \
    xlslib/colors.h \
    xlslib/colinfo.h \
    xlslib/cell.h \
    xlslib/cbridge.h \
    xlslib/boolean.h \
    xlslib/blank.h \
    xlslib/biffsection.h \
    xlslib.h \
    xlconfig.h \
    common/xlsys.h \
    common/xlstypes.h \
    common/xls_pshpack4.h \
    common/xls_pshpack2.h \
    common/xls_pshpack1.h \
    common/xls_poppack.h \
    common/timespan.h \
    common/systype.h \
    common/stringtok.h \
    common/overnew.h \
    oledoc/oleprop.h \
    oledoc/olefs.h \
    oledoc/oledoc.h \
    oledoc/binfile.h \
    localcharset.h \
    version.h \
    radadeaken.h \
    rajaspinbox.h \
    markleht.h \
    ../Protokollitaja/lask.h \
    lugemismasin.h
QT += network \
    printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

RESOURCES += finaal.qrc
RC_FILE = ikoon.rc
FORMS += Seaded.ui \
    ValikKast.ui \
    lehelugeja.ui \
    uhendumiseaken.ui \
    radadeaken.ui
#LIBS += -L.\qextserialport\build -lqextserialport
win32{
    isEqual(QT_MAJOR_VERSION, 4): LIBS += -liconv
}

QMAKE_LFLAGS += -static-libstdc++ \
    -static-libgcc \
    -LLIBDIR
