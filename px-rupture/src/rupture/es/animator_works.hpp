// name: animator_works.hpp
// type: c++
// auth: is0urce
// desc: animator works internal implementation class

#pragma once

#include "animator_component.hpp"
#include "sprite_component.hpp"
#include "sprite_system.hpp"

#include "../app/document.hpp"
#include "../draw/animation.hpp"

#include <px/common/pool_chain.hpp>

#include <map>
#include <string>
#include <vector>

namespace px {

    class animator_works final {
    public:
        uq_ptr<animator_component> make(std::string const& name) {
            auto kv_it = animations.find(name);
            if (kv_it != animations.end()) {
                auto animator = pool.make_uq();

                auto const& v = kv_it->second;
                animator->set_id(kv_it->first.c_str());
                animator->clear();
                for (animation const& a : v) {
                    animator->push_animation(&a);
                }

                return animator;
            }
            return nullptr;
        }

        void update(double time) {
            pool.enumerate([&](animator_component & animator) {
                if (!animator.is_active()) return;
                if (!animator.is_playing()) return;

                auto img = animator.linked<sprite_component>();
                if (!img) return;

                auto current = animator.current();
                auto frame = current->select_frame(time);
                if (frame) {
                    *static_cast<sprite*>(img) = *frame;
                }
            });
        }

        void finish_animations() {
            pool.enumerate([&](animator_component & animator) {
                if (!animator.is_active()) return;
                if (!animator.is_playing()) return;

                if (auto img = animator.linked<sprite_component>()) {

                    auto current = animator.current();
                    auto frame = current->last_frame();
                    if (frame) {
                        *static_cast<sprite*>(img) = *frame;
                    }
                }

                animator.stop();
            });
        }

        void load(sprite_system const* sprites) {
            if (!sprites) throw std::runtime_error("px::animator_works::load(sprites) - sprites is null");

            try {
                auto document = document::load_document("data/animations.json");
                for (auto const& animation_node : document["animations"]) {

                    // read frames
                    std::vector<sprite const*> frames;
                    for (auto const& frame_node : animation_node["frames"]) {
                        std::string frame_name = frame_node;
                        auto img = sprites->frame(frame_name);
                        if (!img) throw std::runtime_error("px::animator_works::load() - no image with name='" + frame_name + "'");
                        frames.push_back(img);
                    }

                    std::string name = animation_node["name"];
                    std::vector<animation> & clip_set = animations[name];  // create entry

                    for (auto const& clip_node : animation_node["animations"]) {

                        clip_set.emplace_back();
                        animation & clip = clip_set.back();

                        clip.has_loop = clip_node["loop"];

                        for (auto const& keyframe_node : clip_node["frames"]) {
                            double time = keyframe_node["time"];
                            unsigned int frame_id = keyframe_node["frame"];

                            clip.frames.push_back({ frames.at(frame_id), time });
                        }
                    }
                }
            }
            catch (std::exception const& exc) {
                throw std::runtime_error(std::string("px::animator_works::load() - error in data, what=") + exc.what());
            }
            catch (...) {
                throw std::runtime_error(std::string("px::animator_works::load() - unhandled error"));
            }
        }

    public:
        ~animator_works() = default;
        animator_works() = default;

    private:
        pool_chain<animator_component, 1024>            pool;
        std::map<std::string, std::vector<animation>>   animations;
    };
}