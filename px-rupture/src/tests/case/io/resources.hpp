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

        std::vector<kv> values = { { "00-terrain", "zero" }
                                 , { "01-terrain", "one" }
                                 , { "02-terrain", "two" }
                                 , { "03-terrain", "three" }
                                 , { "04-terrain", "four" }
                                 , { "05-terrain", "five" } };

        std::string const filename = "src/tests/data/resource.hex";

        test::section("write resource data file");
        {
            resource_writer file;

            for (auto & rec : values) {
                file.copy(rec.name, rec.data, std::strlen(rec.data) + 1);
            }
            file.write(filename);
        }
        
        test::section("resource data file fetch info");
        {
            resource_reader reader(filename);
            test::require(!reader.has_record("invalid-name"));

            resource_file::header info;
            reader.get("invalid-name", info);
            test::require(info.name == resource_file::noluck_name());
            test::require(info.size == 0);
        }

        test::section("read resource data file");
        {
            resource_reader reader(filename);
            char read_data[1024];

            for (size_t j = 0, size = values.size(); j != size; ++j) {

                resource_writer file;
                for (size_t i = 0; i != j; ++i) {
                    auto & rec = values[i];
                   file.link(rec.name, rec.data, std::strlen(rec.data) + 1);
                }
                file.write(filename);

                for (size_t i = 0; i != j; ++i) {
                    auto & rec = values[i];

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
}