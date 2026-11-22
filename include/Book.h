#pragma once
#include <string>

class Book {
public:
    Book() = default;
    Book(int id, std::string title, std::string author, int totalCopies, int availableCopies);

    int getId() const;
    const std::string& getTitle() const;
    const std::string& getAuthor() const;
    int getTotalCopies() const;
    int getAvailableCopies() const;

    bool issueCopy();
    bool returnCopy();

private:
    int id_ = 0;
    std::string title_;
    std::string author_;
    int totalCopies_ = 0;
    int availableCopies_ = 0;
};
