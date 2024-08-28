#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <ctime>

using namespace std;

// Base Class: Entry
class Entry {
protected:
    string date;
    string content;
public:
    Entry(string content) {
        this->content = content;
        time_t now = time(0);
        date = ctime(&now);
    }

    virtual void displayEntry() const {
        cout << "Date: " << date;
        cout << "Content: " << content << endl;
    }

    string getDate() const {
        return date;
    }

    string getContent() const {
        return content;
    }

    void setContent(string newContent) {
        content = newContent;
    }

    virtual ~Entry() {}
};

// Derived Class: TextEntry
class TextEntry : public Entry {
public:
    TextEntry(string content) : Entry(content) {}

    void displayEntry() const override {
        Entry::displayEntry();
    }
};

// Diary Class to manage Entries
class Diary {
private:
    vector<Entry*> entries;
    string fileName;

    void loadEntries() {
        ifstream file(fileName);
        if (!file.is_open()) return;

        string content;
        while (getline(file, content)) {
            entries.push_back(new TextEntry(content));
        }
        file.close();
    }

    void saveEntries() const {
        ofstream file(fileName);
        if (!file.is_open()) return;

        for (const auto& entry : entries) {
            file << entry->getContent() << endl;
        }
        file.close();
    }

public:
    Diary(string fileName) : fileName(fileName) {
        loadEntries();
    }

    void addEntry(Entry* entry) {
        entries.push_back(entry);
        saveEntries();
    }

    void deleteEntry(int index) {
        if (index < 0 || index >= entries.size()) {
            cout << "Invalid entry number!" << endl;
            return;
        }
        delete entries[index];
        entries.erase(entries.begin() + index);
        saveEntries();
        cout << "Entry deleted successfully." << endl;
    }

    void editEntry(int index, string newContent) {
        if (index < 0 || index >= entries.size()) {
            cout << "Invalid entry number!" << endl;
            return;
        }
        entries[index]->setContent(newContent);
        saveEntries();
        cout << "Entry edited successfully." << endl;
    }

    void displayAllEntries() const {
        if(entries.empty()) {
            cout << "No entries found!" << endl;
            return;
        }

        for (size_t i = 0; i < entries.size(); ++i) {
            cout << "Entry #" << i + 1 << endl;
            entries[i]->displayEntry();
            cout << "----------------------------" << endl;
        }
    }

   void searchEntriesByDate(string searchDate) const {
    bool found = false;
    for (const auto& entry : entries) {
        string entryDate = entry->getDate();

        // Extract only the day, month, and year from the date string
        string entryDayMonthYear = entryDate.substr(8, 2) + " " + entryDate.substr(4, 3) + " " + entryDate.substr(20, 4);

        // Compare the formatted date with the user input
        if (entryDayMonthYear == searchDate) {
            entry->displayEntry();
            found = true;
            cout << "----------------------------" << endl;
        }
    }
    if (!found) {
        cout << "No entries found for the date: " << searchDate << endl;
    }
}


    ~Diary() {
        for (auto entry : entries) {
            delete entry;
        }
    }
};

// User Class for managing user interaction
class User {
private:
    string name;
    Diary* diaries[3];
    string categories[3] = { "Work", "Travel", "Personal" };

    void waitForMainMenu() {
        cout << "\nPress Enter to go to the main menu...";
        cin.ignore();
        cin.get();
        system("CLS");
    }

    int selectCategory() {
        int categoryChoice;
        do {
            cout << "Select Category:" << endl;
            for (int i = 0; i < 3; i++) {
                cout << i + 1 << ". " << categories[i] << endl;
            }
            cout << "Enter your choice: ";
            cin >> categoryChoice;
            system("CLS");
        } while (categoryChoice < 1 || categoryChoice > 3);

        return categoryChoice - 1;  // Adjusting for 0-based index
    }

public:
    User(string name) : name(name) {
        diaries[0] = new Diary("work_diary.txt");
        diaries[1] = new Diary("travel_diary.txt");
        diaries[2] = new Diary("personal_diary.txt");
    }

    void addTextEntry() {
        system("CLS");
        int categoryIndex = selectCategory();
        string content;
        cout << "Enter your text entry: ";
        cin.ignore();
        getline(cin, content);
        diaries[categoryIndex]->addEntry(new TextEntry(content));
        cout << "Entry added successfully.";
        waitForMainMenu();
    }

    void deleteEntry() {
        system("CLS");
        int categoryIndex = selectCategory();
        int entryNumber;
        cout << "Enter the entry number to delete: ";
        cin >> entryNumber;
        diaries[categoryIndex]->deleteEntry(entryNumber - 1);  // Adjusting for 0-based index
        waitForMainMenu();
    }

    void editEntry() {
        system("CLS");
        int categoryIndex = selectCategory();
        int entryNumber;
        string newContent;
        cout << "Enter the entry number to edit: ";
        cin >> entryNumber;
        cin.ignore();  // Ignore leftover newline character
        cout << "Enter the new content: ";
        getline(cin, newContent);
        diaries[categoryIndex]->editEntry(entryNumber - 1, newContent);  // Adjusting for 0-based index
        waitForMainMenu();
    }

    void viewEntries() {
        system("CLS");
        int categoryIndex = selectCategory();
        diaries[categoryIndex]->displayAllEntries();
        waitForMainMenu();
    }

    void searchByDate() {
        system("CLS");
        int categoryIndex = selectCategory();
        string date;
        cout << "Enter the date to search (format: day month year): ";
        cin.ignore();
        getline(cin, date);
        diaries[categoryIndex]->searchEntriesByDate(date);
        waitForMainMenu();
    }


    void showMenu() {
        int choice;
        do {
            cout << "\nWelcome to your personal Diary " << name << "! \n\n" << endl;
            cout << "1. Add Text Entry" << endl;
            cout << "2. View All Entries" << endl;
            cout << "3. Search Entries by Date" << endl;
            cout << "4. Edit Entry" << endl;
            cout << "5. Delete Entry" << endl;
            cout << "6. Exit" << endl;
            cout << "Enter your choice: ";
            cin >> choice;

            system("CLS");

            switch (choice) {
            case 1:
                addTextEntry();
                break;
            case 2:
                viewEntries();
                break;
            case 3:
                searchByDate();
                break;
            case 4:
                editEntry();
                break;
            case 5:
                deleteEntry();
                break;
            case 6:
                cout << "Exiting..." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
                waitForMainMenu();
            }
        } while (choice != 6);
    }

    ~User() {
        for (int i = 0; i < 3; i++) {
            delete diaries[i];
        }
    }
};

int main() {
    string userName = "USER1";
    User user(userName);
    user.showMenu();

    return 0;
}
