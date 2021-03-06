// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: option.proto

#ifndef PROTOBUF_option_2eproto__INCLUDED
#define PROTOBUF_option_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3004000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3004000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/generated_enum_reflection.h>
#include <google/protobuf/descriptor.pb.h>
// @@protoc_insertion_point(includes)

namespace protobuf_option_2eproto {
// Internal implementation detail -- do not use these members.
struct TableStruct {
  static const ::google::protobuf::internal::ParseTableField entries[];
  static const ::google::protobuf::internal::AuxillaryParseTableField aux[];
  static const ::google::protobuf::internal::ParseTable schema[1];
  static const ::google::protobuf::internal::FieldMetadata field_metadata[];
  static const ::google::protobuf::internal::SerializationTable serialization_table[];
  static const ::google::protobuf::uint32 offsets[];
};
void AddDescriptors();
inline void InitDefaults() {
}
}  // namespace protobuf_option_2eproto
namespace hdcs {
namespace networking {
}  // namespace networking
}  // namespace hdcs
namespace hdcs {
namespace networking {

enum CompressType {
  CompressTypeNone = 0,
  CompressTypeGzip = 1,
  CompressTypeZlib = 2,
  CompressTypeSnappy = 3,
  CompressTypeLZ4 = 4
};
bool CompressType_IsValid(int value);
const CompressType CompressType_MIN = CompressTypeNone;
const CompressType CompressType_MAX = CompressTypeLZ4;
const int CompressType_ARRAYSIZE = CompressType_MAX + 1;

const ::google::protobuf::EnumDescriptor* CompressType_descriptor();
inline const ::std::string& CompressType_Name(CompressType value) {
  return ::google::protobuf::internal::NameOfEnum(
    CompressType_descriptor(), value);
}
inline bool CompressType_Parse(
    const ::std::string& name, CompressType* value) {
  return ::google::protobuf::internal::ParseNamedEnum<CompressType>(
    CompressType_descriptor(), name, value);
}
// ===================================================================


// ===================================================================

static const int kServiceTimeoutFieldNumber = 20000;
extern ::google::protobuf::internal::ExtensionIdentifier< ::google::protobuf::ServiceOptions,
    ::google::protobuf::internal::PrimitiveTypeTraits< ::google::protobuf::int64 >, 3, false >
  service_timeout;
static const int kMethodTimeoutFieldNumber = 20000;
extern ::google::protobuf::internal::ExtensionIdentifier< ::google::protobuf::MethodOptions,
    ::google::protobuf::internal::PrimitiveTypeTraits< ::google::protobuf::int64 >, 3, false >
  method_timeout;
static const int kRequestCompressTypeFieldNumber = 20001;
extern ::google::protobuf::internal::ExtensionIdentifier< ::google::protobuf::MethodOptions,
    ::google::protobuf::internal::EnumTypeTraits< ::hdcs::networking::CompressType, ::hdcs::networking::CompressType_IsValid>, 14, false >
  request_compress_type;
static const int kResponseCompressTypeFieldNumber = 20002;
extern ::google::protobuf::internal::ExtensionIdentifier< ::google::protobuf::MethodOptions,
    ::google::protobuf::internal::EnumTypeTraits< ::hdcs::networking::CompressType, ::hdcs::networking::CompressType_IsValid>, 14, false >
  response_compress_type;

// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace networking
}  // namespace hdcs

namespace google {
namespace protobuf {

template <> struct is_proto_enum< ::hdcs::networking::CompressType> : ::google::protobuf::internal::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::hdcs::networking::CompressType>() {
  return ::hdcs::networking::CompressType_descriptor();
}

}  // namespace protobuf
}  // namespace google

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_option_2eproto__INCLUDED
