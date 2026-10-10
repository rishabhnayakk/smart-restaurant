#include "Restaurant.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <limits>
#include <cstdlib>

using namespace std;

Restaurant::Restaurant() : head(nullptr), tail(nullptr), nextOrderId(1001) {}

Restaurant::~Restaurant() {
    Order* current = head;
    while (current != nullptr) {
        Order* nextNode = current->next;
        delete current;
        current = nextNode;
    }
}

// Phase 4 UI Functions

void Restaurant::clearScreen() const {
    system("cls"); // For Windows
}

void Restaurant::pauseScreen() const {
    system("pause");
}

void Restaurant::printHeader(const string& title) const {
    cout << "\n====================================================\n";
    cout << " " << title << "\n";
    cout << "====================================================\n\n";
}

void Restaurant::printFooter() const {
    cout << "\n====================================================\n";
}

// Phase 4 File Persistence

void Restaurant::saveOrdersToFile() const {
    ofstream outFile("orders.txt");
    if (!outFile) {
        cerr << "Error: Could not open orders.txt for saving.\n";
        return;
    }
    Order* current = head;
    while (current != nullptr) {
        outFile << current->orderId << ","
                << current->customerName << ","
                << current->foodName << ","
                << current->quantity << ","
                << current->price << ","
                << current->totalBill << ","
                << current->status << "\n";
        current = current->next;
    }
    outFile.close();
}

void Restaurant::loadOrdersFromFile() {
    ifstream inFile("orders.txt");
    if (!inFile) {
        // File doesn't exist yet, just start fresh.
        return;
    }
    
    string line;
    int maxId = 1000;
    while (getline(inFile, line)) {
        if (line.empty()) continue;
        
        stringstream ss(line);
        string token;
        
        int id;
        string cName, fName, status;
        int qty;
        double price, bill;
        
        getline(ss, token, ','); id = stoi(token);
        getline(ss, cName, ',');
        getline(ss, fName, ',');
        getline(ss, token, ','); qty = stoi(token);
        getline(ss, token, ','); price = stod(token);
        getline(ss, token, ','); bill = stod(token);
        getline(ss, status, ',');
        
        Order* newOrder = new Order(id, cName, fName, qty, price);
        newOrder->totalBill = bill;
        newOrder->status = status;
        
        if (head == nullptr) {
            head = newOrder;
            tail = newOrder;
        } else {
            tail->next = newOrder;
            newOrder->prev = tail;
            tail = newOrder;
        }
        if (id > maxId) {
            maxId = id;
        }
    }
    inFile.close();
    nextOrderId = maxId + 1;
}

// Business Logic with UI and Validation

void Restaurant::addOrder() {
    clearScreen();
    printHeader("SMART RESTAURANT MANAGEMENT SYSTEM - ADD ORDER");
    
    string customerName, foodName;
    int quantity;
    double price;

    while (true) {
        cout << "Enter Customer Name: ";
        getline(cin >> ws, customerName);
        if (!customerName.empty()) break;
        cout << "Name cannot be empty. Please try again.\n";
    }

    while (true) {
        cout << "Enter Food Name: ";
        getline(cin >> ws, foodName);
        if (!foodName.empty()) break;
        cout << "Food name cannot be empty. Please try again.\n";
    }

    while (true) {
        cout << "Enter Quantity: ";
        if (cin >> quantity && quantity > 0) break;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Quantity must be a positive integer. Please try again.\n";
    }

    while (true) {
        cout << "Enter Price per item: ";
        if (cin >> price && price > 0) break;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Price must be a positive number. Please try again.\n";
    }

    Order* newOrder = new Order(nextOrderId++, customerName, foodName, quantity, price);

    if (head == nullptr) {
        head = newOrder;
        tail = newOrder;
    } else {
        tail->next = newOrder;
        newOrder->prev = tail;
        tail = newOrder;
    }
    
    saveOrdersToFile(); // Auto save
    
    cout << "\nOrder added successfully! Order ID: " << newOrder->orderId << "\n";
    printFooter();
    pauseScreen();
}

