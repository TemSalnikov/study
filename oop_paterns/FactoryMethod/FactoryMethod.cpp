#include <iostream>
#include <memory>
#include <string>

// 1. Абстрактный продукт — общий интерфейс для всех врагов
class Enemy {
public:
    virtual ~Enemy() = default;
    virtual void attack() = 0; // Каждому врагу придется реализовать атаку
};

// 2. Конкретные продукты
class Zombie : public Enemy {
public:
    void attack() override { std::cout << "Зомби кусает! 🧟\n"; }
};

class Robot : public Enemy {
public:
    void attack() override { std::cout << "Робот стреляет лазером! 🤖\n"; }
};

// 3. Абстрактная фабрика (Создатель)
class Spawner {
public:
    virtual ~Spawner() = default;
    // Тот самый фабричный метод
    virtual std::unique_ptr<Enemy> createEnemy() = 0;
};

// 4. Конкретные фабрики
class ZombieSpawner : public Spawner {
public:
    std::unique_ptr<Enemy> createEnemy() override {
        return std::make_unique<Zombie>();
    }
};

class RobotSpawner : public Spawner {
public:
    std::unique_ptr<Enemy> createEnemy() override {
        return std::make_unique<Robot>();
    }
};

// --- Как это использовать в коде ---
int main() {
    // Представим, что игра перешла на уровень "Технологии"
    std::unique_ptr<Spawner> currentSpawner = std::make_unique<RobotSpawner>();
    
    // Создаем врага, не зная, КТО он конкретно. Знаем лишь, что это Enemy.
    std::unique_ptr<Enemy> enemy = currentSpawner->createEnemy();
    enemy->attack(); // Выведет: Робот стреляет лазером! 🤖

    return 0;
}
