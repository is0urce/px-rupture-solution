// name: resources.hpp

#pragma once

#include <px/dev/test.hpp>

#include <px/io/resource_writer.hpp>
#include <px/io/resource_reader.hpp>

#include <string>

using namespace px;

void test_resources() {

    test::section("io resource test");
    {
        struct kv {
            std::string name;
            char        data[1024];
        };

        std::vector<kv> values = { { "1-terrain", "one" }
                                 , { "2-terrain", "two" }
                                 , { "3-terrain", "three" }
                                 , { "4-terrain", "four" }
                                 , { "5-terrain", "five" } };

        std::string const filename = "src/tests/data/resource.hex";

        test::section("write resource data file");
        {
            resource_writer file;

            for (auto & rec : values) {
                file.link(rec.name, rec.data, std::strlen(rec.data) + 1);
            }
            file.write(filename);
        }
        test::section("read resource data file");
        {
            resource_reader reader(filename);
            char read_data[1024];

            for (auto const& rec : values) {
                test::require(reader.has_record(rec.name));

                std::memset(read_data, 0x00, sizeof read_data);
                reader.read(rec.name, read_data, std::strlen(rec.data) + 1);
                test::require(std::strcmp(rec.data, read_data) == 0);

                std::memset(read_data, 0x00, sizeof read_data);
                reader.read(rec.name, read_data);
                test::require(std::strcmp(rec.data, read_data) == 0);

                // known size
                std::memset(read_data, 0x00, sizeof read_data);
                auto & p1 = reader.get(rec.name);
                p1.read(read_data, std::strlen(rec.data) + 1);
                test::require(std::strcmp(rec.data, read_data) == 0);

                // unknown size, fetch
                std::memset(read_data, 0x00, sizeof read_data);
                resource_reader::header info;
                auto & p2 = reader.get(rec.name, info);
                test::require(info.name = resource_reader::void_name());
                test::require(info.size == std::strlen(rec.data) + 1);
                p2.read(read_data, info.size);
                test::require(std::strcmp(rec.data, read_data) == 0);
            }
        }
    }
}