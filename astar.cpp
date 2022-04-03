#include <SFML/Main.hpp>
#include <SFML/Graphics.hpp>
#include "gridgraph.hpp"
#include <queue>
#include <unistd.h>
#include <iostream>
#include <tgmath.h>

#define WIDTH 400
#define HEIGHT 400

static const sf::Vector2i start(0, 0);

void astar(GridGraph& graph, sf::Vector2i start, sf::Vector2i goal)
{
    auto node_cmp = [] (std::pair<GridGraph::GridPoint*, sf::Vector2i> a, std::pair<GridGraph::GridPoint*, sf::Vector2i> b) {
        return a.first->weight > b.first->weight;
    };
    std::priority_queue<
        std::pair<GridGraph::GridPoint*, sf::Vector2i>, 
        std::vector<std::pair<GridGraph::GridPoint*, sf::Vector2i>>,
        decltype(node_cmp)> fringe(node_cmp)
    ;  
    auto d_heur = [&goal] (int x, int y) { return std::sqrt(std::pow(x - goal.x, 2) + std::pow(y - goal.y, 2)); };
    auto find_fringe = [&graph, &fringe, d_heur] (sf::Vector2i pos) {
        auto cur = graph.get_node(pos.x, pos.y);
        if (pos.x < graph.size.x - 1) {
            auto node = graph.get_node(pos.x + 1, pos.y);
            if (node->state == GridGraph::GridState::FREE || node->state == GridGraph::GridState::GOAL) {
                if (node->state == GridGraph::GridState::GOAL) {
                    node->parent = pos;
                    fringe.push(std::pair<GridGraph::GridPoint*, sf::Vector2i>(node, sf::Vector2i(pos.x, pos.y - 1)));
                    return;
                }
                node->state = GridGraph::GridState::VISITED;
                node->parent = pos;
                fringe.push(std::pair<GridGraph::GridPoint*, sf::Vector2i>(node, sf::Vector2i(pos.x + 1, pos.y)));
            }
        }
        if (pos.x > 0) {
            auto node = graph.get_node(pos.x - 1, pos.y);
            if (node->state == GridGraph::GridState::FREE || node->state == GridGraph::GridState::GOAL) {
                if (node->state == GridGraph::GridState::GOAL) {
                    node->parent = pos;
                    fringe.push(std::pair<GridGraph::GridPoint*, sf::Vector2i>(node, sf::Vector2i(pos.x, pos.y - 1)));
                    return;
                }
                node->state = GridGraph::GridState::VISITED;
                node->parent = pos;
                fringe.push(std::pair<GridGraph::GridPoint*, sf::Vector2i>(node, sf::Vector2i(pos.x - 1, pos.y)));
            }
        }
        if (pos.y < graph.size.y - 1) {
            auto node = graph.get_node(pos.x, pos.y + 1);
            if (node->state == GridGraph::GridState::FREE || node->state == GridGraph::GridState::GOAL) {
                if (node->state == GridGraph::GridState::GOAL) {
                    node->parent = pos;
                    fringe.push(std::pair<GridGraph::GridPoint*, sf::Vector2i>(node, sf::Vector2i(pos.x, pos.y - 1)));
                    return;
                }
                node->state = GridGraph::GridState::VISITED;
                node->parent = pos;
                fringe.push(std::pair<GridGraph::GridPoint*, sf::Vector2i>(node, sf::Vector2i(pos.x, pos.y + 1)));
            }
        }
        if (pos.y > 0) {
            auto node = graph.get_node(pos.x, pos.y - 1);
            if (node->state == GridGraph::GridState::FREE || node->state == GridGraph::GridState::GOAL) {
                if (node->state == GridGraph::GridState::GOAL) {
                    node->parent = pos;
                    fringe.push(std::pair<GridGraph::GridPoint*, sf::Vector2i>(node, sf::Vector2i(pos.x, pos.y - 1)));
                    return;
                }
                node->state = GridGraph::GridState::VISITED;
                node->parent = pos;
                fringe.push(std::pair<GridGraph::GridPoint*, sf::Vector2i>(node, sf::Vector2i(pos.x, pos.y - 1)));
            }
        }
    };
    for (size_t i = 0; i < graph.size.y; i++) {
        for (size_t j = 0; j < graph.size.x; j++) {
            graph.grid[i][j].weight = d_heur(j, i);
        }
    }
    graph.set_node(start.x, start.y, GridGraph::GridState::VISITED, d_heur(0, 0));
    graph.set_node(goal.x, goal.y, GridGraph::GridState::GOAL, 0);
    auto cur = std::pair<GridGraph::GridPoint*, sf::Vector2i>(graph.get_node(start.x, start.y), sf::Vector2i(start.x, start.y));
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Graph Window");
    window.setFramerateLimit(60);
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.setActive();
        window.draw(graph);
        window.display();
        usleep(100000);
        find_fringe(cur.second);
        cur = fringe.top();
        fringe.pop();
        if (cur.first->state == GridGraph::GridState::GOAL) {
            break;
        }
    }
    graph.get_node(0, 0)->state = GridGraph::GridState::PATH;
    while (cur.first->parent != start) {
        cur.first = graph.get_node(cur.first->parent.x, cur.first->parent.y);
        cur.first->state = GridGraph::GridState::PATH;
    }
    window.draw(graph);
    window.display();
    usleep(10000000);
}

