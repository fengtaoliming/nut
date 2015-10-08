﻿
#ifndef ___HEADFILE_CD73C4E6_B2EC_409C_A37E_42DAD0C5CD14_
#define ___HEADFILE_CD73C4E6_B2EC_409C_A37E_42DAD0C5CD14_

#include <assert.h>
#include <stdint.h>
#include <stddef.h>

#include <nut/rc/rc_ptr.h>

namespace nut
{

class OutputStream
{
    NUT_REF_COUNTABLE

public:
    virtual bool is_little_endian() const = 0;

    virtual void set_little_endian(bool le) = 0;

    virtual void write_uint8(uint8_t v) = 0;

    void write_int8(int8_t v)
    {
        write_uint8((uint8_t) v);
    }

    virtual void write_uint16(uint16_t v)
    {
        if (is_little_endian())
        {
            write_uint8((uint8_t) v);
            write_uint8((uint8_t) (v >> 8));
        }
        else
        {
            write_uint8((uint8_t) (v >> 8));
            write_uint8((uint8_t) v);
        }
    }

    void write_int16(int16_t v)
    {
        write_uint16((uint16_t) v);
    }

    virtual void write_uint32(uint32_t v)
    {
        if (is_little_endian())
        {
            write_uint8((uint8_t) v);
            write_uint8((uint8_t) (v >> 8));
            write_uint8((uint8_t) (v >> 16));
            write_uint8((uint8_t) (v >> 24));
        }
        else
        {
            write_uint8((uint8_t) (v >> 24));
            write_uint8((uint8_t) (v >> 16));
            write_uint8((uint8_t) (v >> 8));
            write_uint8((uint8_t) v);
        }
    }

    void write_int32(int32_t v)
    {
        write_uint32((uint32_t) v);
    }

    virtual void write_uint64(uint64_t v)
    {
        if (is_little_endian())
        {
            write_uint8((uint8_t) v);
            write_uint8((uint8_t) (v >> 8));
            write_uint8((uint8_t) (v >> 16));
            write_uint8((uint8_t) (v >> 24));
            write_uint8((uint8_t) (v >> 32));
            write_uint8((uint8_t) (v >> 40));
            write_uint8((uint8_t) (v >> 48));
            write_uint8((uint8_t) (v >> 56));
        }
        else
        {
            write_uint8((uint8_t) (v >> 56));
            write_uint8((uint8_t) (v >> 48));
            write_uint8((uint8_t) (v >> 40));
            write_uint8((uint8_t) (v >> 32));
            write_uint8((uint8_t) (v >> 24));
            write_uint8((uint8_t) (v >> 16));
            write_uint8((uint8_t) (v >> 8));
            write_uint8((uint8_t) v);
        }
    }

    void write_int64(int64_t v)
    {
        write_uint64((uint64_t) v);
    }

    virtual void write_float(float v)
    {
        if (is_little_endian())
        {
            for (size_t i = 0; i < sizeof(float); ++i)
                write_uint8(reinterpret_cast<const uint8_t*>(&v)[i]);
        }
        else
        {
            for (size_t i = 0; i < sizeof(float); ++i)
                write_uint8(reinterpret_cast<const uint8_t*>(&v)[sizeof(float) - i - 1]);
        }
    }

    virtual void write_double(double v)
    {
        if (is_little_endian())
        {
            for (size_t i = 0; i < sizeof(double); ++i)
                write_uint8(reinterpret_cast<const uint8_t*>(&v)[i]);
        }
        else
        {
            for (size_t i = 0; i < sizeof(double); ++i)
                write_uint8(reinterpret_cast<const uint8_t*>(&v)[sizeof(double) - i - 1]);
        }
    }

    virtual size_t write(const void *buf, size_t cb)
    {
        assert(NULL != buf || 0 == cb);
        for (size_t i = 0; i < cb; ++i)
            write_uint8(((const uint8_t*) buf)[i]);
        return cb;
    }
};

}

#endif