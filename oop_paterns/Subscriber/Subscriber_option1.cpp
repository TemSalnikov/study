#include <iostream>
#include <vector>
#include <functional>
#include <string>

class WeatherStationModern {
public:
    // Определяем тип колбэка. Функция принимает (temp, humidity) и ничего не возвращает.
    using EventCallback = std::function<void(float, float)>;

    // Регистрируем подписчика и возвращаем его ID (индекс в векторе)
    size_t subscribe(EventCallback callback) {
        callbacks.push_back(callback);
        return callbacks.size() - 1; // ID для отписки
    }

    // Оповещение всех через обычный вызов сохраненных функций
    void setMeasurements(float temp, float humidity) {
        std::cout << "\n[Станция] Новые данные: T = " << temp << "°C\n";
        for (const auto& callback : callbacks) {
            if (callback) { 
                callback(temp, humidity); 
            }
        }
    }

private:
    std::vector<EventCallback> callbacks;
};

// --- Пример использования ---
void globalLogger(float t, float h) {
    std::cout << "-> [Глобальный лог] Запись: T=" << t << ", H=" << h << "\n";
}

int main() {
    WeatherStationModern station;

    // 1. Подписываем обычную функцию
    station.subscribe(globalLogger);

    // 2. Подписываем лямбда-выражение прямо на лету
    station.subscribe([](float t, float h) {
        std::cout << "-> [Экран Смартфона] Показываю температуру: " << t << "°C\n";
    });

    // 3. Имитируем изменение погоды
    station.setMeasurements(23.5f, 62.0f);
    station.setMeasurements(25.0f, 58.0f);

    return 0;
}
