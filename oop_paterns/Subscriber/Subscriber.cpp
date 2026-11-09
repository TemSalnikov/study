#include <iostream>
#include <vector>
#include <algorithm>

// 1. Интерфейс Наблюдателя (Подписчика)
class IObserver {
public:
    virtual ~IObserver() = default;
    // Этот метод будет вызываться, когда у Издателя что-то изменится
    virtual void update(float temperature, float humidity) = 0;
};

// 2. Интерфейс Издателя (Субъекта)
class IObservable {
public:
    virtual ~IObservable() = default;
    virtual void registerObserver(IObserver* observer) = 0;
    virtual void removeObserver(IObserver* observer) = 0;
    virtual void notifyObservers() = 0; // Оповестить всех
};

// 3. Конкретный Издатель — Метеостанция
class WeatherStation : public IObservable {
private:
    std::vector<IObserver*> observers; // Список указателей на наших подписчиков
    float temperature = 0.0f;
    float humidity = 0.0f;

public:
    void registerObserver(IObserver* observer) override {
        observers.push_back(observer);
    }

    void removeObserver(IObserver* observer) override {
        // Удаляем подписчика из списка
        observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
    }

    void notifyObservers() override {
        for (IObserver* observer : observers) {
            observer->update(temperature, humidity); // Передаем новые данные каждому
        }
    }

    // Метод, который имитирует изменение погоды датчиками
    void setMeasurements(float temp, float humid) {
        temperature = temp;
        humidity = humid;
        notifyObservers(); // Как только данные обновились — сразу шлем уведомления
    }
};

// 4. Конкретный Подписчик №1 — Экран на стене смарт-дома
class PhoneDisplay : public IObserver {
public:
    void update(float temperature, float humidity) override {
        std::cout << "[Экран телефона] Новые данные: Температура = " 
                  << temperature << "°C, Влажность = " << humidity << "%\n";
    }
};

// 5. Конкретный Подписчик №2 — Агрегатор статистики (считает средние показатели)
class LoggerDisplay : public IObserver {
public:
    void update(float temperature, float humidity) override {
        std::cout << "[Системный лог] Запись в файл: T=" << temperature << ", H=" << humidity << "\n";
    }
};

// --- Как это использовать в коде ---
int main() {
    WeatherStation station; // Наш главный датчик

    PhoneDisplay phone;
    LoggerDisplay logger;

    // Подписываем их на метеостанцию
    station.registerObserver(&phone);
    station.registerObserver(&logger);

    std::cout << "--- Первое изменение погоды ---\n";
    station.setMeasurements(22.5f, 60.0f);

    std::cout << "\n--- Телефон отписался от уведомлений ---\n";
    station.removeObserver(&phone);

    std::cout << "\n--- Второе изменение погоды ---\n";
    station.setMeasurements(24.0f, 55.0f); // Уведомление получит только logger

    return 0;
}
