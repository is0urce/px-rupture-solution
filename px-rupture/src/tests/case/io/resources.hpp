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
                file.add(rec.name, rec.data, std::strlen(rec.data) + 1);
            }
            file.write(filename);
        }
        test::section("read resource data file");
        {
            resource_reader reader(filename);

            for (auto & rec : values) {
                auto & point = reader.get(rec.name);
                char read_data[1024];
                test::require(reader.has_record(rec.name));
                point.read(read_data, std::strlen(rec.data) + 1);
                test::require(std::strcmp(rec.data, read_data) == 0);
            }
        }
    }
}