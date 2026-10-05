// Lab 01: a null pointer in a container of raw pointers.
#include <vector>

struct Order {
    long id;
    int qty;
};

int totalQty(const std::vector<Order*>& orders) {
    int total = 0;
    for (const Order* o : orders) total += o->qty;   // crashes on the nullptr
    return total;
}

int main() {
    std::vector<Order*> orders{new Order{1, 10}, nullptr, new Order{3, 5}};
    return totalQty(orders);
}
