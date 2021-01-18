

#include "extensiondocvisitor.h"

ExtensionDocVisitor::ExtensionDocVisitor() {
}

ExtensionDocVisitor::~ExtensionDocVisitor() {
}

void ExtensionDocVisitor::visit(const SymbolLibDocument *doc) {
    // Mutates state of this class by changing what m_extension refers to
    (void) doc;
    m_extension = &m_symbolLibExtension;
}
void ExtensionDocVisitor::visit(const FootprintLibDocument *doc) {
    (void) doc;
    m_extension = &m_footprintLibExtension;
}
void ExtensionDocVisitor::visit(const SchDocument *doc) {
    (void) doc;
    m_extension = &m_schExtension;
}
void ExtensionDocVisitor::visit(const PCBDocument *doc) {
    (void) doc;
    m_extension = &m_PCBExtension;
}
void ExtensionDocVisitor::visit(SymbolLibDocument *doc) {
    // Mutates state of this class by changing what m_extension refers to
    (void) doc;
    m_extension = &m_symbolLibExtension;
}
void ExtensionDocVisitor::visit(FootprintLibDocument *doc) {
    (void) doc;
    m_extension = &m_footprintLibExtension;
}
void ExtensionDocVisitor::visit(SchDocument *doc) {
    (void) doc;
    m_extension = &m_schExtension;
}
void ExtensionDocVisitor::visit(PCBDocument *doc) {
    (void) doc;
    m_extension = &m_PCBExtension;
}
