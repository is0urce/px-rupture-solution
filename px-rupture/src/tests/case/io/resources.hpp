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
        std::string const filename = "src/tests/data/resource.hex";
        char write_data[] = "SUPPOSEDTOBERAWDATA but this is a string";
        test::section("write resource data file");
        {
            resource_writer file;

            file.add("abc.terrain", write_data, sizeof write_data);
            file.add("resource_name.terrain", write_data, sizeof write_data);
            file.add("cave_of_schurlymndanschuntszc_lower_level.terrain", write_data, sizeof write_data);
            file.add("xyz.terrain", write_data, sizeof write_data);
            file.write(filename);
        }
        test::section("read resource data file");
        {
            resource_reader reader(filename);

            auto & point = reader.get("abc.terrain");
            char read_data[1024];
            point.read(read_data, sizeof write_data);

            test::require(std::strcmp(write_data, read_data) == 0);
        }
    }
}