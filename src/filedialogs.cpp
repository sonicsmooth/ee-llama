
#include "dbutils.h"
#include "extensiondocvisitor.h"
#include "filedialogs.h"
#include "filesavevisitor.h"
#include "filesavecopyasvisitor.h"
#include "mainwindow.h"
#include "task.h"

#include <QFileDialog>
#include <QObject>

namespace filedialogs {

static IDocument *docFromMainWindow(const Emdi & emdi, const QMainWindow *mw) {
    // Collect doc and new filename
    const QMdiArea *mdi = static_cast<QMdiArea *>(mw->centralWidget());
    const QMdiSubWindow *sw = mdi->activeSubWindow();
    if (!sw) return nullptr;
    IDocument *doc = emdi.document(sw);
    assert(doc);
    return doc;
}


static std::string fileSaveAsName(const Emdi & emdi, const QMainWindow *mw) {
    // Collect doc and new filename
    IDocument *doc = docFromMainWindow(emdi, mw);
    ExtensionDocVisitor extvisitor;
    doc->accept(&extvisitor);
    QString extension = QString::fromStdString(extvisitor.extension());
    QString qfilename = QFileDialog::getSaveFileName(nullptr, "", extension);
    return qfilename.toStdString();
}
void fileSave(const Emdi & emdi, QObject *parent) {
    // Use visitor pattern to save file
    MainWindow *mw = static_cast<MainWindow *>(parent);
    IDocument *doc = docFromMainWindow(emdi, mw);
    auto task = new Task(mw, [doc] {
        FileSaveVisitor fsv;
        doc->accept(&fsv);
    });
    // Access global numberEmitter in dbutils and connect it to progress slot
    // Then disconnect when thread finishes
    QObject::connect(&dbutils::intEmitter, &IntEmitter::emitInt,
                     mw, &MainWindow::setProgressValue);
    QObject::connect(task, &QThread::started, mw, &MainWindow::startProgress);
    QObject::connect(task, &QThread::finished, mw, &MainWindow::stopProgress);
    QObject::connect(task, &QThread::finished, task, &QThread::deleteLater);
    QObject::connect(task, &QThread::finished, [mw](){
        QObject::disconnect(&dbutils::intEmitter, &IntEmitter::emitInt,
                            mw, &MainWindow::setProgressValue);});
    task->start();
}
void fileSaveAs(const Emdi & emdi, QObject *parent) {
    // Similar to fileSave, except it renames doc too
    // Use visitor pattern to save file
    MainWindow *mw = static_cast<MainWindow *>(parent);
    IDocument *doc = docFromMainWindow(emdi, mw);
    const std::string filename = fileSaveAsName(emdi, mw);
    auto task = new Task(mw, [doc, filename] {
        FileSaveCopyAsVisitor fsv(filename);
        doc->accept(&fsv);
    });
    // Access global numberEmitter in dbutils and connect it to progress slot
    // Then disconnect when thread finishes
    QObject::connect(&dbutils::intEmitter, &IntEmitter::emitInt,
                     mw, &MainWindow::setProgressValue);
    QObject::connect(task, &QThread::started, mw, &MainWindow::startProgress);
    QObject::connect(task, &QThread::finished, mw, &MainWindow::stopProgress);
    QObject::connect(task, &QThread::finished, task, &QThread::deleteLater);
    QObject::connect(task, &QThread::finished, [mw](){
        QObject::disconnect(&dbutils::intEmitter, &IntEmitter::emitInt,
                            mw, &MainWindow::setProgressValue);});
    task->start();
    emdi.renameDocument(doc, filename);
}
void fileSaveCopyAs(const Emdi & emdi, QObject *parent) {
    // Same as fileSaveAs, except it doesn't rename doc
    // Use visitor pattern to save file
    MainWindow *mw = static_cast<MainWindow *>(parent);
    IDocument *doc = docFromMainWindow(emdi, mw);
    const std::string filename = fileSaveAsName(emdi, mw);
    auto task = new Task(mw, [doc, filename] {
        FileSaveCopyAsVisitor fsv(filename);
        doc->accept(&fsv);
    });
    // Access global numberEmitter in dbutils and connect it to progress slot
    // Then disconnect when thread finishes
    QObject::connect(&dbutils::intEmitter, &IntEmitter::emitInt,
                     mw, &MainWindow::setProgressValue);
    QObject::connect(task, &QThread::started, mw, &MainWindow::startProgress);
    QObject::connect(task, &QThread::finished, mw, &MainWindow::stopProgress);
    QObject::connect(task, &QThread::finished, task, &QThread::deleteLater);
    QObject::connect(task, &QThread::finished, [mw](){
        QObject::disconnect(&dbutils::intEmitter, &IntEmitter::emitInt,
                            mw, &MainWindow::setProgressValue);});
    task->start();
}

} // namespace
