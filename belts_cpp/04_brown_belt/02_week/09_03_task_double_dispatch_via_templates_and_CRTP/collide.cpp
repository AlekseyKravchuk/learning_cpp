#include "game_object.h"
#include "geo2d.h"

#include "test_runner.h"

#include <memory>
#include <vector>

using namespace std;

// Определите классы Unit, Building, Tower и Fence так, чтобы они наследовались от GameObject и реализовывали его интерфейс.

template <typename T, typename G>
class Collider : public GameObject {
  public:
    explicit Collider(G geometry)
        : _geometry(geometry) {}

    const G& GetGeometry() const {
        return _geometry;
    }

    bool Collide(const GameObject& that) const final;
    bool CollideWith(const Unit& that) const override;
    bool CollideWith(const Building& that) const override;
    bool CollideWith(const Tower& that) const override;
    bool CollideWith(const Fence& that) const override;

  protected:
    G _geometry;
};

class Unit : public Collider<Unit, geo2d::Point> {
  public:
    explicit Unit(geo2d::Point geometry)
        : Collider(geometry) {}
};

class Building : public Collider<Building, geo2d::Rectangle> {
  public:
    explicit Building(geo2d::Rectangle geometry)
        : Collider(geometry) {}
};

class Tower : public Collider<Tower, geo2d::Circle> {
  public:
    explicit Tower(geo2d::Circle geometry)
        : Collider(geometry) {}
};

class Fence : public Collider<Fence, geo2d::Segment> {
  public:
    explicit Fence(geo2d::Segment geometry)
        : Collider(geometry) {}
};

template <typename T, typename G>
bool Collider<T, G>::Collide(const GameObject& that) const {
    return that.CollideWith(static_cast<const T&>(*this));
}

template <typename T, typename G>
bool Collider<T, G>::CollideWith(const Unit& that) const {
    return geo2d::Collide(_geometry, that.GetGeometry());
}
template <typename T, typename G>
bool Collider<T, G>::CollideWith(const Building& that) const {
    return geo2d::Collide(_geometry, that.GetGeometry());
}
template <typename T, typename G>
bool Collider<T, G>::CollideWith(const Tower& that) const {
    return geo2d::Collide(_geometry, that.GetGeometry());
}
template <typename T, typename G>
bool Collider<T, G>::CollideWith(const Fence& that) const {
    return geo2d::Collide(_geometry, that.GetGeometry());
}

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
