// name: screen_options.hpp
// type: c++
// auth: is0urce
// desc: class

#pragma once

// configuration options ui

#include "../panel.hpp"
#include "../immediate.hpp"
#include "../design.hpp"

#include "../../app/settings.hpp"
#include "../../environment.hpp"

#include <px/app/cfg.hpp>

namespace px {

    class screen_options final
        : public panel {
    public:
        virtual ~screen_options() noexcept override = default;

        screen_options(environment * ctx, cfg * config, bool * flag)
            : context(ctx)
            , configuration(config)
            , open_flag(flag) {
            apply_configuration();
        }

    protected:
        virtual void combine_panel() override {
            if (is_open()) {
                ImVec2 const& screen = ImGui::GetIO().DisplaySize;
                float const options_width = 600.0f;
                combine_options({ screen.x * 0.5f - options_width * 0.5f, screen.y * 0.33f }, options_width);
            }
        }

    private:
        void combine_options(ImVec2 position, float width) {
            immediate::dim();

            ImGui::SetNextWindowPos(position, ImGuiCond_Always);
            ImGui::SetNextWindowSize({ width, 600 });
            ImGui::PushStyleColor(ImGuiCol_WindowBg, { 0.0f, 0.0f, 0.0f, 0.0f });
            ImGui::Begin("##options", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse);
            ImGui::SetWindowFocus();
            float const content_width = width * 0.9f;

            immediate::print("Master Volume", content_width);
            ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.9f);
            if (ImGui::SliderFloat("##volume_master", &volume_master, 0, 100, "%3.0f%%")) {
                configuration->set_float("sound.volume.master", volume_master * 0.01);
                context->set_volume(sound_channel::master, volume_master * 0.01);
            }
            ImGui::PopItemWidth();

            immediate::print("Music Volume", content_width);
            ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.9f);
            if (ImGui::SliderFloat("##volume_music", &volume_music, 0, 100, "%3.0f%%")) {
                configuration->set_float("sound.volume.music", volume_music * 0.01);
                context->set_volume(sound_channel::music, volume_music * 0.01);
            }
            ImGui::PopItemWidth();

            immediate::print("EFX Volume", content_width);
            ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.9f);
            if (ImGui::SliderFloat("##volume_sfx", &volume_sfx, 0, 100, "%3.0f%%")) {
                configuration->set_float("sound.volume.sfx", volume_sfx * 0.01);
                context->set_volume(sound_channel::sfx, volume_sfx * 0.01);
            }
            ImGui::PopItemWidth();

            ImGui::NewLine();
            if (immediate::button("Back##options_back", content_width)) {
                press_back();
            }

            ImGui::End();
            ImGui::PopStyleColor(1);
        }

        bool is_open() const noexcept {
            return context && configuration && is_true(open_flag);
        }

        void close() noexcept {
            clear_flag(open_flag);
        }

        void apply_configuration() {
            volume_master = static_cast<float>(configuration->value("sound.volume.master", 1.0) * 100);
            volume_music = static_cast<float>(configuration->value("sound.volume.music", 1.0) * 100);
            volume_sfx = static_cast<float>(configuration->value("sound.volume.sfx", 1.0) * 100);
            if (context) {
                context->set_volume(sound_channel::master, volume_master * 0.01);
                context->set_volume(sound_channel::music, volume_music * 0.01);
                context->set_volume(sound_channel::sfx, volume_sfx * 0.01);
            }
        }

        void press_back() {
            if (context) {
                context->play_sound(settings::sound_path + std::string("snd_ui_click.wav"), 1.0f);
            }
            close();
        }

    private:
        environment *   context;
        cfg *           configuration;
        bool *          open_flag;
        float           volume_master; // volume mediators in multipliers of 100 (percents)
        float           volume_music;
        float           volume_sfx;
    };
}