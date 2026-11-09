#include <iostream>
#include <string>

class DatabaseConfig {
public:
    // Глобальная точка доступа к единственному экземпляру
    static DatabaseConfig& getInstance() {
        // Инициализируется лениво (при первом вызове) и гарантированно потокобезопасно
        static DatabaseConfig instance; 
        return instance;
    }

    // Жестко запрещаем копирование и присваивание объекта
    DatabaseConfig(const DatabaseConfig&) = delete;
    DatabaseConfig& operator=(const DatabaseConfig&) = delete;
    DatabaseConfig(DatabaseConfig&&) = delete;
    DatabaseConfig& operator=(DatabaseConfig&&) = delete;

    // Бизнес-логика класса
    void setConnectionString(const std::string& url) { connectionUrl = url; }
    std::string getConnectionString() const { return connectionUrl; }

private:
    // Конструктор и деструктор ОБЯЗАТЕЛЬНО приватные, чтобы никто не создал объект через new или на стеке
    DatabaseConfig() { std::cout << "[Singleton] Конфигурация базы данных загружена.\n"; }
    ~DatabaseConfig() = default;

    std::string connectionUrl = "localhost:5432";
};

// --- Как это использовать в коде ---
int main() {
    // Попытка написать: DatabaseConfig config; — вызовет ошибку компиляции

    // Получаем ссылку на наш единственный объект
    DatabaseConfig& config1 = DatabaseConfig::getInstance();
    config1.setConnectionString("mongodb://production-server:27017");

    // В совершенно другой части программы вызываем снова:
    DatabaseConfig& config2 = DatabaseConfig::getInstance();
    
    // Выведет: mongodb://production-server:27017, потому что config1 и config2 — это один и тот же объект
    std::cout << "URL базы данных: " << config2.getConnectionString() << "\n";

    return 0;
}
