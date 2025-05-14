#pragma once

#include "Menu.hpp"
#include "BasicUIComponent.hpp"
#include <vector>
#include <memory>

namespace Lumen {
namespace UI {
namespace Component {

class Panel : public Lumen::UI::Component::BasicUIComponent {
public:
    Panel() : BasicUIComponent(UIComponentTypeTag::PANEL) {}

    // 添加菜单
    void AddMenu(std::unique_ptr<Menu> menu) {
        m_menus.push_back(std::move(menu));
    }

    // 显示菜单
    void ShowMenu(size_t index) {
        if (index < m_menus.size()) {
            // 这里可以添加代码来显示菜单
            // 例如，调用菜单的更新和渲染方法
            m_menus[index]->Update(0.0f); // 假设 delta_time 为 0.0f
        }
    }

    // 更新面板
    void Update(float delta_time) noexcept override {
        for (auto& menu : m_menus) {
            menu->Update(delta_time);
        }
    }

    // 渲染面板
    void Render() noexcept override {
        for (auto& menu : m_menus) {
            menu->Render();
        }
    }

private:
    std::vector<std::unique_ptr<Menu>> m_menus;
};

} // namespace Component
} // namespace UI
} // namespace Lumen