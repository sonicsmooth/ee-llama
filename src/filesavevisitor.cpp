#include "filesavevisitor.h"

FileSaveVisitor::FileSaveVisitor()
{

}

void FileSaveVisitor::visit(SymbolLibDocument *doc) {
    doc->save();
}
void FileSaveVisitor::visit(FootprintLibDocument *doc) {
    doc->save();
}
void FileSaveVisitor::visit(SchDocument *doc) {
    doc->save();
}
void FileSaveVisitor::visit(PCBDocument *doc) {
    doc->save();
}

void FileSaveVisitor::visit(SymbolLibDocument *doc) const {
    doc->save();
}
void FileSaveVisitor::visit(FootprintLibDocument *doc) const {
    doc->save();
}
void FileSaveVisitor::visit(SchDocument *doc) const {
    doc->save();
}
void FileSaveVisitor::visit(PCBDocument *doc) const {
    doc->save();
}

