// name: terrain.cpp

#include "terrain.hpp"

#include "terrain_internal.hpp"

#include <px/memory/memory.hpp>

namespace px {

	terrain::~terrain()
	{
	}
	terrain::terrain()
		: impl(make_uq<terrain_internal>())
	{
	}

	bool terrain::is_transparent(point2 const& location) const
	{
		return impl->is_transparent(location);
	}
	bool terrain::is_traversable(point2 const& location, rl::traverse_options<rl::traverse> const& opts) const
	{
		return impl->is_traversable(location, opts);
	}
}