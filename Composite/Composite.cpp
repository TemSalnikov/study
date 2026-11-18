#include <iostream>
#include <memory>
#include <numeric>
#include <string>
#include <utility>
#include <vector>

class TaskItem {
public:
    virtual ~TaskItem() = default;
    virtual int getMinutes() const = 0;
    virtual void print(int indent = 0) const = 0;
};

class LeafTask final : public TaskItem {
public:
    LeafTask(std::string title, int minutes)
        : title_(std::move(title)), minutes_(minutes) {}

    int getMinutes() const override { return minutes_; }

    void print(int indent = 0) const override {
        std::cout << std::string(indent, ' ')
                  << "- " << title_ << " (" << minutes_ << " мин)\n";
    }

private:
    std::string title_;
    int minutes_;
};

class TaskGroup final : public TaskItem {
public:
    explicit TaskGroup(std::string name) : name_(std::move(name)) {}

    void add(std::unique_ptr<TaskItem> item) {
        if (item) {
            children_.push_back(std::move(item));
        }
    }

    int getMinutes() const override {
        return std::accumulate(
            children_.begin(), children_.end(), 0,
            [](int sum, const auto& child) { return sum + child->getMinutes(); });
    }

    void print(int indent = 0) const override {
        std::cout << std::string(indent, ' ')
                  << "+ " << name_ << " [" << getMinutes() << " мин]\n";
        for (const auto& child : children_) {
            child->print(indent + 2);
        }
    }

private:
    std::string name_;
    std::vector<std::unique_ptr<TaskItem>> children_;
};

int main() {
    auto sprint = std::make_unique<TaskGroup>("Спринт");
    sprint->add(std::make_unique<LeafTask>("Ревью кода", 30));

    auto feature = std::make_unique<TaskGroup>("Новая фича");
    feature->add(std::make_unique<LeafTask>("Проектирование API", 60));
    feature->add(std::make_unique<LeafTask>("Написание тестов", 90));

    sprint->add(std::move(feature));
    sprint->print();
    return 0;
}
