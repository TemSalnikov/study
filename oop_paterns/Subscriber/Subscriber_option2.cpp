#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>

// Интерфейс-маркер для подписчиков
class IModernObserver {
public:
    virtual ~IModernObserver() = default;
    virtual void onWeatherChanged(float temp, float humidity) = 0;
};

class WeatherStationSafe {
private:
    // Храним WEAK указатели. Они не удерживают объект в памяти и позволяют проверить его «живость»
    std::vector<std::weak_ptr<IModernObserver>> observers;

public:
    void subscribe(std::weak_ptr<IModernObserver> obs) {
        observers.push_back(obs);
    }

    void setMeasurements(float temp, float humidity) {
        std::cout << "\n[Станция] Изменение погоды: T = " << temp << "\n";
        
        // Итерируемся и сразу чистим «мертвых» подписчиков
        for (auto it = observers.begin(); it != observers.end(); ) {
            // Пытаемся превратить weak_ptr в shared_ptr (проверяем, жив ли объект)
            if (auto sharedObserver = it->lock()) {
                sharedObserver->onWeatherChanged(temp, humidity);
                ++it;
            } else {
                // Объект уже удален из памяти! Безопасно убираем его из списка подписок
                std::cout << "[Станция] Обнаружен мертвый подписчик. Удаляю из списка.\n";
                it = observers.erase(it);
            }
        }
    }
};

// Конкретный подписчик
class SmartDisplay : public IModernObserver {
private:
    std::string name;
public:
    SmartDisplay(std::string n) : name(std::move(n)) {}
    ~SmartDisplay() { std::cout << name << " уничтожен и удален из памяти.\n"; }

    void onWeatherChanged(float temp, float humidity) override {
        std::cout << "-> [" << name << "] Принял данные: " << temp << "°C\n";
    }
};

// --- Как это использовать в коде ---
int main() {
    WeatherStationSafe station;

    // Создаем подписчиков как shared_ptr
    auto display1 = std::make_shared<SmartDisplay>("Кухонный Дисплей");
    
    station.subscribe(display1); // Неявная конвертация shared_ptr -> weak_ptr

    {
        auto display2 = std::make_shared<SmartDisplay>("Дисплей в Спальне");
        station.subscribe(display2);
        
        station.setMeasurements(21.0f, 50.0f); // Уведомление получат ОБА дисплея
    } // <-- Тут "Дисплей в Спальне" выходит из зоны видимости и УНИЧТОЖАЕТСЯ

    // Метеостанция снова рассылает данные
    station.setMeasurements(22.5f, 48.0f); // Спальня автоматически отфильтруется как "мертвая"

    return 0;
}
