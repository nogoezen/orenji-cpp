#include "AI/Pathfinding.hpp"
#include <algorithm>
#include <limits>

namespace AI
{
    namespace Pathfinding
    {

        AStar::AStar()
            : m_width(0), m_height(0)
        {
            // Fonction de collision par défaut (utilise la grille d'obstacles)
            m_collisionFunction = [this](int x, int y)
            {
                return isObstacle(x, y);
            };
        }

        AStar::~AStar()
        {
        }

        void AStar::setGridSize(int width, int height)
        {
            m_width = width;
            m_height = height;

            // Redimensionner la grille d'obstacles
            m_obstacles.resize(width);
            for (auto &column : m_obstacles)
            {
                column.resize(height, false);
            }
        }

        void AStar::setObstacle(int x, int y, bool isObstacle)
        {
            if (isWithinBounds(x, y))
            {
                m_obstacles[x][y] = isObstacle;
            }
        }

        bool AStar::isObstacle(int x, int y) const
        {
            if (isWithinBounds(x, y))
            {
                return m_obstacles[x][y];
            }
            return true; // Considérer les positions hors limites comme des obstacles
        }

        void AStar::setCollisionFunction(const std::function<bool(int, int)> &function)
        {
            m_collisionFunction = function;
        }

        std::vector<sf::Vector2i> AStar::findPath(int startX, int startY, int goalX, int goalY, bool allowDiagonal)
        {
            // Vérifier si les positions de départ et d'arrivée sont valides
            if (!isWithinBounds(startX, startY) || !isWithinBounds(goalX, goalY))
            {
                return {};
            }

            // Vérifier si le départ ou l'arrivée sont des obstacles
            if (m_collisionFunction(startX, startY) || m_collisionFunction(goalX, goalY))
            {
                return {};
            }

            // Si le départ et l'arrivée sont identiques, retourner un chemin trivial
            if (startX == goalX && startY == goalY)
            {
                return {sf::Vector2i(startX, startY)};
            }

            // Structures de données pour l'algorithme A*
            std::priority_queue<Node *, std::vector<Node *>, NodeComparator> openSet;
            std::unordered_map<int, std::unordered_map<int, Node *>> nodeMap;
            std::unordered_set<int> closedSet; // Utilisé pour garder trace des positions visitées

            // Créer le nœud de départ
            Node *startNode = new Node{
                sf::Vector2i(startX, startY),
                0.0f,                                    // gCost
                heuristic(startX, startY, goalX, goalY), // hCost
                nullptr                                  // parent
            };

            // Ajouter le nœud de départ à l'openSet et à la map
            openSet.push(startNode);
            nodeMap[startX][startY] = startNode;

            // Positions cibles
            sf::Vector2i goalPos(goalX, goalY);

            // Boucle principale de l'algorithme A*
            while (!openSet.empty())
            {
                // Obtenir le nœud avec le coût F le plus bas
                Node *currentNode = openSet.top();
                openSet.pop();

                // Si nous avons atteint l'objectif, reconstruire et retourner le chemin
                if (currentNode->position.x == goalX && currentNode->position.y == goalY)
                {
                    std::vector<sf::Vector2i> path = reconstructPath(currentNode);

                    // Nettoyer la mémoire
                    for (auto &xEntry : nodeMap)
                    {
                        for (auto &yEntry : xEntry.second)
                        {
                            delete yEntry.second;
                        }
                    }

                    return path;
                }

                // Marquer le nœud comme visité
                int posKey = currentNode->position.x * m_height + currentNode->position.y;
                closedSet.insert(posKey);

                // Explorer les voisins
                std::vector<sf::Vector2i> neighbors = getNeighbors(currentNode->position, allowDiagonal);
                for (const auto &neighborPos : neighbors)
                {
                    int neighborPosKey = neighborPos.x * m_height + neighborPos.y;

                    // Ignorer les voisins déjà visités
                    if (closedSet.find(neighborPosKey) != closedSet.end())
                    {
                        continue;
                    }

                    // Calculer le nouveau coût G (coût du départ au voisin via le nœud actuel)
                    float tentativeGCost = currentNode->gCost;

                    // Ajouter un coût supplémentaire pour les mouvements diagonaux
                    if (neighborPos.x != currentNode->position.x && neighborPos.y != currentNode->position.y)
                    {
                        tentativeGCost += 1.414f; // sqrt(2)
                    }
                    else
                    {
                        tentativeGCost += 1.0f;
                    }

                    // Vérifier si le voisin est déjà dans l'openSet
                    Node *neighborNode = nullptr;
                    bool isNew = false;

                    auto xIt = nodeMap.find(neighborPos.x);
                    if (xIt != nodeMap.end())
                    {
                        auto yIt = xIt->second.find(neighborPos.y);
                        if (yIt != xIt->second.end())
                        {
                            neighborNode = yIt->second;
                        }
                    }

                    // Si le voisin n'est pas dans l'openSet ou si le nouveau chemin est meilleur
                    if (!neighborNode)
                    {
                        // Créer un nouveau nœud
                        neighborNode = new Node{
                            neighborPos,
                            tentativeGCost,
                            heuristic(neighborPos.x, neighborPos.y, goalX, goalY),
                            currentNode};

                        nodeMap[neighborPos.x][neighborPos.y] = neighborNode;
                        openSet.push(neighborNode);
                        isNew = true;
                    }
                    else if (tentativeGCost < neighborNode->gCost)
                    {
                        // Mettre à jour les coûts et le parent du nœud existant
                        neighborNode->gCost = tentativeGCost;
                        neighborNode->parent = currentNode;

                        // La priority_queue ne supporte pas la mise à jour directe des priorités,
                        // donc nous ajoutons simplement le nœud à nouveau (ce qui crée un doublon)
                        // mais le nœud avec le coût le plus bas sera traité en premier
                        openSet.push(neighborNode);
                    }
                }
            }

            // Si nous arrivons ici, aucun chemin n'a été trouvé
            // Nettoyer la mémoire
            for (auto &xEntry : nodeMap)
            {
                for (auto &yEntry : xEntry.second)
                {
                    delete yEntry.second;
                }
            }

            return {};
        }

