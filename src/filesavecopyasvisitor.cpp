#include "filesavecopyasvisitor.h"

FileSaveCopyAsVisitor::FileSaveCopyAsVisitor(const std::string & newName) :
    m_newName(newName)
{

}

void FileSaveCopyAsVisitor::visit(SymbolLibDocument *doc) {
    doc->saveCopyAs(m_newName);
}
void FileSaveCopyAsVisitor::visit(FootprintLibDocument *doc) {
    doc->saveCopyAs(m_newName);
}
void FileSaveCopyAsVisitor::visit(SchDocument *doc) {
    doc->saveCopyAs(m_newName);
}
void FileSaveCopyAsVisitor::visit(PCBDocument *doc) {
    doc->saveCopyAs(m_newName);
}

