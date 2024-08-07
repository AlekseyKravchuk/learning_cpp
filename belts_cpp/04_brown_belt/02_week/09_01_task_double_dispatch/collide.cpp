#include "game_object.h"
#include "geo2d.h"

#include "test_runner.h"

#include <memory>
#include <vector>

using namespace std;

// Определите классы Unit, Building, Tower и Fence так, чтобы они наследовались от GameObject и реализовывали его интерфейс.

// Используем шаблон CRTP (Curiously Recurring Template Pattern) => создаем шаблон класса, Collider<T>
// Благодаря CRTP, метод "Collide" необходимо переопределить только в этом шаблоне класса.
template <typename T>
class Collider : public GameObject {
    bool Collide(const GameObject& that) const final {
        // статически приводим тип "*this" к типу "const T&", т.к. на этапе компиляции нам станет известен
        // фактический тип наследника - в результате подстановки фактического типа в шаблонный параметр "T"
        return that.CollideWith(static_cast<const T&>(*this));
    }   
};

// =========================== ОБЪЯВЛЕНИЯ наследников от шаблона "Collider<Unit>" ===========================
// =========== шаблон "Collider<Unit>", в свою очередь, является наследником класса "GameObject" ============

class Unit final : public Collider<Unit> {
  public:
    explicit Unit(geo2d::Point position);
    geo2d::Point GetPointPosition() const;

    bool CollideWith(const Unit& that) const override;
    bool CollideWith(const Building& that) const override;
    bool CollideWith(const Tower& that) const override;
    bool CollideWith(const Fence& that) const override;

  private:
    geo2d::Point _point;
};

class Building final : public Collider<Building> {
  public:
    explicit Building(geo2d::Rectangle geometry);
    geo2d::Rectangle GetRectanglePosition() const;

    bool CollideWith(const Unit& that) const override;
    bool CollideWith(const Building& that) const override;
    bool CollideWith(const Tower& that) const override;
    bool CollideWith(const Fence& that) const override;

  private:
    geo2d::Rectangle _rectangle;
};

class Tower final : public Collider<Tower> {
  public:
    explicit Tower(geo2d::Circle geometry);
    geo2d::Circle GetCirclePosition() const;

    bool CollideWith(const Unit& that) const override;
    bool CollideWith(const Building& that) const override;
    bool CollideWith(const Tower& that) const override;
    bool CollideWith(const Fence& that) const override;

  private:
    geo2d::Circle _circle;
};

class Fence final : public Collider<Fence> {
  public:
    explicit Fence(geo2d::Segment geometry);
    geo2d::Segment GetSegmentPosition() const;

    bool CollideWith(const Unit& that) const override;
    bool CollideWith(const Building& that) const override;
    bool CollideWith(const Tower& that) const override;
    bool CollideWith(const Fence& that) const override;

  private:
    geo2d::Segment _segment;
};
// ======================== КОНЕЦ Объявления наследников от шаблона "Collider<Unit>" ========================

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ============================== ОПРЕДЕЛЕНИЯ наследников шаблона "Collider<T>" =============================
// 1) ============================================== класс Unit =============================================
Unit::Unit(geo2d::Point position)
    : _point(position) {}

geo2d::Point Unit::GetPointPosition() const {
    return _point;
}

bool Unit::CollideWith(const Unit& that) const {
    return geo2d::Collide(_point, that.GetPointPosition());
}

bool Unit::CollideWith(const Building& that) const {
    return geo2d::Collide(_point, that.GetRectanglePosition());
}

bool Unit::CollideWith(const Tower& that) const {
    return geo2d::Collide(_point, that.GetCirclePosition());
}

bool Unit::CollideWith(const Fence& that) const {
    return geo2d::Collide(_point, that.GetSegmentPosition());
}
// ==========================================================================================================

// 2) ============================================ класс Building ===========================================
Building::Building(geo2d::Rectangle geometry)
    : _rectangle(geometry) {}

geo2d::Rectangle Building::GetRectanglePosition() const {
    return _rectangle;
}

bool Building::CollideWith(const Unit& that) const {
    return geo2d::Collide(_rectangle, that.GetPointPosition());
}

bool Building::CollideWith(const Building& that) const {
    return geo2d::Collide(_rectangle, that.GetRectanglePosition());
}

bool Building::CollideWith(const Tower& that) const {
    return geo2d::Collide(_rectangle, that.GetCirclePosition());
}

