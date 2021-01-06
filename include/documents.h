#ifndef DOCUMENTS_H
#define DOCUMENTS_H

#include "idocument.h"
#include "idocvisitor.h"
#include <string>



class SymbolLibDocument    : public IDocument {
private:
    const std::string m_name;
    bool m_activeState;
public:
    SymbolLibDocument(const std::string &);
    ~SymbolLibDocument() override;
    void init() override;
    void done() override;
    bool isActive() override;
    bool supportsUserType(const std::string &) const override;
    QWidget *newView(const std::string &) const override;
    const std::string & name() const override;
    void accept(IDocVisitor *) override;
    void accept(IDocVisitor *) const override;
    void save();
    void save(const std::string &);
};
class FootprintLibDocument : public IDocument {
private:
    const std::string m_name;
    bool m_activeState;
public:
    FootprintLibDocument(const std::string &);
    ~FootprintLibDocument() override;
    void init() override;
    void done() override;
    bool isActive() override;
    bool supportsUserType(const std::string &) const override;
    QWidget *newView(const std::string &) const override;
    const std::string & name() const override;
    void accept(IDocVisitor *) override;
    void accept(IDocVisitor *) const override;
    void save();
    void save(const std::string &);
};
class SchDocument          : public IDocument {
private:
    const std::string m_name;
    bool m_activeState;
public:
    SchDocument(const std::string &);
    ~SchDocument() override;
    void init() override;
    void done() override;
    bool isActive() override;
    bool supportsUserType(const std::string &) const override;
    QWidget *newView(const std::string &) const override;
    const std::string & name() const override;
    void accept(IDocVisitor *) override;
    void accept(IDocVisitor *) const override;
    void save();
    void save(const std::string &);
};
class PCBDocument          : public IDocument {
private:
    const std::string m_name;
    bool m_activeState;
public:
    PCBDocument(const std::string &);
    ~PCBDocument() override;
    void init() override;
    void done() override;
    bool isActive() override;
    bool supportsUserType(const std::string &) const override;
    QWidget *newView(const std::string &) const override;
    const std::string & name() const override;
    void accept(IDocVisitor *) override;
    void accept(IDocVisitor *) const override;
    void save();
    void save(const std::string &);
};




#endif
