// name: animator_works.hpp

#pragma once

#include "animator_component.hpp"

#include "sprite_component.hpp"

#include "sprite_system.hpp"

#include "rupture/draw/animation.hpp"

#include <px/common/pool_chain.hpp>

#include <map>
#include <string>
#include <vector>

#include "rupture/app/document.hpp"

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

				auto img = animator.linked<sprite_component>();
				if (img) {

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
			if (sprites == nullptr) throw std::runtime_error("px::animator_works::load(sprites) - sprites is null");

			auto document = document::load_document("data/animations.json");
			for (auto const& animation_node : document["animations"]) {

				// read frames
				std::vector<sprite const*> frames;
				for (auto const& frame_node : animation_node["frames"]) {
					std::string frame_name = frame_node;
					frames.push_back(sprites->frame(frame_name));
				}

				std::string name = animation_node["name"];
				std::vector<animation> & animation_set = animations[name];

				for (auto const& animatom_node : animation_node["animations"]) {

					animation_set.emplace_back();
					animation & current_set = animation_set.back();

					for (auto const& key : animatom_node["frames"]) {
						double time = key["time"];
						long long frame_id = key["frame"];

						current_set.frames.push_back({ frames[frame_id], time });
					}
					current_set.has_loop = false;
				}
			}
		}

	public:
		~animator_works() = default;
		animator_works() = default;

	private:
		pool_chain<animator_component, 1024>			pool;
		std::map<std::string, std::vector<animation>>	animations;
	};
}