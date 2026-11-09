#include <iostream>

// Интерфейс, который требует наша новая система (работает в КГ)
class MetricScales {
public:
    virtual ~MetricScales() = default;
    virtual double getWeightInKg() = 0;
};

// Старый класс (или чужая библиотека), который мы не можем менять напрямую
class OldBritishScales {
public:
    double getWeightInPounds() {
        return 154.32; // Выдает вес в фунтах
    }
};

// Создаем АДАПТЕР
class WeightAdapter : public MetricScales {
private:
    OldBritishScales oldScales; // Внутри держим старый объект
public:
    double getWeightInKg() override {
        // Переводим фунты в килограммы (1 фунт ~ 0.453 кг)
        return oldScales.getWeightInPounds() * 0.453592;
    }
};

// --- Как это использовать в коде ---
int main() {
    // Новая система хочет работать через MetricScales
    MetricScales* scales = new WeightAdapter();
    
    std::cout << "Вес в килограммах: " << scales->getWeightInKg() << " кг\n";

    delete scales;
    return 0;
}
