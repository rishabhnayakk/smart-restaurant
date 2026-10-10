#include "Restaurant.h"
#include <iostream>
#include <limits>

using namespace std;

void displayMenu() {
    cout << "\n===========================================\n";
    cout << " SMART RESTAURANT MANAGEMENT SYSTEM\n";
    cout << "===========================================\n";
    cout << "1. Add Order\n";
    cout << "2. Search Order\n";
    cout << "3. Update Order\n";
    cout << "4. Delete Order\n";
    cout << "5. Display Orders\n";
    cout << "6. Update Order Status\n";
    cout << "7. Display Orders By Status\n";
    cout << "8. Dashboard\n";
    cout << "9. Total Revenue\n";
    cout << "10. About Project\n";
    cout << "11. Exit\n";
    cout << "===========================================\n";
    cout << "Enter your choice: ";
}

int main() {
    Restaurant restaurant;
    
    // Load previously saved data on startup
    restaurant.loadOrdersFromFile();
    
    int choice;

    do {
        restaurant.clearScreen();
        displayMenu();
        
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\nInvalid input. Please enter a number.\n";
            restaurant.pauseScreen();
            continue;
        }

        switch (choice) {
            case 1: restaurant.addOrder(); break;
            case 2: restaurant.searchOrder(); break;
            case 3: restaurant.updateOrder(); break;
            case 4: restaurant.deleteOrder(); break;
            case 5: restaurant.displayOrders(); break;
            case 6: restaurant.updateOrderStatus(); break;
            case 7: restaurant.displayOrdersByStatus(); break;
            case 8: restaurant.displayDashboard(); break;
            case 9: restaurant.calculateTotalRevenue(); break;
            case 10: restaurant.aboutProject(); break;
            case 11: 
                restaurant.displayExitSummary(); 
                break;
            default:
                cout << "\nInvalid choice. Please select between 1 and 11.\n";
                restaurant.pauseScreen();
        }
    } while (choice != 11);

    return 0;
}
