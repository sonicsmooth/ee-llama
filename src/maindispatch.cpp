#include "maindispatch.h"

#include "filedialogs.h"
#include "misc.h"
#include "newdocs.h"


dispatchMap_t dispatchMap(Emdi & emdi, docVec_t & docVec) {
    // Each entry in dispatch map is a lambda taking a QVariant
    // Typically the QVariant arg is carried over by the action which got triggered
    dispatchMap_t dm;
    dm["actionNewSymbolLibrary"] = [&](const QVariant &){newdocs::newDoc<SymbolLibDocument>("Main Editor", emdi, docVec);};
    dm["actionNewFootprintLibrary"] = [&](const QVariant &){newdocs::newDoc<FootprintLibDocument>("Main Editor", emdi, docVec);};
    dm["actionNewSchematic"] = [&](const QVariant &){newdocs::newDoc<SchDocument>("Main Editor", emdi, docVec);};
    dm["actionNewPCB"] = [&](const QVariant &){newdocs::newDoc<PCBDocument>("Main Editor", emdi, docVec);};
    dm["actionOpen"] = [&](const QVariant &){};
    dm["actionSave"] = [&](const QVariant & v){filedialogs::fileSave(emdi, v.value<MainWindow *>());};
    dm["actionSaveAs"] = [&](const QVariant & v){filedialogs::fileSaveAs(emdi, v.value<MainWindow *>());};
    dm["actionSaveCopyAs"] = [&](const QVariant & v){filedialogs::fileSaveCopyAs(emdi, v.value<MainWindow *>());};
    dm["actionCloseDoc"] = [&](const QVariant &){emdi.closeDocument();};
    dm["actionExit"] = [&](const QVariant &){quitSequence(emdi);};
    dm["actionViewProperties"] = [&](const QVariant & qv){
        if (qv.toBool()) {
            emdi.showDockFrame("Properties");
        } else {
            emdi.closeDockFrame("Properties");
        }};
    dm["actionViewHierarchy"] = [&](const QVariant & qv){
        if (qv.toBool()) {
            emdi.showDockFrame("Hierarchy");
        } else {
            emdi.closeDockFrame("Hierarchy");
        }};
    dm["actionDuplicateMDI"] = [&](const QVariant &){emdi.duplicateMdiFrame();};
    dm["actionDupAndPopoutMDI"] = [&](const QVariant &){emdi.duplicateAndPopoutMdiFrame();};
    return dm;
}

