#pragma once

#include <unordered_map>
#include <string>
#include <memory>

namespace Orenji
{
    /**
     * @brief A template class for caching resources like textures, sounds, and maps.
     *
     * This cache stores shared pointers to resources, indexed by their file paths.
     * It provides methods to add, retrieve, check existence, and remove resources.
     *
     * @tparam T The resource type to be cached
     */
    template <typename T>
    class ResourceCache
    {
    public:
        /**
         * @brief Default constructor
         */
        ResourceCache() = default;

        /**
         * @brief Add a resource to the cache
         *
         * @param id The identifier for the resource (typically a file path)
         * @param resource The resource to cache
         * @return true if the resource was added, false if it already existed
         */
        bool add(const std::string &id, std::shared_ptr<T> resource)
        {
            auto [it, inserted] = m_resources.emplace(id, resource);
            return inserted;
        }

        /**
         * @brief Get a resource from the cache
         *
         * @param id The identifier of the resource
         * @return std::shared_ptr<T> The resource, or nullptr if not found
         */
        std::shared_ptr<T> get(const std::string &id) const
        {
            auto it = m_resources.find(id);
            if (it != m_resources.end())
            {
                return it->second;
            }
            return nullptr;
        }

        /**
         * @brief Check if a resource is loaded in the cache
         *
         * @param id The identifier of the resource
         * @return true if the resource is in the cache, false otherwise
         */
        bool isLoaded(const std::string &id) const
        {
            return m_resources.find(id) != m_resources.end();
        }

        /**
         * @brief Remove a resource from the cache
         *
         * @param id The identifier of the resource
         * @return true if the resource was removed, false if it wasn't in the cache
         */
        bool remove(const std::string &id)
        {
            return m_resources.erase(id) > 0;
        }

        /**
         * @brief Remove all resources from the cache
         */
        void clear()
        {
            m_resources.clear();
        }

        /**
         * @brief Get the number of resources in the cache
         *
         * @return size_t The number of cached resources
         */
        size_t size() const
        {
            return m_resources.size();
        }

    private:
        std::unordered_map<std::string, std::shared_ptr<T>> m_resources;
    };

} // namespace Orenji