void Restaurant::displayOrders() const {
    clearScreen();
    printHeader("SMART RESTAURANT MANAGEMENT SYSTEM - DISPLAY ORDERS");
    
    if (head == nullptr) {
        cout << "No orders exist in the system.\n";
        printFooter();
        pauseScreen();
        return;
    }

    cout << left 
              << setw(10) << "Order ID" 
              << setw(20) << "Customer" 
              << setw(20) << "Food" 
              << setw(10) << "Qty" 
              << setw(15) << "Price" 
              << setw(15) << "Bill" 
              << "Status\n";
    cout << "----------------------------------------------------------------------------------------------------\n";

    Order* current = head;
    while (current != nullptr) {
        cout << left 
                  << setw(10) << current->orderId 
                  << setw(20) << current->customerName 
                  << setw(20) << current->foodName 
                  << setw(10) << current->quantity 
                  << "Rs. " << setw(12) << fixed << setprecision(2) << current->price 
                  << "Rs. " << setw(12) << fixed << setprecision(2) << current->totalBill 
                  << current->status << "\n";
        current = current->next;
    }
    printFooter();
    pauseScreen();
}

void Restaurant::searchOrder() const {
    clearScreen();
    printHeader("SMART RESTAURANT MANAGEMENT SYSTEM - SEARCH ORDER");
    
    int searchId;
    cout << "Enter Order ID to search: ";
    if (!(cin >> searchId)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Returning to menu.\n";
        pauseScreen();
        return;
    }

    Order* current = head;
    while (current != nullptr) {
        if (current->orderId == searchId) {
            cout << "\nOrder Found:\n";
            cout << "Order ID     : " << current->orderId << "\n";
            cout << "Customer Name: " << current->customerName << "\n";
            cout << "Food Name    : " << current->foodName << "\n";
            cout << "Quantity     : " << current->quantity << "\n";
            cout << "Price        : Rs. " << fixed << setprecision(2) << current->price << "\n";
            cout << "Total Bill   : Rs. " << fixed << setprecision(2) << current->totalBill << "\n";
            cout << "Status       : " << current->status << "\n";
            printFooter();
            pauseScreen();
            return;
        }
        current = current->next;
    }
    cout << "\nOrder Not Found.\n";
    printFooter();
    pauseScreen();
}

void Restaurant::updateOrder() {
    clearScreen();
    printHeader("SMART RESTAURANT MANAGEMENT SYSTEM - UPDATE ORDER");
    
    int updateId;
    cout << "Enter Order ID to update: ";
    if (!(cin >> updateId)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input.\n";
        pauseScreen();
        return;
    }

    Order* current = head;
    while (current != nullptr) {
        if (current->orderId == updateId) {
            cout << "\nOrder Found. Enter new details:\n";
            
            while (true) {
                cout << "Enter Customer Name: ";
                getline(cin >> ws, current->customerName);
                if (!current->customerName.empty()) break;
                cout << "Name cannot be empty.\n";
            }

            while (true) {
                cout << "Enter Food Name: ";
                getline(cin >> ws, current->foodName);
                if (!current->foodName.empty()) break;
                cout << "Food name cannot be empty.\n";
            }

            while (true) {
                cout << "Enter Quantity: ";
                if (cin >> current->quantity && current->quantity > 0) break;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Quantity must be > 0.\n";
            }

            while (true) {
                cout << "Enter Price per item: ";
                if (cin >> current->price && current->price > 0) break;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Price must be > 0.\n";
            }

            current->totalBill = current->quantity * current->price;

            saveOrdersToFile();

            cout << "\nOrder updated successfully!\n";
            printFooter();
            pauseScreen();
            return;
        }
        current = current->next;
    }
    cout << "\nOrder Not Found.\n";
    printFooter();
    pauseScreen();
}

