#ifndef DOCUMENTS_H
#define DOCUMENTS_H

#include "idocument.h"
#include "idocvisitor.h"
#include "docthreadwrapper.h"

#include <mutex>
#include <string>
#include <thread>


class SymbolLibDocument    : public IDocument {
private:
    std::string m_name; // can change
    const std::string m_connName; // doesn't change for lifetime of object
    bool m_activeState;
    DocThreadWrapper *m_wrapper;
    //std::mutex m_mutex;
    // todo: add bool dirty
public:
    SymbolLibDocument();
    SymbolLibDocument(const SymbolLibDocument &);
    SymbolLibDocument(const std::string &, DocThreadWrapper * = nullptr);
    ~SymbolLibDocument() override;
    void init() override;
    void done() override;
    bool isActive() override;
    bool supportsUserType(const std::string &) const override;
    QWidget *newView(const std::string &) const override;
    const std::string & name() const override;
    void accept(IDocVisitor *) override;
    void accept(const IDocVisitor *) override;
    void accept(IDocVisitor *) const override;
    void accept(const IDocVisitor *) const override;
    void setWrapper(DocThreadWrapper *) override;
    DocThreadWrapper *wrapper() override;
    void setName(const std::string &) override;
    void save() const;
    void saveAs(const std::string &); // implies internal renaming of doc
    void saveCopyAs(const std::string &) const;
};


class FootprintLibDocument : public IDocument {
private:
    std::string m_name;
    const std::string m_connName;
    bool m_activeState;
    DocThreadWrapper *m_wrapper;
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
    void accept(const IDocVisitor *) override;
    void accept(IDocVisitor *) const override;
    void accept(const IDocVisitor *) const override;
    void setWrapper(DocThreadWrapper *) override;
    DocThreadWrapper *wrapper() override;
    void setName(const std::string &) override;
    void save() const;
    void saveAs(const std::string &); // implies internal renaming of doc
    void saveCopyAs(const std::string &) const;
};
class SchDocument          : public IDocument {
private:
    std::string m_name;
    const std::string m_connName;
    bool m_activeState;
    DocThreadWrapper *m_wrapper;
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
    void accept(const IDocVisitor *) override;
    void accept(IDocVisitor *) const override;
    void accept(const IDocVisitor *) const override;
    void setWrapper(DocThreadWrapper *) override;
    DocThreadWrapper *wrapper() override;
    void setName(const std::string &) override;
    void save() const;
    void saveAs(const std::string &); // implies internal renaming of doc
    void saveCopyAs(const std::string &) const;
};
class PCBDocument          : public IDocument {
private:
    std::string m_name;
    const std::string m_connName;
    bool m_activeState;
    DocThreadWrapper *m_wrapper;
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
    void accept(const IDocVisitor *) override;
    void accept(IDocVisitor *) const override;
    void accept(const IDocVisitor *) const override;
    void setWrapper(DocThreadWrapper *) override;
    DocThreadWrapper *wrapper() override;
    void setName(const std::string &) override;
    void save() const;
    void saveAs(const std::string &); // implies internal renaming of doc
    void saveCopyAs(const std::string &) const;
};




#endif
