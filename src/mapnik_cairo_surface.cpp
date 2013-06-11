// node
#include <node.h>
#include <node_buffer.h>

#include "mapnik_cairo_surface.hpp"
using namespace v8;

Persistent<FunctionTemplate> CairoSurface::constructor;

void CairoSurface::Initialize(Handle<Object> target) {

    HandleScope scope;
    constructor = Persistent<FunctionTemplate>::New(FunctionTemplate::New(CairoSurface::New));
    constructor->InstanceTemplate()->SetInternalFieldCount(1);
    constructor->SetClassName(String::NewSymbol("CairoSurface"));
    NODE_SET_PROTOTYPE_METHOD(constructor, "getData", getData);
    target->Set(String::NewSymbol("CairoSurface"),constructor->GetFunction());
}

CairoSurface::CairoSurface(unsigned int width, unsigned int height) :
    ObjectWrap()
{
}

CairoSurface::~CairoSurface()
{
}

Handle<Value> CairoSurface::New(const Arguments& args)
{
    HandleScope scope;
    if (!args.IsConstructCall())
        return ThrowException(String::New("Cannot call constructor as function, you need to use 'new' keyword"));

    if (args[0]->IsExternal())
    {
        //std::clog << "external!\n";
        Local<External> ext = Local<External>::Cast(args[0]);
        void* ptr = ext->Value();
        CairoSurface* im =  static_cast<CairoSurface*>(ptr);
        im->Wrap(args.This());
        return args.This();
    }

    if (args.Length() == 2)
    {
        if (!args[0]->IsNumber() || !args[1]->IsNumber())
            return ThrowException(Exception::Error(
                                      String::New("CairoSurface 'width' and 'height' must be a integers")));
        CairoSurface* im = new CairoSurface(args[0]->IntegerValue(),args[1]->IntegerValue());
        im->Wrap(args.This());
        return args.This();
    }
    else
    {
        return ThrowException(Exception::Error(
                                  String::New("please provide CairoSurface width and height")));
    }
    return Undefined();
}

Handle<Value> CairoSurface::getData(const Arguments& args)
{
    HandleScope scope;
    CairoSurface* surface = ObjectWrap::Unwrap<CairoSurface>(args.This());
    std::string s = surface->ss_.str();
    node::Buffer *retbuf = node::Buffer::New((char*)s.data(),s.size());
    return scope.Close(retbuf->handle_);
}
