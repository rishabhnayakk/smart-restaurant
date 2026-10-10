#include "Order.h"

using namespace std;

Order::Order(int id, const string& cName, const string& fName, int qty, double prc)
    : orderId(id), customerName(cName), foodName(fName), quantity(qty), price(prc),
      prev(nullptr), next(nullptr) {
    // Automatically calculate totalBill
    totalBill = quantity * price;
    // Initialize status as "Pending"
    status = "Pending";
}