void Restaurant::deleteOrder() {
    clearScreen();
    printHeader("SMART RESTAURANT MANAGEMENT SYSTEM - DELETE ORDER");
    
    int deleteId;
    cout << "Enter Order ID to delete: ";
    if (!(cin >> deleteId)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input.\n";
        pauseScreen();
        return;
    }

    Order* current = head;
    while (current != nullptr) {
        if (current->orderId == deleteId) {
            if (current == head && current == tail) {
                head = nullptr;
                tail = nullptr;
            } else if (current == head) {
                head = head->next;
                head->prev = nullptr;
            } else if (current == tail) {
                tail = tail->prev;
                tail->next = nullptr;
            } else {
                current->prev->next = current->next;
                current->next->prev = current->prev;
            }

            delete current;
            saveOrdersToFile();
            
            cout << "\nOrder deleted successfully!\n";
            printFooter();
            pauseScreen();
            return;
        }
        current = current->next;
    }
    cout << "\nOrder Not Found.\n";
    printFooter();
    pauseScreen();
}

void Restaurant::calculateTotalRevenue() const {
    clearScreen();
    printHeader("SMART RESTAURANT MANAGEMENT SYSTEM - REVENUE");
    
    if (head == nullptr) {
        cout << "No Orders Available.\n";
        printFooter();
        pauseScreen();
        return;
    }

    int totalOrders = 0;
    double totalRevenue = 0.0;
    Order* current = head;
    
    while (current != nullptr) {
        totalOrders++;
        totalRevenue += current->totalBill;
        current = current->next;
    }

    cout << "Today's Total Revenue\n";
    cout << "----------------------------------------\n";
    cout << "Total Orders  : " << totalOrders << "\n";
    cout << "Total Revenue : Rs. " << fixed << setprecision(2) << totalRevenue << "\n";
    printFooter();
    pauseScreen();
}

void Restaurant::updateOrderStatus() {
    clearScreen();
    printHeader("SMART RESTAURANT MANAGEMENT SYSTEM - UPDATE STATUS");
    
    int updateId;
    cout << "Enter Order ID to update status: ";
    if (!(cin >> updateId)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input.\n";
        pauseScreen();
        return;
    }

    Order* current = head;
    while (current != nullptr) {
        if (current->orderId == updateId) {
            cout << "\nSelect Status\n";
            cout << "1 Pending\n";
            cout << "2 Preparing\n";
            cout << "3 Ready\n";
            cout << "4 Served\n";
            cout << "5 Completed\n";
            cout << "6 Cancelled\n";
            
            int statusChoice;
            while (true) {
                cout << "Enter choice (1-6): ";
                if (cin >> statusChoice && statusChoice >= 1 && statusChoice <= 6) break;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid choice. Please enter a number between 1 and 6.\n";
            }

            switch (statusChoice) {
                case 1: current->status = "Pending"; break;
                case 2: current->status = "Preparing"; break;
                case 3: current->status = "Ready"; break;
                case 4: current->status = "Served"; break;
                case 5: current->status = "Completed"; break;
                case 6: current->status = "Cancelled"; break;
            }
            
            saveOrdersToFile();
            
            cout << "\nOrder status updated to " << current->status << " successfully!\n";
            printFooter();
            pauseScreen();
            return;
        }
        current = current->next;
    }
    cout << "\nOrder Not Found.\n";
    printFooter();
    pauseScreen();
}

void Restaurant::displayDashboard() const {
    clearScreen();
    printHeader("SMART RESTAURANT DASHBOARD");
    
    int totalOrders = 0;
    int pending = 0, preparing = 0, ready = 0, served = 0, completed = 0, cancelled = 0;
    double todaysRevenue = 0.0;

    Order* current = head;
    while (current != nullptr) {
        totalOrders++;
        todaysRevenue += current->totalBill;
        
        if (current->status == "Pending") pending++;
        else if (current->status == "Preparing") preparing++;
        else if (current->status == "Ready") ready++;
        else if (current->status == "Served") served++;
        else if (current->status == "Completed") completed++;
        else if (current->status == "Cancelled") cancelled++;
        
        current = current->next;
    }

    cout << left << setw(20) << "Total Orders" << ": " << totalOrders << "\n";
    cout << left << setw(20) << "Pending" << ": " << pending << "\n";
    cout << left << setw(20) << "Preparing" << ": " << preparing << "\n";
    cout << left << setw(20) << "Ready" << ": " << ready << "\n";
    cout << left << setw(20) << "Served" << ": " << served << "\n";
    cout << left << setw(20) << "Completed" << ": " << completed << "\n";
    cout << left << setw(20) << "Cancelled" << ": " << cancelled << "\n\n";
    cout << left << setw(20) << "Today's Revenue" << ": Rs. " << fixed << setprecision(2) << todaysRevenue << "\n";
    printFooter();
    pauseScreen();
}

