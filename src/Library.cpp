#include "Library.h"
#include <algorithm>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <system_error>
#include <utility>

namespace {
std::string lower(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(),
        [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return value;
}
bool safeField(const std::string& value) {
    return !value.empty() && value.find('|') == std::string::npos &&
           value.find('\n') == std::string::npos && value.find('\r') == std::string::npos;
}
bool prepareParent(const std::string& filename) {
    std::filesystem::path p(filename);
    if (!p.has_parent_path()) return true;
    std::error_code ec;
    std::filesystem::create_directories(p.parent_path(), ec);
    return !ec;
}
bool replaceFile(const std::string& temp, const std::string& target) {
    std::error_code ec;
    std::filesystem::rename(temp, target, ec);
    if (!ec) return true;
    std::filesystem::remove(target, ec);
    ec.clear();
    std::filesystem::rename(temp, target, ec);
    return !ec;
}
}

Library::Library(std::string booksFile, std::string membersFile, std::string transactionsFile)
    : booksFile_(std::move(booksFile)), membersFile_(std::move(membersFile)),
      transactionsFile_(std::move(transactionsFile)) {}

bool Library::load() {
    books_.clear(); members_.clear(); records_.clear();
    nextBookId_ = 1001; nextMemberId_ = 501;

    {
        std::ifstream in(booksFile_);
        std::string line;
        while (std::getline(in, line)) {
            std::istringstream row(line);
            std::string id, title, author, total, available;
            if (!std::getline(row,id,'|') || !std::getline(row,title,'|') ||
                !std::getline(row,author,'|') || !std::getline(row,total,'|') ||
                !std::getline(row,available)) continue;
            try {
                int i=std::stoi(id), t=std::stoi(total), a=std::stoi(available);
                if (i<=0 || !safeField(title) || !safeField(author) || t<0 || a<0 || a>t) continue;
                books_.emplace_back(i,title,author,t,a);
                nextBookId_=std::max(nextBookId_,i+1);
            } catch (...) {}
        }
        if (in.bad()) return false;
    }
    {
        std::ifstream in(membersFile_);
        std::string line;
        while (std::getline(in,line)) {
            std::istringstream row(line);
            std::string id,name,email;
            if (!std::getline(row,id,'|') || !std::getline(row,name,'|') || !std::getline(row,email)) continue;
            try {
                int i=std::stoi(id);
                if (i<=0 || !safeField(name) || !safeField(email)) continue;
                members_.emplace_back(i,name,email);
                nextMemberId_=std::max(nextMemberId_,i+1);
            } catch (...) {}
        }
        if (in.bad()) return false;
    }
    {
        std::ifstream in(transactionsFile_);
        std::string line;
        while (std::getline(in,line)) {
            std::istringstream row(line);
            std::string book,member,issued,returned,flag;
            if (!std::getline(row,book,'|') || !std::getline(row,member,'|') ||
                !std::getline(row,issued,'|') || !std::getline(row,returned,'|') ||
                !std::getline(row,flag)) continue;
            try {
                BorrowRecord r{std::stoi(book),std::stoi(member),issued,returned,flag=="1"};
                if (r.bookId>0 && r.memberId>0) records_.push_back(r);
            } catch (...) {}
        }
        if (in.bad()) return false;
    }
    return true;
}

bool Library::saveBooks() const {
    if (!prepareParent(booksFile_)) return false;
    const auto temp=booksFile_+".tmp";
    { std::ofstream out(temp,std::ios::trunc); if(!out) return false;
      for(const auto& b:books_) out<<b.getId()<<'|'<<b.getTitle()<<'|'<<b.getAuthor()<<'|'<<b.getTotalCopies()<<'|'<<b.getAvailableCopies()<<'\n';
      out.flush(); if(!out) return false; }
    return replaceFile(temp,booksFile_);
}
bool Library::saveMembers() const {
    if (!prepareParent(membersFile_)) return false;
    const auto temp=membersFile_+".tmp";
    { std::ofstream out(temp,std::ios::trunc); if(!out) return false;
      for(const auto& m:members_) out<<m.getId()<<'|'<<m.getName()<<'|'<<m.getEmail()<<'\n';
      out.flush(); if(!out) return false; }
    return replaceFile(temp,membersFile_);
}
bool Library::saveRecords() const {
    if (!prepareParent(transactionsFile_)) return false;
    const auto temp=transactionsFile_+".tmp";
    { std::ofstream out(temp,std::ios::trunc); if(!out) return false;
      for(const auto& r:records_) out<<r.bookId<<'|'<<r.memberId<<'|'<<r.issueDate<<'|'<<r.returnDate<<'|'<<(r.returned?1:0)<<'\n';
      out.flush(); if(!out) return false; }
    return replaceFile(temp,transactionsFile_);
}

Book* Library::findBookMutable(int id) {
    for(auto& b:books_) if(b.getId()==id) return &b;
    return nullptr;
}
const Book* Library::findBook(int id) const {
    for(const auto& b:books_) if(b.getId()==id) return &b;
    return nullptr;
}
const Member* Library::findMember(int id) const {
    for(const auto& m:members_) if(m.getId()==id) return &m;
    return nullptr;
}
bool Library::addBook(const std::string& title,const std::string& author,int copies) {
    if(!safeField(title)||!safeField(author)||copies<=0) return false;
    books_.emplace_back(nextBookId_,title,author,copies,copies);
    if(!saveBooks()){books_.pop_back();return false;}
    ++nextBookId_; return true;
}
bool Library::registerMember(const std::string& name,const std::string& email) {
    if(!safeField(name)||!safeField(email)) return false;
    members_.emplace_back(nextMemberId_,name,email);
    if(!saveMembers()){members_.pop_back();return false;}
    ++nextMemberId_; return true;
}
bool Library::issueBook(int bookId,int memberId) {
    auto* b=findBookMutable(bookId);
    if(!b||!findMember(memberId)||!b->issueCopy()) return false;
    records_.push_back({bookId,memberId,"CURRENT","",false});
    if(!saveBooks()||!saveRecords()) {
        b->returnCopy(); records_.pop_back(); saveBooks(); saveRecords(); return false;
    }
    return true;
}
bool Library::returnBook(int bookId,int memberId) {
    auto* b=findBookMutable(bookId);
    if(!b||!findMember(memberId)||!b->returnCopy()) return false;
    auto it=records_.end();
    for(auto i=records_.begin();i!=records_.end();++i)
        if(i->bookId==bookId&&i->memberId==memberId&&!i->returned) it=i;
    if(it==records_.end()){b->issueCopy();return false;}
    it->returned=true; it->returnDate="RETURNED";
    if(!saveBooks()||!saveRecords()) {
        it->returned=false; it->returnDate=""; b->issueCopy(); saveBooks(); saveRecords(); return false;
    }
    return true;
}
std::vector<const Book*> Library::searchByTitle(const std::string& q) const {
    std::vector<const Book*> result; const auto needle=lower(q);
    for(const auto& b:books_) if(lower(b.getTitle()).find(needle)!=std::string::npos) result.push_back(&b);
    return result;
}
std::vector<const Book*> Library::searchByAuthor(const std::string& q) const {
    std::vector<const Book*> result; const auto needle=lower(q);
    for(const auto& b:books_) if(lower(b.getAuthor()).find(needle)!=std::string::npos) result.push_back(&b);
    return result;
}
const std::vector<Book>& Library::getBooks() const{return books_;}
const std::vector<Member>& Library::getMembers() const{return members_;}
const std::vector<BorrowRecord>& Library::getRecords() const{return records_;}
