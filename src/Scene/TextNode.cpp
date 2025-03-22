#include "Scene/TextNode.hpp"

namespace Orenji
{
    TextNode::TextNode(const std::string &name)
        : SceneNode(name)
    {
    }

    TextNode::TextNode(const sf::Font &font, const std::string &text, unsigned int characterSize, const std::string &name)
        : SceneNode(name)
    {
        m_text.setFont(font);
        m_text.setString(text);
        m_text.setCharacterSize(characterSize);
    }

    void TextNode::setText(const std::string &text)
    {
        m_text.setString(text);
    }

    void TextNode::setFont(const sf::Font &font)
    {
        m_text.setFont(font);
    }

    void TextNode::setCharacterSize(unsigned int size)
    {
        m_text.setCharacterSize(size);
    }

    void TextNode::setFillColor(const sf::Color &color)
    {
        m_text.setFillColor(color);
    }

    void TextNode::setOutlineColor(const sf::Color &color)
    {
        m_text.setOutlineColor(color);
    }

    void TextNode::setOutlineThickness(float thickness)
    {
        m_text.setOutlineThickness(thickness);
    }

    void TextNode::setStyle(sf::Uint32 style)
    {
        m_text.setStyle(style);
    }

    sf::Text &TextNode::getText()
    {
        return m_text;
    }

    const sf::Text &TextNode::getText() const
    {
        return m_text;
    }

    void TextNode::centerOrigin()
    {
        sf::FloatRect bounds = m_text.getLocalBounds();
        m_text.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    }

    void TextNode::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
    {
        target.draw(m_text, states);
    }
} // namespace Orenji