void Restaurant::displayOrdersByStatus() const {
    clearScreen();
    printHeader("SMART RESTAURANT MANAGEMENT SYSTEM - STATUS FILTER");
    
    cout << "Select Status to Filter\n";
    cout << "1 Pending\n";
    cout << "2 Preparing\n";
    cout << "3 Ready\n";
    cout << "4 Served\n";
    cout << "5 Completed\n";
    cout << "6 Cancelled\n";
    
    int choice;
    while (true) {
        cout << "Enter choice (1-6): ";
        if (cin >> choice && choice >= 1 && choice <= 6) break;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid choice. Please enter a number between 1 and 6.\n";
    }

    string targetStatus = "";
    switch (choice) {
        case 1: targetStatus = "Pending"; break;
        case 2: targetStatus = "Preparing"; break;
        case 3: targetStatus = "Ready"; break;
        case 4: targetStatus = "Served"; break;
        case 5: targetStatus = "Completed"; break;
        case 6: targetStatus = "Cancelled"; break;
    }

    bool found = false;
    Order* current = head;
    
    cout << "\n" << left 
              << setw(10) << "Order ID" 
              << setw(20) << "Customer" 
              << setw(20) << "Food" 
              << setw(10) << "Qty" 
              << setw(15) << "Price" 
              << setw(15) << "Bill" 
              << "Status\n";
    cout << "----------------------------------------------------------------------------------------------------\n";

    while (current != nullptr) {
        if (current->status == targetStatus) {
            found = true;
            cout << left 
                      << setw(10) << current->orderId 
                      << setw(20) << current->customerName 
                      << setw(20) << current->foodName 
                      << setw(10) << current->quantity 
                      << "Rs. " << setw(12) << fixed << setprecision(2) << current->price 
                      << "Rs. " << setw(12) << fixed << setprecision(2) << current->totalBill 
                      << current->status << "\n";
        }
        current = current->next;
    }

    if (!found) {
        cout << "No Orders Found for status: " << targetStatus << "\n";
    }
    printFooter();
    pauseScreen();
}

void Restaurant::aboutProject() const {
    clearScreen();
    printHeader("ABOUT PROJECT");
    cout << "Project Name  : Smart Restaurant Management System\n";
    cout << "Developer     : Rishabh Nayak\n";
    cout << "Language      : C++17\n";
    cout << "Data Structure: Doubly Linked List\n";
    cout << "Version       : 4.0\n";
    cout << "Description   : A complete, robust, scalable application managing restaurant orders,\n";
    cout << "                sales, and status flows in memory with automatic file persistence.\n";
    cout << "Future Scope  : Login System, MySQL Database Integration, Analytics Dashboard,\n";
    cout << "                Payment Gateway Integration, GUI.\n";
    printFooter();
    pauseScreen();
}

void Restaurant::displayExitSummary() const {
    clearScreen();
    
    int totalOrders = 0;
    int completedOrders = 0;
    int cancelledOrders = 0;
    double todaysRevenue = 0.0;

    Order* current = head;
    while (current != nullptr) {
        totalOrders++;
        todaysRevenue += current->totalBill;
        if (current->status == "Completed") completedOrders++;
        if (current->status == "Cancelled") cancelledOrders++;
        current = current->next;
    }

    cout << "\n========================================\n";
    cout << "               Thank You\n\n";
    cout << "           Today's Summary\n";
    cout << "----------------------------------------\n";
    cout << left << setw(20) << "Total Orders" << ": " << totalOrders << "\n";
    cout << left << setw(20) << "Completed Orders" << ": " << completedOrders << "\n";
    cout << left << setw(20) << "Cancelled Orders" << ": " << cancelledOrders << "\n";
    cout << left << setw(20) << "Revenue" << ": Rs. " << fixed << setprecision(2) << todaysRevenue << "\n";
    cout << "----------------------------------------\n";
    cout << "      Data Saved Successfully\n";
    cout << "========================================\n\n";
}
