#include <iostream>
#include <memory>

// 1. Интерфейс стратегии
class PaymentStrategy {
public:
    virtual ~PaymentStrategy() = default;
    virtual void collectPayment(int amount) = 0;
};

// 2. Вариант оплаты А: Кредитная карта
class CreditCardPayment : public PaymentStrategy {
public:
    void collectPayment(int amount) override {
        std::cout << "Списано " << amount << " руб. с банковской карты. 💳\n";
    }
};

// 3. Вариант оплаты Б: Система Быстрых Платежей (СБП)
class SbpPayment : public PaymentStrategy {
public:
    void collectPayment(int amount) override {
        std::cout << "Оплата " << amount << " руб. успешно проведена по QR-коду СБП. 📱\n";
    }
};

// 4. Контекст — сам магазин
class Order {
private:
    int totalCost = 0;
    std::unique_ptr<PaymentStrategy> paymentMethod; // Храним выбранную стратегию
public:
    void addProduct(int price) { totalCost += price; }
    
    // Позволяет динамически установить или поменять стратегию
    void setPaymentMethod(std::unique_ptr<PaymentStrategy> method) {
        paymentMethod = std::move(method);
    }

    void processOrder() {
        if (!paymentMethod) {
            std::cout << "Ошибка: Не выбран способ оплаты!\n";
            return;
        }
        paymentMethod->collectPayment(totalCost);
    }
};

// --- Как это использовать в коде ---
int main() {
    Order myOrder;
    myOrder.addProduct(1500);
    myOrder.addProduct(4500); // Итого 6000 руб.

    // Пользователь нажал на кнопку "Оплатить картой"
    myOrder.setPaymentMethod(std::make_unique<CreditCardPayment>());
    myOrder.processOrder();

    // Пользователь передумал, отменил и выбрал СБП
    std::cout << "\n--- Смена стратегии ---\n";
    myOrder.setPaymentMethod(std::make_unique<SbpPayment>());
    myOrder.processOrder();

    return 0;
}
