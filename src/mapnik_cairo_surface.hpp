#ifndef __NODE_MAPNIK_CAIRO_H__
#define __NODE_MAPNIK_CAIRO_H__

#include <v8.h>
#include <node_object_wrap.h>

#include <iostream>
#include <sstream>

// cairo
#if defined(HAVE_CAIRO)
#include <cairo.h>
#endif

using namespace v8;

class CairoSurface: public node::ObjectWrap {
public:
    typedef std::stringstream i_stream;
    static Persistent<FunctionTemplate> constructor;
    static void Initialize(Handle<Object> target);
    static Handle<Value> New(const Arguments &args);
    static Handle<Value> getData(const Arguments &args);
    void _ref() { Ref(); }
    void _unref() { Unref(); }
    CairoSurface(unsigned int width, unsigned int height);
    static cairo_status_t write_callback(void *closure,
                                         const unsigned char *data,
                                         unsigned int length)
    {
        if (!closure)
          return CAIRO_STATUS_WRITE_ERROR;
        i_stream* fin = reinterpret_cast<i_stream*>(closure);
        *fin << std::string((const char*)data,(size_t)length);
        return CAIRO_STATUS_SUCCESS;
    }
    mutable i_stream ss_;
private:
    ~CairoSurface();
};

#endif
