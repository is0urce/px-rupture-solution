// name: options.hpp
// type: c++
// auth: is0urce
// desc: class

#pragma once

#include "panel.hpp"

#include "../environment.hpp"

#include <px/app/cfg.hpp>

#include <imgui/imgui.h>

namespace px::ui {

    class options final
        : public panel
    {
    public:
        virtual ~options() override = default;

        options(environment * ctx, cfg * config, bool * flag)
            : context(ctx)
            , configuration(config)
            , open_flag(flag) {
            apply_configuration();
        }

    protected:
        virtual void combine_panel() override {
            if (!context || !configuration || !is_open()) return;
            ImGui::Begin("##options");
            if (ImGui::SliderFloat("##volume_master", &volume_master, 0, 1, "%.2f")) {
                configuration->set_float("volume.master", volume_master);
                context->set_volume(sound_channel::master, volume_master);
            }
            if (ImGui::SliderFloat("##volume_music", &volume_music, 0, 1, "%.2f")) {
                configuration->set_float("volume.music", volume_music);
                context->set_volume(sound_channel::music, volume_music);
            }
            if (ImGui::SliderFloat("##volume_sfx", &volume_sfx, 0, 1, "%.2f")) {
                configuration->set_float("volume.sfx", volume_sfx);
                context->set_volume(sound_channel::sfx, volume_sfx);
            }
            ImGui::End();
        }

    private:
        bool is_open() const noexcept {
            return open_flag && *open_flag;
        }

        void apply_configuration() {
            volume_master = static_cast<float>(configuration->value("volume.master", 1.0));
            volume_music = static_cast<float>(configuration->value("volume.music", 1.0));
            volume_sfx = static_cast<float>(configuration->value("volume.sfx", 1.0));
            if (context) {
                context->set_volume(sound_channel::master, volume_master);
                context->set_volume(sound_channel::music, volume_music);
                context->set_volume(sound_channel::sfx, volume_sfx);
            }
        }

    private:
        environment *   context;
        cfg *           configuration;
        bool *          open_flag;
        float           volume_master;
        float           volume_music;
        float           volume_sfx;
    };
}