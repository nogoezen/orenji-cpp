#pragma once

#include <SFML/System/Vector2.hpp>
#include <vector>
#include <functional>
#include <cmath>
#include <queue>
#include <unordered_map>
#include <unordered_set>

namespace AI
{
    namespace Pathfinding
    {

        /**
         * @brief Node structure for A* pathfinding
         */
        struct Node
        {
            sf::Vector2i position; // Position in grid
            float gCost;           // Cost from start
            float hCost;           // Heuristic cost to goal
            Node *parent;          // Parent node

            /**
             * @brief Get total cost (f = g + h)
             */
            float getFCost() const
            {
                return gCost + hCost;
            }

            /**
             * @brief Equality operator for node comparison
             */
            bool operator==(const Node &other) const
            {
                return position.x == other.position.x && position.y == other.position.y;
            }
        };

        /**
         * @brief Comparator for priority queue
         */
        struct NodeComparator
        {
            bool operator()(const Node *a, const Node *b) const
            {
                // Higher priority for lower f-cost
                if (a->getFCost() == b->getFCost())
                {
                    return a->hCost > b->hCost; // Tie-breaking with h-cost
                }
                return a->getFCost() > b->getFCost();
            }
        };

        /**
         * @brief Hash function for Node
         */
        struct NodeHasher
        {
            std::size_t operator()(const sf::Vector2i &position) const
            {
                return std::hash<int>()(position.x) ^ (std::hash<int>()(position.y) << 1);
            }
        };

        /**
         * @brief A* pathfinding algorithm
         */
        class AStar
        {
        public:
            /**
             * @brief Constructor
             */
            AStar();

            /**
             * @brief Destructor
             */
            ~AStar();

            /**
             * @brief Set grid size
             * @param width Width of the grid
             * @param height Height of the grid
             */
            void setGridSize(int width, int height);

            /**
             * @brief Set obstacle at position
             * @param x X coordinate
             * @param y Y coordinate
             * @param isObstacle True if position is an obstacle
             */
            void setObstacle(int x, int y, bool isObstacle);

            /**
             * @brief Check if position is an obstacle
             * @param x X coordinate
             * @param y Y coordinate
             * @return True if position is an obstacle
             */
            bool isObstacle(int x, int y) const;

            /**
             * @brief Set custom collision check function
             * @param function Function that returns true if position is an obstacle
             */
            void setCollisionFunction(const std::function<bool(int, int)> &function);

            /**
             * @brief Find path from start to goal
             * @param startX Start X coordinate
             * @param startY Start Y coordinate
             * @param goalX Goal X coordinate
             * @param goalY Goal Y coordinate
             * @param allowDiagonal Whether to allow diagonal movement
             * @return Vector of positions forming the path (empty if no path found)
             */
            std::vector<sf::Vector2i> findPath(int startX, int startY, int goalX, int goalY, bool allowDiagonal = true);

            /**
             * @brief Clear all obstacles
             */
            void clearObstacles();

        private:
            /**
             * @brief Get neighbors of a node
             * @param node Current node
             * @param allowDiagonal Whether to allow diagonal movement
             * @return Vector of neighbor positions
             */
            std::vector<sf::Vector2i> getNeighbors(const sf::Vector2i &position, bool allowDiagonal) const;

            /**
             * @brief Calculate heuristic (Manhattan distance)
             * @param x1 Start X coordinate
             * @param y1 Start Y coordinate
             * @param x2 Goal X coordinate
             * @param y2 Goal Y coordinate
             * @return Heuristic value
             */
            float heuristic(int x1, int y1, int x2, int y2) const;

            /**
             * @brief Check if position is within grid bounds
             * @param x X coordinate
             * @param y Y coordinate
             * @return True if position is within bounds
             */
            bool isWithinBounds(int x, int y) const;

            /**
             * @brief Reconstruct path from goal to start
             * @param node Goal node
             * @return Vector of positions forming the path
             */
            std::vector<sf::Vector2i> reconstructPath(Node *node) const;

            int m_width;
            int m_height;
            std::vector<std::vector<bool>> m_obstacles;
            std::function<bool(int, int)> m_collisionFunction;
        };

    }
} // namespace AI::Pathfinding