        void AStar::clearObstacles()
        {
            for (auto &column : m_obstacles)
            {
                std::fill(column.begin(), column.end(), false);
            }
        }

        std::vector<sf::Vector2i> AStar::getNeighbors(const sf::Vector2i &position, bool allowDiagonal) const
        {
            std::vector<sf::Vector2i> neighbors;

            // Voisins orthogonaux (haut, droite, bas, gauche)
            const int dx[] = {0, 1, 0, -1};
            const int dy[] = {-1, 0, 1, 0};

            // Ajouter les voisins orthogonaux
            for (int i = 0; i < 4; ++i)
            {
                int nx = position.x + dx[i];
                int ny = position.y + dy[i];

                if (isWithinBounds(nx, ny) && !m_collisionFunction(nx, ny))
                {
                    neighbors.push_back(sf::Vector2i(nx, ny));
                }
            }

            // Ajouter les voisins diagonaux si autorisés
            if (allowDiagonal)
            {
                const int diagDx[] = {1, 1, -1, -1};
                const int diagDy[] = {-1, 1, 1, -1};

                for (int i = 0; i < 4; ++i)
                {
                    int nx = position.x + diagDx[i];
                    int ny = position.y + diagDy[i];

                    // Vérifier si la diagonale est accessible (pas d'obstacles adjacents)
                    if (isWithinBounds(nx, ny) && !m_collisionFunction(nx, ny))
                    {
                        // Vérifier que les cases adjacentes ne sont pas des obstacles
                        // pour éviter de "couper les coins"
                        int ax1 = position.x + diagDx[i];
                        int ay1 = position.y;
                        int ax2 = position.x;
                        int ay2 = position.y + diagDy[i];

                        if (!m_collisionFunction(ax1, ay1) || !m_collisionFunction(ax2, ay2))
                        {
                            neighbors.push_back(sf::Vector2i(nx, ny));
                        }
                    }
                }
            }

            return neighbors;
        }

        float AStar::heuristic(int x1, int y1, int x2, int y2) const
        {
            // Distance de Manhattan
            return std::abs(x2 - x1) + std::abs(y2 - y1);
        }

        bool AStar::isWithinBounds(int x, int y) const
        {
            return x >= 0 && x < m_width && y >= 0 && y < m_height;
        }

        std::vector<sf::Vector2i> AStar::reconstructPath(Node *node) const
        {
            std::vector<sf::Vector2i> path;

            // Remonter le chemin du nœud d'arrivée au nœud de départ
            while (node)
            {
                path.push_back(node->position);
                node = node->parent;
            }

            // Inverser le chemin pour qu'il aille du départ à l'arrivée
            std::reverse(path.begin(), path.end());

            return path;
        }

    }
} // namespace AI::Pathfinding