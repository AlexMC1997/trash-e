#include "gridgraph.hpp"
GridGraph::GridGraph(std::size_t width, std::size_t height) :
    size(width, height),
    grid(height)
{
    for (auto& row : grid)
        row.resize(width);
}


void GridGraph::set_node(std::size_t x, std::size_t y, GridState state, std::size_t weight) {
    this->grid[y][x].state = state;
    this->grid[y][x].weight = weight;
}

GridGraph::GridPoint* GridGraph::get_node(std::size_t x, std::size_t y) {
    return &this->grid[y][x];
} 

void GridGraph::gen_block(float x1, float x2, float y1, float y2) {
    int ix1 = x1;
    int ix2 = x2 + 1;
    int iy1 = y1;
    int iy2 = y2 + 1;
}

void GridGraph::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    auto wdims = target.getSize();
    sf::Vector2f cellsz(wdims.x / this->size.x, wdims.y / this->size.y);
    
    for (std::size_t i = 0; i < this->size.y; i++) {
        for (std::size_t j = 0; j < this->size.x; j++) {
            sf::RectangleShape gr_rect;
            if (this->grid[i][j].state == GridGraph::GridState::FREE)
                gr_rect.setFillColor(sf::Color::White);
            else if (this->grid[i][j].state == GridGraph::GridState::VISITED) {
                uint8_t r = ((float)grid[i][j].weight / (size.x + size.y)) * 255;
                sf::Color color(r, 255 - r, 0);
                gr_rect.setFillColor(color);
            }
            else if (this->grid[i][j].state == GridGraph::GridState::GOAL)
                gr_rect.setFillColor(sf::Color::Blue);
            else if (this->grid[i][j].state == GridGraph::GridState::PATH)
                gr_rect.setFillColor(sf::Color::Magenta);
            else
                gr_rect.setFillColor(sf::Color::Black);
            gr_rect.setSize(cellsz);
            gr_rect.setPosition(j * cellsz.x, i * cellsz.y);
            target.draw(gr_rect);
        }
    }
}