// name: a_star.hpp
// type: c++ header
// desc: pathfinding
// auth: is0urce

// implementation of A* pathfinding
// a_star::find(start, finish, predicate, steps)

#pragma once

#include <px/common/point.hpp>

#include <memory>
#include <list>
#include <set>
#include <vector>

namespace px {

    class a_star {
    private:
        class coord;
        using coord_ptr = std::shared_ptr<coord>;
        class coord
            : public point2
        {
        public:
            double g_score;
            double f_score;
            coord const* came_from;
        public:
            coord(point2 p, double traversed, double heu, coord const* prev)
                : point2(p)
                , g_score(traversed)
                , f_score(traversed + heu)
                , came_from(prev)
            {
            }
        };

        struct score_less {
            bool operator() (coord_ptr const& a, coord_ptr const& b) const noexcept {
                return a->f_score < b->f_score;
            }
        };

        struct lexicographical_less {
            template <typename LPtr, typename RPtr>
            bool operator() (LPtr const& lhs, RPtr const& rhs) const {
                return lhs->lex_less(*rhs);
            }
            using is_transparent = point2 * ; // this defined to enable homogenical comparation to raw pointers
        };

    public:
        // Predicate - traversable predicate : bool(point)
        // start not included in path
        // finish always inclided (even if traversable predicate returns false)
        template <typename Predicate>
        static auto find(point2 start, point2 finish, Predicate && predicate, unsigned int steps) {
            std::list<point2> path;
            std::multiset<coord_ptr, score_less> open;
            std::set<coord_ptr, lexicographical_less> closed;

            open.insert(std::make_shared<coord>(start, 0, heuristic(start, finish), nullptr));
            while (steps > 0 && !open.empty()) {
                auto top = open.begin();
                coord_ptr current = *top;

                // terminal
                if (*current == finish) {
                    construct_path(current.get(), path);
                    break;
                }

                open.erase(top);
                closed.insert(current);

                // expand
                enum_neighbours(*current, [&](point2 const& neighbor) {
                    if (!predicate(neighbor) && neighbor != finish) return; // traversable callback
                    if (closed.find(&neighbor) != closed.end()) return;     // already in closed list

                    auto it = open.cbegin(), last = open.cend();
                    while (it != last && **it != neighbor) ++it;            // check is point in open list

                    auto score = current->g_score + distance(*current, neighbor);
                    if (it == last) {
                        open.insert(std::make_shared<coord>(neighbor, score, heuristic(neighbor, finish), current.get()));                  // not in open list
                    }
                    else if (score < (*it)->g_score) {
                        open.insert(open.erase(it), std::make_shared<coord>(neighbor, score, heuristic(neighbor, finish), current.get()));  // better option
                    }
                });
                --steps;
            }
            return path;
        }

    private:
        static auto heuristic(point2 const& a, point2 const& b) {
            return a.distance(b);
        }

        static auto distance(point2 const& a, point2 const& b) {
            return a.king_distance(b);
        }

        template <typename Unary>
        static void enum_neighbours(point2 const& c, Unary && fn) {
            fn(point2(c.x() + 1, c.y() + 0));
            fn(point2(c.x() + 0, c.y() + 1));
            fn(point2(c.x() - 1, c.y() + 0));
            fn(point2(c.x() + 0, c.y() - 1));
            fn(point2(c.x() + 1, c.y() + 1));
            fn(point2(c.x() + 1, c.y() - 1));
            fn(point2(c.x() - 1, c.y() + 1));
            fn(point2(c.x() - 1, c.y() - 1));
        }

        // reconstruct path from target to start
        static void construct_path(coord const* step, std::list<point2> & path) {
            while (step) {
                if (!step->came_from) break; // stop there and not include starting point as part of a path, so first step is list::front()
                path.emplace_front(step->x(), step->y());
                step = step->came_from;
            }
        }
    };
}