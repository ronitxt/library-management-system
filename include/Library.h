#pragma once
#include "Book.h"
#include "Member.h"
#include <string>
#include <vector>

struct BorrowRecord {
    int bookId = 0;
    int memberId = 0;
    std::string issueDate;
    std::string returnDate;
    bool returned = false;
};

class Library {
public:
    Library(std::string booksFile, std::string membersFile, std::string transactionsFile);

    bool load();
    bool addBook(const std::string& title, const std::string& author, int copies);
    bool registerMember(const std::string& name, const std::string& email);
    bool issueBook(int bookId, int memberId);
    bool returnBook(int bookId, int memberId);
    const Book* findBook(int id) const;
    const Member* findMember(int id) const;
    std::vector<const Book*> searchByTitle(const std::string& query) const;
    std::vector<const Book*> searchByAuthor(const std::string& query) const;

    const std::vector<Book>& getBooks() const;
    const std::vector<Member>& getMembers() const;
    const std::vector<BorrowRecord>& getRecords() const;

private:
    std::string booksFile_;
    std::string membersFile_;
    std::string transactionsFile_;
    std::vector<Book> books_;
    std::vector<Member> members_;
    std::vector<BorrowRecord> records_;
    int nextBookId_ = 1001;
    int nextMemberId_ = 501;

    Book* findBookMutable(int id);
    bool saveBooks() const;
    bool saveMembers() const;
    bool saveRecords() const;
};
