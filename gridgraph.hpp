#pragma once

#include <SFML/Graphics.hpp>
#include <vector>


class GridGraph : public sf::Drawable
{   
    public:
    enum class GridState {
        BLOCKED,
        FREE,
        VISITED,
        GOAL,
        PATH
    };
    struct GridPoint 
    {
        sf::Vector2i parent;
        std::size_t weight = 0;
        enum GridState state = GridGraph::GridState::FREE;
    };
    GridGraph(std::size_t width, std::size_t height);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void set_node(std::size_t x, std::size_t y, GridState state, std::size_t weight); 
    void gen_block(float x1, float x2, float y1, float y2);
    GridGraph::GridPoint* get_node(std::size_t x, std::size_t y);

    sf::Vector2i size;
    std::vector<std::vector<GridPoint>> grid;
};