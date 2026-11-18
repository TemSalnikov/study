#include <iostream>
#include <memory>

// 1. Реализация
class Device {
public:
    virtual ~Device() = default;
    virtual void turnOn() = 0;
    virtual void setVolume(int percent) = 0;
};

// Конкретные реализации
class Tv : public Device {
public:
    void turnOn() override { std::cout << "TV is ON\n"; }
    void setVolume(int percent) override { std::cout << "TV volume: " << percent << "%\n"; }
};

class Radio : public Device {
public:
    void turnOn() override { std::cout << "Radio is ON\n"; }
    void setVolume(int percent) override { std::cout << "Radio volume: " << percent << "%\n"; }
};

// 2. Абстракция («мост» — указатель на Device)
class Remote {
protected:
    std::shared_ptr<Device> device_;
public:
    explicit Remote(std::shared_ptr<Device> device) : device_(std::move(device)) {}
    virtual ~Remote() = default;

    virtual void power() { device_->turnOn(); }
    virtual void volumeUp() { device_->setVolume(50); }
};

// Уточнённая абстракция
class MuteRemote : public Remote {
public:
    using Remote::Remote;
    void mute() { device_->setVolume(0); }
};

int main() {
    auto tv = std::make_shared<Tv>();
    MuteRemote remote(tv);
    remote.power();
    remote.mute();

    auto radio = std::make_shared<Radio>();
    Remote radioRemote(radio);
    radioRemote.power();
}