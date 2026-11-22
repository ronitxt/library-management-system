#include "Library.h"
#include <iostream>
#include <limits>
#include <string>

namespace {
int readInt(const std::string& prompt) {
    int value;
    std::cout<<prompt;
    if(!(std::cin>>value)){std::cin.clear();std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');return -1;}
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    return value;
}
std::string readText(const std::string& prompt) {
    std::string value; std::cout<<prompt; std::getline(std::cin,value); return value;
}
void showBook(const Book& b) {
    std::cout<<"ID: "<<b.getId()<<" | "<<b.getTitle()<<" | Author: "<<b.getAuthor()
             <<" | Available: "<<b.getAvailableCopies()<<'/'<<b.getTotalCopies()<<'\n';
}
}

int main() {
    Library library("data/books.txt","data/members.txt","data/transactions.txt");
    if(!library.load()){std::cerr<<"Unable to load library files. Check file permissions.\n";return 1;}
    while(true) {
        std::cout<<"\n=====================================\n"
                 <<"       LIBRARY MANAGEMENT SYSTEM\n"
                 <<"=====================================\n"
                 <<"1. Add new book\n2. Register new member\n3. Display all books\n"
                 <<"4. Issue book\n5. Return book\n6. Search book by title\n"
                 <<"7. Search book by author\n8. Search member\n9. View borrowing records\n10. Exit\n";
        int choice=readInt("Enter your choice: ");
        if(choice==1) {
            auto title=readText("Book title: "); auto author=readText("Author: ");
            int copies=readInt("Number of copies: ");
            if(library.addBook(title,author,copies)) {
                const auto& b=library.getBooks().back(); std::cout<<"Book added. Book ID: "<<b.getId()<<'\n';
            } else std::cout<<"Could not add book. Check title, author and copy count.\n";
        } else if(choice==2) {
            auto name=readText("Member name: "); auto email=readText("Member email: ");
            if(library.registerMember(name,email)) std::cout<<"Member registered. Member ID: "<<library.getMembers().back().getId()<<'\n';
            else std::cout<<"Could not register member. Check the details.\n";
        } else if(choice==3) {
            if(library.getBooks().empty()) std::cout<<"No books found.\n";
            for(const auto& b:library.getBooks()) showBook(b);
        } else if(choice==4||choice==5) {
            int book=readInt("Book ID: "); int member=readInt("Member ID: ");
            bool ok=choice==4?library.issueBook(book,member):library.returnBook(book,member);
            std::cout<<(ok?(choice==4?"Book issued successfully.\n":"Book returned successfully.\n"):"Operation failed. Check IDs, availability and active borrowing record.\n");
        } else if(choice==6||choice==7) {
            auto q=readText(choice==6?"Enter title to search: ":"Enter author to search: ");
            auto found=choice==6?library.searchByTitle(q):library.searchByAuthor(q);
            if(found.empty()) std::cout<<"No matching books found.\n";
            for(const auto* b:found) showBook(*b);
        } else if(choice==8) {
            int id=readInt("Member ID: "); const auto* m=library.findMember(id);
            if(!m) std::cout<<"Member not found.\n";
            else std::cout<<"Member ID: "<<m->getId()<<" | Name: "<<m->getName()<<" | Email: "<<m->getEmail()<<'\n';
        } else if(choice==9) {
            const auto& records=library.getRecords();
            if(records.empty()) std::cout<<"No borrowing records.\n";
            for(const auto& r:records)
                std::cout<<"Book "<<r.bookId<<" | Member "<<r.memberId<<" | Issued: "<<r.issueDate
                         <<" | Status: "<<(r.returned?"Returned":"Issued")<<'\n';
        } else if(choice==10) {
            std::cout<<"Thank you for using the Library Management System.\n"; break;
        } else std::cout<<"Invalid choice. Please select 1 to 10.\n";
    }
    return 0;
}
