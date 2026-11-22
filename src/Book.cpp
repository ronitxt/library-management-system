#include "Book.h"
#include <utility>

Book::Book(int id, std::string title, std::string author, int totalCopies, int availableCopies)
    : id_(id), title_(std::move(title)), author_(std::move(author)),
      totalCopies_(totalCopies), availableCopies_(availableCopies) {}

int Book::getId() const { return id_; }
const std::string& Book::getTitle() const { return title_; }
const std::string& Book::getAuthor() const { return author_; }
int Book::getTotalCopies() const { return totalCopies_; }
int Book::getAvailableCopies() const { return availableCopies_; }

bool Book::issueCopy() {
    if (availableCopies_ <= 0) return false;
    --availableCopies_;
    return true;
}

bool Book::returnCopy() {
    if (availableCopies_ >= totalCopies_) return false;
    ++availableCopies_;
    return true;
}
