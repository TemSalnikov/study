#include <iostream>
#include <string>
#include <memory>

// 1. Базовый интерфейс напитка
class Beverage {
public:
    virtual ~Beverage() = default;
    virtual std::string getDescription() const = 0;
    virtual double getCost() const = 0;
};

// 2. Конкретный базовый напиток (чистый Эспрессо)
class Espresso : public Beverage {
public:
    std::string getDescription() const override { return "Эспрессо"; }
    double getCost() const override { return 150.0; }
};

// 3. Базовый класс Декоратора (он САМ является Beverage и хранит внутри Beverage)
class CondimentDecorator : public Beverage {
protected:
    std::unique_ptr<Beverage> wrappedBeverage; // Тот самый объект, который мы "одеваем"
public:
    CondimentDecorator(std::unique_ptr<Beverage> beverage) 
        : wrappedBeverage(std::move(beverage)) {}
};

// 4. Конкретный декоратор №1: Добавка "Молоко"
class Milk : public CondimentDecorator {
public:
    Milk(std::unique_ptr<Beverage> beverage) : CondimentDecorator(std::move(beverage)) {}

    std::string getDescription() const override {
        // Берем описание того, что внутри, и добавляем свое
        return wrappedBeverage->getDescription() + ", Молоко";
    }

    double getCost() const override {
        // Берем цену того, что внутри, и добавляем цену молока
        return wrappedBeverage->getCost() + 40.0;
    }
};

// 5. Конкретный декоратор №2: Добавка "Сироп"
class Syrup : public CondimentDecorator {
public:
    Syrup(std::unique_ptr<Beverage> beverage) : CondimentDecorator(std::move(beverage)) {}

    std::string getDescription() const override {
        return wrappedBeverage->getDescription() + ", Сироп";
    }

    double getCost() const override {
        return wrappedBeverage->getCost() + 30.0;
    }
};

// --- Как это использовать в коде ---
int main() {
    // 1. Заказываем просто эспрессо
    std::unique_ptr<Beverage> order = std::make_unique<Espresso>();
    std::cout << order->getDescription() << " = " << order->getCost() << " руб.\n";

    // 2. Добавляем в этот же заказ молоко (оборачиваем старый заказ в Milk)
    order = std::make_unique<Milk>(std::move(order));
    std::cout << order->getDescription() << " = " << order->getCost() << " руб.\n";

    // 3. Туда же добавляем сироп (оборачиваем получившуюся матрешку в Syrup)
    order = std::make_unique<Syrup>(std::move(order));
    std::cout << order->getDescription() << " = " << order->getCost() << " руб.\n";

    // В итоге выведет: Эспрессо, Молоко, Сироп = 220 руб.
    return 0;
}
