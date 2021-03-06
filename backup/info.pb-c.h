/* Generated by the protocol buffer compiler.  DO NOT EDIT! */

#ifndef PROTOBUF_C_info_2eproto__INCLUDED
#define PROTOBUF_C_info_2eproto__INCLUDED

#include <google/protobuf-c/protobuf-c.h>

PROTOBUF_C_BEGIN_DECLS


typedef struct _Nodeinfo Nodeinfo;


/* --- enums --- */


/* --- messages --- */

struct  _Nodeinfo
{
  ProtobufCMessage base;
  int32_t np;
  int32_t npm;
  int32_t npa;
  double load;
  double uthresh;
  double lthresh;
};
#define NODEINFO__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&nodeinfo__descriptor) \
    , 0, 0, 0, 0, 0, 0 }


/* Nodeinfo methods */
void   nodeinfo__init
                     (Nodeinfo         *message);
size_t nodeinfo__get_packed_size
                     (const Nodeinfo   *message);
size_t nodeinfo__pack
                     (const Nodeinfo   *message,
                      uint8_t             *out);
size_t nodeinfo__pack_to_buffer
                     (const Nodeinfo   *message,
                      ProtobufCBuffer     *buffer);
Nodeinfo *
       nodeinfo__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   nodeinfo__free_unpacked
                     (Nodeinfo *message,
                      ProtobufCAllocator *allocator);
/* --- per-message closures --- */

typedef void (*Nodeinfo_Closure)
                 (const Nodeinfo *message,
                  void *closure_data);

/* --- services --- */


/* --- descriptors --- */

extern const ProtobufCMessageDescriptor nodeinfo__descriptor;

PROTOBUF_C_END_DECLS


#endif  /* PROTOBUF_info_2eproto__INCLUDED */
