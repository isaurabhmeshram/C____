#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

using namespace std;

// Book class represents a book in the library
class Book {
private:
    string title;
    string author;
    string isbn;
    bool available;

public:
    Book() {
        available = true;
    }

    void setTitle(const string& bookTitle) {
        title = bookTitle;
    }

    void setAuthor(const string& bookAuthor) {
        author = bookAuthor;
    }

    void setISBN(const string& bookISBN) {
        isbn = bookISBN;
    }

    void setAvailability(bool isAvailable) {
        available = isAvailable;
    }

    string getTitle() const {
        return title;
    }

    string getAuthor() const {
        return author;
    }

    string getISBN() const {
        return isbn;
    }

    bool isAvailable() const {
        return available;
    }
};

// Library class manages the library operations
class Library {
private:
    string filename; // Name of the file to store book information

public:
    Library(const string& dataFile) {
        filename = dataFile;
    }

    void addBook(const Book& book) {
        ofstream file(filename, ios::app);
        if (!file) {
            cerr << "Error opening the file." << endl;
            return;
        }

        file << book.getTitle() << "," << book.getAuthor() << "," << book.getISBN() << "," << book.isAvailable() << endl;

        file.close();
    }

    void searchBooks(const string& title) {
        ifstream file(filename);
        if (!file) {
            cerr << "Error opening the file." << endl;
            return;
        }

        string line;
        while (getline(file, line)) {
            size_t pos = line.find(",");
            string bookTitle = line.substr(0, pos);

            if (bookTitle == title) {
                cout << line << endl;
                file.close();
                return;
            }
        }

        cout << "Book not found." << endl;
        file.close();
    }

    void borrowBook(const string& title) {
        fstream file(filename);
        if (!file) {
            cerr << "Error opening the file." << endl;
            return;
        }

        string line;
        size_t pos;
        bool found = false;
        while (getline(file, line)) {
            pos = line.find(",");
            string bookTitle = line.substr(0, pos);

            if (bookTitle == title) {
                if (line.substr(pos + 1) == "1") {
                    line.replace(pos + 1, 1, "0"); // Update book availability

                    // Add due date
                    time_t now = time(nullptr);
                    tm* date = localtime(&now);
                    string dueDate = to_string(date->tm_mday) + "/" + to_string(date->tm_mon + 1) + "/" + to_string(date->tm_year + 1900);
                    line += "," + dueDate;

                    found = true;
                    break;
                } else {
                    cout << "Book is not available for borrowing." << endl;
                    file.close();
                    return;
                }
            }
        }

        if (found) {
            ofstream tempFile("temp.txt");
            while (getline(file, line)) {
                tempFile << line << endl;
            }
            file.close();
            tempFile.close();

            remove(filename.c_str());
            rename("temp.txt", filename.c_str());

            cout << "Bookborrowed successfully." << endl;
        } else {
            cout << "Book not found." << endl;
            file.close();
        }
    }
};

int main() {
    Library library("books.txt");

    // Sample usage
    Book book1;
    book1.setTitle("Book 1");
    book1.setAuthor("Author 1");
    book1.setISBN("ISBN 1");
    library.addBook(book1);

    Book book2;
    book2.setTitle("Book 2");
    book2.setAuthor("Author 2");
    book2.setISBN("ISBN 2");
    library.addBook(book2);

    library.searchBooks("Book 1");

    library.borrowBook("Book 2");

    return 0;
}

