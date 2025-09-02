#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>

namespace EntityUtils
{
// Removes entity from its owner vector, thus deleting it
template<class Entity>
static void destroy(const Entity* entity, std::vector<std::unique_ptr<Entity>>& container) {
    for(auto it = container.begin(); it != container.end(); ++it) {
        if ((*it).get() == entity) {
            container.erase(it);
            break;
        }
    }
}

// Removes a raw pointer to entity from a vector
// Used to avoid dangling pointers after calling destroyEntity
template<class Entity>
static void removeRef(const Entity* entity, std::vector<Entity*>& container) {
    std::erase_if(container, [entity](Entity* ptr) {
        return ptr == entity;
    });
}

// Mapping from public const pointer to internal mutable pointer
// Used to avoid const cast
template<class Entity>
static Entity* findMutablePtr(const Entity* ptr, const std::vector<std::unique_ptr<Entity>>& container) {
    if (!ptr) throw std::invalid_argument("Entity ptr cannot be null!");

    for (auto& obj : container) {
        if (obj.get() == ptr)
            return obj.get();
    }
    throw std::invalid_argument("Entity does not belong to this container!");
}
}

#endif // UTILS_HPP