bool Building::CollideWith(const Fence& that) const {
    return geo2d::Collide(_rectangle, that.GetSegmentPosition());
}
// ==========================================================================================================

// 3) ============================================== класс Tower ============================================
Tower::Tower(geo2d::Circle geometry)
    : _circle(geometry) {}

geo2d::Circle Tower::GetCirclePosition() const {
    return _circle;
}

bool Tower::CollideWith(const Unit& that) const {
    return geo2d::Collide(_circle, that.GetPointPosition());
}

bool Tower::CollideWith(const Building& that) const {
    return geo2d::Collide(_circle, that.GetRectanglePosition());
}

bool Tower::CollideWith(const Tower& that) const {
    return geo2d::Collide(_circle, that.GetCirclePosition());
}

bool Tower::CollideWith(const Fence& that) const {
    return geo2d::Collide(_circle, that.GetSegmentPosition());
}
// ==========================================================================================================

// 4) ============================================== класс Fence ============================================
Fence::Fence(geo2d::Segment geometry)
    : _segment(geometry) {}

geo2d::Segment Fence::GetSegmentPosition() const {
    return _segment;
}

bool Fence::CollideWith(const Unit& that) const {
    return geo2d::Collide(_segment, that.GetPointPosition());
}

bool Fence::CollideWith(const Building& that) const {
    return geo2d::Collide(_segment, that.GetRectanglePosition());
}

bool Fence::CollideWith(const Tower& that) const {
    return geo2d::Collide(_segment, that.GetCirclePosition());
}

bool Fence::CollideWith(const Fence& that) const {
    return geo2d::Collide(_segment, that.GetSegmentPosition());
}
// ==========================================================================================================
// ========================== КОНЕЦ ОПРЕДЕЛЕНИЙ наследников шаблона "Collider<Unit>" ========================

// Реализуйте функцию Collide из файла GameObject.h
bool Collide(const GameObject& first, const GameObject& second) {
    return first.Collide(second);
}

void TestAddingNewObjectOnMap() {
    // Юнит-тест моделирует ситуацию, когда на игровой карте уже есть какие-то объекты,
    // и мы хотим добавить на неё новый, например, построить новое здание или башню.
    // Мы можем его добавить, только если он не пересекается ни с одним из существующих.
    using namespace geo2d;

    const vector<shared_ptr<GameObject>> game_map = {
        make_shared<Unit>(Point{3, 3}),
        make_shared<Unit>(Point{5, 5}),
        make_shared<Unit>(Point{3, 7}),
        make_shared<Fence>(Segment{{7, 3}, {9, 8}}),
        make_shared<Tower>(Circle{Point{9, 4}, 1}),
        make_shared<Tower>(Circle{Point{10, 7}, 1}),
        make_shared<Building>(Rectangle{{11, 4}, {14, 6}})};

    for (size_t i = 0; i < game_map.size(); ++i) {
        Assert(
            Collide(*game_map[i], *game_map[i]),
            "An object doesn't collide with itself: " + to_string(i));

        for (size_t j = 0; j < i; ++j) {
            Assert(
                !Collide(*game_map[i], *game_map[j]),
                "Unexpected collision found " + to_string(i) + ' ' + to_string(j));
        }
    }

    auto new_warehouse = make_shared<Building>(Rectangle{{4, 3}, {9, 6}});
    ASSERT(!Collide(*new_warehouse, *game_map[0]));
    ASSERT(Collide(*new_warehouse, *game_map[1]));
    ASSERT(!Collide(*new_warehouse, *game_map[2]));
    ASSERT(Collide(*new_warehouse, *game_map[3]));
    ASSERT(Collide(*new_warehouse, *game_map[4]));
    ASSERT(!Collide(*new_warehouse, *game_map[5]));
    ASSERT(!Collide(*new_warehouse, *game_map[6]));

    auto new_defense_tower = make_shared<Tower>(Circle{{8, 2}, 2});
    ASSERT(!Collide(*new_defense_tower, *game_map[0]));
    ASSERT(!Collide(*new_defense_tower, *game_map[1]));
    ASSERT(!Collide(*new_defense_tower, *game_map[2]));
    ASSERT(Collide(*new_defense_tower, *game_map[3]));
    ASSERT(Collide(*new_defense_tower, *game_map[4]));
    ASSERT(!Collide(*new_defense_tower, *game_map[5]));
    ASSERT(!Collide(*new_defense_tower, *game_map[6]));
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestAddingNewObjectOnMap);
    return 0;
}