int main(int argc, char* argv[])
{
    // sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Graph Window");
    // window.setFramerateLimit(60);
    
    GridGraph graph(20, 20);
    graph.set_node(10, 10, GridGraph::GridState::BLOCKED, 0);
    graph.set_node(10, 11, GridGraph::GridState::BLOCKED, 0);
    graph.set_node(11, 11, GridGraph::GridState::BLOCKED, 0);
    graph.set_node(12, 11, GridGraph::GridState::BLOCKED, 0);
    graph.set_node(12, 12, GridGraph::GridState::BLOCKED, 0);
    graph.set_node(13, 13, GridGraph::GridState::BLOCKED, 0);
    graph.set_node(14, 14, GridGraph::GridState::BLOCKED, 0);
    graph.set_node(15, 15, GridGraph::GridState::BLOCKED, 0);
    graph.set_node(16, 16, GridGraph::GridState::BLOCKED, 0);
    graph.set_node(17, 17, GridGraph::GridState::BLOCKED, 0);
    graph.set_node(18, 18, GridGraph::GridState::BLOCKED, 0);

    graph.set_node(5, 10, GridGraph::GridState::BLOCKED, 0);
    graph.set_node(6, 11, GridGraph::GridState::BLOCKED, 0);
    graph.set_node(7, 11, GridGraph::GridState::BLOCKED, 0);
    graph.set_node(8, 11, GridGraph::GridState::BLOCKED, 0);
    graph.set_node(9, 12, GridGraph::GridState::BLOCKED, 0);
    graph.set_node(10, 13, GridGraph::GridState::BLOCKED, 0);
    graph.set_node(11, 14, GridGraph::GridState::BLOCKED, 0);
    graph.set_node(12, 15, GridGraph::GridState::BLOCKED, 0);
    graph.set_node(13, 16, GridGraph::GridState::BLOCKED, 0);
    graph.set_node(14, 17, GridGraph::GridState::BLOCKED, 0);

    graph.set_node(0, 3, GridGraph::GridState::BLOCKED, 0);
    graph.set_node(1, 3, GridGraph::GridState::BLOCKED, 0);
    graph.set_node(2, 3, GridGraph::GridState::BLOCKED, 0);
    graph.set_node(3, 3, GridGraph::GridState::BLOCKED, 0);
    graph.set_node(4, 3, GridGraph::GridState::BLOCKED, 0);
    graph.set_node(5, 3, GridGraph::GridState::BLOCKED, 0);
    graph.set_node(6, 3, GridGraph::GridState::BLOCKED, 0);
    graph.set_node(7, 3, GridGraph::GridState::BLOCKED, 0);
    graph.set_node(8, 3, GridGraph::GridState::BLOCKED, 0);
    graph.set_node(8, 4, GridGraph::GridState::BLOCKED, 0);
    graph.set_node(8, 5, GridGraph::GridState::BLOCKED, 0);
    graph.set_node(9, 4, GridGraph::GridState::BLOCKED, 0);
    graph.set_node(9, 5, GridGraph::GridState::BLOCKED, 0);
    graph.set_node(10, 5, GridGraph::GridState::BLOCKED, 0);
    
    graph.set_node(3, 15, GridGraph::GridState::BLOCKED, 0);
    graph.set_node(4, 15, GridGraph::GridState::BLOCKED, 0);
    graph.set_node(2, 15, GridGraph::GridState::BLOCKED, 0);


    astar(graph, sf::Vector2i(0, 0), sf::Vector2i(3, 17));
}