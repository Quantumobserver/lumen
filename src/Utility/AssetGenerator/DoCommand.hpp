
#pragma once

#include "AssetGeneratorData.hpp"

std::optional<std::string_view> GetCommand(std::string_view commands,
                                           std::string_view::iterator &iterator) noexcept
{
        if (commands.cend() == iterator) {
                return std::nullopt;
        }

        if (';' == *iterator) {
                ++iterator;
        }
        auto command_begin = iterator;
        while ((commands.cend() != iterator) && (';' != *iterator)) {
                ++iterator;
        }

        return std::string_view{command_begin, iterator};
}

void DoCommand(std::string_view command, AssetGeneratorData &asset_generator_data) noexcept
{
        if ("exit" == command_name) {
                asset_generator_data.Exit();
        }
}

void DoCommands(AssetGeneratorData &asset_generator_data) noexcept
{
        auto &optional_commands = asset_generator_data.commands;

        if (!optional_commands) {
                return;
        }

        std::string_view commands = *optional_commands;
        auto iterator = commands.cbegin();
        if (':' == *iterator) {
                ++iterator;
        }
        for (auto command = GetCommand(commands, iterator); command.has_value();
             command = GetCommand(commands, iterator)) {
                DoCommand(*command, asset_generator_data);
        }

        optional_commands.reset();
}