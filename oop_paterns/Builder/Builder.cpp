#include <iostream>
#include <string>

// Сложный класс, который тяжело создавать через обычный конструктор
class Character {
public:
    std::string name;
    std::string weapon = "Кулаки";
    int armor = 0;
    bool hasMagic = false;

    void printInfo() const {
        std::cout << "Герой: " << name << " | Оружие: " << weapon 
                  << " | Броня: " << armor << " | Магия: " << (hasMagic ? "Да" : "Нет") << "\n";
    }
};

// Класс-строитель
class CharacterBuilder {
private:
    Character character; // Объект, который мы собираем
public:
    CharacterBuilder(const std::string& name) {
        character.name = name;
    }

    // Возвращаем ссылку на сам билдер (идиома Fluent Interface)
    CharacterBuilder& giveWeapon(const std::string& weaponName) {
        character.weapon = weaponName;
        return *this;
    }

    CharacterBuilder& setArmor(int armorValue) {
        character.armor = armorValue;
        return *this;
    }

    CharacterBuilder& enableMagic() {
        character.hasMagic = true;
        return *this;
    }

    // Финальный шаг — забираем готовый объект
    Character build() {
        return character;
    }
};

// --- Как это использовать в коде ---
int main() {
    // Собираем мага в красивую цепочку вызовов:
    Character mage = CharacterBuilder("Гэндальф")
                        .giveWeapon("Посох")
                        .enableMagic()
                        .build(); // Броню не задавали, она останется 0

    // Собираем воина:
    Character warrior = CharacterBuilder("Конан")
                        .giveWeapon("Двуручный меч")
                        .setArmor(100)
                        .build();

    mage.printInfo();
    warrior.printInfo();
    return 0;
}
