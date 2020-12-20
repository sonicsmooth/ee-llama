#ifndef DOCUMENTS_H
#define DOCUMENTS_H

#include "vdocument.h"
#include "menuadder.h"
#include <string>

class SymbolLibDocument    : public IDocument, public IMenuAdder {
private:
    const std::string m_name;
    bool m_activeState;
    menus_t m_menus;
public:
    SymbolLibDocument(const std::string &);
    ~SymbolLibDocument() override;
    void init() override;
    void done() override;
    bool isActive() override;
    bool supportsUserType(const std::string &) const override;
    QWidget *newView(const std::string &) const override;
    const std::string & name() const override;
    const menus_t & menus() const override;
};
class FootprintLibDocument : public IDocument, public IMenuAdder {
private:
    const std::string m_name;
    bool m_activeState;
    menus_t m_menus;
public:
    FootprintLibDocument(const std::string &);
    ~FootprintLibDocument() override;
    void init() override;
    void done() override;
    bool isActive() override;
    bool supportsUserType(const std::string &) const override;
    QWidget *newView(const std::string &) const override;
    const std::string & name() const override;
    const menus_t & menus() const override;
};
class SchDocument          : public IDocument, public IMenuAdder {
private:
    const std::string m_name;
    bool m_activeState;
    menus_t m_menus;
public:
    SchDocument(const std::string &);
    ~SchDocument() override;
    void init() override;
    void done() override;
    bool isActive() override;
    bool supportsUserType(const std::string &) const override;
    QWidget *newView(const std::string &) const override;
    const std::string & name() const override;
    const menus_t & menus() const override;
};
class PCBDocument          : public IDocument, public IMenuAdder {
private:
    const std::string m_name;
    bool m_activeState;
    menus_t m_menus;
public:
    PCBDocument(const std::string &);
    ~PCBDocument() override;
    void init() override;
    void done() override;
    bool isActive() override;
    bool supportsUserType(const std::string &) const override;
    QWidget *newView(const std::string &) const override;
    const std::string & name() const override;
    const menus_t & menus() const override;
};



#endif
