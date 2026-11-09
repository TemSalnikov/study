#include <iostream>
#include <memory>

// Выносим интерфейс, чтобы код не зависел от конкретной реализации
class IDatabase {
public:
    virtual ~IDatabase() = default;
    virtual void executeQuery(const std::string& query) = 0;
};

// Реальная база данных
class ProductionDatabase : public IDatabase {
public:
    void executeQuery(const std::string& query) override {
        std::cout << "Запрос к реальной БД: " << query << "\n";
    }
};

// Класс логики (зависит от интерфейса, а не от Одиночки!)
class UserManager {
private:
    IDatabase& db; // Ссылка на базу данных передается ИЗВНЕ
public:
    UserManager(IDatabase& database) : db(database) {}

    void createNewUser(const std::string& name) {
        db.executeQuery("INSERT INTO users VALUES ('" + name + "')");
    }
};

int main() {
    // Объект создается ОДИН раз на уровне точки входа (main)
    ProductionDatabase realDb;

    // Внедряем его в менеджер пользователей
    UserManager manager(realDb);
    manager.createNewUser("Алексей");

    return 0;
}
