#ifndef STREAM_READER_CLASS_H
#define STREAM_READER_CLASS_H

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/packed_byte_array.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/variant/packed_string_array.hpp>

#include <vector>

using namespace godot;

class StreamReader {
    private:
        int index = 0;
        PackedByteArray data;
    
    public:
        StreamReader(PackedByteArray data) : data(data), index(0) {}

        void set_data(PackedByteArray& data);
        PackedByteArray get_data() const;

        void clear();
        int read_int();
        unsigned int read_uint();
        float read_float();
        uint8_t read_byte();
        bool read_bool();
        String read_string();
        Vector3 read_vector3();
        
        PackedStringArray read_array_string();
        std::vector<Vector3> read_array_vector3();
};

#endif // STREAM_READER_CLASS_H